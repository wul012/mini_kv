# 第六十五次讲解：第三十二版命令执行计数核心

第六十四次讲了第三十一版测试、README、归档和整体增删改。
这次讲第三十二版的核心代码：

```text
include/minikv/command.hpp
src/command.cpp
include/minikv/tcp_server.hpp
src/tcp_server.cpp
```

第三十一版时，运行期观测已经有：

```text
STATS / HEALTH
 -> 查询 Store、WAL、TCP connection stats

server_metrics
 -> 周期输出 active / total / peak 连接统计
```

第三十二版继续补一块更基础的运行信号：

```text
命令执行计数
 -> total_commands
 -> successful_commands
 -> error_commands
```

一句话说，v32 让 mini-kv 不只知道“有多少连接”，也能知道“处理过多少命令，其中多少成功、多少报错”。

---

# 1. 为什么 v32 要做 command metrics

v31 之前，排查运行状态时可以看到：

```text
live_keys
wal_enabled
compact_recommended
active_connections
total_connections
peak_connections
```

这些回答的是：

```text
数据状态怎样
WAL 状态怎样
连接状态怎样
```

但还缺少一个问题：

```text
服务实际处理了多少命令
错误比例大不大
```

比如真实运行时，如果看到：

```text
total_commands=10000
error_commands=8000
```

就能很快判断：

```text
服务不是没流量
而是客户端大量发错命令或参数
```

所以 v32 增加命令层 counters。

---

# 2. CommandProcessorMetrics 的角色

头文件新增：

```cpp
struct CommandProcessorMetrics {
    std::uint64_t total_commands = 0;
    std::uint64_t successful_commands = 0;
    std::uint64_t error_commands = 0;
};
```

字段含义：

```text
total_commands
 -> 已完成处理的非空命令总数

successful_commands
 -> 返回非 ERR 的命令数量

error_commands
 -> 返回 ERR 开头响应的命令数量
```

这里用 `std::uint64_t`，因为命令数是累计值，理论上会持续增长。
用 64 位计数可以避免普通长时间运行中很快溢出。

---

# 3. 什么命令会被计数

第三十二版的规则是：

```text
空行
 -> 不计数

进入 CommandProcessor::execute_trimmed 的命令
 -> 计数

response 以 "ERR " 开头
 -> error_commands +1

其他 response
 -> successful_commands +1
```

举例：

```text
PING
 -> total_commands +1
 -> successful_commands +1

GET key extra
 -> total_commands +1
 -> error_commands +1

QUIT
 -> total_commands +1
 -> successful_commands +1
```

这里 `QUIT` 虽然会关闭连接，但它是一个正常命令，所以算成功。

---

# 4. CommandMetricsTracker 的角色

头文件新增：

```cpp
class CommandMetricsTracker {
public:
    void record(const CommandResult& result);
    CommandProcessorMetrics stats() const;

private:
    std::atomic<std::uint64_t> total_commands_{0};
    std::atomic<std::uint64_t> successful_commands_{0};
    std::atomic<std::uint64_t> error_commands_{0};
};
```

它是实际保存 counters 的对象。

为什么不直接把三个整数放进 `CommandProcessor`？

```text
CLI 只有一个 CommandProcessor，直接放进去也可以
但 TCP server 每个 client thread 都会创建一个 CommandProcessor
如果 counters 放在每个 processor 内部，server 级别就看不到全局累计
```

所以 v32 抽出一个可共享的 tracker：

```text
多个 CommandProcessor
 -> 共享同一个 CommandMetricsTracker
 -> 得到 server 级累计命令数
```

---

# 5. 为什么 tracker 使用 atomic

`CommandMetricsTracker` 的三个字段都是：

```cpp
std::atomic<std::uint64_t>
```

原因是 TCP server 会有多个客户端线程：

```text
client thread 1
 -> CommandProcessor::execute
 -> metrics_tracker.record

client thread 2
 -> CommandProcessor::execute
 -> metrics_tracker.record

server main thread
 -> server_metrics 读取 stats
```

如果用普通整数，就会有数据竞争。

使用 atomic 后：

```text
多个线程可以安全累加
server_metrics 可以安全读取
```

这和前面 TCP connection tracker 的思路一致。

---

# 6. CommandProcessorOptions 新增 metrics_tracker

`CommandProcessorOptions` 从：

