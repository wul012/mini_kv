# 第二十三次讲解：第十版测试、README、运行归档和增删改

第二十二次讲了第十版核心代码：

```text
TcpServer::Options
TcpServer::request_stop
TcpServer::stop_requested
select + accept 轮询
server_main 信号处理
结构化生命周期日志
```

这次讲第十版配套内容：

```text
tests/tcp_server_tests.cpp
CMakeLists.txt
README.md
a/10/解释/说明.md
a/10/图片/
```

它们负责证明：

```text
TcpServer 能启动
TcpServer 能被 request_stop 停止
TcpServer 能输出 listen / stop 结构化日志
真实 server 仍然兼容 inline / RESP
文档和运行归档能说明第十版状态
```

---

# 1. 第十版为什么新增 tcp_server_tests

第十版改的是 TCP server 的运行循环。

这类代码如果只靠手动 smoke test，会有两个问题：

```text
第一，后续改动容易不小心把停止能力破坏
第二，手动启动和停止服务不适合作为长期回归验证
```

所以第十版新增：

```text
tests/tcp_server_tests.cpp
```

它专门验证：

```text
TcpServer::run() 能启动
logger 能收到 event=tcp_listen
request_stop() 能让 run() 退出
logger 能收到 event=tcp_stop
```

这就是第十版最核心的自动化证明。

---

# 2. tcp_server_tests 引入的头文件

```cpp
#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
```

测试只需要两层：

```text
Store
 -> TcpServer 需要一个共享内存数据库引用

TcpServer
 -> 被测试对象
```

它没有引入：

```text
CommandProcessor
RespParser
WriteAheadLog
```

因为这个测试不关心命令执行。

它只关心服务端生命周期。

---

# 3. 标准库头文件

```cpp
#include <algorithm>
#include <cassert>
#include <chrono>
#include <exception>
#include <mutex>
#include <string>
#include <string_view>
#include <thread>
#include <vector>
```

它们分别用于：

```text
algorithm
 -> any_of 搜索日志

cassert
 -> 断言测试条件

chrono
 -> 10ms 等待间隔

exception
 -> 捕获 server.run() 线程里的异常

mutex
 -> 保护 logs vector

thread
 -> 在后台线程运行 server

vector/string/string_view
 -> 保存和匹配日志
```

这个测试本质上是一个小型并发测试。

---

# 4. contains_log 辅助函数

```cpp
bool contains_log(const std::vector<std::string>& logs, std::string_view needle) {
    return std::any_of(logs.begin(), logs.end(), [needle](const std::string& line) {
        return line.find(needle) != std::string::npos;
    });
}
```

它判断日志列表里是否有某个片段。

例如：

```text
event=tcp_listen
event=tcp_stop
```

测试不要求整行完全相等。

因为完整日志还有：

```text
host=127.0.0.1
port=0
```

所以只匹配关键字段即可。

---

# 5. 为什么测试用 port = 0

```cpp
options.port = 0;
```

端口设置为 0 时，操作系统会分配一个临时可用端口。

这样测试不会固定占用：

```text
6379
6399
6400
```

也不容易和其他服务冲突。

这个测试不需要真实客户端连接。

所以它不需要知道系统最终分配了哪个端口。

只要能成功 bind/listen，就足够了。

---

# 6. 为什么 accept_poll_interval 设置成 10ms

```cpp
options.accept_poll_interval = 10ms;
```

生产默认值是：

```text
200ms
```

测试里改成：

```text
10ms
```

原因是：

```text
测试要快
request_stop 后不想等太久
```

第十版把这个值放进 `Options`，一个好处就是：

```text
生产和测试可以使用不同轮询间隔
```

---

# 7. 测试里的日志收集

```cpp
std::mutex logs_mutex;
std::vector<std::string> logs;
options.logger = [&](const std::string& message) {
    std::lock_guard lock{logs_mutex};
    logs.push_back(message);
};
```

`TcpServer` 在后台线程运行。

日志回调也会在后台线程里被调用。

而主测试线程会读取 `logs`。

所以需要：

```text
std::mutex logs_mutex
```

来保护 `logs`。

这避免数据竞争。

---

# 8. 为什么 logger 用捕获 lambda

```cpp
options.logger = [&](const std::string& message) {
    ...
};
```

这里需要捕获：

```text
logs_mutex
logs
```

所以不能用普通函数。

第十版把 `logger` 设计成 `std::function`，正好能接受这种捕获 lambda。

这说明接口设计适合测试。

---

# 9. 后台线程运行 server.run

