# 第三十一次讲解：第十四版 README、归档和整体增删改

第三十次讲了第十四版客户端连接重试核心代码：

```text
ClientOptions
UsageError
parse_nonnegative_int
parse_options
connect_with_retries
last_connect_error
```

这次讲第十四版配套内容：

```text
CMakeLists.txt
README.md
a/14/解释/说明.md
a/14/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.14.0
README 明确客户端连接重试用法
构建、测试、真实 retry smoke 都跑过
关键命令输出已经按 a/14 归档
```

---

# 1. 第十四版的定位

第十四版承接第十三版之后的路线。

第十三版做的是：

```text
localhost / hostname 网络兼容性测试
```

第十四版做的是：

```text
bundled TCP client 连接重试
```

这两版关系很自然：

```text
先确认客户端能在更多 host 写法下连接
再让客户端在服务端稍后上线时也能等一等
```

第十四版仍然不是复杂客户端框架。
它只解决启动连接阶段：

```text
connect 失败后按配置重试
```

---

# 2. CMakeLists.txt 版本号更新

第十四版把版本号改成：

```cmake
project(mini_kv VERSION 0.14.0 LANGUAGES CXX)
```

含义是：

```text
当前构建产物对应第十四次开发迭代
```

这次没有新增 CMake target。

原因是：

```text
客户端重试逻辑直接放在 minikv_client
现有 CTest 目标不需要新增
真实 retry 行为通过 smoke 验证
```

---

# 3. README 当前版本说明

第十四版 README 开头更新为：

```text
Version 14 is a runnable in-memory KV service with configurable TCP client connection retries
```

这句话把版本主题放在：

```text
configurable TCP client connection retries
```

也就是：

```text
TCP 客户端连接重试可配置
```

能力列表新增：

```text
Configurable bundled TCP client connection retries and retry delay
```

这里有两个关键词：

```text
connection retries
 -> 失败后重试多少次

retry delay
 -> 两次尝试之间等多久
```

---

# 4. README 新增客户端等待服务端示例

第十四版 README 新增：

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379 5000 --connect-retries 10 --retry-delay-ms 250
```

这条命令含义是：

```text
连接 127.0.0.1:6379
socket 收发 timeout 是 5000ms
初始连接失败后最多额外重试 10 次
每次重试间隔 250ms
```

它适合：

```text
服务端可能稍后启动
客户端脚本可以先开始等待
```

---

# 5. README 客户端参数说明更新

第十四版客户端用法变成：

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
```

说明文字新增：

```text
--connect-retries controls how many times the client retries after the initial connection attempt fails.
--retry-delay-ms controls the delay between retries and defaults to 250 ms.
```

这里注意：

```text
connect-retries 是“初始失败之后”的额外重试次数
```

所以：

```text
--connect-retries 10
 -> 最多连接尝试 11 次
```

这和代码里的：

```cpp
total_attempts = connect_retries + 1
```

一致。

---

# 6. README Roadmap 调整

第十三版之后 Roadmap 第一项是：

```text
Add configurable client reconnect behavior and command history.
```

第十四版完成了其中的连接重试部分。

所以 Roadmap 更新为：

```text
1. Add command history for the bundled TCP client.
2. Add more external-client compatibility smoke tests.
```

也就是说下一步更自然的是：

```text
客户端命令历史
更多外部客户端兼容 smoke
```

---

# 7. a/14 归档目录

第十四版新增归档：

```text
a/14/图片/
a/14/解释/说明.md
```

截图文件是：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-client-retry-smoke.png
```

说明文档是：

```text
a/14/解释/说明.md
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
CMake 能识别 0.14.0 版本和客户端重试代码
```

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
ClientOptions、parse_options、connect_with_retries、std::this_thread::sleep_for 和错误码保存逻辑可以通过 C++20 / MinGW 编译
```

这一步对第十四版很重要，因为它新增了：

