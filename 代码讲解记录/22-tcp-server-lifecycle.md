# 第二十二次讲解：第十版 TCP Server 生命周期、停止机制和结构化日志

第二十一次讲了第九版的测试、README、运行归档和整体增删改。

这次进入第十版核心代码：

```text
include/minikv/tcp_server.hpp
src/tcp_server.cpp
src/server_main.cpp
```

第十版主要做三件事：

```text
第一，让 TcpServer 可以被请求停止
第二，让 accept 循环可以周期性醒来检查停止条件
第三，让服务端输出结构化生命周期日志
```

第十版不是新增数据库命令。

它补的是“服务运行管理”：

```text
服务启动时能记录日志
客户端接入时能记录日志
外部请求停止时 run() 能退出
server_main 能通过 Ctrl+C / SIGTERM 请求停止
```

---

# 1. 为什么第十版要做服务生命周期

前面版本的 TCP server 是这样的：

```text
bind
listen
while (true)
 -> accept
 -> 开线程处理客户端
```

它能提供服务。

但它有一个明显限制：

```text
run() 一旦进入 accept 循环，就没有项目内的正常退出路径
```

测试和 smoke test 只能从外部强行结束进程。

第十版要补的就是：

```text
让 TcpServer 有停止请求
让 run() 能观察停止请求
让 server_main 能把 Ctrl+C / SIGTERM 变成停止请求
```

这样服务端从“能跑”推进到“更好管理”。

---

# 2. tcp_server.hpp 新增头文件

第十版在 `include/minikv/tcp_server.hpp` 增加：

```cpp
#include <atomic>
#include <chrono>
#include <functional>
```

它们分别服务于三个能力：

```text
atomic
 -> 保存跨线程停止标记

chrono
 -> 表示 accept 轮询间隔

functional
 -> 保存 should_stop 和 logger 回调
```

这说明第十版开始让 `TcpServer` 接收外部运行策略。

---

# 3. Options 新增回调类型

第十版在 `TcpServer::Options` 里新增：

```cpp
using LogHandler = std::function<void(const std::string&)>;
using StopPredicate = std::function<bool()>;
```

这两个类型表示：

```text
LogHandler
 -> TcpServer 有事件时，把日志字符串交给外部处理

StopPredicate
 -> TcpServer 每次循环时，问外部是否已经请求停止
```

为什么用 `std::function`？

因为它可以接收不同形式的 callable：

```text
普通函数
lambda
捕获变量的 lambda
函数对象
```

这让测试和生产入口都能用同一个接口。

---

# 4. Options 新增 accept_poll_interval

第十版新增：

```cpp
std::chrono::milliseconds accept_poll_interval{200};
```

这个值表示：

```text
accept 循环最长等 200ms
如果没有客户端连接，就醒来检查一次停止条件
```

为什么需要它？

因为传统阻塞式 `accept` 会一直卡住。

如果没有新客户端连进来，循环没有机会检查：

```text
stop_requested()
```

第十版用 `select` 先等 listener 是否可读。

如果超时，就回到循环顶部。

这样停止请求最多等待一个轮询间隔就能被观察到。

---

# 5. Options 新增 should_stop 和 logger

第十版新增：

```cpp
StopPredicate should_stop;
LogHandler logger;
```

它们是可选的。

如果外部没有设置：

```text
should_stop 为空
 -> 只看 TcpServer 自己的 request_stop()

logger 为空
 -> 不输出 TcpServer 内部日志
```

这样老的使用方式仍然成立：

```cpp
minikv::TcpServer server{store, options};
server.run();
```

只是有了更多可选能力。

---

# 6. TcpServer 新增公开方法

第十版新增：

```cpp
void request_stop();
bool stop_requested() const;
```

它们的职责是：

```text
request_stop()
 -> 从外部请求 server 停止

stop_requested()
 -> 判断当前是否应该停止
```

其中 `request_stop()` 适合：

```text
测试线程中请求停止
未来管理线程请求停止
未来嵌入式使用场景请求停止
```

`stop_requested()` 则被 `run()` 循环使用。

---

# 7. TcpServer 新增 atomic 停止标记

第十版在私有成员里新增：

```cpp
std::atomic_bool stop_requested_{false};
```

