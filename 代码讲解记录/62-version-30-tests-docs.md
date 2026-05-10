# 第六十二次讲解：第三十版测试、README、归档和整体增删改

第六十一次讲了第三十版周期性 `server_metrics` 日志的核心代码。
这次讲第三十版配套内容：

```text
CMakeLists.txt
README.md
tests/tcp_server_tests.cpp
a/30/解释/说明.md
a/30/图片/
代码讲解记录/README.md
```

它们负责说明和证明：

```text
当前项目版本进入 0.30.0
--metrics-interval-ms 参数可用
server_start 会打印 metrics_interval_ms
TcpServer 启用 metrics 后会输出 event=server_metrics
metrics 日志包含 active / total / peak 连接数
metrics 日志包含 metrics_interval_ms
全量 CTest 通过
真实 server + client metrics smoke 通过
关键命令输出已按 a/30 归档
```

---

# 1. 第三十版的定位

第二十九版做的是：

```text
runtime STATS
runtime HEALTH
connection stats surfaced through commands
```

第三十版做的是：

```text
optional periodic server metrics logs
```

也就是从：

```text
客户端可以查询状态
```

推进到：

```text
服务端可以主动周期输出状态
```

这版不是大功能改造，而是运行观测能力的补齐。

---

# 2. CMakeLists.txt 版本号更新

第三十版把版本号改为：

```cmake
project(mini_kv VERSION 0.30.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然复用已有目标：

```text
minikv_cli
minikv_server
minikv_client
minikv_benchmark
minikv_command_tests
minikv_wal_tests
minikv_tcp_server_tests
minikv_tcp_resp_tests
minikv_tcp_resp_compat_tests
minikv_tcp_resp_concurrency_tests
minikv_client_history_tests
```

变化集中在：

```text
TcpServer
 -> 新增 metrics_log_interval
 -> run loop 新增 event=server_metrics

server_main
 -> 新增 --metrics-interval-ms

tcp_server_tests
 -> 新增周期 metrics 日志断言
```

---

# 3. README 当前版本说明

README 的当前版本从：

```text
Version 29 is a runnable in-memory KV service with runtime health and stats commands:
```

改成：

```text
Version 30 is a runnable in-memory KV service with optional periodic server metrics logs:
```

关键词是：

```text
optional periodic server metrics logs
```

这里有两个含义：

```text
optional
 -> 默认关闭，需要显式传 --metrics-interval-ms

periodic
 -> 按固定时间间隔输出，不依赖客户端主动发 STATS / HEALTH
```

---

# 4. README 能力列表新增 metrics

能力列表新增：

```text
Optional `--metrics-interval-ms` emits periodic structured `event=server_metrics` connection counters
```

这句话对应 v30 的核心能力：

```text
--metrics-interval-ms
 -> 用户入口

periodic structured
 -> 周期性结构化日志

event=server_metrics
 -> 新事件名

connection counters
 -> active / total / peak 连接数
```

README 里没有说它会输出 Store live keys 或 WAL 状态。
这是准确的，因为 v30 的周期 metrics 只输出连接统计。

---

# 5. README 新增运行示例

README 新增了一个 server 启动例子：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 10000
```

这表示：

```text
监听 127.0.0.1:6379
每 10000ms 输出一次 event=server_metrics
```

如果用户想在本机快速看效果，可以把值调小：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 1000
```

不过 README 示例使用 `10000` 更适合作为正常运行默认建议，避免日志太密。

---

# 6. README 结构化日志示例更新

结构化日志示例中，`event=server_start` 从：

```text
event=server_start host=127.0.0.1 port=6379 protocol=inline,resp max_request_bytes=65536 accept_poll_ms=200 auto_compact_wal=false
```

扩展为：

```text
event=server_start host=127.0.0.1 port=6379 protocol=inline,resp max_request_bytes=65536 accept_poll_ms=200 metrics_interval_ms=0 auto_compact_wal=false
```

新增字段是：

```text
metrics_interval_ms=0
```

意思是默认没有开启周期 metrics。

同一段示例还新增：

```text
event=server_metrics host=127.0.0.1 port=6379 active_connections=1 total_connections=1 peak_connections=1 metrics_interval_ms=10000
```

它让用户能直接看到完整日志形态。

---

# 7. README server 参数列表更新

TCP server options 从：

```text
[--accept-poll-ms ms]
```

扩展为：

```text
[--accept-poll-ms ms] [--metrics-interval-ms ms]
```

并新增说明：

```text
`--metrics-interval-ms` enables periodic `event=server_metrics` logs. It is disabled by default and reports active, total, and peak connection counters when enabled.
```

这段说明把三个关键信息写清楚：

```text
启用方式
 -> --metrics-interval-ms

