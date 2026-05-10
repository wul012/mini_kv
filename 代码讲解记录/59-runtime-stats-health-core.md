# 第五十九次讲解：第二十九版运行期 STATS / HEALTH 核心

第五十八次讲了第二十八版 WAL maintenance options、compact stats、测试、README 和归档。
这次讲第二十九版的核心代码：

```text
include/minikv/command.hpp
src/command.cpp
src/tcp_server.cpp
```

第二十八版已经做到：

```text
WALINFO
 -> 查询 WAL 文件大小、记录数、live keys、compact 阈值和 compact counters

--auto-compact-wal
 -> 启动后和 WAL 写命令后可以自动 compact
```

第二十九版继续补运行期观测能力：

```text
STATS
 -> 返回更综合的状态行

HEALTH
 -> 返回更短的 OK 探活行

TCP server connection stats
 -> 通过命令层返回 active / total / peak 连接数
```

一句话说，第二十九版把状态观察从单一 WAL 维护信息推进到“服务运行状态可查询”。

---

# 1. 为什么 v29 要做 STATS / HEALTH

第二十八版的 `WALINFO` 很完整，但它只回答一个问题：

```text
WAL 现在是什么状态
```

运行中的服务还需要回答更多问题：

```text
当前有多少 live key
当前是否启用了 WAL
当前 WAL 是否建议 compact
当前 TCP server 有多少连接
服务是否还健康可响应
```

所以 v29 增加两个命令：

```text
STATS
 -> 面向排查和观察，信息更全

HEALTH
 -> 面向快速探活，信息更短，开头固定为 OK
```

---

# 2. CommandConnectionStats 的角色

头文件新增：

```cpp
struct CommandConnectionStats {
    bool available = false;
    std::uint64_t total_connections = 0;
    std::size_t active_connections = 0;
    std::size_t peak_connections = 0;
};
```

它描述 TCP server 的连接统计。

字段含义：

```text
available
 -> 当前 CommandProcessor 是否能拿到连接统计

total_connections
 -> server 启动以来累计接收过多少连接

active_connections
 -> 当前仍然打开的连接数

peak_connections
 -> 曾经同时在线的最高连接数
```

为什么要有 `available`：

```text
CLI 模式没有 TCP server
单元测试里也可能只构造 CommandProcessor
```

这些情况下连接统计不是 0，而是“不可用”。
所以输出时要区分：

```text
connection_stats_available=no
```

和：

```text
connection_stats_available=yes active_connections=0 total_connections=0 peak_connections=0
```

---

# 3. CommandProcessorOptions 新增 provider

头文件里的 options 从：

```cpp
struct CommandProcessorOptions {
    bool auto_compact_wal = false;
};
```

扩展为：

```cpp
struct CommandProcessorOptions {
    using ConnectionStatsProvider = std::function<CommandConnectionStats()>;

    bool auto_compact_wal = false;
    ConnectionStatsProvider connection_stats;
};
```

这里没有让 `CommandProcessor` 直接依赖 `TcpServer`。
而是让外部传入一个函数：

```text
CommandProcessor
 -> 调用 connection_stats()
 -> 得到 CommandConnectionStats
```

这样好处是：

```text
命令层不需要知道 TCP server 类型
CLI 可以不传 provider
测试可以传一个假的 provider
TCP server 可以传真实 provider
```

这是 v29 的一个关键边界：

```text
CommandProcessor 只负责格式化和返回状态
TcpServer 才负责真实连接统计
```

---

# 4. 为什么使用 std::function

`ConnectionStatsProvider` 定义为：

```cpp
using ConnectionStatsProvider = std::function<CommandConnectionStats()>;
```

原因是 provider 来源很灵活：

```text
单元测试
 -> 可以传 lambda，直接返回固定统计

TCP server
 -> 可以捕获 shared_ptr<TcpServerConnectionTracker>

CLI
 -> 不传，保持空函数
```

如果用继承接口，当前项目会变重。
如果让 `CommandProcessor` 引入 `tcp_server.hpp`，又会让命令层依赖网络层。
`std::function` 在这里刚好够用。

---

# 5. connection_stats 辅助函数

`src/command.cpp` 新增：

```cpp
CommandConnectionStats connection_stats(const CommandProcessorOptions& options) {
    if (options.connection_stats) {
        return options.connection_stats();
    }
    return {};
}
```

它的逻辑很简单：

```text
有 provider
 -> 调用 provider

没有 provider
 -> 返回默认 CommandConnectionStats
 -> available=false
```

这样 `STATS` 和 `HEALTH` 的主逻辑不用重复判断 `std::function` 是否为空。

---

# 6. format_connection_stats

新增：

