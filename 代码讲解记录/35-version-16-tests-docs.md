# 第三十五次讲解：第十六版测试、README、归档和整体增删改

第三十四次讲了第十六版新增测试核心：

```text
tests/tcp_resp_tests.cpp
```

这次讲第十六版配套内容：

```text
CMakeLists.txt
README.md
a/16/解释/说明.md
a/16/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.16.0
tcp_resp_tests 进入构建
tcp_resp_tests 纳入 CTest
真实 minikv_server 能被外部 RESP 客户端访问
关键命令输出已经按 a/16 归档
```

---

# 1. 第十六版的定位

第十五版完成了 bundled TCP client 本地历史：

```text
:history
!!
!N
```

第十六版转向协议兼容性：

```text
外部客户端通过 TCP 发送 RESP pipeline
server 返回标准 RESP frame
```

它的价值是：

```text
mini-kv 不只服务自己的 bundled client
也开始验证外部 RESP 客户端式访问
```

这里的“外部客户端式”指的是：

```text
不用项目内部函数
不用 CommandProcessor 直调
不用 RespParser 直调
只通过 socket 发送原始 RESP 字节
```

---

# 2. CMakeLists.txt 版本号更新

第十六版把版本号改成：

```cmake
project(mini_kv VERSION 0.16.0 LANGUAGES CXX)
```

含义是：

```text
当前构建产物对应第十六次开发迭代
```

这一版没有新增生产库源码。

新增的是测试目标：

```text
tests/tcp_resp_tests.cpp
```

也就是说：

```text
第十六版主要提升验证覆盖
不是改变用户命令行为
```

---

# 3. CMake 新增 minikv_tcp_resp_tests

第十六版新增：

```cmake
add_executable(minikv_tcp_resp_tests
    tests/tcp_resp_tests.cpp
)

target_link_libraries(minikv_tcp_resp_tests PRIVATE minikv)

add_test(NAME tcp_resp_tests COMMAND minikv_tcp_resp_tests)
```

这三段分别表示：

```text
add_executable
 -> 生成一个测试可执行文件

target_link_libraries
 -> 测试链接 minikv 核心库

add_test
 -> 把测试注册进 CTest
```

第十六版之后，CTest 从 8 个测试变成：

```text
9 个测试
```

新增的是：

```text
tcp_resp_tests
```

---

# 4. 为什么测试链接 minikv

`tcp_resp_tests.cpp` 直接用：

```text
minikv::Store
minikv::TcpServer
```

所以它链接：

```cmake
minikv
```

测试没有单独重写服务端逻辑。
它用的就是项目真实 TCP server。

这能保证：

```text
测试覆盖的是生产代码路径
不是测试自己模拟出来的假 server
```

---

# 5. README 当前版本更新

README 里的当前版本从：

```text
Version 15 is a runnable in-memory KV service with bundled TCP client session history:
```

改成：

```text
Version 16 is a runnable in-memory KV service with external RESP-over-TCP compatibility coverage:
```

这句话把第十六版定位说清楚：

```text
核心变化是外部 RESP-over-TCP 兼容性覆盖
```

不是新命令。
不是新存储结构。
不是新客户端交互能力。

---

# 6. README 能力列表新增项

能力列表新增：

```text
External-client style RESP-over-TCP smoke coverage for pipelined requests
```

它说明现在项目已经验证：

```text
外部客户端风格
RESP over TCP
pipeline 请求
```

三个关键词合在一起，就是第十六版的重点。

---

# 7. CLion 目标列表新增测试

README 的 CLion 目标列表新增：

```text
minikv_tcp_resp_tests
```

这样用户在 CLion 里可以直接运行该测试目标。

到第十六版，CLion 里重要目标包括：

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
minikv_client_history_tests
```

---

# 8. README 测试说明新增 tcp_resp_tests

第十六版在测试说明中补充：

```text
tcp_resp_tests uses a raw socket like an external client,
sends pipelined RESP PING / SET / GET / SIZE / QUIT requests,
and checks exact RESP frames.
```

拆开看：

```text
raw socket
 -> 不走项目内部调用

external client
 -> 模拟外部客户端访问

pipelined RESP
 -> 多条 RESP 请求连续发送

exact RESP frames
 -> 精确校验协议字节
