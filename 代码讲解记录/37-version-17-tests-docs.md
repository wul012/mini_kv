# 第三十七次讲解：第十七版测试、README、归档和整体增删改

第三十六次讲了第十七版新增测试核心：

```text
tests/tcp_resp_compat_tests.cpp
```

这次讲第十七版配套内容：

```text
CMakeLists.txt
README.md
a/17/解释/说明.md
a/17/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.17.0
tcp_resp_compat_tests 进入构建
tcp_resp_compat_tests 纳入 CTest
真实 minikv_server 能返回更多 RESP 响应类型
关键命令输出已经按 a/17 归档
```

---

# 1. 第十七版的定位

第十六版完成：

```text
外部 raw socket 发送 RESP pipeline
PING / SET / GET / SIZE / QUIT 精确响应匹配
```

第十七版继续补：

```text
null bulk
integer
command error
usage error
protocol error
```

也就是从：

```text
成功路径可用
```

推进到：

```text
常见边界和错误路径也可验证
```

---

# 2. CMakeLists.txt 版本号更新

第十七版把版本号改成：

```cmake
project(mini_kv VERSION 0.17.0 LANGUAGES CXX)
```

这表示当前构建产物对应第十七次迭代。

这一版没有修改生产库源码。

它主要新增：

```text
tests/tcp_resp_compat_tests.cpp
```

也就是说：

```text
第十七版是测试覆盖增强版
```

---

# 3. CMake 新增 tcp_resp_compat_tests

第十七版新增：

```cmake
add_executable(minikv_tcp_resp_compat_tests
    tests/tcp_resp_compat_tests.cpp
)

target_link_libraries(minikv_tcp_resp_compat_tests PRIVATE minikv)

add_test(NAME tcp_resp_compat_tests COMMAND minikv_tcp_resp_compat_tests)
```

这三段分别做：

```text
生成测试 exe
链接 minikv 核心库
注册 CTest 用例
```

第十七版之后，CTest 从 9 个测试变成：

```text
10 个测试
```

新增的是：

```text
tcp_resp_compat_tests
```

---

# 4. 为什么新增单独测试目标

第十六版已经有：

```text
tcp_resp_tests
```

为什么第十七版不直接把所有场景塞进去？

原因是两者关注点不同：

```text
tcp_resp_tests
 -> 成功路径 pipeline：PING / SET / GET / SIZE / QUIT

tcp_resp_compat_tests
 -> 兼容边界：null bulk、integer、error、protocol error
```

分开之后，测试名字能直接说明失败范围。

如果未来某个测试失败，定位更快：

```text
成功路径失败
还是兼容边界失败
```

---

# 5. README 当前版本更新

README 当前版本从：

```text
Version 16 is a runnable in-memory KV service with external RESP-over-TCP compatibility coverage:
```

改成：

```text
Version 17 is a runnable in-memory KV service with broader RESP-over-TCP compatibility coverage:
```

关键词是：

```text
broader
```

表示第十七版不是重新做 RESP，而是在第十六版基础上扩覆盖面。

---

# 6. README 能力列表新增项

第十七版新增能力说明：

```text
RESP-over-TCP compatibility tests for null bulk replies, errors, `DEL`, `EXPIRE`, and `TTL`
```

它对应的测试内容是：

```text
GET missing
 -> null bulk reply

NOPE / GET durable extra
 -> errors

DEL / EXPIRE / TTL
 -> integer replies and state transitions
```

这让 README 里的能力列表能反映真实测试覆盖。

---

# 7. CLion 目标列表新增项

README 的 CLion 目标列表新增：

```text
minikv_tcp_resp_compat_tests
```

这样在 CLion 中可以直接运行第十七版新测试。

到第十七版，测试目标里和 RESP / TCP 相关的有：

```text
minikv_resp_tests
minikv_tcp_server_tests
minikv_tcp_resp_tests
minikv_tcp_resp_compat_tests
```

它们从不同层次覆盖协议：

```text
RESP parser 单元层
TCP server 生命周期层
RESP-over-TCP 成功路径
RESP-over-TCP 兼容边界
```

---

# 8. README 测试说明更新

第十七版把测试说明扩展为：

```text
tcp_resp_compat_tests extends the same raw-socket coverage to null bulk replies,
integer replies, command errors, protocol errors, DEL, EXPIRE, and TTL.
```

