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

38-tcp-resp-concurrency-tests.md
 -> 第十八版并发 RESP-over-TCP 测试核心：4 个 raw socket 客户端、active / total / peak 连接统计和精确响应校验

39-version-18-tests-docs.md
 -> 第十八版 tcp_resp_concurrency_tests、server_main 日志加锁、README、CMake、a/18 归档和整体增删改

40-wal-replay-report.md
 -> 第十九版 WAL replay report 和恢复加固：applied / skipped / truncated、坏记录跳过和尾部半条记录保护

41-version-19-tests-docs.md
 -> 第十九版 wal_tests、snapshot_tests、README、CMake、a/19 归档和整体增删改

42-version-20-ci-docs.md
 -> 第二十版 GitHub Actions CI、README、CMake、a/20 归档和整体增删改

43-client-history-persistence.md
 -> 第二十一版客户端持久化历史核心：load_from_file、save_to_file 和 --history-file

44-version-21-tests-docs.md
 -> 第二十一版 client_history_tests、真实历史文件 smoke、README、CMake、a/21 归档和整体增删改

45-snapshot-atomic-save.md
 -> 第二十二版 Snapshot 原子保存核心：TempSnapshotFile、临时文件写入和目标文件替换

46-version-22-tests-docs.md
 -> 第二十二版 snapshot_tests、真实 CLI SAVE/LOAD smoke、README、CMake、a/22 归档和整体增删改

47-wal-checksum-core.md
 -> 第二十三版 WAL checksum 核心：WAL2、FNV-1a、checksum_failed_records 和旧格式兼容

48-version-23-tests-docs.md
 -> 第二十三版 wal_tests、真实 CLI WAL checksum smoke、README、CMake、a/23 归档和整体增删改

49-wal-compaction-core.md
 -> 第二十四版 WAL compact 核心：WriteAheadLog::compact、TempWalFile、COMPACT 命令和命令层 WAL 互斥

50-version-24-tests-docs.md
 -> 第二十四版 wal_tests、command_tests、真实 CLI WAL compact smoke、README、CMake、a/24 归档和整体增删改

51-wal-repair-core.md
 -> 第二十五版 WAL repair 核心：WalRepairReport、WriteAheadLog::repair、CLI/server --repair-wal 和 event=wal_repair

52-version-25-tests-docs.md
 -> 第二十五版 wal_tests、真实 CLI/server WAL repair smoke、README、CMake、a/25 归档和整体增删改

53-wal-maintenance-core.md
 -> 第二十六版 WAL maintenance 核心：WalMaintenanceReport、maintenance_report、WALINFO、WAL stats 和 compact hint

54-version-26-tests-docs.md
 -> 第二十六版 wal_tests、command_tests、真实 CLI/server WALINFO smoke、README、CMake、a/26 归档和整体增删改

55-wal-auto-compact-core.md
 -> 第二十七版 WAL auto compact 核心：WalAutoCompactReport、compact_if_recommended、CommandProcessorOptions 和 --auto-compact-wal

56-version-27-tests-docs.md
 -> 第二十七版 wal_tests、真实 CLI/server auto compact smoke、README、CMake、a/27 归档和整体增删改

57-wal-maintenance-options-stats.md
 -> 第二十八版 WAL maintenance options 和 compact stats 核心：WalMaintenanceOptions、WalCompactionStats、可配置阈值和 WALINFO 统计字段

58-version-28-tests-docs.md
 -> 第二十八版 wal_tests、真实 CLI/server custom thresholds smoke、README、CMake、a/28 归档和整体增删改

59-runtime-stats-health-core.md
 -> 第二十九版运行期 STATS / HEALTH 核心：CommandConnectionStats、ConnectionStatsProvider、STATS、HEALTH 和 TCP 连接统计注入

60-version-29-tests-docs.md
 -> 第二十九版 command_tests、wal_tests、tcp_server_tests、真实 CLI/server STATS HEALTH smoke、README、CMake、a/29 归档和整体增删改

61-server-metrics-logging-core.md
 -> 第三十版周期性 server metrics 日志核心：metrics_log_interval、listener_wait_interval、event=server_metrics 和 --metrics-interval-ms

62-version-30-tests-docs.md
 -> 第三十版 tcp_server_tests、真实 server metrics smoke、README、CMake、a/30 归档和整体增删改

63-line-editor-core.md
 -> 第三十一版客户端行编辑核心：LineEditorBuffer、LineEditorHistoryNavigator、read_client_line、终端按键读取和非交互 fallback

64-version-31-tests-docs.md
 -> 第三十一版 line_editor_tests、真实 client line editor smoke、README、CMake、a/31 归档和整体增删改

65-command-metrics-core.md
 -> 第三十二版命令执行计数核心：CommandProcessorMetrics、CommandMetricsTracker、STATS/HEALTH counters、TCP server 共享 tracker 和 server_metrics counters

66-version-32-tests-docs.md
 -> 第三十二版 command_tests、tcp_server_tests、真实 command metrics smoke、README、CMake、a/32 归档和整体增删改
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

第十八版增加了并发 RESP-over-TCP 客户端验证链路：

```text
tcp_resp_concurrency_tests
 -> 启动 TcpServer 临时端口
 -> 4 个 raw socket 客户端先同时连接并保持打开
 -> 等待 active_connections=4
 -> 统一放行客户端发送 SET / GET / PING / TTL / DEL / GET / QUIT pipeline
 -> 每个客户端精确比对自己的 RESP 响应
 -> 检查 total_connections=4、active_connections=0、peak_connections=4
 -> 检查 Store 最终回到空

external RESP concurrency smoke
 -> 启动真实 minikv_server.exe
 -> Python 启动 4 个 raw socket RESP 客户端
 -> MATCH concurrent_clients True
 -> 服务端日志出现 peak_connections=4
 -> server_main logger 加锁后并发日志逐行完整输出
```

第十九版增加了 WAL / Snapshot 恢复加固链路：

