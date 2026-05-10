# 第六十一次讲解：第三十版周期性 Server Metrics 日志核心

第六十次讲了第二十九版的测试、README、归档和整体增删改。
这次讲第三十版的核心代码：

```text
include/minikv/tcp_server.hpp
src/tcp_server.cpp
src/server_main.cpp
```

第二十九版已经做到：

```text
STATS
 -> 客户端主动查询 Store / WAL / TCP connection stats

HEALTH
 -> 客户端主动查询简短健康状态

TcpServerConnectionTracker
 -> 维护 active / total / peak 连接数
```

第三十版继续补运行期观测能力：

```text
--metrics-interval-ms
 -> 让服务端按固定间隔主动输出 event=server_metrics
```

一句话说，v29 是“用户来问状态”，v30 是“服务端自己按周期报状态”。

---

# 1. 为什么 v30 要做周期 metrics 日志

v29 的 `STATS` / `HEALTH` 已经能查状态，但它们有一个前提：

```text
必须有客户端主动发送命令
```

实际调试服务端时，还会遇到这类问题：

```text
服务端没人连时是否还活着
连接数是否长期不降
压力测试过程中连接峰值有没有变化
不想开客户端，只想看 server stdout
```

所以 v30 增加服务端主动日志：

```text
event=server_metrics
```

它和 `STATS` / `HEALTH` 的定位不一样：

```text
STATS / HEALTH
 -> 请求响应式
 -> 客户端问，服务端答

server_metrics
 -> 周期日志式
 -> 服务端按时间自己输出
```

---

# 2. TcpServer::Options 新增 metrics_log_interval

头文件里给服务端配置增加一个字段：

```cpp
std::chrono::milliseconds metrics_log_interval{0};
```

完整位置在：

```cpp
struct Options {
    std::string host = "127.0.0.1";
    std::uint16_t port = 6379;
    int backlog = 64;
    std::chrono::milliseconds accept_poll_interval{200};
    std::chrono::milliseconds metrics_log_interval{0};
    std::size_t max_request_bytes = 64 * 1024;
    bool auto_compact_wal = false;
    StopPredicate should_stop;
    Logger logger;
};
```

这里默认值是 `0`，意思是：

```text
默认不启用周期 metrics 日志
```

为什么不默认开启？

```text
旧版本用户启动 server 不应该突然多出周期日志
测试和脚本依赖 stdout 时不应该被新日志打扰
有需要的人通过 --metrics-interval-ms 显式开启
```

所以这个字段的语义是：

```text
metrics_log_interval == 0
 -> disabled

metrics_log_interval > 0
 -> enabled, 每隔这么多毫秒输出一次 event=server_metrics
```

---

# 3. metrics_logging_enabled

`src/tcp_server.cpp` 新增了一个很小的判断函数：

```cpp
bool metrics_logging_enabled(const TcpServer::Options& options) {
    return options.metrics_log_interval > std::chrono::milliseconds::zero();
}
```

它把“是否启用”的规则收口到一个地方。

如果以后启用条件要调整，比如：

```text
interval 必须大于等于 10ms
logger 必须存在
运行模式必须是 server
```

就可以先改这个函数，而不是到处找 `metrics_log_interval > 0`。

当前逻辑很直接：

```text
0ms
 -> 不打周期日志

正数 ms
 -> 打周期日志
```

---

# 4. listener_wait_interval 的作用

v30 还有一个容易忽略的函数：

```cpp
std::chrono::milliseconds listener_wait_interval(const TcpServer::Options& options) {
    if (metrics_logging_enabled(options) && options.metrics_log_interval < options.accept_poll_interval) {
        return options.metrics_log_interval;
    }
    return options.accept_poll_interval;
}
```

它解决的是监听循环“睡多久”的问题。

原来 server 主循环是：

```text
select(listener, accept_poll_interval)
```

也就是最多睡 `accept_poll_interval`，醒来后检查：

```text
有没有新连接
有没有 stop 请求
```

v30 增加了周期 metrics 后，循环还要检查：

```text
metrics 时间到了没有
```

假设配置是：

```text
accept_poll_interval = 200ms
metrics_log_interval = 50ms
```

如果还是睡 200ms，那么 50ms 的 metrics 不可能准时输出。

所以 `listener_wait_interval` 的规则是：

```text
如果 metrics interval 更短
 -> select 最多睡 metrics interval

否则
 -> 继续睡 accept_poll_interval
```

这不是为了追求毫秒级精确，而是为了避免明显延迟。

---

# 5. 为什么不单独开 metrics 线程

v30 没有新增一个后台线程专门打 metrics。

当前实现放在 `TcpServer::run()` 的主监听循环里：

