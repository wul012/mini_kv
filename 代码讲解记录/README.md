# mini-kv 代码讲解记录

本目录用于永久记录对 `mini-kv` 项目的分次代码讲解。

讲解风格参考项目根目录下的 `解释代码格式说明`：

```text
先说明文件或类的角色
再给核心流程
然后逐段解释代码
最后做一句话总结
```

## 讲解目录

```text
01-store-and-command.md
 -> Store 与 CommandProcessor：内存数据库和命令解析核心

02-cli-main.md
 -> main.cpp：本地 CLI 命令行入口

03-server-main.md
 -> server_main.cpp：TCP 服务端启动入口

04-tcp-server.md
 -> tcp_server.hpp / tcp_server.cpp：TCP 网络通信层

05-tests-build-docs.md
 -> tests、CMakeLists.txt、README.md、AGENTS.md 等辅助文件

06-client-main.md
 -> client_main.cpp：第二版新增 TCP 客户端入口

07-version-2-build-docs.md
 -> 第二版 CMakeLists.txt、README.md 等构建和说明文件改动

08-ttl-store-command.md
 -> 第三版 TTL：Store 过期时间和 CommandProcessor 新命令

09-version-3-tests-docs.md
 -> 第三版测试、README、a/3 归档和整体增删改

10-wal-core.md
 -> 第四版 WAL：WriteAheadLog、CommandProcessor 写前日志和 EXPIREAT

11-version-4-tests-docs.md
 -> 第四版入口参数、wal_tests、README、a/4 归档和整体增删改

12-snapshot-core.md
 -> 第五版快照核心：SnapshotFile、Store 快照导出恢复和 SAVE / LOAD

13-version-5-tests-docs.md
 -> 第五版 snapshot_tests、README、a/5 归档、AGENTS 环境规则和整体增删改

14-benchmark-main.md
 -> 第六版 benchmark_main.cpp：本地吞吐测试入口和结果输出

15-version-6-tests-docs.md
 -> 第六版 stress_tests、CMake、README、a/6 归档和整体增删改

16-resp-parser-core.md
 -> 第七版 RESP parser 核心：RespParser、complete/incomplete/error 和 consumed

17-version-7-tests-docs.md
 -> 第七版 resp_tests、CMake、README、a/7 归档和整体增删改

18-tcp-resp-integration.md
 -> 第八版 TCP server 接入 RESP：自动协议识别、pending 缓冲和 RESP 执行链路

19-version-8-tests-docs.md
 -> 第八版 RESP 响应格式、client 调整、README、a/8 归档和整体增删改

20-ping-resp-hardening.md
 -> 第九版 PING 命令和 RESP 协议加固：探活命令、参数数量限制、bulk 长度限制和 PONG 编码

21-version-9-tests-docs.md
 -> 第九版 command_tests、resp_tests、README、a/9 归档和整体增删改

22-tcp-server-lifecycle.md
 -> 第十版 TCP Server 生命周期：request_stop、select 轮询、信号停止和结构化日志

23-version-10-tests-docs.md
 -> 第十版 tcp_server_tests、CMake、README、a/10 归档和整体增删改

24-connection-metrics.md
 -> 第十一版 TCP 连接生命周期追踪：connection_id、active / total / peak 指标和关闭日志

25-version-11-tests-docs.md
 -> 第十一版 tcp_server_tests、README、a/11 归档和整体增删改

26-server-limits-client-timeout.md
 -> 第十二版服务端请求上限和客户端超时：max_request_bytes、请求拒绝日志、server 参数和 client timeout

27-version-12-tests-docs.md
 -> 第十二版 tcp_server_tests、README、a/12 归档和整体增删改

28-localhost-network-tests.md
 -> 第十三版 localhost 网络兼容测试：getaddrinfo(AF_UNSPEC)、测试 socket RAII 和 hostname 场景

29-version-13-tests-docs.md
 -> 第十三版 README、CMake、a/13 归档和整体增删改

30-client-retry-core.md
 -> 第十四版客户端连接重试核心：ClientOptions、parse_options、connect_with_retries 和错误码保存

31-version-14-tests-docs.md
 -> 第十四版 README、CMake、a/14 归档、真实 retry smoke 和整体增删改

32-client-history-core.md
 -> 第十五版客户端本地历史核心：ClientHistory、resolve_client_input、:history、!! 和 !N

33-version-15-tests-docs.md
 -> 第十五版 client_history_tests、README、CMake、a/15 归档和整体增删改

34-tcp-resp-external-client-tests.md
 -> 第十六版外部 RESP-over-TCP 测试核心：raw socket、pipeline、精确 RESP frame、连接日志和统计

35-version-16-tests-docs.md
 -> 第十六版 tcp_resp_tests、README、CMake、a/16 归档和整体增删改

36-tcp-resp-compat-tests.md
 -> 第十七版 RESP-over-TCP 兼容测试核心：null bulk、integer、command error、usage error 和 protocol error

37-version-17-tests-docs.md
 -> 第十七版 tcp_resp_compat_tests、README、CMake、a/17 归档和整体增删改
```