```text
WriteAheadLog::replay_with_report
 -> 读取 WAL 所有行
 -> 跳过空白行
 -> 坏记录计入 skipped_records
 -> 末尾无换行记录视为疑似部分写入
 -> 计入 skipped_records 和 truncated_records
 -> 返回 applied / skipped / truncated 报告

minikv_cli / minikv_server
 -> 启动时调用 replay_with_report
 -> 输出 WAL replayed / skipped / truncated

snapshot_tests
 -> 坏 snapshot load 返回 false
 -> 不替换当前 Store
```

第二十版增加了跨平台 CI 链路：

```text
.github/workflows/ci.yml
 -> GitHub Actions 触发 push / pull_request / workflow_dispatch
 -> ubuntu-latest / macos-latest / windows-latest 矩阵展开
 -> actions/checkout@v4 检出代码
 -> cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
 -> cmake --build build --config Release --parallel
 -> ctest --test-dir build -C Release --output-on-failure
```

第二十一版增加了 TCP client 持久化历史链路：

```text
minikv_client --history-file path
 -> parse_options 解析 history_file
 -> ClientHistory::load_from_file 启动时加载旧命令
 -> :history / !! / !N 可以使用跨会话历史
 -> 发送命令并收到响应后 ClientHistory::save_to_file
 -> 下次客户端启动继续复用同一个历史文件
```

第二十二版增加了 Snapshot 原子保存链路：

```text
SAVE path
 -> SnapshotFile::save
 -> Store::snapshot_items 导出当前有效数据
 -> 在目标目录生成 .tmp. 临时文件
 -> 写入 snapshot header 和 ENTRY 行
 -> flush / close 成功
 -> Windows 用 MoveFileExW 替换目标文件
 -> 非 Windows 用 std::filesystem::rename 替换目标文件
 -> 失败时 TempSnapshotFile 析构清理临时文件
```

第二十三版增加了 WAL checksum 链路：

```text
SET / DEL / EXPIRE
 -> CommandProcessor 调用 WriteAheadLog::append
 -> append 把原始 WAL record 编码为 WAL2 <checksum> <record>
 -> replay 时 WAL2 先验证 checksum
 -> checksum mismatch 计入 skipped_records 和 checksum_failed_records
 -> 旧格式 SET / DEL / EXPIREAT 裸记录仍然兼容 replay
 -> CLI / server 启动输出 checksum failure 统计
```

第二十四版增加了 WAL compact 链路：

```text
COMPACT
 -> CommandProcessor 检查参数和 WAL 是否启用
 -> wal_command_mutex 阻止同进程 WAL 写入交错
 -> WriteAheadLog::compact
 -> Store::snapshot_items 导出当前 live key / value / expires_at
 -> 写临时 WAL 文件
 -> 每个 live key 写 WAL2 SET
 -> 有 TTL 的 key 再写 WAL2 EXPIREAT
 -> 替换目标 WAL
 -> 下次启动 replay compact 后的短 WAL
```

第二十五版增加了 WAL repair 链路：

```text
minikv_cli.exe data.wal --repair-wal
minikv_server.exe 6379 127.0.0.1 data.wal --repair-wal
 -> WriteAheadLog::repair
 -> replay_with_report 恢复可用记录并统计坏记录
 -> compact 把恢复后的 live state 重写为干净 WAL2
 -> CLI 输出 repaired to N keys
 -> server 输出 event=wal_repair compacted_keys=N
 -> 后续 CLI / TCP 命令在修复后的 Store 上运行
```

第二十六版增加了 WAL maintenance 链路：

```text
WriteAheadLog::maintenance_report
 -> 读取 WAL 文件 bytes
 -> 统计非空 WAL records
 -> Store::size 得到 live_keys
 -> 判断 compact_recommended

WALINFO
 -> 返回 wal_bytes / wal_records / live_keys / compact_recommended

CLI / server 启动
 -> replay 或 repair 后输出 WAL stats
 -> compact_recommended=true 时输出 compact hint
```

第二十七版增加了自动 WAL compact 链路：

```text
WriteAheadLog::compact_if_recommended
 -> 先生成 before WalMaintenanceReport
 -> compact_recommended=false 时不动作
 -> compact_recommended=true 时复用 compact_wal_file 重写短 WAL
 -> 再生成 after WalMaintenanceReport
 -> 通过 WalAutoCompactReport 返回 compact 前后状态

CommandProcessorOptions::auto_compact_wal
 -> CLI / server 显式开启 --auto-compact-wal 后置为 true
 -> WAL 模式 SET / DEL / EXPIRE 成功后触发 compact_if_recommended

CLI / server 启动
 -> replay 或 repair 后可先执行启动期 auto compact
 -> CLI 输出 WAL auto compact
 -> server 输出 event=wal_auto_compact 或 event=wal_auto_compact_skipped
```

第二十八版增加了 WAL maintenance options 和 compact stats 链路：