```cpp
struct CommandProcessorOptions {
    using ConnectionStatsProvider = std::function<CommandConnectionStats()>;

    bool auto_compact_wal = false;
    ConnectionStatsProvider connection_stats;
};
```

扩展为：

```cpp
struct CommandProcessorOptions {
    using ConnectionStatsProvider = std::function<CommandConnectionStats()>;

    bool auto_compact_wal = false;
    ConnectionStatsProvider connection_stats;
    std::shared_ptr<CommandMetricsTracker> metrics_tracker;
};
```

这个字段用于外部注入共享 tracker。

典型场景：

```text
CLI
 -> 不传 metrics_tracker
 -> CommandProcessor 自己创建一个 tracker

TCP server
 -> TcpServer 创建一个共享 tracker
 -> 每个 client CommandProcessor 都使用同一个 tracker
```

这样不会破坏原有构造方式。

---

# 7. CommandProcessor 新增 metrics_tracker_

类里新增：

```cpp
std::shared_ptr<CommandMetricsTracker> metrics_tracker_;
```

构造函数现在是：

```cpp
CommandProcessor::CommandProcessor(Store& store, WriteAheadLog* wal, CommandProcessorOptions options)
    : store_(store),
      wal_(wal),
      options_(std::move(options)),
      metrics_tracker_(options_.metrics_tracker ? options_.metrics_tracker : std::make_shared<CommandMetricsTracker>()) {
}
```

逻辑是：

```text
options 里传了 metrics_tracker
 -> 使用外部共享 tracker

没有传
 -> 自己创建一个 tracker
```

所以旧代码：

```cpp
CommandProcessor processor{store};
```

仍然可用。

---

# 8. CommandProcessor::metrics

新增公开方法：

```cpp
CommandProcessorMetrics CommandProcessor::metrics() const {
    return metrics_tracker_->stats();
}
```

它主要用于测试，也可以用于未来其它入口。

比如单元测试可以直接检查：

```cpp
assert(processor.metrics().total_commands == 1);
```

而不是只能通过 `STATS` 字符串间接判断。

这让 counters 的核心语义更容易测试。

---

# 9. execute 被拆成 execute 和 execute_trimmed

v32 之前，`CommandProcessor::execute` 里直接完成全部事情：

```text
trim
解析命令
执行命令
返回 CommandResult
```

v32 把它拆成两层：

```cpp
CommandResult CommandProcessor::execute(std::string_view line) {
    const std::string trimmed = trim_copy(line);
    if (trimmed.empty()) {
        return {};
    }

    CommandResult result = execute_trimmed(trimmed);
    metrics_tracker_->record(result);
    return result;
}
```

和：

```cpp
CommandResult CommandProcessor::execute_trimmed(std::string_view trimmed) {
    std::istringstream input{std::string{trimmed}};
    ...
}
```

这样做的好处是：

```text
execute 负责公共流程
 -> trim
 -> 空行不计数
 -> 调用真正执行
 -> 记录 metrics

execute_trimmed 保留原来的命令处理主体
```

改动范围小，也不需要在每个 `return` 前手动写计数。

---

# 10. 为什么空行不计数

`execute` 开头仍然是：

```cpp
const std::string trimmed = trim_copy(line);
if (trimmed.empty()) {
    return {};
}
```

空行直接返回，不调用：

```cpp
metrics_tracker_->record(result);
```

原因是空行在这个项目里一直是“无操作”：

```text
用户敲空行
 -> 不打印响应
 -> 不应该污染 total_commands
```

这样 counters 更接近“真实命令数量”。

---

# 11. record 如何判断错误

`CommandMetricsTracker::record`：

```cpp
void CommandMetricsTracker::record(const CommandResult& result) {
    total_commands_.fetch_add(1);
    if (result.response.rfind("ERR ", 0) == 0) {
        error_commands_.fetch_add(1);
        return;
    }

    successful_commands_.fetch_add(1);
}
```

这里使用：

```cpp
result.response.rfind("ERR ", 0) == 0
```

意思是：

```text
response 是否以 "ERR " 开头
```

项目内命令层错误都统一是这种形式：

```text
ERR usage: GET key
ERR unknown command
ERR WAL not enabled
ERR wal append failed
ERR snapshot save failed
```

所以这种判断和现有输出风格匹配。

---

# 12. 为什么 STATS 看到的是执行前快照

