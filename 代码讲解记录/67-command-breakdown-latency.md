# 第六十七次讲解：第三十三版命令分项与延迟指标核心

第六十五次讲了第三十二版的命令总计数：

```text
total_commands
successful_commands
error_commands
```

第三十三版继续把这套运行时观测能力往前推一步：

```text
按命令类型拆分统计
命令执行耗时统计
STATS / HEALTH / server_metrics / tcp_stop 统一输出新指标
```

涉及的核心文件是：

```text
include/minikv/command.hpp
src/command.cpp
src/tcp_server.cpp
```

一句话总结：

```text
v33 让 mini-kv 不只知道“处理了多少命令”，还知道“哪些命令多、哪些命令错、命令总体耗时和单类命令耗时怎样”。
```

---

# 1. CommandBreakdownMetrics 的角色

第三十三版新增结构：

```cpp
struct CommandBreakdownMetrics {
    std::string command;
    std::uint64_t total_commands = 0;
    std::uint64_t successful_commands = 0;
    std::uint64_t error_commands = 0;
    std::uint64_t total_latency_ns = 0;
    std::uint64_t max_latency_ns = 0;
};
```

它表示一个命令桶的统计。

例如：

```text
PING
 -> total_commands = 20
 -> successful_commands = 20
 -> error_commands = 0
 -> total_latency_ns = 所有 PING 执行耗时之和
 -> max_latency_ns = 单次 PING 最大耗时
```

再比如：

```text
GET
 -> 成功 GET 和 usage error GET 都会归到 GET 桶
```

这样做的好处是：

```text
命令写错和参数写错不会被混成一个大 error 数字
可以看出错误集中在哪类命令
可以看出慢命令集中在哪类命令
```

---

# 2. CommandProcessorMetrics 的扩展

第三十二版的 `CommandProcessorMetrics` 只有三个总计数。

第三十三版扩展为：

```cpp
struct CommandProcessorMetrics {
    std::uint64_t total_commands = 0;
    std::uint64_t successful_commands = 0;
    std::uint64_t error_commands = 0;
    std::uint64_t total_latency_ns = 0;
    std::uint64_t max_latency_ns = 0;
    std::vector<CommandBreakdownMetrics> command_breakdown;
};
```

新增字段含义：

```text
total_latency_ns
 -> 所有已记录命令耗时之和，单位纳秒

max_latency_ns
 -> 所有已记录命令里单次最大耗时

command_breakdown
 -> 每个命令桶的统计列表
```

平均耗时没有作为字段保存，而是在格式化输出时计算：

```text
avg_latency_ns = total_latency_ns / total_commands
```

原因很简单：

```text
平均值是派生数据
保存总耗时和总次数更稳定
每次输出时再计算即可
```

---

# 3. 为什么 tracker 从 atomic 改成 mutex

v32 的 tracker 只需要三个数字，所以可以直接用：

```cpp
std::atomic<std::uint64_t>
```

v33 增加了：

```text
总计数
总耗时
最大耗时
按命令名分组的 map
```

这时如果仍然每个字段都用 atomic，会出现一个问题：

```text
读取 snapshot 时，total_commands 可能已经更新
但对应 command_breakdown 还没更新
最终看到的快照可能前后不一致
```

所以 v33 改成：

```cpp
mutable std::mutex mutex_;
CommandProcessorMetrics totals_;
std::map<std::string, CommandBreakdownMetrics> breakdown_;
```

写入时统一加锁：

```text
更新总计数
更新总耗时
更新最大耗时
更新对应命令桶
```

读取时也统一加锁：

```text
复制 totals_
把 map 里的命令桶按 key 顺序拷贝进 vector
返回一致快照
```

这牺牲了一点极限性能，但换来更容易理解、更可靠的指标快照。对 mini-kv 当前阶段来说，这是更合适的取舍。

---

# 4. command_token：先识别命令名

`CommandProcessor::execute` 现在在真正执行命令前，会先提取命令名：

```cpp
const std::string command = command_token(trimmed);
```

`command_token` 做的事情很小：

```text
从 trimmed 输入里读取第一个 token
转成大写
返回给 metrics tracker
```

