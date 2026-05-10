# 第三十九次讲解：第十八版测试、README、归档和整体增删改

第三十八次讲了第十八版新增测试核心：

```text
tests/tcp_resp_concurrency_tests.cpp
```

这次讲第十八版配套内容：

```text
src/server_main.cpp
CMakeLists.txt
README.md
a/18/解释/说明.md
a/18/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.18.0
tcp_resp_concurrency_tests 进入构建
tcp_resp_concurrency_tests 纳入 CTest
真实 minikv_server 能同时服务 4 个 RESP 客户端
server_main 并发日志输出不再粘行
关键命令输出已经按 a/18 归档
```

---

# 1. 第十八版的定位

第十七版完成：

```text
RESP-over-TCP null bulk / integer / error / protocol error 覆盖
```

第十八版继续补：

```text
并发 RESP client coverage
```

它不是新增命令。

它是把协议验证从：

```text
单连接
```

推进到：

```text
多连接并发
```

---

# 2. server_main 并发日志问题

第十八版真实 smoke 时发现一个小问题：

```text
多个连接线程几乎同时输出 event=tcp_client_closed
stdout 中偶尔出现两条结构化日志粘在一行
```

这不是协议响应错误。

但是它会影响可观测性：

```text
日志行不完整
脚本解析困难
人工查看也别扭
```

并发测试把这个问题暴露出来，所以第十八版顺手修了。

---

# 3. server_main 新增 mutex include

`src/server_main.cpp` 新增：

```cpp
#include <mutex>
```

因为日志输出要加锁：

```cpp
std::mutex log_mutex;
std::lock_guard lock{log_mutex};
```

这是标准库互斥锁，不引入额外依赖。

---

# 4. logger 加锁输出

第十八版把 logger 改成：

```cpp
std::mutex log_mutex;
options.logger = [&log_mutex](const std::string& message) {
    std::lock_guard lock{log_mutex};
    std::cout << message << '\n';
};
```

含义是：

```text
任意连接线程要写一条日志
 -> 先拿 log_mutex
 -> 完整写入 message + newline
 -> 释放 log_mutex
```

这样多个线程不会把两条日志交叉写到同一行里。

---

# 5. 为什么只给 logger 加锁

连接线程里的日志主要通过：

```text
TcpServer::Options::logger
```

回调到 `server_main`。

所以给这个 logger 加锁，就能保护：

```text
event=tcp_listen
event=tcp_client_accepted
event=tcp_request_rejected
event=tcp_client_closed
event=tcp_stop
```

这些由 TcpServer 触发的结构化日志。

`server_start`、`server_hint`、`server_stopped` 是主线程启动和停止阶段输出，和连接线程重叠风险小。

---

# 6. CMakeLists.txt 版本号更新

第十八版把版本号改成：

```cmake
project(mini_kv VERSION 0.18.0 LANGUAGES CXX)
```

这表示当前构建产物对应第十八次迭代。

本版新增了测试源码，也修改了 `server_main.cpp`。

所以它既是：

```text
测试覆盖增强
```

也是：

```text
并发日志可观测性修复
```

---

# 7. CMake 新增 tcp_resp_concurrency_tests

第十八版新增：

```cmake
add_executable(minikv_tcp_resp_concurrency_tests
    tests/tcp_resp_concurrency_tests.cpp
)

target_link_libraries(minikv_tcp_resp_concurrency_tests PRIVATE minikv)

add_test(NAME tcp_resp_concurrency_tests COMMAND minikv_tcp_resp_concurrency_tests)
```

这三段分别表示：

```text
生成并发 RESP 测试可执行文件
链接 minikv 核心库
注册进 CTest
```

第十八版之后，CTest 从 10 个测试变成：

```text
11 个测试
```

新增的是：

```text
tcp_resp_concurrency_tests
```

---

# 8. README 当前版本更新

README 当前版本从：

```text
Version 17 is a runnable in-memory KV service with broader RESP-over-TCP compatibility coverage:
```

改成：

```text
Version 18 is a runnable in-memory KV service with concurrent RESP-over-TCP client coverage:
```

关键词是：

```text
concurrent RESP-over-TCP client coverage
```

也就是第十八版的核心价值：

```text
外部 RESP 客户端不只单个能跑
多个同时连也能被验证
```

---

# 9. README 能力列表新增项

第十八版 README 新增两条能力说明：

```text
Thread-safe server stdout logging for concurrent connection events
Concurrent RESP-over-TCP raw-socket client coverage with active and peak connection checks
```

第一条对应：

```text
server_main logger 加锁
```

第二条对应：

```text
tcp_resp_concurrency_tests
```

也就是本版既补了测试，也修了并发日志输出。

---

# 10. CLion 目标列表新增项

README 的 CLion 目标列表新增：

```text
minikv_tcp_resp_concurrency_tests
```

到第十八版，RESP/TCP 相关测试目标包括：

```text
minikv_resp_tests
minikv_tcp_server_tests
minikv_tcp_resp_tests
minikv_tcp_resp_compat_tests
minikv_tcp_resp_concurrency_tests
```

覆盖层次更完整：