为什么不用普通 `bool`？

因为第十版会出现跨线程场景：

```text
线程 A:
 -> server.run()

线程 B:
 -> server.request_stop()
```

普通 `bool` 在多线程读写下会有数据竞争。

`std::atomic_bool` 可以保证：

```text
一个线程写入停止标记
另一个线程安全读取停止标记
```

---

# 8. request_stop 的实现

`src/tcp_server.cpp` 里新增：

```cpp
void TcpServer::request_stop() {
    stop_requested_.store(true);
}
```

它只做一件事：

```text
把内部停止标记设为 true
```

它不直接关闭 socket。

它也不强行杀客户端线程。

这是有意的。

第十版先实现的是：

```text
让 listener accept loop 停止
```

已经接入的客户端线程仍然按自己的连接生命周期退出。

---

# 9. stop_requested 的实现

```cpp
bool TcpServer::stop_requested() const {
    return stop_requested_.load() || (options_.should_stop && options_.should_stop());
}
```

它合并了两种停止来源：

```text
内部停止标记
 -> request_stop()

外部停止条件
 -> options.should_stop()
```

只要其中一个为真，就表示：

```text
server 应该停止 accept loop
```

这里的判断：

```cpp
options_.should_stop && options_.should_stop()
```

意思是：

```text
如果外部设置了 should_stop，就调用它
如果没有设置，就跳过
```

---

# 10. log_event 辅助函数

第十版在 `tcp_server.cpp` 匿名命名空间新增：

```cpp
void log_event(const TcpServer::Options& options, const std::string& message) {
    if (options.logger) {
        options.logger(message);
    }
}
```

这是一层小包装。

它避免每个地方都写：

```cpp
if (options.logger) {
    options.logger(...);
}
```

后续新增日志点时，只要调用：

```cpp
log_event(options_, "event=...");
```

即可。

---

# 11. endpoint_fields 辅助函数

第十版新增：

```cpp
std::string endpoint_fields(const TcpServer::Options& options) {
    return "host=" + options.host + " port=" + std::to_string(options.port);
}
```

它负责拼接通用字段：

```text
host=127.0.0.1 port=6400
```

这样多个日志事件可以保持统一格式：

```text
event=tcp_listen host=127.0.0.1 port=6400
event=tcp_client_accepted host=127.0.0.1 port=6400
event=tcp_stop host=127.0.0.1 port=6400
```

这就是结构化日志最简单的一种形式：

```text
key=value key=value key=value
```

---

# 12. 为什么日志用 key=value

旧日志更像自然语言：

```text
mini-kv server listening on 127.0.0.1:6379
```

人能看懂。

但程序解析不方便。

第十版改成：

```text
event=tcp_listen host=127.0.0.1 port=6379
```

好处是：

```text
能按 event 搜索
能按 host / port 过滤
测试可以稳定匹配 event=tcp_listen
以后接日志系统也更自然
```

第十版没有引入完整日志库。

它先建立项目自己的轻量日志约定。

---

# 13. to_timeval：chrono 到 timeval

`select` 使用的是 C 风格的 `timeval`。

第十版项目里使用的是 C++ 风格的：

```cpp
std::chrono::milliseconds
```

所以需要转换：

```cpp
timeval to_timeval(std::chrono::milliseconds timeout) {
    if (timeout < std::chrono::milliseconds::zero()) {
        timeout = std::chrono::milliseconds::zero();
    }

    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timeout);
    const auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(timeout - seconds);

    timeval value{};
    value.tv_sec = static_cast<long>(seconds.count());
    value.tv_usec = static_cast<long>(microseconds.count());
    return value;
}
```

逻辑是：

```text
负数间隔先压成 0
把总毫秒拆成秒和微秒
填入 timeval
```

这样 `accept_poll_interval` 就能被 `select` 使用。

---

# 14. 为什么负数 timeout 要压成 0

```cpp
if (timeout < std::chrono::milliseconds::zero()) {
    timeout = std::chrono::milliseconds::zero();
}
```

这是防御性处理。

虽然默认值是：

```text
200ms
```

测试里设置的是：

```text
10ms
```

但如果未来有人误设成负数，不应该把非法值传给 `select`。

压成 0 的含义是：