```text
run loop
 -> 检查是否到 metrics 时间
 -> 等待 listener 可读
 -> accept client
```

这样做的好处是：

```text
不增加线程生命周期管理
不用新增停止同步逻辑
不用担心 metrics 线程和 server stop 互相等待
不改变 TcpServer 的析构语义
```

这个项目目前的服务端结构是：

```text
主线程负责监听
每个 client 一个 detached thread
```

周期 metrics 只是读连接统计并写一行日志，放在监听循环里足够轻量。

---

# 6. metrics_fields 复用 stats_fields

v30 新增：

```cpp
std::string metrics_fields(const TcpServerConnectionStats& stats, std::chrono::milliseconds interval) {
    return stats_fields(stats) + " metrics_interval_ms=" + std::to_string(interval.count());
}
```

这里先复用 v29 已有的：

```cpp
std::string stats_fields(const TcpServerConnectionStats& stats) {
    return "active_connections=" + std::to_string(stats.active_connections) +
           " total_connections=" + std::to_string(stats.total_connections) +
           " peak_connections=" + std::to_string(stats.peak_connections);
}
```

也就是说，metrics 日志里的连接字段和 `tcp_stop` 等日志保持一致：

```text
active_connections=...
total_connections=...
peak_connections=...
```

然后再追加：

```text
metrics_interval_ms=...
```

为什么要把 interval 也打出来？

```text
看日志时能知道当前 server 用的周期是多少
smoke test 可以确认参数真的生效
以后多个 server 输出汇总时更容易排查配置差异
```

---

# 7. run() 启动后先记录 endpoint

`TcpServer::run()` 里原来已有：

```cpp
const auto actual_port = socket_bound_port(listener.get());
bound_port_.store(actual_port);
const std::string endpoint = endpoint_fields(options_, actual_port);
log_event(options_, "event=tcp_listen " + endpoint + " max_request_bytes=" +
                        std::to_string(options_.max_request_bytes));
```

这里的 `endpoint` 是：

```text
host=127.0.0.1 port=6379
```

如果端口配置为 `0`，系统会自动分配临时端口。
所以必须在 `bind_listener` 后用 `socket_bound_port` 拿到真实端口，再放进日志。

v30 的 metrics 日志也复用这个 `endpoint`：

```text
event=server_metrics host=... port=...
```

这样同一个 server 运行期间，listen、accepted、closed、metrics、stop 的日志都有同样的 host/port 字段。

---

# 8. 初始化 next_metrics_log

v30 在 `tcp_listen` 日志之后新增：

```cpp
const bool metrics_enabled = metrics_logging_enabled(options_);
auto next_metrics_log = std::chrono::steady_clock::now() + options_.metrics_log_interval;
```

这里有两个点。

第一，`metrics_enabled` 只算一次。

```text
Options 在 TcpServer 构造后不会动态修改
所以 run loop 里不用每轮重新判断配置含义
```

第二，使用 `std::chrono::steady_clock`。

```text
steady_clock 是单调时钟
适合做间隔计时
不会被系统时间调整影响
```

如果使用系统墙钟，用户修改系统时间、NTP 校时等都可能影响周期判断。
这里用 `steady_clock` 是正确选择。

---

# 9. 为什么第一条 metrics 不是立即输出

初始化是：

```cpp
auto next_metrics_log = std::chrono::steady_clock::now() + options_.metrics_log_interval;
```

所以第一条 metrics 会在一个完整 interval 后输出。

比如：

```text
--metrics-interval-ms 10000
```

那第一条 `event=server_metrics` 大约在 server 启动 10 秒后出现。

这样语义更自然：

```text
interval 表示间隔
不是启动时立刻打一条
```

启动时已经有：

```text
event=server_start
event=tcp_listen
```

所以没必要立刻再输出一条 metrics。

---

# 10. 主循环里的 metrics 检查

核心逻辑在 `while (!stop_requested())` 里面：

```cpp
if (metrics_enabled) {
    const auto now = std::chrono::steady_clock::now();
    if (now >= next_metrics_log) {
        log_event(options_, "event=server_metrics " + endpoint + " " +
                                metrics_fields(connection_tracker_->stats(), options_.metrics_log_interval));
        do {
            next_metrics_log += options_.metrics_log_interval;
        } while (next_metrics_log <= now);
    }
}
```

流程是：

```text
如果启用了 metrics
 -> 取当前 steady_clock 时间
 -> 如果 now 已经过了下一次日志时间
 -> 读取 connection_tracker_ 的 stats
 -> 输出 event=server_metrics
 -> 推进 next_metrics_log 到未来
```

输出示例：

