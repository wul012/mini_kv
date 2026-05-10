# 第二十七次讲解：第十二版测试、README、运行归档和增删改

第二十六次讲了第十二版核心代码：

```text
TcpServer::Options::max_request_bytes
serve_client 请求缓冲上限检查
event=tcp_request_rejected
server_main 新增 --max-request-bytes / --accept-poll-ms
client_main 新增 timeout_ms
```

这次讲第十二版配套内容：

```text
tests/tcp_server_tests.cpp
CMakeLists.txt
README.md
a/12/解释/说明.md
a/12/图片/
```

它们负责证明：

```text
正常 TCP 请求仍然能跑通
超限请求会被拒绝
拒绝日志包含 pending_bytes 和 max_request_bytes
客户端 timeout 参数在真实运行中能生效
第十二版构建、测试、运行验证都有归档
```

---

# 1. 第十二版为什么继续改 tcp_server_tests

第十一版的 `tcp_server_tests` 已经验证：

```text
server 能启动
request_stop 能让 run() 退出
真实 TCP PING / QUIT 能跑通
connection_id 能分配
active / total / peak 指标能变化
accept / close / stop 日志能记录
```

第十二版新增请求上限后，测试目标变成：

```text
正常请求不能被误伤
过大的未完成请求必须被拒绝
拒绝日志必须可观察
连接统计不能被破坏
```

所以测试不是另起炉灶，而是在第十一版真实 TCP 测试基础上继续加场景。

---

# 2. 测试里把 max_request_bytes 调小

测试中设置：

```cpp
options.max_request_bytes = 8;
```

为什么设成 8？

因为测试要构造一个很小的超限请求：

```text
0123456789
```

这串内容长度是 10 字节。

所以测试条件很清楚：

```text
max_request_bytes = 8
pending_bytes = 10
10 > 8
 -> 应该拒绝
```

如果测试仍然用默认 65536，就要构造很长的数据，不利于阅读，也不利于截图归档。

---

# 3. 正常 PING / QUIT 仍然先跑一遍

第十二版测试没有一上来只测错误路径，而是先执行：

```text
PING
QUIT
```

并验证响应包含：

```text
PONG
BYE
```

这一步的意义是：

```text
证明 max_request_bytes=8 不会影响正常短命令
```

`PING\nQUIT\n` 是两条完整 inline 命令。
服务端能及时解析并消费它们，所以不会因为总输入超过 8 字节就误拒绝。

这也验证了第十二版的检查位置是合理的：

```text
先尽量处理完整命令
再检查剩余 pending 是否超限
```

---

# 4. 超限 inline 请求测试

测试发送：

```cpp
exchange_inline(bound_port, "0123456789");
```

注意这里没有换行。

它模拟的是：

```text
客户端发来 10 字节 inline 内容
但是一直没有形成完整的一行命令
```

服务端收到后：

```text
pending = "0123456789"
pending.size() = 10
max_request_bytes = 8
```

所以触发：

```text
event=tcp_request_rejected
ERR line too long
```

测试断言：

```cpp
assert(too_long_response.find("ERR line too long\n") != std::string::npos);
```

这证明 inline 协议的超限返回没有变成 RESP 格式。

---

# 5. 等待拒绝日志出现

网络测试有线程调度。
客户端收到响应时，服务端线程可能还没来得及把日志完全写入测试数组。

所以测试使用等待逻辑：

```text
等待 event=tcp_request_rejected 出现
```

这和第十一版等待连接关闭日志的做法一致。

测试要避免这种不稳定：

```text
日志马上查不到
但过几毫秒就写入了
```

所以第十二版继续采用：

```text
带截止时间的轮询等待
```

这样测试更稳。

---

# 6. 第十二版新增日志断言

测试最后检查日志包含：

```text
event=tcp_request_rejected
connection_id=2
pending_bytes=10
max_request_bytes=8
total_connections=2
peak_connections=1
active_connections=0
```

这些字段分别证明：

```text
event=tcp_request_rejected
 -> 确实走了请求拒绝路径

connection_id=2
 -> 第二个 TCP 连接触发拒绝

pending_bytes=10
 -> 拒绝时缓冲区有 10 字节

max_request_bytes=8
 -> 使用的是测试设置的上限，不是硬编码默认值

total_connections=2
 -> PING/QUIT 一个连接，超限请求一个连接

peak_connections=1
 -> 两个连接不是并发重叠

active_connections=0
 -> 测试结束时连接都正确关闭
```

这样第十二版不只测了“返回错误”，还测了：

