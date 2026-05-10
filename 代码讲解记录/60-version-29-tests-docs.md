# 第六十次讲解：第二十九版测试、README、归档和整体增删改

第五十九次讲了第二十九版运行期 `STATS` / `HEALTH` 的核心代码。
这次讲第二十九版配套内容：

```text
CMakeLists.txt
README.md
tests/command_tests.cpp
tests/wal_tests.cpp
tests/tcp_server_tests.cpp
a/29/解释/说明.md
a/29/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.29.0
STATS 命令可用
HEALTH 命令可用
CLI 模式可以输出 WAL 状态但没有 TCP 连接统计
TCP server 模式可以输出真实连接统计
全量 CTest 通过
真实 CLI smoke 通过
真实 server + client smoke 通过
关键命令输出已经按 a/29 归档
```

---

# 1. 第二十九版的定位

第二十六版做的是：

```text
WALINFO
compact hint
```

第二十七版做的是：

```text
--auto-compact-wal
```

第二十八版做的是：

```text
configurable compact thresholds
compact counters
```

第二十九版做的是：

```text
runtime STATS
runtime HEALTH
connection stats surfaced through commands
```

也就是从：

```text
WAL maintenance 可观察
```

推进到：

```text
服务运行状态也可查询
```

---

# 2. CMakeLists.txt 版本号更新

第二十九版把版本号改为：

```cmake
project(mini_kv VERSION 0.29.0 LANGUAGES CXX)
```

本版没有新增 executable target。
仍然复用已有目标：

```text
minikv_cli
minikv_server
minikv_client
minikv_command_tests
minikv_wal_tests
minikv_tcp_server_tests
```

变化主要在：

```text
minikv 库
 -> CommandProcessor 新增 STATS / HEALTH
 -> TcpServer 给每个连接注入 connection stats provider

测试
 -> command_tests / wal_tests / tcp_server_tests 增加覆盖
```

---

# 3. README 当前版本更新

README 当前版本从：

```text
Version 28 is a runnable in-memory KV service with configurable WAL compaction maintenance:
```

改成：

```text
Version 29 is a runnable in-memory KV service with runtime health and stats commands:
```

关键词是：

```text
runtime health and stats commands
```

这说明本版重点不是存储格式，也不是 WAL 压缩策略，而是运行期状态查询。

---

# 4. README 能力列表新增 STATS / HEALTH

能力列表新增：

```text
Runtime `STATS` and `HEALTH` commands expose live keys, WAL maintenance state, and server connection counters
```

这句话对应三类输出：

```text
live keys
 -> Store 当前有效 key 数

WAL maintenance state
 -> wal_enabled / wal_records / compact_recommended / counters

server connection counters
 -> active_connections / total_connections / peak_connections
```

---

# 5. README 命令列表更新

CLI commands 新增：

```text
STATS
HEALTH
```

位置在：

```text
COMPACT
WALINFO
STATS
HEALTH
HELP
EXIT
```

这样用户可以把三个观察命令连起来理解：

```text
WALINFO
 -> WAL 维护细节

STATS
 -> 运行期统计

HEALTH
 -> 快速探活
```

---

# 6. README TCP 示例更新

TCP protocol 示例也加入：

```text
STATS
HEALTH
```

原因是 TCP server 也走同一个：

```text
CommandProcessor
```

更重要的是，TCP server 模式下才有真实连接统计。
所以 `STATS` / `HEALTH` 在 TCP 下会比 CLI 多出：

```text
connection_stats_available=yes
active_connections=...
total_connections=...
peak_connections=...
```

---

# 7. README 新增命令说明段落

README 新增说明：

```text
STATS returns a compact key-value status line ...
HEALTH returns an OK ... line ...
```

这里明确了两个命令的差异：

```text
STATS
 -> 字段更全，适合排查

HEALTH
 -> 开头固定 OK，适合探活
```

---

# 8. README WAL persistence 段落更新

WAL persistence 段落把原来的：

```text
use `WALINFO` at runtime to inspect the same maintenance signal
```