```text
event=server_metrics host=127.0.0.1 port=6379 active_connections=1 total_connections=2 peak_connections=1 metrics_interval_ms=10000
```

---

# 11. do while 推进时间的原因

这里没有简单写：

```cpp
next_metrics_log = now + options_.metrics_log_interval;
```

而是写成：

```cpp
do {
    next_metrics_log += options_.metrics_log_interval;
} while (next_metrics_log <= now);
```

原因是它保留了原始周期节奏。

举例：

```text
interval = 100ms
计划时间 = 100, 200, 300, 400 ...
```

如果某一轮因为系统调度或者连接处理延迟，实际到了 350ms 才醒：

```text
now = 350ms
next_metrics_log = 100ms
```

循环会把 next 推到：

```text
100 -> 200 -> 300 -> 400
```

最终下一次计划是 400ms。

这比 `now + interval = 450ms` 更稳定，因为它不会每次延迟都把节奏往后漂移。

同时它也不会补打多条历史 metrics。
如果错过了 100、200、300，只打一条当前 metrics，然后把下一次计划推到未来。

---

# 12. metrics 读取的是 connection_tracker_

日志里调用：

```cpp
connection_tracker_->stats()
```

`connection_tracker_` 是 v11 之后就存在的连接统计对象。

它内部字段是原子变量：

```cpp
std::atomic<std::uint64_t> total_connections{0};
std::atomic_size_t active_connections{0};
std::atomic_size_t peak_connections{0};
```

所以 metrics 线程，也就是当前监听线程，可以在 client 线程并发连接和关闭时安全读取。

v30 没有新增另一份统计状态，而是复用同一个 tracker。

这能避免两类问题：

```text
STATS / HEALTH 看到一套数
server_metrics 看到另一套数
```

现在所有连接观测都来自同一个来源。

---

# 13. wait_for_listener 改用 listener_wait_interval

原来的监听等待是：

```cpp
if (!wait_for_listener(listener.get(), options_.accept_poll_interval)) {
    continue;
}
```

v30 改成：

```cpp
if (!wait_for_listener(listener.get(), listener_wait_interval(options_))) {
    continue;
}
```

也就是说：

```text
没有 metrics
 -> 行为和以前一样，用 accept_poll_interval

metrics interval 更长
 -> 仍然用 accept_poll_interval

metrics interval 更短
 -> 用 metrics interval 更频繁醒来
```

这个改动影响的是主循环醒来的频率，不影响实际 accept 行为。
一旦有客户端连接，`select` 会提前返回，不会非要等满 timeout。

---

# 14. server_main 的 usage 更新

`src/server_main.cpp` 的 usage 从：

```text
[--accept-poll-ms ms]
```

扩展为：

```text
[--accept-poll-ms ms] [--metrics-interval-ms ms]
```

对应代码：

```cpp
void print_usage(const char* program) {
    std::cerr << "Usage: " << program
              << " [port] [host] [wal_path] [--repair-wal] [--auto-compact-wal]"
                 " [--wal-compact-min-records count] [--wal-compact-record-ratio ratio]"
                 " [--wal-compact-min-bytes bytes] [--max-request-bytes bytes]"
                 " [--accept-poll-ms ms] [--metrics-interval-ms ms]\n";
}
```

这保证用户参数写错或漏值时，帮助文本能看到新参数。

---

# 15. server_main 参数解析

新增解析分支：

```cpp
if (argument == "--metrics-interval-ms") {
    if (++index >= argc) {
        print_usage(argv[0]);
        return 2;
    }
    options.metrics_log_interval = parse_positive_milliseconds(argv[index], "metrics-interval-ms");
    continue;
}
```

这段和前面的 `--accept-poll-ms` 保持同一模式：

```text
先确认后面还有值
再用 parse_positive_milliseconds 解析
写入 TcpServer::Options
然后 continue
```

如果用户只写：

```powershell
minikv_server.exe --metrics-interval-ms
```

程序会打印 usage 并返回 `2`。

如果用户写：

```powershell
minikv_server.exe --metrics-interval-ms abc
```

解析会抛异常，最后进入统一错误处理，返回失败。

---

# 16. 为什么复用 parse_positive_milliseconds

`parse_positive_milliseconds` 原来已经给 `--accept-poll-ms` 使用：

```cpp
std::chrono::milliseconds parse_positive_milliseconds(const char* text, std::string_view name) {
    const auto value = parse_positive_size(text, name);
    return std::chrono::milliseconds{static_cast<long long>(value)};
}
```

它背后调用 `parse_positive_size`，所以具备这些校验：

```text
不能为空
不能是负数
必须完整解析为数字
不能是 0
不能超过 size_t 上限
```