```text
WalMaintenanceOptions
 -> compact_min_records
 -> compact_record_ratio
 -> compact_min_bytes
 -> 控制 compact_recommended 的判断阈值

WalCompactionStats
 -> manual_compactions
 -> auto_compactions
 -> repair_compactions
 -> compacted_keys / records_removed / bytes_saved
 -> 统计本进程内 compact 做过多少维护工作

WALINFO / WAL stats / event=wal_stats
 -> 输出当前 WAL 状态
 -> 输出当前 compact 阈值
 -> 输出当前 compact counters
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
 -> 解析 PING / SET / GET / DEL / EXPIRE / TTL / SIZE / SAVE / LOAD / COMPACT / WALINFO / STATS / HEALTH / HELP / EXIT 命令，并在第四版支持可选 WAL、第五版支持手动快照、第九版支持 PING 探活、第二十四版支持 WAL compact、第二十六版支持 WAL maintenance 查询、第二十七版支持 CommandProcessorOptions::auto_compact_wal 和写命令后的自动 WAL compact、第二十八版扩展 WALINFO 输出 compact 阈值和统计字段、第二十九版支持运行期 STATS / HEALTH 和 CommandProcessorOptions::connection_stats、第三十二版支持 CommandProcessorMetrics / CommandMetricsTracker 并在 STATS / HEALTH 输出 total_commands、successful_commands、error_commands

src/main.cpp
 -> 本地命令行交互入口，第四版支持可选 WAL 路径；第二十五版支持 --repair-wal 启动期修复 WAL；第二十六版启动后输出 WAL stats 和 WAL hint；第二十七版支持 --auto-compact-wal 和启动期自动 WAL compact；第二十八版支持 --wal-compact-min-records、--wal-compact-record-ratio、--wal-compact-min-bytes

include/minikv/tcp_server.hpp
src/tcp_server.cpp
 -> 网络服务端和 socket 通信；第八版接入 RESP；第十版支持 request_stop、select 轮询停止和结构化生命周期日志；第十一版支持 connection_id、active / total / peak 连接指标；第十二版支持可配置请求上限和 event=tcp_request_rejected；第二十七版把 auto_compact_wal 传给每个连接的 CommandProcessor；第二十九版把 connection stats provider 注入每个连接的 CommandProcessor，让 STATS / HEALTH 能返回 active / total / peak 连接统计；第三十二版为所有连接共享 CommandMetricsTracker，并在 server_metrics / tcp_stop 输出 command counters

src/server_main.cpp
 -> TCP 服务端启动器，第四版支持可选 WAL 路径；第十版支持 Ctrl+C / SIGTERM 停止标记和结构化启动日志；第十二版支持 --max-request-bytes 和 --accept-poll-ms；第十八版为 TcpServer logger 加锁，避免并发连接事件输出粘行；第二十五版支持 --repair-wal 和 event=wal_repair；第二十六版支持 event=wal_stats 和 event=wal_compact_hint；第二十七版支持 --auto-compact-wal、event=wal_auto_compact 和 event=wal_auto_compact_skipped；第二十八版支持 WAL compact 阈值参数，并在 event=wal_stats 中输出阈值和 compact counters

src/client_main.cpp
 -> TCP 客户端启动器；第十二版支持可选 timeout_ms 设置 socket 收发超时；第十四版支持 --connect-retries 和 --retry-delay-ms；第十五版接入本地历史命令；第二十一版支持 --history-file 持久化历史；第三十一版接入 read_client_line，在交互终端支持行编辑和历史浏览，在重定向 stdin 时保持 getline 行为

src/benchmark_main.cpp
 -> 第六版 benchmark 启动器，用于本地吞吐观察

include/minikv/client_history.hpp
src/client_history.cpp
 -> 第十五版客户端本地历史模块，负责 :history、!!、!N 和普通输入的 send/local 判定；第二十一版新增历史文件 load_from_file / save_to_file

include/minikv/line_editor.hpp
src/line_editor.cpp
 -> 第三十一版客户端行编辑模块，负责 LineEditorBuffer、LineEditorHistoryNavigator、read_client_line、Windows / Unix-like 按键读取、ANSI 行重绘和非交互 getline fallback

include/minikv/resp.hpp
src/resp.cpp
 -> 第七版 RESP parser，负责把 Redis 风格请求解析成参数列表；第八版新增 RESP 响应格式化；第九版新增参数数量和 bulk string 长度限制

include/minikv/wal.hpp
src/wal.cpp
 -> 第四版 WAL 持久化模块，负责 append 和 replay；第十九版新增 WalReplayReport、replay_with_report、坏记录跳过和尾部半条记录保护；第二十三版新增 WAL2 checksum、checksum mismatch 跳过和 checksum_failed_records；第二十四版新增 WriteAheadLog::compact，用当前 live state 重写短 WAL；第二十五版新增 WalRepairReport 和 WriteAheadLog::repair，启动时可把 damaged WAL 重写为干净 WAL2；第二十六版新增 WalMaintenanceReport 和 maintenance_report，用于 WALINFO 和 compact hint；第二十七版新增 WalAutoCompactReport 和 compact_if_recommended，用于可选自动 WAL compact；第二十八版新增 WalMaintenanceOptions 和 WalCompactionStats，用于可配置 compact 阈值和 manual/auto/repair compact 统计

include/minikv/snapshot.hpp
src/snapshot.cpp
 -> 第五版 Snapshot 持久化模块，负责保存和加载完整数据集；第十九版通过测试确认坏 snapshot 不会替换当前 Store；第二十二版支持临时文件写完整后替换目标 snapshot

tests/
 -> 验证 Store、CommandProcessor、CommandMetricsTracker、WAL、WAL checksum、WAL compact、WAL repair、WAL maintenance、WAL 自动 compact、WAL compact 阈值配置、WAL compact counters、STATS / HEALTH、TCP connection stats provider、Snapshot、Snapshot 原子保存、并发压力、PING、RESP parser、TCP server 生命周期、连接指标、请求上限、localhost / hostname 网络兼容行为、外部客户端式 RESP-over-TCP pipeline、RESP-over-TCP 兼容边界、并发 RESP-over-TCP 客户端、持久化恢复加固、客户端本地历史、持久化历史文件行为和客户端行编辑核心逻辑

CMakeLists.txt
 -> 构建核心库、CLI、服务端、客户端、benchmark 和测试目标

.github/workflows/ci.yml
 -> 第二十版 GitHub Actions 跨平台 CI，负责在 Linux、macOS、Windows 上执行 CMake build 和 CTest
```

## 第二十九版补充理解

第二十九版新增运行期观测链路：

```text
STATS
 -> CommandProcessor 收集 Store live_keys
 -> 有 WAL 时读取 WalMaintenanceReport
 -> 有 connection_stats provider 时读取 active / total / peak 连接指标
 -> 返回一行完整 key-value 状态

HEALTH
 -> 复用同一套 Store / WAL / connection stats 信息
 -> 返回以 OK 开头的短状态行
 -> 适合快速探活

TcpServer
 -> 每个 client thread 已经持有 TcpServerConnectionTracker
 -> v29 把 tracker 包成 CommandProcessorOptions::connection_stats
 -> STATS / HEALTH 因此可以在真实 TCP 连接中返回连接统计
```