## 项目整体理解

`mini-kv` 是一个 C++20 / CMake 编写的迷你 Redis 风格内存键值服务。

它的核心链路是：

```text
用户输入命令
 -> CommandProcessor 解析命令
 -> Store 读写内存数据
 -> CLI 或 TCP Server 把结果返回给用户
```

第三版增加了 TTL 链路：

```text
EXPIRE key seconds
 -> CommandProcessor 解析秒数
 -> Store 记录 expires_at

TTL key
 -> Store 检查 key 是否存在、是否过期、是否有 expires_at
 -> CommandProcessor 返回 -2 / -1 / 剩余秒数
```

第四版增加了 WAL 链路：

```text
程序启动并传入 wal_path
 -> WriteAheadLog replay 到 Store
 -> CommandProcessor 持有 WAL 指针

SET / DEL / EXPIRE
 -> 先 append WAL
 -> 再修改 Store

程序下次启动
 -> replay SET / DEL / EXPIREAT
 -> 恢复未过期数据
```

第五版增加了 Snapshot 链路：

```text
SAVE path
 -> SnapshotFile::save
 -> Store 导出当前有效 key / value / expires_at
 -> 写入 snapshot 文件

LOAD path
 -> SnapshotFile::load
 -> 解析 snapshot 文件
 -> Store 替换为 snapshot 中仍然有效的数据
```

第六版增加了 Benchmark 和 Stress Test 链路：

```text
minikv_benchmark
 -> 直接测试 Store set/get/erase
 -> 再测试 CommandProcessor SET/GET
 -> 输出耗时和 ops/sec

stress_tests
 -> 多线程共享一个 Store
 -> 并发 SET / GET / EXPIRE / TTL / erase
 -> 验证 snapshot_items / restore_snapshot 和最终 key 状态
```

第七版增加了 RESP Parser 链路：

```text
RESP array-of-bulk-strings
 -> RespParser::parse_command
 -> 得到 arguments 和 consumed
 -> RespParser::to_inline_command
 -> 桥接到现有 CommandProcessor
```

第八版增加了 TCP RESP 链路：

```text
TCP server 收到请求
 -> pending.front() == '*'
 -> RespParser::parse_command
 -> CommandProcessor::execute
 -> RespParser::to_resp_response
 -> 返回 RESP frame

非 '*' 开头
 -> 保持 inline 文本协议
```

第九版增加了 PING 和 RESP 加固链路：

```text
PING [message]
 -> CommandProcessor 统一处理
 -> CLI / inline TCP / RESP TCP 都可使用

RESP parser
 -> 限制 array 参数数量最多 1024
 -> 限制单个 bulk string 最多 65536 字节
 -> 超限请求返回 RESP error
```

第十版增加了 TCP Server 生命周期管理链路：

```text
TcpServer::request_stop
 -> 设置 atomic 停止标记
 -> accept loop 周期性醒来检查 stop_requested
 -> run() 退出并记录 event=tcp_stop

server_main
 -> 安装 Ctrl+C / SIGTERM 信号处理
 -> 把 should_stop 和 logger 注入 TcpServer::Options
 -> 输出 event=server_start / event=server_stopped
```

第十一版增加了 TCP 连接观测链路：

```text
TCP client accepted
 -> TcpServerConnectionTracker 分配 connection_id
 -> active_connections +1
 -> total_connections +1
 -> 更新 peak_connections
 -> event=tcp_client_accepted

client thread exit
 -> ConnectionCloseLogger 析构
 -> active_connections -1
 -> event=tcp_client_closed
```

第十二版增加了 TCP 请求限制和客户端超时链路：

```text
minikv_server --max-request-bytes bytes
 -> TcpServer::Options::max_request_bytes
 -> serve_client 检查 pending.size()
 -> 超限返回 ERR line too long / -ERR request too long
 -> event=tcp_request_rejected

minikv_client timeout_ms
 -> parse_positive_milliseconds
 -> set_socket_timeout
 -> SO_RCVTIMEO / SO_SNDTIMEO
```

第十三版增加了 TCP hostname 兼容测试链路：

```text
tcp_server_tests
 -> exchange_inline(host, port, payload)
 -> connect_test_socket
 -> getaddrinfo(AF_UNSPEC)
 -> 依次尝试系统返回的 IPv4 / IPv6 / hostname 候选地址
 -> localhost server / localhost client 场景
```

第十四版增加了 TCP client 连接重试链路：

```text
minikv_client --connect-retries count --retry-delay-ms ms
 -> parse_options
 -> ClientOptions
 -> connect_with_retries
 -> connect_to_server 失败后打印 connect attempt
 -> sleep retry_delay
 -> 服务端上线后重试连接成功
```