```text
不等待，立即检查 listener 状态
```

---

# 15. wait_for_listener 的作用

第十版新增：

```cpp
bool wait_for_listener(SocketHandle listener, std::chrono::milliseconds timeout)
```

它的作用是：

```text
等待 listener socket 在 timeout 内变为可读
```

对监听 socket 来说：

```text
可读
 -> 有客户端等待 accept
```

所以 `run()` 现在不是直接阻塞在 `accept`。

它变成：

```text
先 select 等一小段时间
如果没有连接，就回循环检查 stop
如果有连接，再 accept
```

---

# 16. fd_set 的准备

```cpp
fd_set read_fds;
FD_ZERO(&read_fds);
FD_SET(listener, &read_fds);
```

这三行是 `select` 的标准准备：

```text
创建一个 socket 集合
先清空
把 listener 放进去
```

然后把它交给：

```cpp
select(...)
```

让操作系统帮我们等待这个 socket 是否可读。

---

# 17. Windows 和非 Windows 的 select 差异

第十版写了平台分支：

```cpp
#ifdef _WIN32
    const int result = select(0, &read_fds, nullptr, nullptr, &wait_time);
#else
    const int result = select(listener + 1, &read_fds, nullptr, nullptr, &wait_time);
#endif
```

区别是：

```text
Windows:
 -> 第一个参数被忽略，可以传 0

Unix-like:
 -> 第一个参数需要是最大 fd + 1
```

这和前面 socket 类型差异一样：

```text
Windows 用 SOCKET
非 Windows 用 int fd
```

第十版继续保持跨平台结构。

---

# 18. select 被信号打断怎么办

`select` 返回负数表示错误。

但有一种情况不应该当成真正错误：

```text
被信号打断
```

第十版处理：

```cpp
if (result < 0) {
#ifdef _WIN32
    if (WSAGetLastError() == WSAEINTR) {
        return false;
    }
#else
    if (errno == EINTR) {
        return false;
    }
#endif
    throw std::runtime_error{socket_error_message("select failed")};
}
```

意思是：

```text
如果只是被中断
 -> 返回 false，让 run() 回循环检查 stop

如果是真错误
 -> 抛异常
```

这和第十版的信号退出目标配合得上。

---

# 19. wait_for_listener 的返回值

```cpp
return result > 0 && FD_ISSET(listener, &read_fds);
```

含义是：

```text
result > 0
 -> 至少有一个 socket 就绪

FD_ISSET(listener, &read_fds)
 -> 就绪的是 listener
```

返回 `true` 时：

```text
可以 accept
```

返回 `false` 时：

```text
没有连接，或者只是被信号打断
```

---

# 20. run() 的新版整体流程

第十版 `TcpServer::run()` 变成：

```cpp
void TcpServer::run() {
    NetworkRuntime runtime;
    SocketGuard listener = bind_listener(options_);
    log_event(options_, "event=tcp_listen " + endpoint_fields(options_));

    while (!stop_requested()) {
        if (!wait_for_listener(listener.get(), options_.accept_poll_interval)) {
            continue;
        }

        if (stop_requested()) {
            break;
        }

        SocketHandle client = accept(listener.get(), nullptr, nullptr);
        ...
    }

    log_event(options_, "event=tcp_stop " + endpoint_fields(options_));
}
```

核心变化是：

```text
while (true)
 -> while (!stop_requested())

直接 accept
 -> wait_for_listener 后再 accept

无退出日志
 -> 退出时记录 event=tcp_stop
```

---

# 21. 监听成功日志

```cpp
log_event(options_, "event=tcp_listen " + endpoint_fields(options_));
```

这行发生在：

```text
bind_listener 成功之后
进入 accept 循环之前
```

它表示：

```text
服务端已经完成 bind/listen
现在可以接受连接
```

测试里就用这个事件判断：

```text
server 已经真正启动
```

---

# 22. 为什么 accept 前还要再检查 stop

```cpp
if (stop_requested()) {
    break;
}
```

这个检查发生在 `select` 返回 true 后、`accept` 前。

它是一个小防御。

可能出现的情况是：

```text
select 返回
同时另一个线程请求 stop
```

这时没必要继续 accept。

直接 break 更符合停止语义。

---

# 23. 客户端接入日志