```text
thread 头文件
新的参数解析分支
新的异常类型
连接重试循环
```

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
客户端入口代码改动没有破坏已有核心库、持久化、RESP、TCP server 生命周期、请求限制和 hostname 兼容测试
```

虽然 CTest 里没有单独的 client retry 单元测试，但全量构建和现有测试能证明：

```text
项目整体没有回归
```

真实 retry 行为由下一张 smoke 截图证明。

---

# 11. 04-client-retry-smoke.png

第十四版真实 smoke 的关键点是：

```text
先启动客户端
等客户端打印 retry 日志
再启动服务端
```

客户端命令：

```text
.\cmake-build-debug\minikv_client.exe 127.0.0.1 6415 --connect-retries 20 --retry-delay-ms 200
```

客户端输入：

```text
PING v14-retry
QUIT
```

服务端延迟启动：

```text
.\cmake-build-debug\minikv_server.exe 6415 127.0.0.1 --accept-poll-ms 50
```

客户端 stderr 证明重试发生：

```text
connect attempt 1/21 failed: connect failed: WSA error 10061
retrying in 200 ms
```

客户端 stdout 证明重试后连上并执行命令：

```text
connected to mini-kv at 127.0.0.1:6415
connect retries: 20, retry delay: 200 ms
v14-retry
BYE
```

服务端 stdout 证明真实连接进入服务端：

```text
event=tcp_client_accepted
event=tcp_client_closed
```

---

# 12. 为什么 smoke 要先等 retry 日志

第十四版调试时发现一个实际问题：

```text
如果客户端启动后很快服务端也启动
第一次 connect 可能没有明显失败日志
```

所以最终 smoke 脚本采用：

```text
先启动客户端
等待 client stderr 中出现 connect attempt
再启动服务端
```

这样能证明的不是“客户端刚好连上”，而是：

```text
客户端确实经历过一次失败
客户端确实进入了重试等待
服务端上线后客户端确实靠重试连上
```

这才是第十四版要验证的行为。

---

# 13. 本次代码增删改总览

第十四版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.14.0

README.md
 -> 当前版本说明更新为 Version 14
 -> 新增 TCP client 等待服务端上线示例
 -> TCP client options 新增 --connect-retries 和 --retry-delay-ms
 -> Roadmap 移除已完成的 client connection retry

src/client_main.cpp
 -> 新增 thread 头文件
 -> 新增 ClientOptions
 -> 新增 UsageError
 -> 新增 parse_nonnegative_int
 -> connect_to_server 保存 last_connect_error
 -> 新增 connect_with_retries
 -> print_usage 新增 retry 参数
 -> 新增 parse_options
 -> main 改为使用 ClientOptions 和 connect_with_retries
 -> 连接成功后输出 connect retries / retry delay 提示
```

第十四版新增：

```text
a/14/图片/01-cmake-configure.png
a/14/图片/02-cmake-build.png
a/14/图片/03-ctest.png
a/14/图片/04-client-retry-smoke.png
a/14/解释/说明.md
代码讲解记录/30-client-retry-core.md
代码讲解记录/31-version-14-tests-docs.md
```

第十四版删除：

```text
无源码文件删除
```

---

# 14. 第十四版之后当前能力总览

到第十四版为止，mini-kv 已经具备：

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
CTest 自动化覆盖
运行归档
```

---

# 15. 第十四版和前几版的关系

第十二版：

```text
客户端能设置 socket timeout
```

第十三版：

```text
客户端连接路径覆盖 localhost / hostname 场景
```

第十四版：

```text
客户端可以在连接阶段重试等待服务端上线
```

连起来就是：

```text
能连接
 -> 能限制等待时间
 -> 能覆盖更多 host 写法
 -> 能在服务端稍后上线时重试连接
```

下一步如果继续做命令历史，客户端体验会更完整。

---

# 第三十一次总结

第十四版配套工作证明了三件事：

```text
README
 -> 新参数和等待服务端上线示例已经写清楚

构建 / CTest
 -> 新客户端代码不破坏现有项目

真实 retry smoke
 -> 客户端先失败、打印重试日志、服务端上线后成功连接并执行命令
```

一句话总结：

```text
第十四版不仅给 minikv_client 增加了连接重试参数，还用真实进程 smoke 证明客户端可以先于服务端启动，并在服务端上线后自动连上继续执行命令。
```