扩展成：

```text
use `WALINFO`, `STATS`, or `HEALTH` at runtime to inspect the same maintenance signal
```

原因是 v29 中 `STATS` / `HEALTH` 也会读取：

```text
WalMaintenanceReport
```

所以它们同样能看到：

```text
compact_recommended
compact thresholds
compact counters
```

---

# 9. command_tests 覆盖 usage

`tests/command_tests.cpp` 新增：

```cpp
result = processor.execute("STATS extra");
assert(result.response == "ERR usage: STATS");
```

以及：

```cpp
result = processor.execute("HEALTH extra");
assert(result.response == "ERR usage: HEALTH");
```

这证明：

```text
STATS 不接收参数
HEALTH 不接收参数
```

它们和 `SIZE`、`COMPACT`、`WALINFO` 一样，都是单独命令。

---

# 10. command_tests 覆盖无 WAL 模式

普通 `CommandProcessor processor{store};` 没有 WAL，也没有 TCP provider。

测试新增断言：

```cpp
result = processor.execute("STATS");
assert(result.response.find("live_keys=1") != std::string::npos);
assert(result.response.find("wal_enabled=no") != std::string::npos);
assert(result.response.find("connection_stats_available=no") != std::string::npos);
```

这说明：

```text
无 WAL 时 STATS 仍然可用
但会明确输出 wal_enabled=no
```

`HEALTH` 同理：

```cpp
assert(result.response.find("OK live_keys=1") != std::string::npos);
assert(result.response.find("wal_enabled=no") != std::string::npos);
assert(result.response.find("compact_recommended=na") != std::string::npos);
```

这里的 `na` 很关键：

```text
没有 WAL
 -> compact 建议不适用
 -> compact_recommended=na
```

---

# 11. command_tests 覆盖 fake provider

测试构造一个假的 provider：

```cpp
stats_options.connection_stats = [] {
    minikv::CommandConnectionStats stats;
    stats.available = true;
    stats.total_connections = 7;
    stats.active_connections = 2;
    stats.peak_connections = 3;
    return stats;
};
```

再构造：

```cpp
minikv::CommandProcessor stats_processor{store, nullptr, stats_options};
```

然后断言：

```text
connection_stats_available=yes
active_connections=2
total_connections=7
peak_connections=3
```

这证明命令层不依赖真实 TCP server，也能通过 provider 机制输出连接统计。

---

# 12. command_tests 覆盖 HELP

测试新增：

```cpp
assert(result.response.find("STATS") != std::string::npos);
assert(result.response.find("HEALTH") != std::string::npos);
```

这保证 CLI 启动帮助文本和新增命令同步。

---

# 13. wal_tests 覆盖 WAL 模式下 STATS

`tests/wal_tests.cpp` 在手动 COMPACT 后新增：

```cpp
result = processor.execute("STATS");
assert(result.response.find("live_keys=1") != std::string::npos);
assert(result.response.find("wal_enabled=yes") != std::string::npos);
assert(result.response.find("wal_records=1") != std::string::npos);
assert(result.response.find("compact_recommended=no") != std::string::npos);
assert(result.response.find("manual_compactions=1") != std::string::npos);
assert(result.response.find("connection_stats_available=no") != std::string::npos);
```

这证明 `STATS` 能把 v28 的 WAL maintenance 信息整合进运行期状态。

---

# 14. wal_tests 覆盖 WAL 模式下 HEALTH

同一段测试继续断言：

```cpp
result = processor.execute("HEALTH");
assert(result.response.find("OK live_keys=1") != std::string::npos);
assert(result.response.find("wal_enabled=yes") != std::string::npos);
assert(result.response.find("compact_recommended=no") != std::string::npos);
```

这说明：

```text
HEALTH 不是只返回简单 PONG
它会带上关键运行信号
```

但它仍然比 `STATS` 更短。

---

# 15. tcp_server_tests 覆盖真实连接统计

`tests/tcp_server_tests.cpp` 把原来的请求：

```text
PING
QUIT
```

扩展为：