拆开看：

```text
same raw-socket coverage
 -> 沿用第十六版外部客户端式测试方法

null bulk replies
 -> GET missing

integer replies
 -> TTL / EXPIRE / DEL

command errors
 -> NOPE / bad usage

protocol errors
 -> invalid RESP null bulk command argument
```

---

# 9. README Roadmap 调整

第十六版 roadmap 里还有：

```text
Add more RESP compatibility cases for additional command/result types.
```

第十七版已经补了这一项的第一轮。

所以 roadmap 改成：

```text
1. Add persistent client history or interactive line editing.
2. Add concurrent RESP client stress coverage.
```

含义是：

```text
单连接 RESP 兼容覆盖已经更厚
下一步可以看并发 RESP 客户端压力
```

---

# 10. a/17 归档目录

第十七版新增归档：

```text
a/17/图片/
a/17/解释/说明.md
```

图片目录里有：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-tcp-resp-compat-tests.png
05-external-resp-compat-smoke.png
```

解释目录里有：

```text
说明.md
```

这延续了项目规则：

```text
关键命令输出保存为 PNG
每张图有文字说明
按版本归档
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
CMake 能识别 0.17.0
CMake 能识别 tests/tcp_resp_compat_tests.cpp
CMake 能生成 minikv_tcp_resp_compat_tests
```

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
36 个构建步骤成功
```

意义：

```text
新增 tcp_resp_compat_tests.cpp 能通过 C++20 / MinGW 编译
已有 CLI、server、client、benchmark、测试目标都没有编译回归
```

---

# 13. 为什么构建步骤变成 36

第十六版 clean build 是 34 个构建步骤。

第十七版新增一个测试可执行文件：

```text
minikv_tcp_resp_compat_tests
```

它需要：

```text
编译 tests/tcp_resp_compat_tests.cpp
链接 minikv_tcp_resp_compat_tests.exe
```

所以 clean build 增加到 36 个步骤是正常的。

---

# 14. 03-ctest.png

第三张图对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
10 个测试全部通过
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
client_history_tests
```

意义：

```text
第十七版新增 tcp_resp_compat_tests 通过
前面十六版积累的核心能力没有回归
```

---

# 15. 第十七版测试矩阵

到第十七版，测试层大致是：

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
 -> raw socket RESP pipeline 成功路径

tcp_resp_compat_tests
 -> raw socket RESP null bulk、integer、error、protocol error

client_history_tests
 -> bundled client 本地历史解析
```

新增的测试把 RESP-over-TCP 从：

```text
能处理常规 pipeline
```

推进到：

```text
能处理更多结果类型和错误路径
```

---

# 16. 04-tcp-resp-compat-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_tcp_resp_compat_tests.exe
```

结果：

```text
PASS
退出码 0
```

该测试成功时没有正文输出。

所以归档截图记录：

```text
执行命令
PASS 结论
退出码
```

它单独证明新增测试可以脱离完整 CTest 独立运行。

---

# 17. tcp_resp_compat_tests 证明什么

它证明：

```text
GET missing
 -> $-1

SET durable value
 -> +OK inserted

TTL durable
 -> :-1

EXPIRE durable 120
 -> :1

DEL durable
 -> :1

删除后 TTL durable
 -> :-2

NOPE
 -> -ERR unknown command

GET durable extra
 -> -ERR usage: GET key

非法 null bulk 参数
 -> -ERR null RESP bulk strings are not supported