到第二十九版为止，运行期观察入口变成：

```text
WALINFO
 -> 观察 WAL maintenance 细节

STATS
 -> 观察 Store、WAL 和 TCP 连接统计

HEALTH
 -> 快速确认服务可响应和关键状态

server event logs
 -> 观察生命周期、连接、拒绝请求、WAL replay / repair / compact 事件
```

## 第三十版补充理解

第三十版新增周期性 server metrics 日志链路：

```text
minikv_server --metrics-interval-ms ms
 -> server_main 解析为 TcpServer::Options::metrics_log_interval
 -> TcpServer::run 使用 steady_clock 计算 next_metrics_log
 -> 监听循环到点读取 connection_tracker_->stats()
 -> 输出 event=server_metrics
 -> 日志包含 active_connections / total_connections / peak_connections / metrics_interval_ms
```

到第三十版为止，运行期观测入口变成：

```text
WALINFO
 -> WAL maintenance 细节

STATS
 -> Store、WAL 和 TCP 连接统计

HEALTH
 -> OK 探活和关键运行信号

server event logs
 -> 生命周期、连接、拒绝请求、WAL replay / repair / compact 事件

server_metrics
 -> 周期性连接统计日志，适合长时间运行和压力调试观察
```

## 第三十一版补充理解

第三十一版新增 bundled TCP client 行编辑链路：

```text
minikv_client 主循环
 -> 构造 mini-kv@host:port> prompt
 -> read_client_line(prompt, history.entries(), line)
 -> 交互终端下启用 LineEditorBuffer 和 LineEditorHistoryNavigator
 -> 普通字符、Backspace、Delete、Left / Right、Home / End 修改当前行
 -> Up / Down 浏览 ClientHistory 中的历史命令
 -> 回车后把完整 line 返回给 client_main
 -> client_main 继续调用 resolve_client_input、send_all 和 print_response
```

非交互输入保持旧行为：

```text
stdin 或 stdout 不是终端
 -> read_client_line 调用 fallback_read_line
 -> fallback_read_line 打印 prompt
 -> std::getline(std::cin, line)
 -> 管道输入、重定向 stdin 和 smoke test 继续可用
```

到第三十一版为止，客户端体验链路变成：

```text
LineEditorBuffer
 -> 当前行文本和光标位置

LineEditorHistoryNavigator
 -> Up / Down 历史浏览和草稿恢复

ClientHistory
 -> :history / !! / !N / --history-file

client_main
 -> 把最终 line 发送给 server，并在响应成功后保存历史
```

## 第三十二版补充理解

第三十二版新增 command metrics 链路：

```text
CommandProcessor::execute
 -> trim 输入
 -> 空行直接返回，不计数
 -> 非空命令调用 execute_trimmed
 -> 返回 CommandResult 后交给 CommandMetricsTracker::record
 -> response 以 ERR 开头则 error_commands +1
 -> 其他响应则 successful_commands +1
 -> total_commands 始终 +1
```

STATS / HEALTH 现在会读取同一个 tracker：

```text
STATS
 -> live_keys / WAL 状态
 -> total_commands / successful_commands / error_commands
 -> connection_stats

HEALTH
 -> OK / WAL compact signal
 -> total_commands / successful_commands / error_commands
 -> connection_stats
```

TCP server 中所有连接共享同一个 command metrics tracker：

```text
TcpServer 构造
 -> 创建 command_metrics_tracker_

serve_client
 -> CommandProcessorOptions::metrics_tracker = command_metrics_tracker_

每个 client CommandProcessor
 -> record 到同一个 tracker

server_metrics / tcp_stop
 -> 输出 total_commands / successful_commands / error_commands
```

到第三十二版为止，运行期观测入口变成：

```text
WALINFO
 -> WAL maintenance 细节

STATS
 -> Store、WAL、command counters、TCP 连接统计

HEALTH
 -> OK 探活、compact signal、command counters、连接统计

server_metrics
 -> 周期性连接统计和命令统计

tcp_stop
 -> 服务停止时的最终连接统计和命令统计
```

## 第三十三版讲解索引补充

```text
67-command-breakdown-latency.md
 -> 第三十三版命令分项与延迟指标核心：CommandBreakdownMetrics、latency counters、UNKNOWN 命令桶、format_command_metrics 和 TCP server 复用指标格式

68-version-33-tests-docs.md
 -> 第三十三版 command_tests、tcp_server_tests、真实 command breakdown latency smoke、README、CMake、a/33 归档和整体增删改
```

## 第三十三版补充理解

第三十三版把 v32 的 command counters 升级成 command metrics：

```text
CommandProcessor::execute
 -> trim 输入
 -> 空输入不计数
 -> command_token 提取第一个命令词并转成大写
 -> steady_clock 记录 started_at
 -> execute_trimmed 执行原有命令逻辑
 -> 计算 elapsed_ns
 -> CommandMetricsTracker::record(command, result, elapsed_ns)
```

`CommandMetricsTracker` 现在维护两层数据：

```text
totals_
 -> total_commands
 -> successful_commands
 -> error_commands
 -> total_latency_ns
 -> max_latency_ns

breakdown_
 -> PING / GET / SET / ...
 -> UNKNOWN
 -> 每个命令桶自己的 total / success / error / latency
```

v33 选择用 mutex，而不是继续堆 atomic：

```text
record 时一次性更新 totals_ 和 breakdown_
stats 时一次性复制总计和每个命令桶
```

这样 `STATS`、`HEALTH`、`server_metrics` 和 `tcp_stop` 看到的是一致快照。

未知命令不会按原始输入无限建桶，而是统一归到：

```text
UNKNOWN
```

输出格式统一由 `format_command_metrics` 生成：

```text
total_commands=...
successful_commands=...
error_commands=...
total_latency_ns=...
avg_latency_ns=...
max_latency_ns=...
command_breakdown=COMMAND:total/success/error/total_latency_ns/avg_latency_ns/max_latency_ns;...
```

到第三十三版为止，运行时观测入口变成：

