# 第三十三次讲解：第十五版测试、README、归档和整体增删改

第三十二次讲了第十五版客户端本地历史核心代码：

```text
ClientInputAction
ClientInputResult
ClientHistory
resolve_client_input
client_main 接入本地历史命令
```

这次讲第十五版配套内容：

```text
CMakeLists.txt
README.md
tests/client_history_tests.cpp
a/15/解释/说明.md
a/15/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.15.0
client_history 模块进入构建
client_history_tests 纳入 CTest
真实 minikv_client 能执行 :history / !! / !N
关键命令输出已经按 a/15 归档
```

---

# 1. 第十五版的定位

第十四版完成了客户端连接重试。

第十五版继续补客户端体验：

```text
查看本次会话命令
重复上一条命令
重复指定编号命令
```

这不是服务端能力，而是 bundled client 本地能力。

它的价值是：

```text
用户调试 TCP server 时不用反复手敲同一条命令
```

---

# 2. CMakeLists.txt 版本号更新

第十五版把版本号改成：

```cmake
project(mini_kv VERSION 0.15.0 LANGUAGES CXX)
```

含义是：

```text
当前构建产物对应第十五次开发迭代
```

这次不只是版本号变化。
第十五版还把新模块加入核心库：

```cmake
src/client_history.cpp
```

这样：

```text
minikv_client
client_history_tests
```

都能链接同一份历史解析逻辑。

---

# 3. CMake 新增 client_history_tests

第十五版新增测试目标：

```cmake
add_executable(minikv_client_history_tests
    tests/client_history_tests.cpp
)

target_link_libraries(minikv_client_history_tests PRIVATE minikv)

add_test(NAME client_history_tests COMMAND minikv_client_history_tests)
```

这说明：

```text
客户端历史不是只靠真实 smoke 验证
也进入了 CTest 自动化测试
```

第十五版之后，CTest 从 7 个测试变成：

```text
8 个测试
```

新增的是：

```text
client_history_tests
```

---

# 4. client_history_tests 的角色

新增文件：

```text
tests/client_history_tests.cpp
```

它直接测试：

```text
ClientHistory
resolve_client_input
```

不启动 TCP server。
不创建 socket。

原因是：

```text
历史解析逻辑本身是纯客户端本地逻辑
可以用更小的单元测试覆盖
```

真实网络链路再交给 smoke 验证。

---

# 5. 测试空历史

测试第一步：

```cpp
minikv::ClientHistory history{3};

auto result = minikv::resolve_client_input(history, ":history");
```

断言：

```text
action = local
command 为空
messages = history is empty
history.empty()
```

这证明：

```text
空历史执行 :history 不会发送给服务端
只会本地提示 history is empty
```

---

# 6. 测试普通命令进入历史

测试执行：

```cpp
resolve_client_input(history, "PING one");
resolve_client_input(history, "SET name mini-kv");
```

断言：

```text
action = send
command = 原命令
history.size() 变成 1 / 2
```

这证明：

```text
普通命令会被记录
并且会被标记为需要发送给服务端
```

---

# 7. 测试 :history 输出编号

当历史里有两条：

```text
PING one
SET name mini-kv
```

执行：

```text
:history
```

预期输出：

```text
1  PING one
2  SET name mini-kv
```

这里的编号是 1-based。

也就是用户后面可以用：

```text
!1
!2
```

重复对应命令。

---

# 8. 测试 !!

测试执行：

```cpp
resolve_client_input(history, "!!");
```

当上一条历史是：

```text
SET name mini-kv
```

预期：

```text
action = send
command = SET name mini-kv
messages = repeating: SET name mini-kv
```

同时历史大小变成 3。

这证明：

```text
重复出来的命令也算一次真实发送
所以也会进入历史
```

---

# 9. 测试 !1 和容量裁剪

测试历史容量是：

```text
3
```

执行 `!1` 后，历史会继续追加重复出来的命令。

超过容量后，最旧的记录被裁剪。

测试断言最终历史是：

```text
SET name mini-kv
SET name mini-kv
PING one
```

这证明：

```text
历史只保留最近 max_entries 条
旧记录会从前面移除
```

---

# 10. 测试不存在的历史编号

测试执行：

```text
!99
```

预期：

```text
action = local
messages = history entry not found: 99
```

这证明：

```text
不存在的历史编号不会发给服务端
```

这类输入属于客户端本地错误。

---

# 11. 测试 !abc

测试执行：

```text
!abc
```

预期：

```text
action = send
command = !abc
```

原因是：

```text
只有 ! 后面全是数字时，才按历史编号处理
```

`!abc` 不是历史命令。
所以它走普通命令路径。

---

# 12. 测试非法容量

测试最后验证：

```cpp
minikv::ClientHistory invalid{0};
```

必须抛：

```text
std::invalid_argument
```

这证明：

```text
ClientHistory 不接受 0 容量
```

边界规则写进测试后，后面改代码不容易误伤。

---

# 13. README 当前版本说明

第十五版 README 开头更新为：

```text
Version 15 is a runnable in-memory KV service with bundled TCP client session history
```

能力列表新增：

```text
Bundled TCP client local session history with :history, !!, and !N
```

这把第十五版主题写清楚：

```text
客户端本地会话历史
```

---

# 14. README 新增历史命令说明

第十五版 README 新增：

```text
:history  Show commands sent during the current client session.
!!        Repeat the previous sent command.
!N        Repeat history entry N, using 1-based indexing.
```

