# 第六十六次讲解：第三十二版测试、README、归档和整体增删改

第六十五次讲了第三十二版命令执行计数核心代码。
这次讲第三十二版配套内容：

```text
CMakeLists.txt
README.md
tests/command_tests.cpp
tests/tcp_server_tests.cpp
a/32/解释/说明.md
a/32/图片/
代码讲解记录/README.md
```

它们负责说明和证明：

```text
当前项目版本进入 0.32.0
CommandProcessor 能统计 total / successful / error commands
STATS / HEALTH 会输出 command counters
TCP server 多连接共享 CommandMetricsTracker
server_metrics 和 tcp_stop 会输出 command counters
真实 server + client smoke 能看到 error_commands=1
代码讲解记录随开发同步补齐
```

---

# 1. 第三十二版的定位

第三十一版做的是：

```text
bundled TCP client line editing
```

第三十二版做的是：

```text
runtime command counters
```

也就是重新回到运行期观测方向。

它补的是：

```text
命令总数
成功命令数
错误命令数
```

这些字段会进入：

```text
STATS
HEALTH
server_metrics
tcp_stop
```

所以 v32 是对 v29 / v30 观测体系的继续扩展。

---

# 2. CMakeLists.txt 版本号更新

第三十二版把版本号改为：

```cmake
project(mini_kv VERSION 0.32.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然沿用 v31 的目标：

```text
minikv_cli
minikv_server
minikv_client
minikv_benchmark
minikv_store_tests
minikv_command_tests
minikv_wal_tests
minikv_snapshot_tests
minikv_stress_tests
minikv_resp_tests
minikv_tcp_server_tests
minikv_tcp_resp_tests
minikv_tcp_resp_compat_tests
minikv_tcp_resp_concurrency_tests
minikv_client_history_tests
minikv_line_editor_tests
```

变化集中在：

```text
CommandProcessor
TcpServer
command_tests
tcp_server_tests
README
归档和讲解记录
```

---

# 3. command_tests 新增 metrics 直测

`tests/command_tests.cpp` 新增一段独立测试：

```cpp
minikv::Store metrics_store;
minikv::CommandProcessor metrics_processor{metrics_store};
assert(metrics_processor.metrics().total_commands == 0);
assert(metrics_processor.metrics().successful_commands == 0);
assert(metrics_processor.metrics().error_commands == 0);
```

它先确认新建 processor 的 counters 都是 0。

然后测试空行：

```cpp
result = metrics_processor.execute("");
assert(result.response.empty());
assert(metrics_processor.metrics().total_commands == 0);
```

这证明：

```text
空行不计入 total_commands
```

---

# 4. command_tests 覆盖成功命令

测试：

```cpp
result = metrics_processor.execute("PING");
assert(result.response == "PONG");
assert(metrics_processor.metrics().total_commands == 1);
assert(metrics_processor.metrics().successful_commands == 1);
assert(metrics_processor.metrics().error_commands == 0);
```

这里验证：

```text
PING 是正常命令
total +1
successful +1
error 不变
```

这也是最基本的成功路径。

---

# 5. command_tests 覆盖错误命令

测试：

```cpp
result = metrics_processor.execute("GET name extra");
assert(result.response == "ERR usage: GET key");
assert(metrics_processor.metrics().total_commands == 2);
assert(metrics_processor.metrics().successful_commands == 1);
assert(metrics_processor.metrics().error_commands == 1);
```

这里故意发一个 usage error：

```text
GET name extra
```

它会返回：

```text
ERR usage: GET key
```

所以：

```text
total +1
error +1
successful 不变
```

---

# 6. command_tests 覆盖 STATS 输出

接着执行：

```cpp
result = metrics_processor.execute("STATS");
assert(result.response.find("total_commands=2") != std::string::npos);
assert(result.response.find("successful_commands=1") != std::string::npos);
assert(result.response.find("error_commands=1") != std::string::npos);
```

注意这里 `STATS` 响应里是：

```text
total_commands=2
```

不是 3。

原因是：

```text
STATS 生成响应时读取的是当前命令之前已经完成的 counters
STATS 返回后，才被 record 为成功命令
```

所以后面继续断言：

```cpp
assert(metrics_processor.metrics().total_commands == 3);
assert(metrics_processor.metrics().successful_commands == 2);
assert(metrics_processor.metrics().error_commands == 1);
```

这把“响应快照”和“执行后内部状态”的关系测清楚了。

---

# 7. command_tests 覆盖 HEALTH 输出

然后：

```cpp
result = metrics_processor.execute("HEALTH");
assert(result.response.find("total_commands=3") != std::string::npos);
assert(result.response.find("successful_commands=2") != std::string::npos);
assert(result.response.find("error_commands=1") != std::string::npos);
```

这说明 `HEALTH` 也带 command counters。

因此 v32 后：

```text
STATS
 -> 完整状态观察

HEALTH
 -> 探活 + counters 快照