默认状态
 -> disabled by default

输出内容
 -> active, total, peak connection counters
```

---

# 8. README 测试说明更新

测试段落里 `tcp_server_tests` 的说明从只检查：

```text
listen / accept / reject / close / stop log events
active / total / peak connection metrics surfaced through STATS / HEALTH
```

扩展为还检查：

```text
periodic `event=server_metrics` logs
```

这说明 v30 的新能力不是只写在 README 里，而是进入了自动化测试。

---

# 9. README Roadmap 更新

v29 之后 Roadmap 里还有：

```text
Add structured periodic server metrics logging.
```

v30 完成后，这一项被移除。

新的 Roadmap 是：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add command execution counters and error counters.
```

这反映项目阶段继续往两个方向走：

```text
客户端体验
 -> 行编辑、方向键、补全等

运行观测
 -> 命令执行次数、错误次数等更细粒度指标
```

---

# 10. tcp_server_tests 启用 metrics

`tests/tcp_server_tests.cpp` 里主测试 server 的 options 新增：

```cpp
options.metrics_log_interval = 20ms;
```

这段和已有配置放在一起：

```cpp
options.host = "127.0.0.1";
options.port = 0;
options.accept_poll_interval = 10ms;
options.metrics_log_interval = 20ms;
options.max_request_bytes = 8;
```

含义是：

```text
绑定本机临时端口
accept 轮询间隔 10ms
metrics 输出间隔 20ms
单请求上限 8 bytes
```

为什么测试里用 20ms？

```text
足够短，测试不用等太久
又不是 1ms 这种过度激进值，避免不同机器调度抖动太明显
```

---

# 11. tcp_server_tests 等待 metrics 日志

测试新增：

```cpp
bool metrics_logged = false;
for (int attempt = 0; attempt < 100; ++attempt) {
    {
        std::lock_guard lock{logs_mutex};
        metrics_logged = contains_log(logs, "event=server_metrics");
    }
    if (metrics_logged) {
        break;
    }
    std::this_thread::sleep_for(10ms);
}

assert(metrics_logged);
```

这里最多等待：

```text
100 * 10ms = 1000ms
```

测试不是立刻断言，而是轮询等待。

原因是 metrics 日志由 server 线程按时间输出，测试线程和 server 线程之间存在调度顺序。
如果立刻断言，可能 server 还没来得及输出第一条 metrics。

所以用“最多等一小段时间”的方式更稳定。

---

# 12. 为什么等 metrics 放在客户端请求前

这段等待发生在第一次 `exchange_inline` 之前。

也就是说，它验证的是：

```text
即使没有客户端连接
server 也会按周期输出 event=server_metrics
```

这是 v30 很重要的语义。

如果只在客户端连接后检查 metrics，就只能证明：

```text
有连接活动时能打日志
```

而当前测试能证明：

```text
server idle 时也能打日志
```

这更符合“周期 server metrics”的定位。

---

# 13. tcp_server_tests 最终日志断言

测试最后已有一批日志断言：

```cpp
assert(contains_log(logs, "event=tcp_listen"));
assert(contains_log(logs, "event=tcp_client_accepted"));
assert(contains_log(logs, "event=tcp_client_closed"));
assert(contains_log(logs, "event=tcp_request_rejected"));
assert(contains_log(logs, "event=tcp_stop"));
```

v30 追加：