```cpp
minikv::TcpServer server{store, options};
std::exception_ptr failure;
std::thread server_thread{[&] {
    try {
        server.run();
    } catch (...) {
        failure = std::current_exception();
    }
}};
```

为什么要开线程？

因为：

```text
server.run() 会进入监听循环
当前测试线程还要继续等待日志、调用 request_stop
```

如果直接在主线程调用 `server.run()`，测试就卡住了。

所以测试让 server 在后台跑。

---

# 10. 为什么要保存 exception_ptr

`server.run()` 如果抛异常，异常发生在后台线程。

后台线程里的异常不会自动传回主线程。

所以测试用：

```cpp
std::exception_ptr failure;
```

保存异常。

线程 join 后再检查：

```cpp
if (failure) {
    std::rethrow_exception(failure);
}
```

这样如果 server 启动或 select 出错，测试会失败，而不是悄悄吞掉异常。

---

# 11. 等待 event=tcp_listen

```cpp
bool started = false;
for (int attempt = 0; attempt < 100; ++attempt) {
    {
        std::lock_guard lock{logs_mutex};
        started = contains_log(logs, "event=tcp_listen");
    }
    if (started) {
        break;
    }
    std::this_thread::sleep_for(10ms);
}

assert(started);
```

这段最多等待：

```text
100 * 10ms = 1000ms
```

它不是盲目 sleep 一秒。

而是：

```text
每 10ms 看一次日志
看到 event=tcp_listen 就立刻继续
```

这让测试既稳定又尽量快。

---

# 12. 为什么 event=tcp_listen 是启动完成信号

`TcpServer::run()` 里：

```cpp
SocketGuard listener = bind_listener(options_);
log_event(options_, "event=tcp_listen " + endpoint_fields(options_));
```

也就是说，只有在：

```text
bind 成功
listen 成功
```

之后才会记录：

```text
event=tcp_listen
```

所以测试看到这条日志，就能确认：

```text
server 已经真正进入可监听状态
```

---

# 13. 停止前断言

```cpp
assert(!server.stop_requested());
```

在调用 `request_stop()` 之前，server 不应该已经处于停止状态。

这条断言能发现两类问题：

```text
stop_requested_ 初始值错误
options.should_stop 误返回 true
```

第十版测试里的 `options.should_stop` 没有设置。

所以这里主要验证内部 atomic 初始值是：

```text
false
```

---

# 14. 调用 request_stop 并 join

```cpp
server.request_stop();
server_thread.join();
```

这就是第十版停止能力的核心验证。

流程是：

```text
主线程调用 request_stop
 -> stop_requested_ = true
server.run() 在线程里下一次轮询醒来
 -> stop_requested() 返回 true
 -> while 循环退出
 -> server_thread 可以 join
```

如果 `run()` 没有正确观察停止标记，`join()` 就会卡住。

CTest 会超时或测试无法结束。

---

# 15. 重新抛出后台异常

```cpp
if (failure) {
    std::rethrow_exception(failure);
}
```

这行放在 `join()` 后。

含义是：

```text
如果 server.run() 抛过异常
就在主线程重新抛出
让测试失败
```

这样测试结果不会出现“线程内部失败但主线程以为成功”的假阳性。

---

# 16. 停止后断言

```cpp
assert(server.stop_requested());
```

调用 `request_stop()` 后，`stop_requested()` 应该返回 true。

这验证：

```text
request_stop()
 -> stop_requested_.store(true)
 -> stop_requested()
 -> stop_requested_.load()
```

这条内部状态链路是通的。

---

# 17. 断言 listen 和 stop 日志

```cpp
std::lock_guard lock{logs_mutex};
assert(contains_log(logs, "event=tcp_listen"));
assert(contains_log(logs, "event=tcp_stop"));
```

最后检查两个关键事件：

```text
event=tcp_listen
 -> server 成功监听

event=tcp_stop
 -> server.run() 正常退出
```

这不是测试 stdout。

这是直接测试：

```text
TcpServer::Options::logger
```

是否收到了内部生命周期事件。

---

# 18. CMakeLists.txt 新增测试目标

第十版把版本号更新为：

```cmake
project(mini_kv VERSION 0.10.0 LANGUAGES CXX)
```

并新增：

```cmake
add_executable(minikv_tcp_server_tests
    tests/tcp_server_tests.cpp
)

target_link_libraries(minikv_tcp_server_tests PRIVATE minikv)

add_test(NAME tcp_server_tests COMMAND minikv_tcp_server_tests)
```

这三步对应：