例子：

```text
"ping"
 -> PING

"GET key extra"
 -> GET

"BADCOMMAND"
 -> BADCOMMAND
```

注意，它只负责拿到第一个词，不负责判断命令是否正确执行。

---

# 5. metrics_command_name：控制命令桶数量

如果用户输入：

```text
BAD1
BAD2
BAD3
```

如果每个未知命令都创建一个桶，指标会越来越碎。

所以 v33 增加了 `metrics_command_name`：

```text
已知命令
 -> 使用命令名本身，例如 PING / GET / SET

未知命令
 -> 统一归到 UNKNOWN
```

这就是为什么 smoke 里执行：

```text
BADCOMMAND
```

最终会进入：

```text
UNKNOWN:1/0/1/...
```

这样既能知道有未知命令错误，又不会让任意输入污染指标维度。

---

# 6. execute 如何记录耗时

第三十三版的 `CommandProcessor::execute` 流程变成：

```text
trim 输入
如果是空输入，直接返回，不计数
提取 command token
记录 started_at
调用 execute_trimmed 执行原有命令逻辑
计算 elapsed_ns
调用 metrics_tracker_->record(command, result, elapsed_ns)
返回 result
```

关键点是：

```text
计时包住 execute_trimmed
```

也就是说，命令解析、Store 操作、WAL 操作、Snapshot 操作都会被计入耗时。

这比只在网络层计时更准确，因为网络收发时间不属于命令处理器自身。

---

# 7. record 的更新规则

`CommandMetricsTracker::record` 现在接收：

```cpp
std::string_view command
const CommandResult& result
std::uint64_t elapsed_ns
```

更新规则：

```text
elapsed_ns 为 0 时修正为 1
total_commands +1
total_latency_ns += elapsed_ns
max_latency_ns = max(max_latency_ns, elapsed_ns)
```

然后找到对应命令桶：

```text
PING / GET / SET / ...
UNKNOWN
```

对桶做同样的更新：

```text
bucket.total_commands +1
bucket.total_latency_ns += elapsed_ns
bucket.max_latency_ns = max(bucket.max_latency_ns, elapsed_ns)
```

最后按响应判断成功或失败：

```text
response 以 "ERR " 开头
 -> error_commands +1

其他响应
 -> successful_commands +1
```

---

# 8. STATS / HEALTH 的输出格式

v33 把命令指标统一格式化为：

```text
total_commands=...
successful_commands=...
error_commands=...
total_latency_ns=...
avg_latency_ns=...
max_latency_ns=...
command_breakdown=...
```

其中 `command_breakdown` 的格式是：

```text
COMMAND:total/success/error/total_latency_ns/avg_latency_ns/max_latency_ns
```

多个命令用分号隔开：

```text
GET:1/0/1/12000/12000/12000;PING:1/1/0/8000/8000/8000;UNKNOWN:1/0/1/6000/6000/6000
```

如果还没有任何命令被记录：

```text
command_breakdown=none
```

---

# 9. TCP server 如何复用同一套格式

v32 时 TCP server 自己拼接了：

```text
total_commands
successful_commands
error_commands
```

v33 后，`src/tcp_server.cpp` 里的 `command_metrics_fields` 改为：

```cpp
return format_command_metrics(metrics);
```

也就是说：

```text
STATS / HEALTH
 -> format_command_metrics

server_metrics / tcp_stop
 -> format_command_metrics
```

两条输出链路共享同一种格式。

这样以后如果继续给命令指标加字段，不需要分别维护两套字符串拼接逻辑。

---

# 10. 一句话总结

第三十三版把 command metrics 从“总量计数器”升级成“可定位问题的命令观测面”：

```text
总命令数
成功命令数
错误命令数
总耗时
平均耗时
最大耗时
每类命令的 total / success / error / latency
未知命令统一归桶 UNKNOWN
```

从现在开始，看到错误数升高时，可以进一步看：

```text
是 GET 参数错？
是 UNKNOWN 命令多？
还是某个命令耗时异常？
```

这就是 v33 对运行调试能力的主要提升。