```cpp
std::string format_connection_stats(const CommandConnectionStats& stats) {
    if (!stats.available) {
        return " connection_stats_available=no";
    }

    return " connection_stats_available=yes active_connections=" + std::to_string(stats.active_connections) +
           " total_connections=" + std::to_string(stats.total_connections) +
           " peak_connections=" + std::to_string(stats.peak_connections);
}
```

输出设计仍然沿用前几版的 key-value 风格。

不可用时：

```text
connection_stats_available=no
```

可用时：

```text
connection_stats_available=yes active_connections=1 total_connections=1 peak_connections=1
```

这样 CLI 和 TCP server 的输出格式统一，但语义清楚。

---

# 7. format_stats 的角色

`STATS` 使用：

```cpp
std::string format_stats(std::size_t live_keys,
                         WriteAheadLog* wal,
                         const std::optional<WalMaintenanceReport>& wal_report,
                         const CommandConnectionStats& stats)
```

它把三类信息拼成一行：

```text
Store 状态
 -> live_keys

WAL 状态
 -> wal_enabled
 -> wal_bytes / wal_records / compact_recommended
 -> compact thresholds
 -> compact counters

连接状态
 -> connection_stats_available
 -> active / total / peak
```

无 WAL 时，输出类似：

```text
live_keys=1 wal_enabled=no connection_stats_available=no
```

有 WAL 时，输出会更长：

```text
live_keys=1 wal_enabled=yes wal_bytes=40 wal_records=1 wal_live_keys=1 compact_recommended=no ...
```

注意这里用的是 `wal_live_keys`：

```text
live_keys
 -> STATS 本身看到的 live key 数

wal_live_keys
 -> WalMaintenanceReport 里的 live key 数
```

两者正常情况下相同。
单独命名是为了避免字段含义混在一起。

---

# 8. format_health 的角色

`HEALTH` 使用：

```cpp
std::string format_health(std::size_t live_keys,
                          WriteAheadLog* wal,
                          const std::optional<WalMaintenanceReport>& wal_report,
                          const CommandConnectionStats& stats)
```

它的输出更短，并且开头固定是：

```text
OK
```

典型输出：

```text
OK live_keys=1 wal_enabled=yes compact_recommended=no connection_stats_available=yes active_connections=1 total_connections=1 peak_connections=1
```

如果没有 WAL：

```text
OK live_keys=1 wal_enabled=no compact_recommended=na connection_stats_available=no
```

这里 `compact_recommended=na` 的意思是：

```text
不是不建议 compact
而是没有 WAL，所以 compact 建议不适用
```

这比直接输出 `no` 更准确。

---

# 9. STATS 命令入口

`CommandProcessor::execute` 新增：

```cpp
if (command == "STATS") {
    if (has_extra_token(input)) {
        return usage("STATS");
    }

    std::optional<WalMaintenanceReport> wal_report;
    if (wal_ != nullptr) {
        std::lock_guard lock(wal_command_mutex());
        wal_report = wal_->maintenance_report(store_);
    }

    const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
    return {format_stats(live_keys, wal_, wal_report, connection_stats(options_))};
}
```

规则是：

```text
STATS 不能带参数
有 WAL 时读取 maintenance_report
无 WAL 时直接用 Store::size()
最后附加连接统计
```

为什么有 WAL 时用 `wal_report->live_keys`：

```text
maintenance_report 已经调用过 Store::size()
它的 live_keys 和 WAL 文件状态是同一次报告里的字段
```

这样输出更一致。

---

# 10. HEALTH 命令入口

`HEALTH` 入口和 `STATS` 很像：

```cpp
if (command == "HEALTH") {
    if (has_extra_token(input)) {
        return usage("HEALTH");
    }

    std::optional<WalMaintenanceReport> wal_report;
    if (wal_ != nullptr) {
        std::lock_guard lock(wal_command_mutex());
        wal_report = wal_->maintenance_report(store_);
    }

    const std::size_t live_keys = wal_report.has_value() ? wal_report->live_keys : store_.size();
    return {format_health(live_keys, wal_, wal_report, connection_stats(options_))};
}
```

它和 `STATS` 共享同一套数据来源。

区别只在输出目标：

```text
STATS
 -> 面向观察，字段更多

HEALTH
 -> 面向探活，字段更短，开头固定 OK
```

---

# 11. HELP 文本更新

帮助文本新增：

```text
STATS
HEALTH
```

所以 CLI 启动后可以看到：

```text
COMPACT
WALINFO
STATS
HEALTH
HELP
EXIT
```

这让用户不需要看 README 也能发现新命令。

---

# 12. TCP server 如何注入连接统计

`src/tcp_server.cpp` 的 `serve_client` 里原来只设置：

```cpp
CommandProcessorOptions command_options;
command_options.auto_compact_wal = auto_compact_wal;
CommandProcessor processor{store, wal, command_options};
```