```

这比“能跑通”更严格。

---

# 9. README Roadmap 调整

第十五版 Roadmap 是：

```text
1. Add more external-client compatibility smoke tests.
2. Add persistent client history or interactive line editing.
```

第十六版已经补了外部 RESP smoke。

所以 Roadmap 调整为：

```text
1. Add persistent client history or interactive line editing.
2. Add more RESP compatibility cases for additional command/result types.
```

含义是：

```text
外部客户端兼容性已经起步
后面可以继续扩大 RESP 命令和结果类型覆盖
```

---

# 10. a/16 归档目录

第十六版新增归档：

```text
a/16/图片/
a/16/解释/说明.md
```

其中 `图片` 保存关键命令截图：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-tcp-resp-tests.png
05-external-resp-smoke.png
```

`解释/说明.md` 解释每张截图对应的命令、结果和意义。

这延续了项目规则：

```text
关键命令输出需要保存为 PNG 截图
并且按版本放进 a/<version>/
```

---

# 11. 01-cmake-configure.png

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
CMake 能识别 0.16.0
CMake 能识别 tests/tcp_resp_tests.cpp
CMake 能生成 minikv_tcp_resp_tests 构建目标
```

这是所有后续构建和测试的前提。

---

# 12. 02-cmake-build.png

第二张图对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完整编译链接
34 个构建步骤成功
```

意义：

```text
新增 tcp_resp_tests.cpp 能通过 C++20 / MinGW 编译
已有 CLI、server、client、benchmark、测试目标都没有编译回归
```

---

# 13. 为什么是 34 个构建步骤

第十五版构建步骤是 32 个左右。

第十六版新增一个测试可执行文件：

```text
minikv_tcp_resp_tests
```

它会带来新的编译和链接步骤。

所以 clean build 里出现更多步骤是正常的。

---

# 14. 03-ctest.png

第三张图对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
9 个测试全部通过
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
client_history_tests
```

意义：

```text
第十六版新增 tcp_resp_tests 通过
前面十五版积累的核心能力没有回归
```

---

# 15. 9 个测试各自覆盖什么

到第十六版，测试层大致分工是：

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
 -> 多线程并发和 snapshot 一致性

resp_tests
 -> RESP parser 和响应格式

tcp_server_tests
 -> inline TCP、请求限制、生命周期日志、hostname 兼容

tcp_resp_tests
 -> raw socket RESP pipeline 和精确响应帧

client_history_tests
 -> bundled client 本地历史解析
```

新增的 `tcp_resp_tests` 正好补在：

```text
resp_tests 和 tcp_server_tests 中间
```

它把 RESP parser 和 TCP server 组合起来验证。

---

# 16. 04-tcp-resp-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_tcp_resp_tests.exe
```

结果：

```text
PASS
退出码 0
```

这个测试程序成功时不输出正文。

所以截图里记录的是：

```text
执行了哪个 exe
结果为 PASS
退出码为 0
```

它单独证明：

```text
新增测试本身可以独立运行
不是只在 CTest 里偶然通过
```

---

# 17. tcp_resp_tests 具体证明什么

`tcp_resp_tests` 证明：

```text
TcpServer 能监听临时端口
测试客户端能通过 raw socket 连接
server 能识别 RESP 请求
server 能连续处理 pipeline
PING external 返回 bulk string
SET 返回 simple string
GET 返回 bulk string
SIZE 返回 integer
QUIT 返回 simple string 并关闭连接
连接统计和结构化日志正确
```

核心断言是：

```text
response == expected
```

expected 是完整 RESP 响应字节：

```text
$8\r\nexternal\r\n
+OK inserted\r\n
$11\r\nresp-client\r\n
:1\r\n
+BYE\r\n
```

---

# 18. 05-external-resp-smoke.png

第五张图对应真实进程 smoke：

```text
minikv_server.exe 6417 127.0.0.1 --accept-poll-ms 50
```

然后用 Python 原始 socket 连接：

```text
127.0.0.1:6417
```

发送同样的 RESP pipeline：

```text
PING external
SET name resp-client
GET name
SIZE
QUIT
```

结果里关键字段是：

```text
MATCH True
```

这表示 Python 客户端收到的原始响应字节和预期完全一致。

---

# 19. 为什么还要真实 smoke

`tcp_resp_tests` 已经会启动 `TcpServer`。

但真实 smoke 仍然有价值：

```text
tcp_resp_tests
 -> 进程内测试 TcpServer 类