`STATS` 的处理仍在 `execute_trimmed` 里。

也就是说：

```text
STATS 执行时
 -> 读取 metrics_tracker_->stats()
 -> 生成 response
 -> 返回给 execute
 -> execute 再 record 这次 STATS
```

因此 `STATS` 响应里看到的是：

```text
当前 STATS 命令之前已经完成的命令数
```

举例：

```text
PING
BADCOMMAND
STATS
```

`STATS` 输出：

```text
total_commands=2 successful_commands=1 error_commands=1
```

然后 `STATS` 本身执行成功，内部 counters 变成：

```text
total_commands=3 successful_commands=2 error_commands=1
```

这个行为在测试和 smoke 里都有验证。

---

# 13. format_command_metrics

`src/command.cpp` 新增格式化函数：

```cpp
std::string format_command_metrics(const CommandProcessorMetrics& metrics) {
    return " total_commands=" + std::to_string(metrics.total_commands) +
           " successful_commands=" + std::to_string(metrics.successful_commands) +
           " error_commands=" + std::to_string(metrics.error_commands);
}
```

注意开头带一个空格。

因为它会拼到已有 response 后面：

```cpp
response += format_command_metrics(command_metrics);
```

最终输出类似：

```text
live_keys=0 wal_enabled=no total_commands=2 successful_commands=1 error_commands=1 connection_stats_available=no
```

继续沿用 key-value 单行格式。

---

# 14. STATS 输出 command metrics

`format_stats` 新增参数：

```cpp
const CommandProcessorMetrics& command_metrics
```

然后在连接统计前追加：

```cpp
response += format_command_metrics(command_metrics);
response += format_connection_stats(stats);
```

字段顺序是：

```text
Store / WAL 状态
 -> command counters
 -> connection stats
```

这样 `STATS` 的定位更完整：

```text
当前数据状态
当前 WAL 状态
当前命令处理统计
当前连接统计
```

---

# 15. HEALTH 输出 command metrics

`format_health` 也新增同样的参数：

```cpp
const CommandProcessorMetrics& command_metrics
```

并追加：

```cpp
response += format_command_metrics(command_metrics);
response += format_connection_stats(stats);
```

典型输出：

```text
OK live_keys=0 wal_enabled=no compact_recommended=na total_commands=3 successful_commands=2 error_commands=1 connection_stats_available=yes active_connections=1 total_connections=1 peak_connections=1
```

`HEALTH` 仍然以 `OK` 开头。

它不是只返回“活着”，而是返回：

```text
活着 + 关键运行信号
```

v32 让命令错误数量也进入这个健康视图。

---

# 16. TCP server 为什么要共享 tracker

`TcpServer` 每接收一个 client，会启动一个线程：

```cpp
std::thread{serve_client, ...}.detach();
```

`serve_client` 内部会创建：

```cpp
CommandProcessor processor{store, wal, command_options};
```

如果不共享 tracker，就会变成：

```text
连接 1 的 CommandProcessor 有自己的 counters
连接 2 的 CommandProcessor 有自己的 counters
连接 3 的 CommandProcessor 有自己的 counters
```

这样 `server_metrics` 就没法知道全局命令数。

所以 v32 给 `TcpServer` 新增：

```cpp
std::shared_ptr<CommandMetricsTracker> command_metrics_tracker_;
```

构造时创建一次：

```cpp
command_metrics_tracker_(std::make_shared<CommandMetricsTracker>())
```

所有 client thread 共享它。

---

# 17. TcpServer::command_metrics

头文件新增：

```cpp
CommandProcessorMetrics command_metrics() const;
```

实现：

```cpp
CommandProcessorMetrics TcpServer::command_metrics() const {
    return command_metrics_tracker_->stats();
}
```

这和已有的：

```cpp
TcpServerConnectionStats connection_stats() const;
```

形成对应关系。

测试可以直接检查：

```cpp
auto command_metrics = server.command_metrics();
assert(command_metrics.total_commands == 4);
```

这样不必只靠日志字符串判断。

---

# 18. serve_client 如何注入 tracker

`serve_client` 参数新增：

```cpp
std::shared_ptr<CommandMetricsTracker> command_metrics_tracker
```

然后：