```text
RESP parser
TCP server 生命周期
RESP 成功 pipeline
RESP 兼容边界
RESP 并发客户端
```

---

# 11. README 测试说明更新

README 的测试说明新增：

```text
tcp_resp_concurrency_tests holds multiple raw-socket RESP clients open at once,
releases them together, verifies exact per-client responses,
and checks active, total, and peak connection metrics.
```

拆开看：

```text
holds multiple clients open
 -> 先让多个客户端同时保持连接

releases them together
 -> 统一放行发送 pipeline

exact per-client responses
 -> 每个客户端校验自己的精确响应

active / total / peak
 -> 检查连接指标
```

---

# 12. README Roadmap 调整

第十七版 roadmap 里还有：

```text
Add concurrent RESP client stress coverage.
```

第十八版已经完成小规模并发客户端覆盖。

所以 Roadmap 改成：

```text
1. Add persistent client history or interactive line editing.
2. Harden WAL and snapshot recovery for partial or corrupt files.
```

含义是：

```text
协议和 TCP 覆盖已经比较厚
下一阶段可以转向客户端体验或持久化可靠性
```

---

# 13. a/18 归档目录

第十八版新增归档：

```text
a/18/图片/
a/18/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-tcp-resp-concurrency-tests.png
05-external-resp-concurrency-smoke.png
```

解释目录包含：

```text
说明.md
```

继续满足项目规则：

```text
关键命令输出保存为 PNG
每张图有解释
按版本归档
```

---

# 14. 01-cmake-configure.png

第一张图对应命令：

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
CMake 能识别 0.18.0
CMake 能识别 tests/tcp_resp_concurrency_tests.cpp
CMake 能生成 minikv_tcp_resp_concurrency_tests
```

---

# 15. 02-cmake-build.png

第二张图对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完整编译链接
38 个构建步骤成功
```

意义：

```text
新增并发测试能编译
server_main 日志加锁修复能编译
已有 CLI、server、client、benchmark、测试目标没有编译回归
```

---

# 16. 为什么构建步骤变成 38

第十七版 clean build 是 36 个构建步骤。

第十八版新增一个测试可执行文件：

```text
minikv_tcp_resp_concurrency_tests
```

它带来：

```text
编译 tests/tcp_resp_concurrency_tests.cpp
链接 minikv_tcp_resp_concurrency_tests.exe
```

所以 clean build 增加到 38 个步骤。

---

# 17. 03-ctest.png

第三张图对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

测试列表：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
resp_tests
tcp_server_tests
tcp_resp_tests
tcp_resp_compat_tests
tcp_resp_concurrency_tests
client_history_tests
```

意义：

```text
第十八版新增并发 RESP 测试通过
前面十七版积累的能力没有回归
```

---

# 18. 第十八版测试矩阵

到第十八版，测试层大致是：

```text
store_tests
 -> Store 基础读写、删除、TTL

command_tests
 -> CommandProcessor 命令解析和执行

wal_tests
 -> WAL append / replay

snapshot_tests
 -> Snapshot save / load

stress_tests
 -> Store 并发和 snapshot 一致性

resp_tests
 -> RESP parser 和响应格式

tcp_server_tests
 -> inline TCP、请求限制、生命周期日志、hostname 兼容

tcp_resp_tests
 -> raw socket RESP pipeline 成功路径

tcp_resp_compat_tests
 -> raw socket RESP null bulk、integer、error、protocol error

tcp_resp_concurrency_tests
 -> 多个 raw socket RESP 客户端同时连接和执行

client_history_tests
 -> bundled client 本地历史解析
```

---

# 19. 04-tcp-resp-concurrency-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_tcp_resp_concurrency_tests.exe
```

结果：

```text
PASS
退出码 0
```

测试成功时不输出正文，所以截图记录：

```text
执行命令
PASS 结论
退出码
```

它证明新增并发测试可以单独运行。

---

# 20. tcp_resp_concurrency_tests 证明什么

它证明：

```text
4 个 raw socket RESP 客户端可以同时连接同一个 TcpServer
服务端 active_connections 能达到 4
每个客户端独立 key 的 pipeline 响应完全正确
最终 Store 大小回到 0
最终 total_connections=4
最终 active_connections=0
最终 peak_connections=4
```

这比单连接测试更接近真实 server 使用场景。

---

# 21. 05-external-resp-concurrency-smoke.png

第五张图对应真实进程 smoke：

```text
minikv_server.exe 6419 127.0.0.1 --accept-poll-ms 50
```

Python 启动 4 个线程。

每个线程创建一个 raw socket RESP 客户端。

它们先通过 barrier 等待，然后一起发送：

```text
SET
GET
PING
TTL
DEL
GET
QUIT
```

关键结果：

```text
client 0 match True
client 1 match True
client 2 match True
client 3 match True
MATCH concurrent_clients True
```

说明 4 个外部客户端收到的原始 RESP 字节都完全符合预期。

---

# 22. smoke 服务端日志

真实 smoke 中服务端日志出现：

```text
active_connections=1 peak_connections=1
active_connections=2 peak_connections=2
active_connections=3 peak_connections=3
active_connections=4 peak_connections=4
```