```

并且最终连接统计为：

```text
total_connections=2
active_connections=0
peak_connections=1
```

---

# 18. 05-external-resp-compat-smoke.png

第五张图对应真实进程 smoke：

```text
minikv_server.exe 6418 127.0.0.1 --accept-poll-ms 50
```

然后用 Python 原始 socket 做两次连接。

第一条连接发送 compatibility pipeline：

```text
GET missing
SET durable value
TTL durable
EXPIRE durable 120
DEL durable
TTL durable
NOPE
GET durable extra
QUIT
```

第二条连接发送非法 RESP：

```text
*1\r\n$-1\r\n
```

关键结果：

```text
MATCH compatibility True
MATCH protocol_error True
```

这说明 Python 客户端收到的原始 RESP 字节和预期完全一致。

---

# 19. 真实 smoke 的意义

`tcp_resp_compat_tests` 是进程内测试：

```text
测试代码直接创建 TcpServer 对象
```

真实 smoke 是进程外测试：

```text
启动 minikv_server.exe
Python 作为外部进程连接它
```

它额外覆盖：

```text
server_main 参数解析
真实 exe 启动
真实端口监听
外部进程 socket 访问
stdout 结构化日志
stderr 是否干净
```

所以两者互补。

---

# 20. smoke 服务端日志

真实 smoke 中服务端 stdout 包含：

```text
event=server_start
event=server_hint
event=tcp_listen
event=tcp_client_accepted
event=tcp_client_closed
event=tcp_client_accepted
event=tcp_client_closed
```

这里有两次 accepted / closed。

对应：

```text
第一条 compatibility pipeline 连接
第二条 protocol error 连接
```

stderr 是：

```text
<empty>
```

说明没有运行期错误输出。

---

# 21. 本次代码增删改总览

第十七版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.17.0
 -> 新增 minikv_tcp_resp_compat_tests 测试目标
 -> CTest 新增 tcp_resp_compat_tests

README.md
 -> 当前版本说明更新为 Version 17
 -> 能力列表新增 RESP-over-TCP compatibility tests
 -> CLion 目标列表新增 minikv_tcp_resp_compat_tests
 -> 测试说明新增 tcp_resp_compat_tests 的 null bulk / integer / error / protocol error 覆盖
 -> Roadmap 调整为持久历史或交互式行编辑、并发 RESP client stress
```

第十七版新增：

```text
tests/tcp_resp_compat_tests.cpp
a/17/图片/01-cmake-configure.png
a/17/图片/02-cmake-build.png
a/17/图片/03-ctest.png
a/17/图片/04-tcp-resp-compat-tests.png
a/17/图片/05-external-resp-compat-smoke.png
a/17/解释/说明.md
代码讲解记录/36-tcp-resp-compat-tests.md
代码讲解记录/37-version-17-tests-docs.md
```

第十七版删除：

```text
无源码文件删除
```

---

# 22. 第十七版之后当前能力总览

到第十七版为止，mini-kv 已经具备：

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
RESP-over-TCP null bulk / integer / error / protocol error 验证
CTest 自动化覆盖
运行归档
代码讲解记录
```

---

# 23. 第十七版和前几版的关系

第七版：

```text
有 RESP parser
```

第八版：

```text
TCP server 接入 RESP
```

第九版：

```text
RESP parser 增加限制和 PING 兼容
```

第十六版：

```text
外部 RESP pipeline 成功路径可重复验证
```

第十七版：

```text
RESP-over-TCP 的 null bulk、integer、error、protocol error 也可重复验证
```

这条线越来越清楚：

```text
先实现协议
再实现 TCP 集成
再补成功路径
再补边界和错误路径
```

---

# 24. 第十七版之后适合继续做什么

第十七版已经把单连接 RESP 兼容覆盖加厚。

下一步自然方向是：

```text
并发 RESP client stress
 -> 多个 raw socket 同时读写

持久客户端历史
 -> client history 写入本地文件

交互式行编辑
 -> 上下键、补全、快捷键

WAL / Snapshot 恢复可靠性
 -> 损坏文件、部分写入、校验

CI
 -> 每次提交自动跑 build 和 ctest
```

如果继续按版本推进，最顺的下一版是：

```text
并发 RESP client stress coverage
```

因为它直接接在 v17 的 RESP 兼容测试后面。

---

# 25. 第十七版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v17-run 已删除
真实 smoke 启动的 minikv_server 已停止
检查后没有 minikv_server / minikv_client 残留进程
关键截图保留在 a/17/图片/
归档说明保留在 a/17/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第三十七次总结

第十七版配套工作证明了三件事：

```text
tcp_resp_compat_tests
 -> RESP-over-TCP 兼容边界进入自动化测试

真实 external RESP compatibility smoke
 -> minikv_server.exe 能被 Python raw socket 验证 null bulk、integer、error 和 protocol error

a/17
 -> 配置、构建、CTest、单独测试、真实 smoke 都有截图和说明
```

一句话总结：

```text
第十七版把 RESP-over-TCP 测试从成功 pipeline 扩展到更多真实边界：不存在 key、TTL 状态、删除、未知命令、参数错误和协议错误都能通过自动化测试与真实进程 smoke 复核。
```