```cpp
CommandProcessorOptions command_options;
command_options.auto_compact_wal = auto_compact_wal;
command_options.connection_stats = [tracker] {
    const auto stats = tracker->stats();
    return CommandConnectionStats{true, stats.total_connections, stats.active_connections, stats.peak_connections};
};
command_options.metrics_tracker = std::move(command_metrics_tracker);
CommandProcessor processor{store, wal, command_options};
```

也就是说，同一个 `CommandProcessorOptions` 同时注入两种观测能力：

```text
connection_stats provider
 -> 让 STATS / HEALTH 能看到连接数

metrics_tracker
 -> 让 STATS / HEALTH 能看到命令数
```

---

# 19. server_metrics 日志新增 command counters

`src/tcp_server.cpp` 新增：

```cpp
std::string command_metrics_fields(const CommandProcessorMetrics& metrics) {
    return "total_commands=" + std::to_string(metrics.total_commands) +
           " successful_commands=" + std::to_string(metrics.successful_commands) +
           " error_commands=" + std::to_string(metrics.error_commands);
}
```

`metrics_fields` 从：

```cpp
stats_fields(stats) + " metrics_interval_ms=..."
```

扩展为：

```cpp
stats_fields(stats) + " " + command_metrics_fields(command_metrics) +
    " metrics_interval_ms=..."
```

所以 v32 的周期日志类似：

```text
event=server_metrics host=127.0.0.1 port=6379 active_connections=0 total_connections=1 peak_connections=1 total_commands=5 successful_commands=4 error_commands=1 metrics_interval_ms=50
```

现在长时间运行时可以直接从 server stdout 看：

```text
连接数量变化
命令数量变化
错误数量变化
```

---

# 20. tcp_stop 日志也带 command counters

停止日志从：

```cpp
event=tcp_stop ... active_connections=... total_connections=... peak_connections=...
```

扩展为：

```cpp
log_event(options_, "event=tcp_stop " + endpoint + " " + stats_fields(connection_tracker_->stats()) +
                        " " + command_metrics_fields(command_metrics_tracker_->stats()));
```

最终类似：

```text
event=tcp_stop host=127.0.0.1 port=6379 active_connections=0 total_connections=1 peak_connections=1 total_commands=10 successful_commands=9 error_commands=1
```

这让服务停止时也能留下一个最终累计快照。

---

# 21. v32 没有改变命令响应语义

这一版没有改变：

```text
SET / GET / DEL / EXPIRE / TTL / SIZE
SAVE / LOAD
COMPACT / WALINFO
STATS / HEALTH 的基础含义
PING / QUIT
RESP 响应格式
WAL 文件格式
Snapshot 文件格式
```

新增的是附加字段：

```text
total_commands
successful_commands
error_commands
```

以及日志中附加的 counters。

原有客户端如果只查找已有字段，不会受影响。

---

# 22. v32 核心增删改

新增：

```text
CommandProcessorMetrics
CommandMetricsTracker
CommandProcessorOptions::metrics_tracker
CommandProcessor::metrics()
CommandProcessor::execute_trimmed()
format_command_metrics()
TcpServer::command_metrics()
TcpServer::command_metrics_tracker_
server_metrics 日志中的 command counters
tcp_stop 日志中的 command counters
```

修改：

```text
CommandProcessor::execute
 -> 空行仍直接返回
 -> 非空命令转给 execute_trimmed
 -> 返回后统一 record metrics

format_stats / format_health
 -> 输出 total_commands / successful_commands / error_commands

serve_client
 -> 注入共享 CommandMetricsTracker

metrics_fields
 -> 同时输出 connection stats 和 command counters
```

保持不变：

```text
命令解析主体
Store / WAL / Snapshot 操作
连接生命周期统计
客户端输入和响应处理
```

---

# 第六十五次总结

第三十二版核心链路是：

```text
CommandProcessor::execute
 -> trim 空行
 -> execute_trimmed 执行真实命令
 -> CommandMetricsTracker::record 统计结果
 -> STATS / HEALTH 读取 metrics_tracker_->stats()

TcpServer
 -> 创建共享 CommandMetricsTracker
 -> 每个 client CommandProcessor 注入同一个 tracker
 -> server_metrics / tcp_stop 输出 command counters
```

一句话总结：

```text
第三十二版把命令执行结果纳入运行期观测体系，让 CLI、TCP STATS/HEALTH、周期 server_metrics 和 tcp_stop 都能看到累计命令数、成功数和错误数，为后续按命令类型统计和延迟统计打基础。
```