```text
STATS
 -> Store、WAL、command metrics、TCP connection stats

HEALTH
 -> OK 探活、compact signal、command metrics、connection stats

server_metrics
 -> 周期性连接统计、命令总计、错误数、延迟和命令桶

tcp_stop
 -> 服务停止时的最终连接统计和命令指标快照
```

## 第三十四版讲解索引补充

```text
69-client-command-completion.md
 -> 第三十四版客户端命令补全核心：LineEditorCompletion、Tab KeyKind、默认补全候选和 read_client_line 接入补全

70-version-34-tests-docs.md
 -> 第三十四版 line_editor_tests、真实 client completion smoke、README、CMake、a/34 归档和整体增删改
```

## 第三十四版补充理解

第三十四版把客户端交互链路继续补齐：

```text
read_client_line
 -> terminal_input_available 判断交互终端
 -> 非交互输入继续 fallback_read_line
 -> 交互终端创建 LineEditorBuffer
 -> 创建 LineEditorHistoryNavigator
 -> 创建 LineEditorCompletion
 -> 按键循环中识别 Tab
 -> completion.complete(buffer.text(), buffer.cursor())
 -> 成功时 set_text 并 redraw_line
```

`LineEditorCompletion` 只负责纯补全逻辑：

```text
输入当前文本和光标位置
只补第一个 token
只在光标位于 token 末尾时补
大小写不敏感匹配
唯一匹配补完整命令并加空格
多匹配只扩展最长公共前缀
不能补全时返回 nullopt
```

默认候选包括：

```text
PING / SET / GET / DEL / EXPIRE / TTL / SIZE / SAVE / LOAD
COMPACT / WALINFO / STATS / STATSJSON / RESETSTATS / HEALTH / HELP / EXIT / QUIT
:history
```

第三十四版后，客户端输入体验变成：

```text
Tab
 -> 命令补全

Up / Down
 -> 历史浏览

Left / Right / Home / End
 -> 光标移动

Backspace / Delete
 -> 当前行编辑
```

重定向输入仍然保持脚本友好：

```text
stdin 或 stdout 不是终端
 -> 不启用按键编辑
 -> std::getline 逐行读取
```

## 第三十五版讲解索引补充

```text
71-command-metrics-export-reset.md
 -> 第三十五版指标导出与重置核心：CommandMetricsTracker::reset、STATSJSON、RESETSTATS、JSON 格式化和客户端补全候选同步

72-version-35-tests-docs.md
 -> 第三十五版 command_tests、tcp_server_tests、line_editor_tests、真实 statsjson/reset smoke、README、CMake、a/35 归档和整体增删改

73-windows-ci-nominmax-fix.md
 -> Windows CI 的 NOMINMAX 修复：阻止 Windows 头文件 min/max 宏污染 std::numeric_limits 和 std::min / std::max

74-client-key-completion.md
 -> 第三十六版客户端 Key 补全核心：LineEditorCompletionOptions、key_candidates、key-oriented command 第二 token 补全和 client_main known_keys

75-version-36-tests-docs.md
 -> 第三十六版 line_editor_tests、真实 client key smoke、README、CMake、a/36 归档和整体增删改

76-server-metrics-file-export.md
 -> 第三十七版 Server Metrics 文件导出核心：MetricsExportHandler、export_metrics_event、--metrics-file 和 server_metrics 文件落盘

77-version-37-tests-docs.md
 -> 第三十七版 tcp_server_tests、真实 metrics file smoke、README、CMake、a/37 归档和整体增删改

78-client-key-cache-persistence.md
 -> 第三十八版客户端 Key Cache 持久化核心：ClientKeyCache、--key-cache-file、key cache load/save 和跨会话补全候选

79-version-38-tests-docs.md
 -> 第三十八版 client_history_tests、真实 key cache 双会话 smoke、README、CMake、a/38 归档和整体增删改
```

## 第三十五版补充理解

第三十五版继续完善运行期观测链路：

```text
STATS
 -> 人看的 key-value 快照

STATSJSON
 -> 脚本看的 JSON 快照

RESETSTATS
 -> 清空 command metrics，重新开始观察下一段命令窗口
```

`CommandMetricsTracker` 新增：

```text
reset()
 -> 加锁
 -> totals_ 清零
 -> breakdown_ 清空
```

成功的 `RESETSTATS` 有特殊计数规则：

```text
RESETSTATS 成功
 -> 清空指标
 -> 不把 RESETSTATS 自己写入新窗口

RESETSTATS usage error
 -> 正常计入错误命令
```

`STATSJSON` 和 `STATS` 使用同一份快照来源：

```text
Store live_keys
WAL maintenance report
CommandProcessorMetrics
CommandConnectionStats
```

区别只在输出格式：

```text
STATS
 -> 一行 key-value

STATSJSON
 -> JSON object
```

TCP server 中，所有连接共享同一个 `CommandMetricsTracker`：

```text
TcpServer
 -> command_metrics_tracker_
 -> serve_client 注入每个 CommandProcessor
```

所以任意连接执行成功的 `RESETSTATS`，都会清空 server 级命令指标窗口。

客户端补全也同步认识新命令：

```text
R
 -> RESETSTATS 后带一个补全空格

STATSJ
 -> STATSJSON 后带一个补全空格
```

到第三十五版为止，运行期观测入口变成：

```text
WALINFO
 -> WAL 维护状态

STATS
 -> 人读状态快照

STATSJSON
 -> 机器读状态快照

RESETSTATS
 -> 重置命令指标观察窗口

HEALTH
 -> 快速探活
```

## Windows CI 补充修复

第三十五版推到 GitHub 后，Linux 和 macOS 通过，但 Windows 在 Build 阶段失败。

失败点集中在：

```text
src/client_main.cpp
std::numeric_limits<int>::max()
std::numeric_limits<DWORD>::max()
```

根因是 Windows 头文件可能定义 `max` 宏，MSVC 预处理后破坏了标准库调用。

补充修复为：