```text
配置生效
日志可观测
连接指标仍然正确
```

---

# 7. CMakeLists.txt 版本号更新

第十二版把项目版本更新为：

```cmake
project(mini_kv VERSION 0.12.0 LANGUAGES CXX)
```

版本号的意义是：

```text
第十二次开发迭代
 -> 0.12.0
```

本次没有新增测试目标，也没有新增库文件。

CMake 改动主要用于标记：

```text
当前构建产物对应第十二版功能
```

---

# 8. README 当前版本说明

第十二版 README 开头更新为：

```text
Version 12 is a runnable in-memory KV service with configurable TCP request limits and client socket timeout tuning
```

这句话把当前重点放在：

```text
configurable TCP request limits
client socket timeout tuning
```

也就是：

```text
服务端请求上限可配置
客户端 socket timeout 可调
```

README 的能力列表也新增：

```text
Configurable TCP request byte limit and optional client socket timeout
```

---

# 9. README 新增服务端启动示例

第十二版新增显式限制示例：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --max-request-bytes 65536 --accept-poll-ms 200
```

这条命令展示：

```text
端口是 6379
host 是 127.0.0.1
请求缓冲上限是 65536 字节
accept 轮询间隔是 200ms
```

它的价值是：

```text
用户不用翻源码也知道第十二版新增参数怎么写
```

---

# 10. README 新增客户端 timeout 示例

第十二版 README 还新增：

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379 5000
```

含义是：

```text
连接 127.0.0.1:6379
socket 接收和发送超时设置为 5000ms
```

这个示例对应 `client_main.cpp` 的新用法：

```text
minikv_client.exe [host] [port] [timeout_ms]
```

---

# 11. README 新增请求拒绝日志示例

第十二版 README 的日志示例增加：

```text
event=tcp_listen host=127.0.0.1 port=6379 max_request_bytes=65536
event=tcp_request_rejected host=127.0.0.1 port=6379 connection_id=1 reason=request_too_long pending_bytes=70000 max_request_bytes=65536
```

这里重点是两个字段配对：

```text
pending_bytes=70000
max_request_bytes=65536
```

读日志的人可以马上知道：

```text
这个连接不是普通断开
而是因为未完成请求缓冲超过上限被拒绝
```

---

# 12. README 新增 Server options

第十二版 README 增加：

```text
minikv_server.exe [port] [host] [wal_path] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

并解释：

```text
--max-request-bytes limits the buffered bytes for one pending TCP request.
Inline over-limit requests return ERR line too long.
RESP over-limit requests return -ERR request too long.
```

这里把实现细节转成用户能直接理解的运行规则：

```text
限制对象
 -> 一个 pending TCP request

inline 超限
 -> ERR line too long

RESP 超限
 -> -ERR request too long
```

---

# 13. README 新增 Client options

第十二版 README 增加：

```text
minikv_client.exe [host] [port] [timeout_ms]
```

并解释：

```text
timeout_ms sets both receive and send socket timeouts for the bundled client.
```

这句话对应代码里的：

```text
SO_RCVTIMEO
SO_SNDTIMEO
```

也就是收和发都设置。

---

# 14. README Roadmap 调整

第十二版之后，Roadmap 更新为：

```text
1. Add broader network compatibility tests.
2. Add configurable client reconnect behavior and command history.
```

为什么把 reconnect 和 command history 放到后面？

因为第十二版已经补了：

```text
服务端请求边界
客户端 socket timeout
```

下一步更自然的是：

```text
网络兼容测试
客户端体验增强
```

---

# 15. a/12 运行归档

第十二版归档目录是：

```text
a/12/图片/
a/12/解释/说明.md
```

这次保存了 5 张关键命令截图：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-request-limit-test.png
05-limit-timeout-smoke.png
```

它们分别对应：

```text
01
 -> cmake -S . -B cmake-build-debug
 -> 验证 CMake 配置成功

02
 -> cmake --build cmake-build-debug --clean-first
 -> 验证完整编译链接成功

03
 -> ctest --test-dir cmake-build-debug --output-on-failure
 -> 验证 7 个测试全部通过

04
 -> minikv_tcp_server_tests.exe
 -> 单独验证请求限制测试 PASS

05
 -> 真实 server + bundled client + Python socket smoke
 -> 验证 CLI 配置请求上限、client timeout、超限拒绝日志
```

---

# 16. 真实 smoke test 验证了什么

第十二版真实运行验证启动：

```text
minikv_server.exe 6412 127.0.0.1 --max-request-bytes 8 --accept-poll-ms 50
```