第十版在 accept 成功后新增：

```cpp
log_event(options_, "event=tcp_client_accepted " + endpoint_fields(options_));
```

每接入一个客户端，就记录一次：

```text
event=tcp_client_accepted host=127.0.0.1 port=6400
```

第十版 smoke test 里会看到两次：

```text
一次来自 bundled minikv_client
一次来自 Python RESP socket
```

这说明：

```text
inline 和 RESP 两条 TCP 连接都真正接入过 server
```

---

# 24. 停止日志 event=tcp_stop

循环退出后：

```cpp
log_event(options_, "event=tcp_stop " + endpoint_fields(options_));
```

这表示：

```text
TcpServer::run() 的 accept loop 已经退出
listener 即将由 SocketGuard 自动关闭
```

注意它不是：

```text
进程彻底退出
```

进程退出由 `server_main` 再记录：

```text
event=server_stopped
```

这是两层日志：

```text
TcpServer 层
 -> event=tcp_stop

server_main 进程入口层
 -> event=server_stopped
```

---

# 25. server_main.cpp 新增信号处理头文件

第十版在 `server_main.cpp` 新增：

```cpp
#include <csignal>
#include <string_view>
```

`csignal` 用于：

```text
SIGINT
SIGTERM
std::signal
std::sig_atomic_t
```

`string_view` 用于：

```text
quote_value(std::string_view value)
```

---

# 26. shutdown_requested 全局标记

```cpp
volatile std::sig_atomic_t shutdown_requested = 0;
```

这是信号处理函数和主循环之间共享的标记。

为什么不用普通 `bool`？

因为信号处理函数能在程序任意位置被打断执行。

C/C++ 对信号处理函数里能安全做的事情限制很严格。

`std::sig_atomic_t` 是适合在信号处理函数里读写的整数类型。

---

# 27. 信号处理函数只设置标记

```cpp
void handle_shutdown_signal(int) {
    shutdown_requested = 1;
}
```

这里没有：

```text
打印日志
关闭 socket
释放资源
修改复杂对象
```

只做一件事：

```text
设置 shutdown_requested = 1
```

这是正确方向。

信号处理函数越简单越安全。

---

# 28. should_stop 把信号标记暴露给 TcpServer

```cpp
bool should_stop() {
    return shutdown_requested != 0;
}
```

后面会赋给：

```cpp
options.should_stop = should_stop;
```

于是 `TcpServer::stop_requested()` 会检查：

```text
内部 request_stop 标记
外部 should_stop 函数
```

当用户按 `Ctrl+C` 时：

```text
handle_shutdown_signal
 -> shutdown_requested = 1
 -> should_stop() 返回 true
 -> TcpServer::run() 退出
```

---

# 29. install_shutdown_handlers

```cpp
void install_shutdown_handlers() {
    std::signal(SIGINT, handle_shutdown_signal);
    std::signal(SIGTERM, handle_shutdown_signal);
}
```

它注册两个停止信号：

```text
SIGINT
 -> 通常来自 Ctrl+C

SIGTERM
 -> 通常来自外部请求进程终止
```

第十版不是直接拦截所有信号。

只处理最常见的停止请求。

---

# 30. quote_value 的作用

```cpp
std::string quote_value(std::string_view value) {
    std::string quoted = "\"";
    for (const char ch : value) {
        if (ch == '\\' || ch == '"') {
            quoted.push_back('\\');
        }
        quoted.push_back(ch);
    }
    quoted.push_back('"');
    return quoted;
}
```

它把日志字段值包装成带引号的字符串。

例如：

```text
SET name mini-kv
```

会变成：

```text
"SET name mini-kv"
```

如果值里包含：

```text
\ 或 "
```

会先加反斜杠转义。

这样日志更稳定。

---

# 31. 为什么 server_main 打开 unitbuf

第十版新增：

```cpp
std::cout << std::unitbuf;
std::cerr << std::unitbuf;
```

意思是：

```text
每次输出后自动 flush
```

为什么需要？

因为 smoke test 会用后台进程启动 server，并把 stdout 重定向到文件。

如果不自动 flush，日志可能还在缓冲区里，测试脚本短时间内读不到：

```text
event=tcp_listen
```