```text
CMakeLists.txt
 -> Windows 下给 minikv PUBLIC 定义 NOMINMAX 和 WIN32_LEAN_AND_MEAN

所有直接包含 Windows 头文件的 .cpp
 -> 在 include 前源码级定义 NOMINMAX
```

这样 CMake 目标和直接编译单文件两条路径都有保护。

## 第三十六版讲解索引补充

```text
74-client-key-completion.md
 -> 第三十六版客户端 Key 补全核心：LineEditorCompletionOptions、key_candidates、key-oriented command 第二 token 补全和 client_main known_keys

75-version-36-tests-docs.md
 -> 第三十六版 line_editor_tests、真实 client key smoke、README、CMake、a/36 归档和整体增删改
```

## 第三十六版补充理解

第三十六版继续完善客户端交互体验：

```text
命令补全
 -> 第一个 token

key 补全
 -> GET / SET / DEL / EXPIRE / TTL 的第二个 token
```

补全候选现在拆成：

```text
command_candidates
 -> PING / SET / GET / STATSJSON / ...

key_candidates
 -> 客户端本会话观察到的 key
```

客户端维护 key 候选的规则：

```text
SET key value 成功
 -> 记住 key

DEL key 返回 1 或 0
 -> 移除 key

LOAD path 成功
 -> 清空候选
```

key 补全保持保守：

```text
只补第二个 token
key 大小写敏感
光标在 token 中间不补
第二个 token 后已有文本不补
非 key-oriented command 不补 key
```

非交互输入不变：

```text
stdin 或 stdout 不是终端
 -> fallback_read_line
 -> std::getline
```

## 第三十七版讲解索引补充

```text
76-server-metrics-file-export.md
 -> 第三十七版 Server Metrics 文件导出核心：MetricsExportHandler、export_metrics_event、--metrics-file 和 server_metrics 文件落盘

77-version-37-tests-docs.md
 -> 第三十七版 tcp_server_tests、真实 metrics file smoke、README、CMake、a/37 归档和整体增删改
```

## 第三十七版补充理解

第三十七版把服务端指标从“能打印”推进到“能保存”：

```text
stdout
 -> 现场看日志

metrics file
 -> 长跑后复盘
```

核心新开关：

```text
--metrics-file path
```

它和已有的周期采样开关配套使用：

```text
--metrics-interval-ms 10000 --metrics-file data/server.metrics.log
```

导出的内容保持和 `event=server_metrics` 一致：

```text
active_connections
total_connections
peak_connections
total_commands
successful_commands
error_commands
total_latency_ns
avg_latency_ns
max_latency_ns
command_breakdown
metrics_interval_ms
```

`TcpServer::Options` 里把普通日志和指标导出拆开：

```text
logger
 -> 所有生命周期日志

metrics_exporter
 -> server_metrics / tcp_stop
```

所以连接建立、连接关闭这类事件仍然只走普通日志，不会进入 metrics file。

真实运行时，文件打开策略是：

```text
std::ios::trunc
```

也就是每次 server 启动生成一份新的本轮指标文件。
后续如果要支持长时间生产式留存，可以继续做：

```text
按大小切分
按时间切分
保留最近 N 份
压缩旧文件
```

## 第三十八版讲解索引补充

```text
78-client-key-cache-persistence.md
 -> 第三十八版客户端 Key Cache 持久化核心：ClientKeyCache、--key-cache-file、key cache load/save 和跨会话补全候选

79-version-38-tests-docs.md
 -> 第三十八版 client_history_tests、真实 key cache 双会话 smoke、README、CMake、a/38 归档和整体增删改
```

## 第三十八版补充理解

第三十八版把客户端 key 补全从“本会话可用”推进到“可选跨会话可用”：

```text
不传 --key-cache-file
 -> 和 v36 一样，只在当前客户端进程里记住 key

传 --key-cache-file
 -> 启动时加载 key
 -> 成功 SET / DEL / LOAD 后保存更新
```

新核心类是：

```text
ClientKeyCache
```

它负责：

```text
去重
容量裁剪
load_from_file
save_to_file
add / remove / clear 返回是否发生变化
```

客户端更新规则保持保守：

```text
SET key value + OK ...
 -> 记住 key

DEL key + 1 / 0
 -> 移除 key

LOAD path + OK loaded ...
 -> 清空 key cache
```

真实 smoke 用两个客户端会话验证：

```text
session 1 写入 alpha / alpine
 -> client.keys 保存 alpha / alpine

session 2 再启动
 -> 加载 2 个 key
 -> DEL alpha
 -> client.keys 只剩 alpine
```

这样 v38 不改变服务端协议，也不需要全量 key 查询命令，就先把日常交互体验往前推了一步。
## 第三十九版讲解索引补充
```text
80-metrics-file-rotation.md
 -> 第三十九版 Metrics 文件轮转核心：MetricsFileWriter、max_bytes、keep_files、轮转顺序和 server_main 参数接入

81-version-39-tests-docs.md
 -> 第三十九版 metrics_file_tests、真实 metrics rotation smoke、README、CMake、a/39 归档和整体增删改
```

## 第三十九版补充理解
第三十九版把服务端 metrics 文件从“能导出”推进到“能长期导出”：

```text
--metrics-file
 -> 指标落盘

--metrics-file-max-bytes
 -> 主文件超过阈值前轮转

--metrics-file-keep
 -> 控制最多保留多少份旧 metrics 文件
```

核心新增模块是：

```text
MetricsFileWriter
```

它封装了：

```text
创建父目录
启动时 trunc 主文件
写入后 flush
写入前判断大小
按 .1 / .2 / .3 顺序保留旧文件
keep=0 时丢弃旧文件
```

服务端入口只负责解析参数并把 writer 接进 `metrics_exporter` 回调。

这样后续如果继续做 JSONL 指标、压缩旧日志、按时间切分，也可以优先扩展这个小模块，而不把 `server_main.cpp` 变成文件系统逻辑集中地。
## 第四十版讲解索引补充
```text
82-key-listing-core.md
 -> 第四十版 KEYS 列表命令核心：Store::keys、KEYS 输出格式、排序 live keys、metrics 归类和客户端命令补全

83-version-40-tests-docs.md
 -> 第四十版 store/command/line_editor 测试、真实 KEYS smoke、README、CMake、a/40 归档和整体增删改
```