v29 扩展为：

```cpp
CommandProcessorOptions command_options;
command_options.auto_compact_wal = auto_compact_wal;
command_options.connection_stats = [tracker] {
    const auto stats = tracker->stats();
    return CommandConnectionStats{true, stats.total_connections, stats.active_connections, stats.peak_connections};
};
CommandProcessor processor{store, wal, command_options};
```

这里捕获的是：

```text
std::shared_ptr<TcpServerConnectionTracker> tracker
```

它本来就是每个 client thread 已经持有的连接追踪器。
所以不需要增加新的全局状态。

---

# 13. 为什么 provider 捕获 shared_ptr

`serve_client` 是每个连接线程运行的函数。
连接统计由 `TcpServerConnectionTracker` 管理。

捕获 `shared_ptr` 的好处是：

```text
client thread 存活期间 tracker 一定有效
provider 被 CommandProcessor 调用时不会悬空
TcpServer 本身也继续持有同一个 tracker
```

所以 `STATS` 在 TCP 连接里执行时可以看到实时值：

```text
active_connections=1
total_connections=1
peak_connections=1
```

---

# 14. 为什么 CLI 不注入连接统计

`src/main.cpp` 没有新增任何连接统计 provider。

CLI 里仍然只设置：

```cpp
minikv::CommandProcessorOptions command_options;
command_options.auto_compact_wal = options->auto_compact_wal;
minikv::CommandProcessor processor{store, wal.has_value() ? &*wal : nullptr, command_options};
```

这说明：

```text
CLI 不是 server
没有连接生命周期
也没有 active / total / peak 指标
```

所以 CLI 下 `STATS` / `HEALTH` 会返回：

```text
connection_stats_available=no
```

这不是功能缺失，而是正确表达当前运行模式。

---

# 15. WAL 锁仍然沿用 wal_command_mutex

`STATS` 和 `HEALTH` 读取 WAL report 时也使用：

```cpp
std::lock_guard lock(wal_command_mutex());
wal_report = wal_->maintenance_report(store_);
```

原因和 `WALINFO` 一样：

```text
SET / DEL / EXPIRE / COMPACT 可能正在修改 WAL
STATS / HEALTH 读取 WAL 文件大小、记录数和 counters
```

为了避免输出中间态，命令层继续用同一个 WAL 命令互斥锁。

---

# 16. 和 WALINFO 的关系

三个命令定位不同：

```text
WALINFO
 -> 只看 WAL maintenance 细节

STATS
 -> 看 Store + WAL + connection stats

HEALTH
 -> 看最短健康状态
```

也就是说：

```text
WALINFO 是维护细节命令
STATS 是状态观察命令
HEALTH 是探活命令
```

它们共享同一套底层信息，避免出现不同命令对同一状态给出不同判断。

---

# 17. v29 核心新增输出示例

CLI 下：

```text
STATS
 -> live_keys=1 wal_enabled=yes ... connection_stats_available=no

HEALTH
 -> OK live_keys=1 wal_enabled=yes compact_recommended=no connection_stats_available=no
```

TCP server 下：

```text
STATS
 -> live_keys=1 wal_enabled=yes ... connection_stats_available=yes active_connections=1 total_connections=1 peak_connections=1

HEALTH
 -> OK live_keys=1 wal_enabled=yes compact_recommended=no connection_stats_available=yes active_connections=1 total_connections=1 peak_connections=1
```

---

# 18. 第二十九版核心增删改

新增：

```text
CommandConnectionStats
CommandProcessorOptions::ConnectionStatsProvider
connection_stats()
format_connection_stats()
format_stats()
format_health()
STATS 命令
HEALTH 命令
TcpServer 向 CommandProcessor 注入连接统计 provider
```

修改：

```text
HELP
 -> 增加 STATS / HEALTH

CommandProcessorOptions
 -> 保留 auto_compact_wal
 -> 新增 connection_stats provider
```

保持不变：

```text
SET / GET / DEL / EXPIRE / TTL / SIZE 行为
WALINFO 输出
COMPACT 行为
auto compact 行为
WAL 文件格式
RESP 解析和响应格式
```

---

# 第五十九次总结

第二十九版核心链路是：

```text
TCP server 已有 TcpServerConnectionTracker
 -> serve_client 捕获 tracker
 -> 注入 CommandProcessorOptions::connection_stats
 -> CommandProcessor 执行 STATS / HEALTH
 -> 输出 live_keys、WAL 状态和 connection stats
```

一句话总结：

```text
第二十九版没有改变数据读写语义，而是给正在运行的 mini-kv 增加了可查询的健康状态和统计状态，让 CLI、TCP 和 WAL maintenance 的观察入口更完整。
```