```

两者都能看到错误命令累计。

---

# 8. tcp_server_tests 覆盖 TCP STATS

`tests/tcp_server_tests.cpp` 原来的请求是：

```text
PING
STATS
HEALTH
QUIT
```

v32 新增断言：

```cpp
assert(response.find("total_commands=1") != std::string::npos);
assert(response.find("successful_commands=1") != std::string::npos);
assert(response.find("error_commands=0") != std::string::npos);
```

这里 `STATS` 在 `PING` 之后执行。

所以它看到：

```text
total_commands=1
successful_commands=1
error_commands=0
```

这证明 TCP server 注入的共享 tracker 已经被 `CommandProcessor` 使用。

---

# 9. tcp_server_tests 覆盖 server.command_metrics

第一条连接执行完：

```text
PING
STATS
HEALTH
QUIT
```

测试新增：

```cpp
auto command_metrics = server.command_metrics();
assert(command_metrics.total_commands == 4);
assert(command_metrics.successful_commands == 4);
assert(command_metrics.error_commands == 0);
```

这说明四条命令都进入了 server 共享 tracker：

```text
PING
STATS
HEALTH
QUIT
```

并且全部成功。

随后测试一个超长请求：

```cpp
const auto too_long_response = exchange_inline("127.0.0.1", bound_port, "0123456789");
```

这个请求没有换行，超过 pending limit 后由 TCP server 拒绝。

测试确认 command metrics 仍然是：

```text
total_commands=4
successful_commands=4
error_commands=0
```

因为它没有进入 `CommandProcessor`，不是一个已解析命令。

---

# 10. tcp_server_tests 覆盖日志字段

最终日志断言新增：

```cpp
assert(contains_log(logs, "total_commands="));
assert(contains_log(logs, "error_commands="));
```

结合已有：

```cpp
assert(contains_log(logs, "event=server_metrics"));
assert(contains_log(logs, "metrics_interval_ms=20"));
```

说明：

```text
周期 server_metrics 日志里已经包含 command counters
```

同时 `tcp_stop` 也会输出 counters。

---

# 11. README 当前版本说明

README 当前版本从：

```text
Version 31 is a runnable in-memory KV service with bundled TCP client line editing:
```

改成：

```text
Version 32 is a runnable in-memory KV service with runtime command counters:
```

关键词是：

```text
runtime command counters
```

也就是本版重点是运行期统计，不是新命令或新协议。

---

# 12. README 能力列表更新

原来的：

```text
Runtime `STATS` and `HEALTH` commands expose live keys, WAL maintenance state, and server connection counters
Optional `--metrics-interval-ms` emits periodic structured `event=server_metrics` connection counters
```

改为：

```text
Runtime `STATS` and `HEALTH` commands expose live keys, WAL maintenance state, server connection counters, and command counters
Optional `--metrics-interval-ms` emits periodic structured `event=server_metrics` connection and command counters
```

并新增：

```text
Command execution counters track total, successful, and error command results
```

这三句分别说明：

```text
STATS / HEALTH 能看到 counters
server_metrics 能看到 counters
counters 具体包含 total / successful / error
```

---

# 13. README STATS / HEALTH 说明更新

说明从：

```text
live key count, WAL maintenance details, TCP connection counters
```

扩展为：

```text
live key count, command counters, WAL maintenance details, TCP connection counters
```

`HEALTH` 说明也变成：

```text
same WAL, command counter, and connection signals
```

也就是：

```text
HEALTH 不只是 OK
它也带 command counter 信号
```

---

# 14. README 结构化日志示例更新

`event=server_metrics` 示例从：

```text
event=server_metrics ... active_connections=1 total_connections=1 peak_connections=1 metrics_interval_ms=10000
```

扩展为：

```text
event=server_metrics ... active_connections=1 total_connections=1 peak_connections=1 total_commands=10 successful_commands=9 error_commands=1 metrics_interval_ms=10000
```

`event=tcp_stop` 示例也新增：

```text
total_commands=10 successful_commands=9 error_commands=1
```

这说明 counters 不只在命令响应里，也会进入 server 日志。

---

# 15. README 测试说明更新

测试段落新增：

```text
command_tests verifies command parsing, command execution counters, error counters, `STATS` / `HEALTH`, and connection-stat provider formatting.
```

`tcp_server_tests` 的说明也从：

```text
active, total, and peak connection metrics
```

扩展为：

```text
active, total, peak, command, and error counters
```

说明 v32 的两个测试层级是：

```text
command_tests
 -> 命令层 counters 语义

tcp_server_tests
 -> TCP server 共享 counters 和日志输出