因此 `--metrics-interval-ms 0` 在 CLI 参数里是不合法的。

注意这里和 `Options` 默认值不矛盾：

```text
默认值 0
 -> 程序内部表示 disabled

用户显式传 0
 -> 被拒绝，因为启用参数必须是正数
```

这种设计可以避免用户以为 `0ms` 是“无限快输出”。

---

# 17. server_start 日志新增 metrics_interval_ms

v30 把启动日志从：

```text
event=server_start ... accept_poll_ms=200 auto_compact_wal=false
```

扩展为：

```text
event=server_start ... accept_poll_ms=200 metrics_interval_ms=0 auto_compact_wal=false
```

代码是：

```cpp
std::cout << "event=server_start host=" << options.host << " port=" << options.port
          << " protocol=inline,resp max_request_bytes=" << options.max_request_bytes
          << " accept_poll_ms=" << options.accept_poll_interval.count()
          << " metrics_interval_ms=" << options.metrics_log_interval.count()
          << " auto_compact_wal=" << true_false(options.auto_compact_wal) << '\n';
```

这样即使没有等到第一条周期 metrics，也能从启动日志看出：

```text
metrics 是否启用
启用周期是多少
```

默认情况下会显示：

```text
metrics_interval_ms=0
```

也就是关闭。

---

# 18. event=server_metrics 的字段设计

v30 的周期日志长这样：

```text
event=server_metrics host=127.0.0.1 port=6379 active_connections=0 total_connections=0 peak_connections=0 metrics_interval_ms=10000
```

字段含义：

```text
event=server_metrics
 -> 日志事件类型，便于 grep 和机器解析

host / port
 -> 当前 server 绑定端点

active_connections
 -> 当前仍在线连接数

total_connections
 -> server 启动以来累计接收连接数

peak_connections
 -> server 启动以来同时在线连接峰值

metrics_interval_ms
 -> 当前配置的 metrics 周期
```

它仍然沿用项目之前的结构化日志风格：

```text
key=value key=value key=value
```

没有引入 JSON，是为了保持当前日志简单、可读、易 grep。

---

# 19. v30 和 v29 的观测链路关系

到 v30 为止，运行期观测入口变成四类：

```text
WALINFO
 -> 查询 WAL maintenance 细节

STATS
 -> 查询 Store / WAL / connection stats

HEALTH
 -> 查询简短健康状态

server_metrics
 -> server 周期主动输出 connection stats
```

它们不是互相替代，而是各有用途：

```text
排查单次状态
 -> STATS

负载均衡或脚本探活
 -> HEALTH

WAL 维护判断
 -> WALINFO

长时间运行观测
 -> server_metrics
```

---

# 20. v30 没有改变命令语义

这一版没有改动：

```text
SET / GET / DEL / EXPIRE / TTL / SIZE
SAVE / LOAD
COMPACT / WALINFO
STATS / HEALTH
PING / QUIT
RESP 协议
WAL 文件格式
Snapshot 文件格式
```

新增能力只影响：

```text
server CLI 参数
server 启动日志
server 周期日志
TcpServer 测试中的日志断言
README 和归档说明
```

所以它属于观测增强，不属于存储语义变更。

---

# 21. 第三十版核心增删改

新增：

```text
TcpServer::Options::metrics_log_interval
metrics_logging_enabled()
listener_wait_interval()
metrics_fields()
--metrics-interval-ms 参数
event=server_metrics 周期日志
server_start 日志里的 metrics_interval_ms
```

修改：

```text
TcpServer::run()
 -> 主循环先检查是否到了 metrics 输出时间
 -> wait_for_listener 使用 listener_wait_interval

server_main.cpp
 -> usage 新增 --metrics-interval-ms
 -> 参数解析新增 --metrics-interval-ms
 -> event=server_start 新增 metrics_interval_ms
```

保持不变：

```text
连接 accept / client thread 生命周期
connection_tracker_ 的 active / total / peak 统计来源
STATS / HEALTH 的返回格式
默认 server 行为
```

---

# 第六十一次总结

第三十版核心链路是：

```text
用户启动 server 并传入 --metrics-interval-ms
 -> server_main 解析为 TcpServer::Options::metrics_log_interval
 -> TcpServer::run 初始化 next_metrics_log
 -> 监听循环周期检查 steady_clock
 -> 到点后读取 connection_tracker_->stats()
 -> 输出 event=server_metrics
 -> 推进下一次 metrics 时间
```

一句话总结：

```text
第三十版没有改变 mini-kv 的读写能力，而是把 v29 已经具备的连接统计从“客户端主动查询”扩展成“服务端可选周期上报”，让长期运行和压力调试时更容易直接从 server 日志观察连接状态。
```