```text
生成测试 exe
链接 minikv 核心库
注册到 CTest
```

所以运行：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

时会多一个：

```text
tcp_server_tests
```

---

# 19. 为什么 tcp_server_tests 链接 minikv

`tcp_server_tests.cpp` 用到了：

```text
minikv::Store
minikv::TcpServer
```

这些实现都在：

```text
minikv
```

库里。

所以 CMake 需要：

```cmake
target_link_libraries(minikv_tcp_server_tests PRIVATE minikv)
```

否则会出现链接错误。

---

# 20. README Current version 更新

第十版 README 把当前版本描述改成：

```text
Version 10 is a runnable in-memory KV service with graceful TCP shutdown and structured lifecycle logging
```

关键词是：

```text
graceful TCP shutdown
structured lifecycle logging
```

这正好对应第十版核心：

```text
TcpServer 可请求停止
server_main 支持停止信号
event=... 日志
```

---

# 21. README 能力列表新增第十版能力

第十版 README 新增：

```text
Graceful TCP server stop support and structured event=... lifecycle logs
```

同时测试覆盖描述从：

```text
store, command, WAL, snapshot, stress, and RESP behavior
```

更新为：

```text
store, command, WAL, snapshot, stress, RESP, and TCP server behavior
```

这说明：

```text
测试范围已经覆盖 TCP server 生命周期
```

---

# 22. README CLion 目标列表新增 tcp_server_tests

第十版 README 在 CLion target 列表里新增：

```text
minikv_tcp_server_tests
```

这样在 CLion 里打开项目时，用户能直接找到并运行第十版新增测试。

这和 CMake 新增测试目标保持一致。

---

# 23. README TCP protocol 新增结构化日志说明

第十版 README 新增日志示例：

```text
event=server_start host=127.0.0.1 port=6379 protocol=inline,resp
event=tcp_listen host=127.0.0.1 port=6379
event=tcp_client_accepted host=127.0.0.1 port=6379
event=tcp_stop host=127.0.0.1 port=6379
event=server_stopped host=127.0.0.1 port=6379
```

这些对应两层：

```text
server_main 层：
  event=server_start
  event=server_stopped

TcpServer 层：
  event=tcp_listen
  event=tcp_client_accepted
  event=tcp_stop
```

文档里还说明：

```text
Use Ctrl+C or SIGTERM to request a graceful server stop.
```

这把第十版停止方式写清楚了。

---

# 24. README stress/tests 章节更新

第十版在测试说明里补充：

```text
tcp_server_tests starts a server on an ephemeral port, requests stop through the server API, and checks the structured listen/stop log events.
```

这句话说明三个重点：

```text
ephemeral port
 -> 不抢固定端口

requests stop through the server API
 -> 验证 request_stop

checks structured listen/stop log events
 -> 验证 logger 和生命周期事件
```

---

# 25. README Roadmap 调整

第九版 Roadmap 是：

```text
1. Add graceful shutdown and structured logging.
2. Add broader network compatibility tests.
```

第十版完成了第一项。

所以 Roadmap 更新为：

```text
1. Add broader network compatibility tests.
2. Add connection lifecycle tracking and client concurrency metrics.
```

也就是说，下一阶段可以继续补：

```text
更多网络兼容测试
连接数统计
客户端并发指标
```

---

# 26. a/10 运行归档

第十版新增归档目录：

```text
a/10/图片/
a/10/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-graceful-stop-test.png
05-structured-tcp-smoke.png
```

这延续项目规则：

```text
关键命令输出保存为 PNG
说明文档解释每张图的命令、结果和意义
```

---

# 27. 01-cmake-configure.png

对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
Configuring done
Generating done
Build files have been written to cmake-build-debug
```

意义：

```text
CMake 能识别第十版版本号、源码改动和新增 tcp_server_tests 目标
```

---

# 28. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完整编译并链接所有目标
```

意义：

```text
TcpServer 停止接口、select 轮询、server_main 信号处理、结构化日志和新增测试都能通过编译
```

---

# 29. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
7/7 tests passed
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
第十版新增 TCP server 生命周期测试通过
旧功能测试也全部通过
```

---

# 30. 04-graceful-stop-test.png

对应命令：

```text
.\cmake-build-debug\minikv_tcp_server_tests.exe
```

结果：

```text
Exit code: 0
Result: PASS
```

意义：

```text
单独证明 TcpServer 可以启动、被 request_stop 停止，并产生日志事件
```

这张图和 CTest 有一点重叠。

但它更聚焦第十版核心能力。

---

# 31. 05-structured-tcp-smoke.png

对应验证：

```text
启动 minikv_server 6400 127.0.0.1
用 minikv_client 发送 inline PING / QUIT
用 Python socket 发送 RESP PING / QUIT
检查 server stdout 结构化日志
```

结果：

```text
inline:
  PONG
  BYE