开启 `unitbuf` 后，生命周期日志能及时写入文件。

---

# 32. server_main 设置 should_stop 和 logger

第十版新增：

```cpp
minikv::TcpServer::Options options;
options.should_stop = should_stop;
options.logger = [](const std::string& message) {
    std::cout << message << '\n';
};
```

这里把 `server_main` 和 `TcpServer` 接起来：

```text
TcpServer 想知道是否停止
 -> 调用 should_stop()

TcpServer 想输出日志
 -> 调用 logger(message)
 -> server_main 打印到 stdout
```

这保持了模块边界：

```text
TcpServer 不直接依赖 std::cout
server_main 决定日志写到哪里
```

---

# 33. WAL replay 日志结构化

旧版 WAL 日志是：

```text
WAL: path (N records replayed)
```

第十版改成：

```cpp
std::cout << "event=wal_replay path=" << quote_value(wal->path().string()) << " records=" << replayed
          << '\n';
```

输出类似：

```text
event=wal_replay path="data\mini-kv.wal" records=12
```

这样 WAL replay 也进入同一套结构化日志风格。

---

# 34. server_start 和 server_hint

第十版把启动提示改成：

```cpp
std::cout << "event=server_start host=" << options.host << " port=" << options.port
          << " protocol=inline,resp" << '\n';
std::cout << "event=server_hint command=" << quote_value("SET name mini-kv") << '\n';
```

对应输出：

```text
event=server_start host=127.0.0.1 port=6400 protocol=inline,resp
event=server_hint command="SET name mini-kv"
```

第一行是机器更容易读的状态。

第二行保留人类提示：

```text
可以试试 SET name mini-kv
```

但也用 key=value 形式表达。

---

# 35. server_stopped

`server.run()` 返回后：

```cpp
std::cout << "event=server_stopped host=" << options.host << " port=" << options.port << '\n';
```

这表示：

```text
TcpServer::run() 已经退出
server_main 即将结束 main()
```

如果是 Ctrl+C 正常停止，理想日志顺序是：

```text
event=server_start ...
event=server_hint ...
event=tcp_listen ...
event=tcp_stop ...
event=server_stopped ...
```

第十版 smoke 因为是测试清理时强制停进程，所以看到的是启动和接入日志。

`tcp_server_tests` 则专门验证 `event=tcp_stop`。

---

# 36. 第十版核心链路总览

第十版之后，服务启动链路是：

```text
server_main
 -> 设置 unitbuf
 -> 设置 options.should_stop
 -> 设置 options.logger
 -> install_shutdown_handlers
 -> 打印 event=server_start
 -> server.run()
```

`TcpServer::run()` 链路是：

```text
NetworkRuntime
 -> bind_listener
 -> event=tcp_listen
 -> while !stop_requested
 -> select 等 listener 可读或超时
 -> accept
 -> event=tcp_client_accepted
 -> detached client thread
 -> 停止后 event=tcp_stop
```

停止链路是：

```text
request_stop()
 -> stop_requested_ = true
 -> run() 下次轮询醒来
 -> stop_requested() 返回 true
 -> accept loop 退出
```

Ctrl+C 链路是：

```text
SIGINT
 -> handle_shutdown_signal
 -> shutdown_requested = 1
 -> should_stop() 返回 true
 -> TcpServer::run() 退出
 -> server_main 打印 event=server_stopped
```

---

# 37. 第十版没有改变的地方

第十版没有修改：

```text
CommandProcessor 命令语义
Store 数据结构
RESP parser 解析规则
WAL / Snapshot 格式
client 交互方式
```

它主要围绕：

```text
TcpServer 的运行循环
server_main 的进程入口
测试和文档
```

这让第十版风险比较集中。

从功能角度看：

```text
用户的 SET / GET / PING 行为不变
服务端的生命周期管理变强
```

---

# 第二十二次总结

第十版核心代码把 TCP server 从：

```text
只能一直 accept
```

推进到：

```text
可以被请求停止
可以周期性检查停止条件
可以输出结构化生命周期日志
```

一句话总结：

```text
第十版没有增加新的数据命令，而是让 mini-kv 的网络服务从“能跑起来”更进一步变成“能被管理、能被观察、能在测试里优雅退出”的服务。
```