```cpp
assert(contains_log(logs, "event=server_metrics"));
assert(contains_log(logs, "metrics_interval_ms=20"));
```

第一条确认：

```text
周期日志确实出现
```

第二条确认：

```text
日志里包含配置的周期值
```

这比只检查 `event=server_metrics` 更好，因为它能发现这种错误：

```text
日志打了，但没有带 metrics_interval_ms 字段
日志打了，但 interval 字段不对
```

---

# 14. tcp_server_tests 保留原有连接统计断言

v30 没有删除 v29 的连接统计测试。

请求仍然是：

```text
PING
STATS
HEALTH
QUIT
```

断言仍然包括：

```cpp
assert(response.find("PONG\n") != std::string::npos);
assert(response.find("connection_stats_available=yes") != std::string::npos);
assert(response.find("active_connections=1") != std::string::npos);
assert(response.find("total_connections=1") != std::string::npos);
assert(response.find("peak_connections=1") != std::string::npos);
assert(response.find("OK live_keys=0") != std::string::npos);
assert(response.find("BYE\n") != std::string::npos);
```

这说明：

```text
新增周期日志没有破坏 STATS / HEALTH
```

同一个 `TcpServerConnectionTracker` 同时服务于：

```text
STATS / HEALTH 响应
server_metrics 日志
tcp_stop 日志
```

---

# 15. a/30 归档目录

第三十版新增归档：

```text
a/30/图片/
a/30/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-tcp-server-tests.png
05-server-metrics-smoke.png
06-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

这符合项目规则：

```text
关键命令结果保存为 PNG 截图
解释文档说明每张截图对应命令、结果和意义
```

---

# 16. 01-cmake-configure.png

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
CMake 能识别 0.30.0
新增 metrics_log_interval 没有破坏工程配置
```

---

# 17. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
干净构建通过
```

意义：

```text
TcpServer::Options 新字段
server_main 参数解析
event=server_metrics 输出
tcp_server_tests 新断言
```

都可以完整编译链接。

---

# 18. 03-ctest.png

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
v30 没有破坏既有 Store / Command / WAL / Snapshot / RESP / TCP / client history 能力
```

这对观测类改动很重要。
因为观测功能应该尽量不影响核心读写语义。

---

# 19. 04-tcp-server-tests.png

对应命令：

```text
.\cmake-build-debug\minikv_tcp_server_tests.exe
```

结果：

```text
退出码 0
```

重点覆盖：

```text
metrics_log_interval = 20ms
event=server_metrics 出现
metrics_interval_ms=20 出现
STATS / HEALTH 连接统计仍正常
请求过长拒绝逻辑仍正常
tcp_stop 日志仍正常
```

---

# 20. 05-server-metrics-smoke.png

这是最能说明 v30 可用性的截图。

它真实启动：

```text
minikv_server.exe
```

并传入：

```text
--accept-poll-ms 25
--metrics-interval-ms 50
```

然后用真实：

```text
minikv_client.exe
```

发送：

```text
PING
STATS
HEALTH
QUIT
```

关键验证：

```text
client 输出包含 PONG
client 输出包含 connection_stats_available=yes
client 输出包含 OK live_keys=0
server stdout 包含 event=server_metrics
server stdout 包含 metrics_interval_ms=50
```

这证明：

```text
不是只有单元测试里的 fake logger 可用
真实 server 进程也会输出周期 metrics
```

验证结束后，本轮启动的 server 进程已经停止。

---

# 21. 06-version-docs-check.png

这张图是版本和文档一致性检查。

检查项包括：

```text
git diff --check
CMakeLists.txt 版本为 0.30.0
README 当前版本为 Version 30
README 包含 --metrics-interval-ms
tcp_server.hpp 包含 metrics_log_interval
tcp_server.cpp 包含 event=server_metrics
server_main.cpp 解析 --metrics-interval-ms
tcp_server_tests.cpp 覆盖 metrics_interval_ms=20
```

结果：

```text
全部 PASS
```

这类检查的意义是：