```text
PING
STATS
HEALTH
QUIT
```

新增断言：

```cpp
assert(response.find("connection_stats_available=yes") != std::string::npos);
assert(response.find("active_connections=1") != std::string::npos);
assert(response.find("total_connections=1") != std::string::npos);
assert(response.find("peak_connections=1") != std::string::npos);
assert(response.find("OK live_keys=0") != std::string::npos);
```

这证明：

```text
TCP server 连接内执行 STATS / HEALTH
 -> 能看到当前连接本身
 -> active_connections=1
```

这比单纯检查 server 日志更进一步：

```text
连接统计不仅在日志里
也通过命令返回给客户端
```

---

# 16. 真实 CLI smoke

归档里的 `06-cli-stats-health-smoke.png` 对应真实运行：

```text
minikv_cli.exe <wal_path> --auto-compact-wal --wal-compact-min-records 4 --wal-compact-record-ratio 2 --wal-compact-min-bytes 1048576
```

输入流程：

```text
SET observed one
STATS
HEALTH
WALINFO
QUIT
```

关键输出：

```text
OK inserted
live_keys=1 wal_enabled=yes ... connection_stats_available=no
OK live_keys=1 wal_enabled=yes compact_recommended=no connection_stats_available=no
```

意义是：

```text
真实 CLI 可执行新命令
WAL 信息正常
CLI 没有连接统计，正确返回 no
```

---

# 17. 真实 server smoke

归档里的 `07-server-stats-health-smoke.png` 对应真实启动：

```text
minikv_server.exe <port> 127.0.0.1 <wal_path> --auto-compact-wal ...
```

再用真实客户端连接：

```text
minikv_client.exe 127.0.0.1 <port> ...
```

输入：

```text
SET srv value
STATS
HEALTH
QUIT
```

关键输出：

```text
connection_stats_available=yes
active_connections=1
total_connections=1
peak_connections=1
OK live_keys=1
```

意义是：

```text
真实 TCP server 连接统计可以通过命令返回给真实 client
```

验证结束后本轮启动的 server 进程已停止。

---

# 18. a/29 归档目录

第二十九版新增归档：

```text
a/29/图片/
a/29/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-command-tests.png
05-wal-tests.png
06-cli-stats-health-smoke.png
07-server-stats-health-smoke.png
08-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 19. 01-cmake-configure.png

对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
配置成功
构建文件写入 cmake-build-debug
```

意义：

```text
CMake 能识别 0.29.0
新增头文件依赖没有破坏配置
```

---

# 20. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
清理后完整构建通过
```

意义：

```text
CommandConnectionStats
ConnectionStatsProvider
STATS / HEALTH
TCP provider 注入
新增测试
```

都能从干净状态编译链接。

---

# 21. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

意义：

```text
v29 没有破坏既有功能
```

---

# 22. 04-command-tests.png

对应命令：

```text
.\cmake-build-debug\minikv_command_tests.exe
```

结果：

```text
退出码 0
```

重点覆盖：

```text
STATS usage
HEALTH usage
无 WAL 输出
无 connection provider 输出
fake provider 输出
HELP 包含新命令
```

---

# 23. 05-wal-tests.png

对应命令：

```text
.\cmake-build-debug\minikv_wal_tests.exe
```

结果：

```text
退出码 0
```

重点覆盖：

```text
WAL 模式下 STATS 读取 maintenance report
WAL 模式下 HEALTH 返回 compact_recommended
v28 compact thresholds 和 counters 仍然通过
```

---

# 24. 08-version-docs-check.png

检查项包括：

```text
git diff --check
CMakeLists.txt 版本为 0.29.0
README Version 29
README 包含 STATS / HEALTH
command.hpp 包含 ConnectionStatsProvider
command.cpp 包含 STATS / HEALTH
tcp_server.cpp 注入 connection_stats provider
command_tests / tcp_server_tests 覆盖新命令
```

结果：

```text
全部 PASS
```

---

# 25. 第二十九版整体增删改

第二十九版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.29.0

README.md
 -> 当前版本更新为 Version 29
 -> 能力列表新增 STATS / HEALTH
 -> CLI 命令列表新增 STATS / HEALTH
 -> TCP 示例新增 STATS / HEALTH
 -> WAL persistence 说明 STATS / HEALTH 也能查看 maintenance signal
 -> 测试说明新增 STATS / HEALTH 覆盖
 -> Roadmap 移除 runtime health and metrics commands，推进到 periodic server metrics logging

include/minikv/command.hpp
 -> 新增 CommandConnectionStats
 -> CommandProcessorOptions 新增 ConnectionStatsProvider

src/command.cpp
 -> 新增 connection_stats
 -> 新增 format_connection_stats
 -> 新增 format_stats
 -> 新增 format_health
 -> 新增 STATS 命令
 -> 新增 HEALTH 命令
 -> HELP 新增 STATS / HEALTH

src/tcp_server.cpp
 -> serve_client 为 CommandProcessor 注入 connection_stats provider

tests/command_tests.cpp
 -> 覆盖 STATS / HEALTH usage
 -> 覆盖无 WAL / 无 provider 输出
 -> 覆盖 fake provider 连接统计输出
 -> 覆盖 HELP

tests/wal_tests.cpp
 -> 覆盖 WAL 模式下 STATS / HEALTH

tests/tcp_server_tests.cpp
 -> 覆盖真实 TCP 连接内 STATS / HEALTH 返回连接统计
```

