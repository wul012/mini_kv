# 第二十九次讲解：第十三版 README、归档和整体增删改

第二十八次讲了第十三版核心测试代码：

```text
TestSocketGuard
TestAddrInfoGuard
connect_test_socket
exchange_inline(host, port, payload)
localhost server 场景
```

这次讲第十三版配套内容：

```text
CMakeLists.txt
README.md
a/13/解释/说明.md
a/13/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.13.0
README 明确 Version 13 的主题是 TCP hostname compatibility coverage
构建、测试、localhost smoke 都跑过
关键命令输出已经按 a/13 归档
```

---

# 1. 第十三版的定位

第十三版不是一个大功能版。

它更像一次网络层可靠性补强：

```text
第十二版
 -> 服务端请求边界可配置
 -> 客户端 socket timeout 可配置

第十三版
 -> 测试覆盖 localhost / hostname 解析场景
 -> 测试客户端使用 AF_UNSPEC
```

也就是说，第十三版回答的是：

```text
mini-kv 不只在 127.0.0.1 这条最窄路径能跑
它在 localhost 这种真实常用写法下也跑得通
```

---

# 2. CMakeLists.txt 版本号更新

第十三版把版本号改成：

```cmake
project(mini_kv VERSION 0.13.0 LANGUAGES CXX)
```

这说明：

```text
当前构建产物对应第十三次开发迭代
```

本次没有新增 CMake target。
原因是：

```text
测试仍然放在 minikv_tcp_server_tests
没有新增独立测试可执行文件
```

所以 CMake 改动很小：

```text
0.12.0
 -> 0.13.0
```

---

# 3. README 当前版本说明

第十三版 README 开头更新为：

```text
Version 13 is a runnable in-memory KV service with broader TCP hostname compatibility coverage
```

这句话把版本主题放在：

```text
broader TCP hostname compatibility coverage
```

也就是：

```text
更广的 TCP hostname 兼容性覆盖
```

README 的能力列表新增：

```text
Broader TCP compatibility tests for localhost and address-family agnostic client resolution
```

这里有两个关键词：

```text
localhost
 -> 用户常见启动和连接写法

address-family agnostic
 -> 不预设 IPv4 或 IPv6
```

---

# 4. README 新增 localhost 启动示例

第十三版 README 新增：

```powershell
.\build\Debug\minikv_server.exe 6379 localhost --max-request-bytes 65536
```

这条命令说明：

```text
server 的 host 不一定只能写 127.0.0.1
也可以写 localhost
```

它也和第十三版真实 smoke 对齐：

```text
minikv_server.exe 6413 localhost --max-request-bytes 16 --accept-poll-ms 50
```

文档示例和真实验证链路一致，后面排查问题更轻松。

---

# 5. README 测试说明更新

第十三版 README 中 `tcp_server_tests` 的说明变成：

```text
tcp_server_tests starts servers on ephemeral ports,
sends real inline TCP requests,
verifies configurable request-limit rejection,
covers localhost hostname resolution with address-family agnostic test sockets,
requests stop through the server API,
and checks structured logs plus connection metrics.
```

这段话对应测试里的几层能力：

```text
ephemeral ports
 -> port = 0，避免固定端口冲突

real inline TCP requests
 -> 真 socket，不是 mock

request-limit rejection
 -> 第十二版请求上限

localhost hostname resolution
 -> 第十三版新增覆盖

request_stop
 -> 第十版生命周期

structured logs + metrics
 -> 第十 / 十一 / 十二版观测能力
```

一个测试文件现在串起了多版 TCP 能力。

---

# 6. README Roadmap 调整

第十二版 Roadmap 第一项是：

```text
Add broader network compatibility tests.
```

第十三版完成了第一步，所以 Roadmap 调整为：

```text
1. Add configurable client reconnect behavior and command history.
2. Add more external-client compatibility smoke tests.
```

这说明下一步自然转向：

```text
客户端体验
更多外部客户端兼容验证
```

第十三版不是网络兼容性的终点，只是把最基础的 hostname 解析场景先补上。

---

# 7. a/13 归档目录

第十三版新增归档：

```text
a/13/图片/
a/13/解释/说明.md
```

截图文件是：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-localhost-network-test.png
05-localhost-smoke.png
```

说明文档是：

```text
a/13/解释/说明.md
```

它逐张解释了命令、结果和意义。

---

# 8. 01-cmake-configure.png

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
CMake 能识别 0.13.0 版本和当前测试代码
```