```text
防止代码改了但 README 没同步
防止版本号忘记升级
防止测试没有覆盖新参数
防止 diff 存在尾随空格等格式问题
```

---

# 22. a/30/解释/说明.md 的内容

说明文档记录了：

```text
本轮版本目标
每张截图对应命令
每条命令的结果
每条命令的意义
真实 smoke 的 server/client 参数和输入
本轮 server 进程已停止
临时目录 tmp-v30-run 已删除
```

这让以后回看 v30 时，不需要重新猜：

```text
这张截图跑的是什么
为什么要跑这个命令
它证明了什么
```

---

# 23. 第三十版整体增删改

第三十版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.30.0

include/minikv/tcp_server.hpp
 -> TcpServer::Options 新增 metrics_log_interval

src/tcp_server.cpp
 -> 新增 metrics_logging_enabled
 -> 新增 listener_wait_interval
 -> 新增 metrics_fields
 -> TcpServer::run 新增周期 event=server_metrics 输出
 -> wait_for_listener 改用 listener_wait_interval

src/server_main.cpp
 -> usage 新增 --metrics-interval-ms
 -> 参数解析新增 --metrics-interval-ms
 -> event=server_start 新增 metrics_interval_ms

tests/tcp_server_tests.cpp
 -> 测试启用 metrics_log_interval=20ms
 -> 等待 event=server_metrics
 -> 断言 metrics_interval_ms=20

README.md
 -> 当前版本更新为 Version 30
 -> 能力列表新增 --metrics-interval-ms
 -> 新增周期 metrics server 启动示例
 -> 结构化日志示例新增 event=server_metrics
 -> server options 新增 --metrics-interval-ms
 -> 测试说明新增 periodic event=server_metrics
 -> Roadmap 移除 periodic server metrics logging

a/30/解释/说明.md
 -> 新增 v30 运行调试归档说明

a/30/图片/
 -> 新增 v30 关键命令 PNG 截图
```

第三十版新增代码讲解记录：

```text
代码讲解记录/61-server-metrics-logging-core.md
代码讲解记录/62-version-30-tests-docs.md
```

第三十版删除：

```text
无源码文件删除
无功能删除
```

---

# 24. 第三十版之后当前能力总览

到第三十版为止，mini-kv 已经具备：

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
periodic server metrics logs
Snapshot
Benchmark
Stress tests
GitHub Actions CI
运行归档
代码讲解记录
```

运行观测链路现在是：

```text
WALINFO
 -> WAL 维护细节

STATS
 -> Store + WAL + TCP connection stats

HEALTH
 -> OK 探活 + 关键运行信号

server lifecycle logs
 -> server_start / tcp_listen / accepted / rejected / closed / tcp_stop

server_metrics logs
 -> 周期性 active / total / peak 连接统计
```

---

# 25. 第三十版之后适合继续做什么

客户端体验方向：

```text
交互式行编辑
方向键浏览历史
命令补全
```

运行观测方向：

```text
命令执行计数
命令错误计数
最近一次 compact 的 before / after 详情
compact failure counters
```

工程质量方向：

```text
格式化检查
Sanitizer CI
更细粒度 benchmark 输出
```

v30 完成后，Roadmap 里最自然的下一步是：

```text
command execution counters and error counters
```

因为它能和当前 `STATS` / `HEALTH` / `server_metrics` 组成更完整的运行观测面。

---

# 26. 第三十版清理结论

本轮运行后清理结果是：

```text
tmp-v30-run 已删除
server metrics smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/30/图片/
归档说明保留在 a/30/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第六十二次总结

第三十版配套工作证明了三件事：

```text
tcp_server_tests
 -> 周期 server metrics 日志进入自动化测试，且检查 metrics_interval_ms 字段

真实 server/client smoke
 -> --metrics-interval-ms 在真实进程里可用，server stdout 会输出 event=server_metrics

README 和 a/30 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第三十版把 mini-kv 的运行观测从“可查询”推进到“可周期上报”，并通过单元测试、全量 CTest、真实 server smoke、README 和 a/30 归档证明这条新日志链路可用。
```