## 第四十版补充理解
第四十版只推进一个主题：

```text
服务端提供稳定的 key-listing command。
```

核心链路是：

```text
Store::keys()
 -> 加锁
 -> 清理过期 key
 -> 收集 key
 -> 字典序排序

CommandProcessor::execute("KEYS")
 -> 校验不能带额外参数
 -> 返回 key_count=N keys=...
```

这个版本刻意没有继续做客户端自动刷新 key cache。

原因是版本粒度要合适：

```text
v40
 -> 服务端 KEYS 基础能力

后续版本
 -> 客户端调用 KEYS 并刷新本地 key cache
```

这样每一版都能独立验证，也不会把功能范围一下子铺太大。
## 第四十一版讲解索引补充
```text
84-client-key-cache-refresh.md
 -> 第四十一版客户端 Key Cache 刷新核心：parse_key_list_response、ClientKeyCache::replace、KEYS 响应解析和 key cache 替换保存

85-version-41-tests-docs.md
 -> 第四十一版 client_history_tests、真实 key cache refresh smoke、README、CMake、a/41 归档和整体增删改
```

## 第四十一版补充理解
第四十一版把两条已经存在的能力接起来：

```text
v38
 -> 客户端 key cache 可以持久化

v40
 -> 服务端 KEYS 可以返回当前 live keys

v41
 -> 客户端收到 KEYS 的合法响应后，用它替换本地 key cache
```

刷新规则是：

```text
用户运行 KEYS
 -> server 返回 key_count=N keys=...
 -> client parse_key_list_response
 -> ClientKeyCache::replace
 -> 如果配置了 --key-cache-file 且缓存有变化，就保存文件
```

这个设计保持可选：

```text
不运行 KEYS
 -> 继续用 SET / DEL / LOAD 做增量观察

运行 KEYS
 -> 主动和服务端当前 key 集合对齐
```

这样 v41 既完成 Roadmap 的客户端刷新目标，又没有额外引入新的交互命令或过大的功能面。

## 第四十二版讲解索引补充
```text
86-metrics-jsonl-export.md
 -> 第四十二版 Metrics JSONL 导出核心：MetricsExportFormat、metrics_json_line、metrics exporter 双通道和 --metrics-file-format

87-version-42-tests-docs.md
 -> 第四十二版 tcp_server_tests、真实 JSONL metrics smoke、README、CMake、a/42 归档和整体增删改
```

## 第四十二版补充理解
第四十二版没有把 stdout 日志改成 JSON。

它只给 metrics file 增加格式选择：

```text
默认 text
 -> 保持旧行为

--metrics-file-format jsonl
 -> metrics file 写一行一个 JSON object
```

核心设计是“双通道”：

```text
server stdout
 -> 继续输出 event=... 文本，方便人直接看

metrics_exporter
 -> 根据 MetricsExportFormat 输出 text 或 jsonl，方便采集器读
```

这样 v42 完成了 Roadmap 的 JSONL metrics export，又没有破坏已有日志、测试和用户习惯。

## 第四十三版讲解索引补充
```text
88-keys-prefix-filter.md
 -> 第四十三版 KEYS 前缀过滤核心：Store::keys_with_prefix、KEYS [prefix]、format_prefixed_keys 和客户端缓存保护

89-version-43-tests-docs.md
 -> 第四十三版 store/command/client_history_tests、真实 prefix KEYS smoke、README、CMake、a/43 归档和整体增删改
```

## 第四十三版补充理解
第四十三版做的是：

```text
KEYS
 -> 仍然返回全量 key_count=N keys=...

KEYS prefix
 -> 返回 key_count=N prefix=prefix keys=...
```

这里最重要的点不是过滤本身，而是响应格式区分。

原因是 v41 已经让 bundled client 在看到合法全量 `KEYS` 响应时刷新本地 key cache：

```text
key_count=N keys=...
```

如果过滤响应也长得一样，客户端就会把全量 cache 错误替换成子集。

所以 v43 的规则是：

```text
无参数 KEYS
 -> 可刷新全量 key cache

带 prefix 的 KEYS
 -> 只显示过滤结果，不刷新全量 key cache
```

这样既完成了 Roadmap 的 prefix matching，也保护了之前版本的客户端体验。

## 第四十四版讲解索引补充
```text
90-info-command-version-injection.md
 -> 第四十四版 INFO 与版本注入核心：CMake configure_file、minikv/version.hpp、CommandRuntimeInfo 和 INFO 输出字段

91-version-44-tests-docs.md
 -> 第四十四版 command/line_editor_tests、真实 INFO smoke、README、CMake、a/44 归档和整体增删改
```

## 第四十四版补充理解
第四十四版的目标不是增加业务命令，而是让外部控制面能识别 mini-kv：

```text
INFO
 -> version
 -> protocol
 -> uptime_seconds
 -> live_keys
 -> wal_enabled
 -> metrics_enabled
 -> max_request_bytes
```

最关键的是版本号来源：

```text
CMake project(mini_kv VERSION ...)
 -> configure_file 生成 minikv/version.hpp
 -> INFO 使用 minikv::version
```

这样以后 Node 或发布证据链读取 mini-kv 版本时，拿到的是构建配置中的真实版本，而不是源码里手写的字符串。

v44 先做 text `INFO`，不做 `INFOJSON`。

原因是：

```text
v44
 -> 先确定字段语义和版本注入方式

v45
 -> 再把同一批元信息稳定成 JSON object
```

这样版本边界更清楚，也方便 Node 后续分阶段接入。

## 第四十五版讲解索引补充
```text
92-infojson-command-core.md
 -> 第四十五版 INFOJSON 命令核心：format_info_json、protocol JSON array、INFOJSON 分发和补全入口

93-version-45-tests-docs.md
 -> 第四十五版 command/line_editor_tests、真实 INFOJSON smoke、README、a/45 归档和整体增删改
```