external-resp-smoke
 -> 真实启动 minikv_server.exe
 -> 外部 Python 进程连接
```

第二种更接近用户实际运行方式。

它覆盖：

```text
server_main 参数解析
server_main 创建 TcpServer
真实 exe 监听端口
外部进程 socket 访问
服务端 stdout 结构化日志
```

---

# 20. smoke 的服务端日志

真实 smoke 中服务端输出包含：

```text
event=server_start
event=server_hint
event=tcp_listen
event=tcp_client_accepted
event=tcp_client_closed
```

这说明：

```text
server_main 正常启动
TcpServer 正常监听
外部客户端连接成功
客户端退出后连接关闭
```

验证结束后，真实 server 已停止。

---

# 21. 本次代码增删改总览

第十六版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.16.0
 -> 新增 minikv_tcp_resp_tests 测试目标
 -> CTest 新增 tcp_resp_tests

README.md
 -> 当前版本说明更新为 Version 16
 -> 能力列表新增 external RESP-over-TCP pipeline smoke coverage
 -> CLion 目标列表新增 minikv_tcp_resp_tests
 -> 测试说明新增 tcp_resp_tests 的 raw socket / pipeline / exact frame 覆盖
 -> Roadmap 移除已完成的外部客户端 smoke 起步项
```

第十六版新增：

```text
tests/tcp_resp_tests.cpp
a/16/图片/01-cmake-configure.png
a/16/图片/02-cmake-build.png
a/16/图片/03-ctest.png
a/16/图片/04-tcp-resp-tests.png
a/16/图片/05-external-resp-smoke.png
a/16/解释/说明.md
代码讲解记录/34-tcp-resp-external-client-tests.md
代码讲解记录/35-version-16-tests-docs.md
```

第十六版删除：

```text
无源码文件删除
```

---

# 22. 第十六版之后当前能力总览

到第十六版为止，mini-kv 已经具备：

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
client_main 可配置连接重试和重试间隔
client_main 本地会话历史 :history / !! / !N
外部客户端式 RESP-over-TCP pipeline 验证
CTest 自动化覆盖
运行归档
代码讲解记录
```

---

# 23. 第十六版和前几版的关系

第七版：

```text
有 RESP parser
```

第八版：

```text
TCP server 能识别 RESP 并返回 RESP frame
```

第九版：

```text
PING 和 RESP parser 限制更完整
```

第十到第十三版：

```text
TCP server 生命周期、日志、指标、请求限制、hostname 兼容逐步稳定
```

第十六版：

```text
把这些能力组合起来
用外部客户端式 RESP pipeline 验证真实 TCP 协议链路
```

也就是从：

```text
组件能用
```

推进到：

```text
真实协议链路有可重复验证
```

---

# 24. 第十六版之后适合继续做什么

第十六版已经证明 RESP-over-TCP pipeline 起步可用。

后续自然方向是：

```text
RESP null bulk string
 -> GET missing

RESP error
 -> unknown command / bad argument count / oversized request

更多命令
 -> DEL / EXPIRE / TTL / SAVE / LOAD

多个客户端
 -> 并发 RESP client 连接

客户端体验
 -> 持久 history 或交互式行编辑
```

这些都能沿用第十六版的测试套路：

```text
raw socket
pipeline
exact response frame
结构化日志
连接统计
```

---

# 25. 第十六版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v16-run 已删除
真实 smoke 启动的 minikv_server 已停止
检查后没有 minikv_server / minikv_client 残留进程
关键截图保留在 a/16/图片/
归档说明保留在 a/16/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第三十五次总结

第十六版配套工作证明了三件事：

```text
tcp_resp_tests
 -> 外部客户端式 RESP pipeline 有自动化测试

真实 external RESP smoke
 -> minikv_server.exe 能被 Python raw socket 按 RESP 协议访问

a/16
 -> 配置、构建、CTest、单独测试、真实 smoke 都有截图和说明
```

一句话总结：

```text
第十六版没有扩大命令集合，而是把 RESP-over-TCP 的真实可访问性补成可重复验证：CTest 覆盖类内 server 链路，真实 smoke 覆盖 minikv_server.exe 进程链路。
```