这说明：

```text
4 个客户端确实同时保持连接
```

关闭日志里最终出现：

```text
active_connections=0 total_connections=4 peak_connections=4
```

说明连接全部正常关闭。

---

# 23. 日志粘行修复的验证

第十八版第一次真实 smoke 时发现：

```text
两个 event=tcp_client_closed 偶尔会粘在同一行
```

加锁修复后重新跑 smoke。

最终归档截图中可以看到：

```text
每条 event=tcp_client_closed 独立成行
```

这证明：

```text
server_main 的 stdout 日志输出在并发连接事件下可读性更稳定
```

---

# 24. 本次代码增删改总览

第十八版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.18.0
 -> 新增 minikv_tcp_resp_concurrency_tests 测试目标
 -> CTest 新增 tcp_resp_concurrency_tests

README.md
 -> 当前版本说明更新为 Version 18
 -> 能力列表新增线程安全 server stdout logging
 -> 能力列表新增 concurrent RESP-over-TCP raw-socket client coverage
 -> CLion 目标列表新增 minikv_tcp_resp_concurrency_tests
 -> 测试说明新增 tcp_resp_concurrency_tests 的并发客户端覆盖
 -> Roadmap 调整为客户端体验和 WAL / Snapshot 恢复可靠性

src/server_main.cpp
 -> include <mutex>
 -> 为 TcpServer::Options::logger 添加 log_mutex
 -> 每条 TcpServer 结构化日志加锁后完整输出
```

第十八版新增：

```text
tests/tcp_resp_concurrency_tests.cpp
a/18/图片/01-cmake-configure.png
a/18/图片/02-cmake-build.png
a/18/图片/03-ctest.png
a/18/图片/04-tcp-resp-concurrency-tests.png
a/18/图片/05-external-resp-concurrency-smoke.png
a/18/解释/说明.md
代码讲解记录/38-tcp-resp-concurrency-tests.md
代码讲解记录/39-version-18-tests-docs.md
```

第十八版删除：

```text
无源码文件删除
```

---

# 25. 第十八版之后当前能力总览

到第十八版为止，mini-kv 已经具备：

```text
内存 key-value 存储
命令解析
CLI 模式
TCP server
TCP client
TTL 过期
WAL 增量持久化
Snapshot 手动完整状态保存和加载
Benchmark 本地吞吐观察
Stress test 并发压力验证
RESP 请求解析器
TCP RESP 请求/响应
PING 探活命令
RESP parser 参数数量限制
RESP parser bulk string 长度限制
TcpServer 可请求停止
server_main 支持 Ctrl+C / SIGTERM 停止标记
结构化生命周期日志
并发连接事件下 stdout 日志按行输出
TCP connection_id
TCP active / total / peak connection metrics
TCP request byte limit
event=tcp_request_rejected 请求拒绝日志
server_main 可配置 max_request_bytes 和 accept_poll_ms
client_main 可配置 socket timeout
localhost / hostname 网络兼容性测试
client_main 可配置连接重试和重试间隔
client_main 本地会话历史 :history / !! / !N
外部客户端式 RESP-over-TCP pipeline 验证
RESP-over-TCP null bulk / integer / error / protocol error 验证
并发 RESP-over-TCP 客户端验证
CTest 自动化覆盖
运行归档
代码讲解记录
```

---

# 26. 第十八版和前几版的关系

第十六版：

```text
单个 RESP 客户端成功 pipeline
```

第十七版：

```text
单个 RESP 客户端兼容边界和错误路径
```

第十八版：

```text
多个 RESP 客户端同时连接和执行
```

这三版合起来让 RESP-over-TCP 覆盖形成了一个小闭环：

```text
能跑
 -> 边界能跑
 -> 并发也能跑
```

---

# 27. 第十八版之后适合继续做什么

第十八版之后，TCP / RESP 测试已经比较厚。

下一步自然方向是：

```text
持久客户端历史或交互式行编辑
 -> 提升 bundled client 使用体验

WAL / Snapshot 恢复加固
 -> 处理部分写入、损坏文件、校验和恢复策略

CI
 -> 每次提交自动跑 build 和 ctest

更长时间的压力测试
 -> 把当前小规模并发 smoke 扩成可选长测
```

---

# 28. 第十八版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v18-run 已删除
真实 smoke 启动的 minikv_server 已停止
检查后没有 minikv_server / minikv_client 残留进程
关键截图保留在 a/18/图片/
归档说明保留在 a/18/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第三十九次总结

第十八版配套工作证明了三件事：

```text
tcp_resp_concurrency_tests
 -> 并发 RESP-over-TCP 客户端进入自动化测试

真实 external RESP concurrency smoke
 -> minikv_server.exe 能同时服务 4 个 Python raw socket RESP 客户端

server_main 日志加锁
 -> 并发连接事件下 stdout 结构化日志不再粘行
```

一句话总结：

```text
第十八版把 RESP-over-TCP 覆盖推进到小规模并发场景，并顺手修稳真实服务端的并发日志输出，让协议响应、连接指标和可观测性都经得住 4 个外部客户端同时访问。
```