第十五版增加了 TCP client 本地历史链路：

```text
minikv_client 读取用户输入
 -> resolve_client_input
 -> :history 本地打印历史
 -> !! 重复上一条已发送命令
 -> !N 重复第 N 条历史命令
 -> local action 不发给服务端
 -> send action 发送 resolved.command
```

第十六版增加了外部客户端式 RESP-over-TCP 验证链路：

```text
tcp_resp_tests
 -> 启动 TcpServer 临时端口
 -> raw socket 发送 RESP pipeline
 -> PING / SET / GET / SIZE / QUIT 连续执行
 -> 精确比对 RESP 响应帧
 -> 检查 event=tcp_client_closed 和连接统计

external RESP smoke
 -> 启动真实 minikv_server.exe
 -> Python raw socket 发送同样 pipeline
 -> MATCH True 证明原始响应字节完全符合预期
```

第十七版增加了 RESP-over-TCP 兼容边界验证链路：

```text
tcp_resp_compat_tests
 -> raw socket 发送 GET missing / SET / TTL / EXPIRE / DEL / NOPE / GET extra / QUIT
 -> 精确比对 $-1 / :-1 / :1 / :-2 / -ERR / +BYE
 -> 第二条连接发送非法 null bulk 参数
 -> 精确比对 protocol error
 -> 检查 total_connections=2 和 peak_connections=1

external RESP compat smoke
 -> 启动真实 minikv_server.exe
 -> Python raw socket 复跑 compatibility pipeline 和 protocol error
 -> MATCH compatibility True
 -> MATCH protocol_error True
```

从运行方式看，它现在有四种入口：

```text
CLI 模式
 -> main.cpp
 -> 读取键盘输入
 -> 调用 CommandProcessor
 -> 打印结果

TCP Server 模式
 -> server_main.cpp
 -> TcpServer 监听端口
 -> 客户端通过 TCP 发送命令
 -> 调用 CommandProcessor
 -> 通过 socket 返回结果

TCP Client 模式
 -> client_main.cpp
 -> 连接 minikv_server
 -> 读取用户输入
 -> 通过 TCP 发送命令
 -> 打印服务端响应

Benchmark 模式
 -> benchmark_main.cpp
 -> 在进程内执行固定读写场景
 -> 打印耗时和吞吐
```

从代码职责看：

```text
include/minikv/store.hpp
src/store.cpp
 -> 保存 key-value 数据，并在第三版支持 TTL 过期时间

include/minikv/command.hpp
src/command.cpp
 -> 解析 PING / SET / GET / DEL / EXPIRE / TTL / SIZE / SAVE / LOAD / HELP / EXIT 命令，并在第四版支持可选 WAL、第五版支持手动快照、第九版支持 PING 探活

src/main.cpp
 -> 本地命令行交互入口，第四版支持可选 WAL 路径

include/minikv/tcp_server.hpp
src/tcp_server.cpp
 -> 网络服务端和 socket 通信；第八版接入 RESP；第十版支持 request_stop、select 轮询停止和结构化生命周期日志；第十一版支持 connection_id、active / total / peak 连接指标；第十二版支持可配置请求上限和 event=tcp_request_rejected

src/server_main.cpp
 -> TCP 服务端启动器，第四版支持可选 WAL 路径；第十版支持 Ctrl+C / SIGTERM 停止标记和结构化启动日志；第十二版支持 --max-request-bytes 和 --accept-poll-ms

src/client_main.cpp
 -> TCP 客户端启动器；第十二版支持可选 timeout_ms 设置 socket 收发超时；第十四版支持 --connect-retries 和 --retry-delay-ms；第十五版接入本地历史命令

src/benchmark_main.cpp
 -> 第六版 benchmark 启动器，用于本地吞吐观察

include/minikv/client_history.hpp
src/client_history.cpp
 -> 第十五版客户端本地历史模块，负责 :history、!!、!N 和普通输入的 send/local 判定

include/minikv/resp.hpp
src/resp.cpp
 -> 第七版 RESP parser，负责把 Redis 风格请求解析成参数列表；第八版新增 RESP 响应格式化；第九版新增参数数量和 bulk string 长度限制

include/minikv/wal.hpp
src/wal.cpp
 -> 第四版 WAL 持久化模块，负责 append 和 replay

include/minikv/snapshot.hpp
src/snapshot.cpp
 -> 第五版 Snapshot 持久化模块，负责保存和加载完整数据集

tests/
 -> 验证 Store、CommandProcessor、WAL、Snapshot、并发压力、PING、RESP parser、TCP server 生命周期、连接指标、请求上限、localhost / hostname 网络兼容行为、外部客户端式 RESP-over-TCP pipeline、RESP-over-TCP 兼容边界和客户端本地历史行为

CMakeLists.txt
 -> 构建核心库、CLI、服务端、客户端、benchmark 和测试目标
```