## 第四十五版补充理解
第四十五版严格按跨项目路线图做 mini-kv 自己的结构化身份入口：

```text
INFOJSON
 -> version
 -> server.protocol
 -> server.uptime_seconds
 -> server.max_request_bytes
 -> store.live_keys
 -> wal.enabled
 -> metrics.enabled
```

它和 `STATSJSON` 的边界是：

```text
INFOJSON
 -> 我是谁、当前能力边界是什么

STATSJSON
 -> 当前运行指标和命令统计是什么
```

这一版最重要的是不越界：

```text
不接 Node
不改 Java
不做 COMMANDSJSON
不替代 STATSJSON
```

这样 v45 可以独立验证，后续 Node v54 再消费这个稳定字段。

## 第四十六版讲解索引补充
```text
94-command-catalog-core.md
 -> 第四十六版 COMMANDS / COMMANDSJSON 命令分级核心：CommandCatalogEntry、统一命令表、text catalog 和 JSON catalog

95-version-46-tests-docs.md
 -> 第四十六版 command/line_editor_tests、真实 COMMANDSJSON smoke、README、a/46 归档和整体增删改
```

## 第四十六版补充理解
第四十六版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v54-post-infojson-roadmap.md
```

计划合理点在于：

```text
mini-kv v46 只补自身命令能力描述
不做 ACL
不做用户权限
不改 Node
不改变现有命令语义
```

v46 的核心输出是：

```text
COMMANDS
 -> text 命令能力摘要

COMMANDSJSON
 -> JSON 命令能力 catalog
```

字段包括：

```text
name
category
mutates_store
touches_wal
stable
description
```

这让后续 Node v55 可以只读判断：

```text
write command count
admin command count
command catalog available
```

但 mini-kv 自己仍然保持基础设施实验位，不承担订单、库存、支付的权威存储。

## 第四十七版讲解索引补充
```text
96-keysjson-inventory-core.md
 -> 第四十七版 KEYSJSON key inventory 核心：format_keys_json、1000 条上限、prefix 过滤和 COMMANDSJSON catalog 更新

97-version-47-tests-docs.md
 -> 第四十七版 command/line_editor_tests、真实 KEYSJSON smoke、README、a/47 归档和整体增删改
```

## 第四十七版补充理解
第四十七版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v56-post-dashboard-control-roadmap.md
```

计划合理点在于：

```text
mini-kv v47 只补只读 key inventory
不读取 value
不做分页游标
不做 glob
不改变 KEYS 文本语义
不接 Node
```

v47 新增：

```text
KEYSJSON
KEYSJSON prefix
```

输出字段：

```text
prefix
key_count
keys
truncated
limit
```

这里的上限是：

```text
limit=1000
```

这样 Node v58 以后可以安全展示 key 列表，但不会把 mini-kv 变成无限制 scan 服务，更不会让 mini-kv 承担订单系统权威存储。

## 第四十八版讲解索引补充
```text
98-explainjson-command-core.md
 -> 第四十八版 EXPLAINJSON 命令解释核心：CommandExplain、explain_command、format_explain_json 和不执行目标命令的分发入口

99-version-48-tests-docs.md
 -> 第四十八版 command/line_editor_tests、真实 EXPLAINJSON smoke、README、a/48 归档和整体增删改
```

## 第四十八版补充理解
第四十八版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v59-post-preflight-control-roadmap.md
```

计划合理点在于：

```text
mini-kv v48 只补 raw command explain 依据
不执行被 explain 的命令
不做 ACL
不做事务
不新增后台扫描
不让 mini-kv 承担订单权威存储
```

v48 新增：

```text
EXPLAINJSON command
```

输出字段：

```text
command
category
mutates_store
touches_wal
key
requires_value
ttl_sensitive
allowed_by_parser
warnings
```

这样 Node v61 以后可以在 operation execution preview 里解释 mini-kv raw command 的风险，但仍然不能绕过 mini-kv 自己的执行边界。

## 第四十九版讲解索引补充
```text
100-explainjson-side-effects-hardening-core.md
 -> 第四十九版 EXPLAINJSON side effects 覆盖加固核心：TTL / EXPIRE、admin / meta 命令和具体副作用标签

101-version-49-tests-docs.md
 -> 第四十九版 command_tests、真实 EXPLAINJSON side_effects smoke、README、a/49 归档和整体增删改
```

## 第四十九版补充理解
第四十九版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v59-post-preflight-control-roadmap.md
```

计划合理点在于：

```text
mini-kv v49 只补 EXPLAINJSON 覆盖增强
不新增事务
不新增 ACL
不让 mini-kv 承担订单权威存储
```

v49 新增解释字段：

```text
side_effects
```

典型标签：

```text
store_write
store_ttl_update
wal_append_when_enabled
snapshot_file_write
store_replace_from_snapshot
wal_rewrite_when_enabled
metrics_reset
connection_close
metadata_read
```

这样 Node v65 以后可以接入更完整的 approval evidence，但 mini-kv 仍然只负责解释自身命令语义，不负责审批和真实业务执行。

## 第五十版讲解索引补充
```text
102-explainjson-schema-digest-core.md
 -> 第五十版 EXPLAINJSON schema / digest 核心：schema_version、command_digest、side_effect_count 和规范化 digest source

103-version-50-tests-docs.md
 -> 第五十版 command_tests、真实 EXPLAINJSON digest smoke、README、a/50 归档和整体增删改
```

## 第五十版补充理解
第五十版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v65-post-upstream-evidence-roadmap.md
```

计划合理点在于：

```text
mini-kv v50 只补 EXPLAINJSON 证据校验字段
不新增事务
不新增集群
不承担订单权威存储
```

v50 新增解释字段：

```text
schema_version
command_digest
side_effect_count
```

其中：

```text
schema_version=1
command_digest=fnv1a64:<16位hex>
side_effect_count=side_effects 数组长度
```

这样 Node v67 可以把 mini-kv 上游证据纳入 digest-aware verification，而 mini-kv 仍然只提供自身命令解释，不参与真实执行 gate。