然后用 bundled client：

```text
minikv_client.exe 127.0.0.1 6412 2000
```

输入：

```text
PING
QUIT
```

客户端输出里能看到：

```text
socket timeout: 2000 ms
PONG
BYE
```

这证明：

```text
client timeout 参数解析成功
正常请求没有被 max_request_bytes=8 误伤
```

然后用 Python socket 发送：

```text
0123456789
```

服务端返回：

```text
ERR line too long
```

服务端日志包含：

```text
event=tcp_request_rejected
pending_bytes=10
max_request_bytes=8
reason=request_too_long
```

这证明真实 server 运行时也能按 CLI 参数拒绝超限请求。

---

# 17. 本次代码增删改总览

第十二版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.12.0

README.md
 -> 当前版本说明更新为 Version 12
 -> 新增 server 请求上限和 accept poll 示例
 -> 新增 client timeout 示例
 -> 新增 tcp_request_rejected 日志说明
 -> 新增 server/client options
 -> 更新 Roadmap

include/minikv/tcp_server.hpp
 -> TcpServer::Options 新增 max_request_bytes

src/tcp_server.cpp
 -> 新增 request_limit_fields
 -> serve_client 新增 max_request_bytes 参数
 -> pending 超过上限时返回协议错误
 -> 新增 event=tcp_request_rejected 日志
 -> event=tcp_listen 新增 max_request_bytes

src/server_main.cpp
 -> 新增 parse_positive_size
 -> 新增 parse_positive_milliseconds
 -> 新增 --max-request-bytes
 -> 新增 --accept-poll-ms
 -> event=server_start 输出 max_request_bytes / accept_poll_ms

src/client_main.cpp
 -> 新增 timeout_ms 参数
 -> 新增 socket timeout 设置
 -> Windows 使用 DWORD + setsockopt
 -> POSIX 使用 timeval + setsockopt
 -> 连接成功后输出 socket timeout 提示

tests/tcp_server_tests.cpp
 -> 设置 options.max_request_bytes = 8
 -> 验证正常 PING / QUIT
 -> 验证 10 字节无换行 inline 请求被拒绝
 -> 验证 event=tcp_request_rejected 和 pending/max 字段
```

第十二版新增：

```text
a/12/图片/01-cmake-configure.png
a/12/图片/02-cmake-build.png
a/12/图片/03-ctest.png
a/12/图片/04-request-limit-test.png
a/12/图片/05-limit-timeout-smoke.png
a/12/解释/说明.md
代码讲解记录/26-server-limits-client-timeout.md
代码讲解记录/27-version-12-tests-docs.md
```

第十二版删除：

```text
无源码文件删除
```

---

# 18. 第十二版之后当前能力总览

到第十二版为止，mini-kv 已经具备：

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
TCP connection_id
TCP active / total / peak connection metrics
TCP request byte limit
event=tcp_request_rejected 请求拒绝日志
server_main 可配置 max_request_bytes 和 accept_poll_ms
client_main 可配置 socket timeout
CTest 自动化覆盖
运行归档
```

网络层现在是：

```text
能接请求
能解析 inline / RESP
能返回协议响应
能被请求停止
能记录连接生命周期
能统计连接指标
能限制过大未完成请求
能给客户端设置收发超时
```

---

# 19. 第十二版和前几版的关系

第十版：

```text
让 TcpServer 能停止
让生命周期日志结构化
```

第十一版：

```text
让连接生命周期可观察
让并发连接数量可统计
```

第十二版：

```text
让请求缓冲边界可配置
让超限拒绝可观察
让 bundled client 的网络等待可配置
```

这三版是一条连续路线：

```text
服务可管理
 -> 服务可观察
 -> 服务有边界
```

下一步更适合继续补：

```text
更广的网络兼容测试
客户端重连策略
客户端命令历史
更多端到端 smoke 场景
```

---

# 第二十七次总结

第十二版配套工作证明了四件事：

```text
tcp_server_tests
 -> 正常请求和超限请求都覆盖到

README
 -> server limit、client timeout、请求拒绝日志说明清楚

a/12
 -> 构建、测试、请求限制测试、真实 smoke 都有截图和说明

整体增删改
 -> 没有新增复杂模块，只是在网络边界和运行配置上向前补强
```

一句话总结：

```text
第十二版不仅把请求上限和客户端超时写进代码，还用自动化测试、真实运行 smoke、README 和 a/12 归档证明它们在真实 TCP 链路里确实生效。
```