它强调两点：

```text
history 是 current client session
!N 是 1-based indexing
```

也就是：

```text
退出客户端后历史不保留
第一条历史是 !1，不是 !0
```

---

# 15. README 测试说明更新

第十五版 README 里测试目标列表新增：

```text
minikv_client_history_tests
```

测试说明也新增：

```text
client_history_tests verifies the bundled client's local :history, !!, and !N session history behavior.
```

这样 README 和 CMake / CTest 是对应的。

---

# 16. README Roadmap 调整

第十四版 Roadmap 第一项是：

```text
Add command history for the bundled TCP client.
```

第十五版完成后，Roadmap 调整为：

```text
1. Add more external-client compatibility smoke tests.
2. Add persistent client history or interactive line editing.
```

也就是说：

```text
本地会话历史已完成
后面可以继续做持久历史或更完整的交互行编辑
```

---

# 17. a/15 归档目录

第十五版新增归档：

```text
a/15/图片/
a/15/解释/说明.md
```

截图文件是：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-client-history-tests.png
05-client-history-smoke.png
```

说明文档是：

```text
a/15/解释/说明.md
```

---

# 18. 01-cmake-configure.png

对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
配置成功
```

意义：

```text
CMake 能识别 0.15.0、client_history.cpp 和 client_history_tests
```

---

# 19. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
完整编译链接 32 个构建步骤
```

相比第十四版，构建步骤增加，是因为新增了：

```text
client_history.cpp
minikv_client_history_tests
```

意义：

```text
客户端历史模块、客户端接入和新增测试目标都能通过 C++20 / MinGW 编译
```

---

# 20. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
8 个测试全部通过
```

新增测试：

```text
client_history_tests
```

意义：

```text
第十五版客户端历史单元测试通过，同时旧功能没有回归
```

---

# 21. 04-client-history-tests.png

对应命令：

```text
.\cmake-build-debug\minikv_client_history_tests.exe
```

结果：

```text
PASS
```

这个测试程序成功时不输出正文，所以归档截图记录了命令和 PASS 结论。

它单独证明：

```text
:history
!!
!N
容量裁剪
空历史提示
非法容量拒绝
```

这些逻辑都成立。

---

# 22. 05-client-history-smoke.png

真实 smoke 启动：

```text
minikv_server.exe 6416 127.0.0.1 --accept-poll-ms 50
```

客户端：

```text
minikv_client.exe 127.0.0.1 6416 2000
```

输入：

```text
PING first
:history
!!
!1
QUIT
```

结果：

```text
PING first
 -> first

:history
 -> 1  PING first

!!
 -> repeating: PING first
 -> first

!1
 -> repeating: PING first
 -> first

QUIT
 -> BYE
```

服务端日志出现：

```text
event=tcp_client_accepted
event=tcp_client_closed
```

意义：

```text
真实客户端能处理本地历史命令
重复命令能转成真实 inline 命令发送给服务端
本地 :history 不会污染服务端协议
```

---

# 23. 本次代码增删改总览

第十五版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.15.0
 -> minikv 库新增 src/client_history.cpp
 -> 新增 minikv_client_history_tests 测试目标
 -> CTest 新增 client_history_tests

README.md
 -> 当前版本说明更新为 Version 15
 -> 能力列表新增本地 session history
 -> 新增 :history / !! / !N 说明
 -> 测试目标列表新增 minikv_client_history_tests
 -> Roadmap 移除已完成的 command history

src/client_main.cpp
 -> include minikv/client_history.hpp
 -> 连接成功后创建 ClientHistory
 -> 每行输入先 resolve_client_input
 -> 本地消息先打印
 -> local action 不发送给服务端
 -> send action 发送 resolved.command
 -> QUIT / EXIT 判断改用 resolved.command
```

第十五版新增：

```text
include/minikv/client_history.hpp
src/client_history.cpp
tests/client_history_tests.cpp
a/15/图片/01-cmake-configure.png
a/15/图片/02-cmake-build.png
a/15/图片/03-ctest.png
a/15/图片/04-client-history-tests.png
a/15/图片/05-client-history-smoke.png
a/15/解释/说明.md
代码讲解记录/32-client-history-core.md
代码讲解记录/33-version-15-tests-docs.md
```

第十五版删除：

```text
无源码文件删除
```

---

# 24. 第十五版之后当前能力总览

到第十五版为止，mini-kv 已经具备：

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
CTest 自动化覆盖
运行归档
```

---

# 25. 第十五版和前几版的关系

第十四版：

```text
客户端能等待服务端上线
```

第十五版：

```text
客户端能记住本次会话命令
客户端能重复历史命令
```

连起来就是：

```text
客户端更容易连上
 -> 连上后更容易反复调试命令
```

下一步继续做外部客户端 smoke 或持久历史，都是自然方向。

---

# 第三十三次总结

第十五版配套工作证明了三件事：

```text
client_history_tests
 -> 历史解析和边界行为有单元测试

真实 history smoke
 -> minikv_client 真实连接 server 后能执行 :history / !! / !N

a/15
 -> 构建、测试、单独历史测试、真实 history smoke 都有截图和说明
```

一句话总结：

```text
第十五版不仅给 minikv_client 增加了本地会话历史，还用单元测试和真实 TCP smoke 证明本地历史命令不会发给服务端，重复命令会走真实服务端执行链路。
```