第二十九版新增：

```text
a/29/图片/01-cmake-configure.png
a/29/图片/02-cmake-build.png
a/29/图片/03-ctest.png
a/29/图片/04-command-tests.png
a/29/图片/05-wal-tests.png
a/29/图片/06-cli-stats-health-smoke.png
a/29/图片/07-server-stats-health-smoke.png
a/29/图片/08-version-docs-check.png
a/29/解释/说明.md
代码讲解记录/59-runtime-stats-health-core.md
代码讲解记录/60-version-29-tests-docs.md
```

第二十九版删除：

```text
无源码文件删除
```

---

# 26. 第二十九版之后当前能力总览

到第二十九版为止，mini-kv 已经具备：

```text
内存 key-value 存储
TTL
CLI
TCP server
TCP client
RESP-over-TCP
WAL append / replay
WAL2 checksum
WAL repair
manual COMPACT
WALINFO
startup WAL stats
auto compact
configurable compact thresholds
compact counters
STATS
HEALTH
TCP connection stats through commands
Snapshot
Benchmark
Stress tests
GitHub Actions CI
运行归档
代码讲解记录
```

运行期观测链路现在是：

```text
WALINFO
 -> WAL 维护细节

STATS
 -> Store + WAL + TCP connection stats

HEALTH
 -> OK 探活 + 关键运行信号

server logs
 -> 结构化生命周期事件
```

---

# 27. 第二十九版之后适合继续做什么

运行期观测方向：

```text
周期性 server metrics 日志
最近一次 compact 的 before / after 详情
compact failure counters
命令执行计数
错误计数
```

客户端体验方向：

```text
交互式行编辑
方向键历史浏览
命令补全
```

工程质量方向：

```text
格式化检查
Sanitizer CI
更细粒度 benchmark 输出
```

---

# 28. 第二十九版清理结论

本轮运行后清理结果是：

```text
tmp-v29-run 已删除
server smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/29/图片/
归档说明保留在 a/29/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第六十次总结

第二十九版配套工作证明了三件事：

```text
command_tests / wal_tests
 -> STATS / HEALTH 在无 WAL、有 WAL、fake provider 三类场景下都有覆盖

tcp_server_tests / 真实 server smoke
 -> TCP server 能把 active / total / peak 连接统计通过命令返回给客户端

README 和 a/29 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十九版把 mini-kv 的运行期观测从“WAL 可查”扩展到“服务状态和连接状态可查”，并用单元测试、全量 CTest、真实 CLI smoke、真实 server smoke 和 a/29 归档证明新能力可用。
```