```

---

# 16. README Roadmap 更新

v31 的 Roadmap 是：

```text
1. Add command execution counters and error counters.
2. Add interactive command completion for the bundled TCP client.
```

v32 完成第一项后改成：

```text
1. Add per-command breakdown and latency counters.
2. Add interactive command completion for the bundled TCP client.
```

这很自然。

现在已经有总数：

```text
total_commands
successful_commands
error_commands
```

下一步可以更细：

```text
SET 成功多少次
GET miss 多少次
每类命令耗时多少
```

---

# 17. 真实 v32 smoke

本轮真实 smoke 启动：

```text
minikv_server.exe <port> 127.0.0.1 --accept-poll-ms 25 --metrics-interval-ms 50
```

然后用真实：

```text
minikv_client.exe 127.0.0.1 <port> 5000
```

输入：

```text
PING
BADCOMMAND
STATS
HEALTH
QUIT
```

关键输出：

```text
PONG
ERR unknown command
live_keys=0 wal_enabled=no total_commands=2 successful_commands=1 error_commands=1 ...
OK live_keys=0 wal_enabled=no compact_recommended=na total_commands=3 successful_commands=2 error_commands=1 ...
BYE
```

这验证了：

```text
错误命令会让 error_commands=1
STATS 看到 PING + BADCOMMAND 两条已完成命令
HEALTH 看到 PING + BADCOMMAND + STATS 三条已完成命令
```

server log 也出现：

```text
event=server_metrics ... total_commands=5 successful_commands=4 error_commands=1 metrics_interval_ms=50
```

说明周期日志也能看到累计 counters。

---

# 18. a/32 归档目录

第三十二版新增归档：

```text
a/32/图片/
a/32/解释/说明.md
```

图片目录会包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-command-tests.png
05-tcp-server-tests.png
06-command-metrics-smoke.png
07-version-docs-check.png
08-code-explanation-check.png
```

解释目录包含：

```text
说明.md
```

继续按项目规则保存关键命令截图和解释。

---

# 19. 第三十二版整体增删改

第三十二版新增：

```text
CommandProcessorMetrics
CommandMetricsTracker
CommandProcessorOptions::metrics_tracker
CommandProcessor::metrics()
CommandProcessor::execute_trimmed()
TcpServer::command_metrics()
TcpServer::command_metrics_tracker_
```

第三十二版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.32.0

include/minikv/command.hpp
 -> 新增 command metrics 类型和 tracker

src/command.cpp
 -> execute 统一记录 metrics
 -> STATS / HEALTH 输出 total_commands / successful_commands / error_commands

include/minikv/tcp_server.hpp
 -> TcpServer 暴露 command_metrics()
 -> 保存共享 CommandMetricsTracker

src/tcp_server.cpp
 -> serve_client 注入共享 tracker
 -> server_metrics 输出 command counters
 -> tcp_stop 输出 command counters

tests/command_tests.cpp
 -> 覆盖空行不计数、成功命令、错误命令、STATS / HEALTH counters

tests/tcp_server_tests.cpp
 -> 覆盖 TCP STATS counters、server.command_metrics()、server_metrics 日志 counters

README.md
 -> 当前版本更新为 Version 32
 -> 能力列表、STATS/HEALTH 说明、日志示例、测试说明和 Roadmap 同步更新

a/32/
 -> 新增 v32 运行调试归档

代码讲解记录/
 -> 新增 65 和 66
```

第三十二版删除：

```text
无源码文件删除
无功能删除
```

---

# 20. 第三十二版之后当前能力总览

到第三十二版为止，mini-kv 已经具备：

```text
内存 key-value 存储
TTL
CLI
TCP server
TCP client
TCP client line editing
TCP client local history
TCP client persistent history
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
periodic server metrics logs
command execution counters
command error counters
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
 -> WAL maintenance 细节

STATS
 -> Store + WAL + command counters + TCP connection stats

HEALTH
 -> OK 探活 + compact signal + command counters + connection stats

server_metrics
 -> 周期性 connection stats + command counters

tcp_stop
 -> 停止时输出最终 connection stats + command counters
```

---

# 21. 第三十二版之后适合继续做什么

运行观测方向：

```text
按命令类型统计执行次数
按命令类型统计错误次数
命令耗时 / latency counters
GET miss / hit 计数
```

客户端体验方向：

```text
命令补全
历史搜索
更好的错误提示
```

工程质量方向：

```text
格式化检查
Sanitizer CI
更细粒度 benchmark 输出
```

v32 后最自然的下一步是：

```text
per-command breakdown and latency counters
```

因为当前已有总量 counters，下一步可以把它拆细。

---

# 22. 第三十二版清理结论

本轮运行结束时需要确认：

```text
tmp-v32-run 已删除
command metrics smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/32/图片/
归档说明保留在 a/32/解释/说明.md
```

`git diff --check` 应为：

```text
PASS
```

---

# 第六十六次总结

第三十二版配套工作证明了三件事：

```text
command_tests
 -> 命令层 counters 的成功、错误、空行和 STATS/HEALTH 快照语义可验证

tcp_server_tests / 真实 smoke
 -> TCP server 多连接共享 command metrics，并且 STATS、HEALTH、server_metrics 都能看到 counters

README、a/32、代码讲解记录
 -> 用户说明、版本号、测试截图、归档解释和代码讲解同步完成
```

一句话总结：

```text
第三十二版把 command counters 接入了 mini-kv 的核心运行观测链路，让命令层、TCP server、周期 metrics 日志和文档归档都能统一展示 total、successful、error 三类累计指标。
```