RESP:
  +PONG\r\n
  +BYE\r\n

server stdout:
  event=server_start
  event=server_hint
  event=tcp_listen
  event=tcp_client_accepted
```

意义：

```text
第十版服务入口真实可运行
inline 和 RESP 协议没有被生命周期改动破坏
结构化日志在真实 server 进程里能看到
```

---

# 32. 为什么 smoke 里 server 是强制停止

第十版支持 Ctrl+C / SIGTERM 请求 graceful stop。

但 smoke test 用 `Start-Process` 后台启动服务，为了保证测试结束一定清理干净，脚本在验证完成后执行：

```text
Stop-Process -Force
```

所以 `05-structured-tcp-smoke.png` 里主要验证：

```text
server_start
tcp_listen
tcp_client_accepted
inline / RESP 功能
```

而不是验证：

```text
event=tcp_stop
event=server_stopped
```

`event=tcp_stop` 已经由：

```text
04-graceful-stop-test.png
tcp_server_tests
```

专门验证。

---

# 33. 第十版文件变化总表

第十版相关增删改可以整理成：

```text
新增：
  tests/tcp_server_tests.cpp
  a/10/图片/
  a/10/解释/说明.md
  代码讲解记录/22-tcp-server-lifecycle.md
  代码讲解记录/23-version-10-tests-docs.md

修改：
  CMakeLists.txt
    版本 0.10.0
    新增 minikv_tcp_server_tests
    新增 CTest: tcp_server_tests

  README.md
    Version 10
    新增 graceful TCP shutdown
    新增 structured lifecycle logging
    新增 minikv_tcp_server_tests 目标说明
    新增 event=... 日志示例
    更新 Roadmap

  include/minikv/tcp_server.hpp
    Options 新增 LogHandler
    Options 新增 StopPredicate
    Options 新增 accept_poll_interval
    Options 新增 should_stop
    Options 新增 logger
    TcpServer 新增 request_stop()
    TcpServer 新增 stop_requested()
    TcpServer 新增 atomic stop_requested_

  src/tcp_server.cpp
    新增 log_event
    新增 endpoint_fields
    新增 to_timeval
    新增 wait_for_listener
    run() 改为可停止循环
    run() 新增 event=tcp_listen / event=tcp_client_accepted / event=tcp_stop

  src/server_main.cpp
    新增 SIGINT / SIGTERM 处理
    新增 shutdown_requested 标记
    新增 quote_value
    stdout / stderr 开启 unitbuf
    注入 options.should_stop 和 options.logger
    日志改为结构化 key=value

删除：
  无源码文件删除
```

---

# 34. 第十版之后当前能力总览

到第十版为止，mini-kv 已经具备：

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
CTest 自动化覆盖
运行归档
```

服务运行层现在是：

```text
server_main
 -> 配置 TcpServer::Options
 -> 设置 should_stop
 -> 设置 logger
 -> 安装信号处理
 -> 调用 server.run()

TcpServer
 -> bind/listen
 -> event=tcp_listen
 -> select 轮询 listener
 -> accept client
 -> event=tcp_client_accepted
 -> request_stop 或 should_stop 后退出
 -> event=tcp_stop
```

---

# 35. 第十版和前几版的关系

第七版到第九版主要在补协议：

```text
第七版：RESP parser
第八版：TCP server 接入 RESP
第九版：PING 和 RESP parser 加固
```

第十版开始补服务管理：

```text
TcpServer 生命周期
停止请求
结构化日志
```

所以项目路线从：

```text
数据能力
 -> 网络协议
 -> 服务可管理性
```

继续往前走。

---

# 第二十三次总结

第十版配套工作证明了四件事：

```text
tcp_server_tests
 -> TcpServer 能启动、能被 request_stop 停止、能输出 listen/stop 事件

CMake / CTest
 -> 新测试目标被纳入自动化验证

README
 -> 第十版服务生命周期能力和日志格式说明清楚

a/10
 -> 构建、测试、停止测试、真实 TCP smoke 都有截图和说明
```

一句话总结：

```text
第十版不仅写了可停止的 TcpServer，还把这个能力纳入测试、文档、真实 smoke 和运行归档，完成了从代码到验证的闭环。
```