这一步证明项目结构没有被破坏。

---

# 9. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完整编译链接 29 个构建步骤
```

意义：

```text
新增的 getaddrinfo / freeaddrinfo / socket RAII 测试代码可以通过 C++20 编译
```

这一步尤其重要，因为网络头文件和 socket 类型在 Windows / POSIX 上差异比较多。

---

# 10. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
7 个测试全部通过
```

测试包括：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
resp_tests
tcp_server_tests
```

意义：

```text
第十三版 localhost 网络测试通过
旧的 Store、CommandProcessor、WAL、Snapshot、Stress、RESP、请求上限和连接指标没有回归
```

---

# 11. 04-localhost-network-test.png

对应命令：

```text
.\cmake-build-debug\minikv_tcp_server_tests.exe
```

结果：

```text
退出码为 0
PASS
```

这个测试程序成功时不输出正文，所以归档截图里记录了命令和 PASS 结论。

它验证：

```text
127.0.0.1 场景仍然通过
请求上限场景仍然通过
localhost 绑定和连接场景通过
localhost 场景下连接统计正常
```

---

# 12. 05-localhost-smoke.png

真实 smoke 启动：

```text
.\cmake-build-debug\minikv_server.exe 6413 localhost --max-request-bytes 16 --accept-poll-ms 50
```

bundled client：

```text
.\cmake-build-debug\minikv_client.exe localhost 6413 2000
```

输入：

```text
PING v13-localhost
QUIT
```

输出证明：

```text
socket timeout: 2000 ms
v13-localhost
BYE
```

然后 Python socket 通过：

```text
socket.create_connection(('localhost', 6413))
```

发送 17 字节无换行请求：

```text
01234567890123456
```

服务端返回：

```text
ERR line too long
```

服务端日志包含：

```text
event=tcp_request_rejected ... pending_bytes=17 max_request_bytes=16
```

这个 smoke 证明：

```text
真实进程下 localhost host 能启动
bundled client 能用 localhost 连接
请求上限仍然生效
客户端 timeout 仍然生效
```

---

# 13. 本次代码增删改总览

第十三版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.13.0

README.md
 -> 当前版本说明更新为 Version 13
 -> 新增 localhost server 示例
 -> 新增 localhost / address-family agnostic 测试覆盖说明
 -> Roadmap 移除已完成的 broader network compatibility tests

tests/tcp_server_tests.cpp
 -> 非 Windows 分支新增 netdb.h
 -> 新增 address_error_message
 -> 新增 TestSocketGuard
 -> 新增 TestAddrInfoGuard
 -> 新增 connect_test_socket
 -> exchange_inline 新增 host 参数
 -> 原有 127.0.0.1 场景改用新连接函数
 -> 新增 localhost server / localhost client 场景
```

第十三版新增：

```text
a/13/图片/01-cmake-configure.png
a/13/图片/02-cmake-build.png
a/13/图片/03-ctest.png
a/13/图片/04-localhost-network-test.png
a/13/图片/05-localhost-smoke.png
a/13/解释/说明.md
代码讲解记录/28-localhost-network-tests.md
代码讲解记录/29-version-13-tests-docs.md
```

第十三版删除：

```text
无源码文件删除
```

---

# 14. 第十三版之后当前能力总览

到第十三版为止，mini-kv 已经具备：

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
localhost / hostname 网络兼容性测试
CTest 自动化覆盖
运行归档
```

---

# 15. 第十三版和前几版的关系

第十版：

```text
服务可管理
```

第十一版：

```text
服务可观察
```

第十二版：

```text
服务有边界
```

第十三版：

```text
服务网络路径被更多场景验证
```

连起来就是：

```text
能启动和停止
 -> 能观察连接
 -> 能限制异常请求
 -> 能在 localhost / hostname 场景下被验证
```

下一步继续做客户端重连、命令历史或更多外部客户端 smoke，会更顺。

---

# 第二十九次总结

第十三版配套工作证明了三件事：

```text
tcp_server_tests
 -> localhost 和 AF_UNSPEC 连接路径已经纳入自动化测试

真实 smoke
 -> minikv_server localhost + minikv_client localhost 能跑通

a/13
 -> 构建、测试、单独 TCP 测试、真实 localhost smoke 都有截图和说明
```

一句话总结：

```text
第十三版把网络兼容性从“代码看起来支持 hostname”推进到“自动化测试和真实运行都证明 localhost 链路可用”。
```
