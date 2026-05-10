# 第三十四次讲解：第十六版外部 RESP-over-TCP 测试核心

第三十三次讲了第十五版测试、README、归档和整体增删改。

这次进入第十六版新增测试核心：

```text
tests/tcp_resp_tests.cpp
```

第十六版主要补一件事：

```text
像外部 Redis/RESP 客户端一样，通过原始 TCP socket 发送 RESP pipeline，
验证 mini-kv server 的 RESP-over-TCP 链路能真实工作。
```

前面第八版已经让 TCP server 支持 RESP。
第十六版不改生产代码，而是补一层更贴近真实客户端的验证：

```text
启动 TcpServer
 -> 用 raw socket 连接 server
 -> 发送多条 RESP 请求拼在一起的 pipeline
 -> 一次性读完整响应
 -> 精确比对 RESP frame
 -> 检查连接日志和连接统计
```

---

# 1. 为什么需要 tcp_resp_tests

之前已经有：

```text
resp_tests
 -> 测 RespParser 本身

tcp_server_tests
 -> 测 inline TCP 请求、请求上限、生命周期日志、localhost 兼容
```

但是还缺一个场景：

```text
真实 TCP 连接里发送 RESP 请求
并且一次连接里连续发送多条请求
```

这就是第十六版新增 `tcp_resp_tests` 的位置。

它证明的不是 parser 单点能力，而是完整链路：

```text
socket bytes
 -> TcpServer pending buffer
 -> RESP 自动识别
 -> RespParser::parse_command
 -> CommandProcessor::execute
 -> RespParser::to_resp_response
 -> socket response bytes
```

---

# 2. 文件入口

新增文件：

```text
tests/tcp_resp_tests.cpp
```

开头包含两个项目头文件：

```cpp
#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
```

这里没有直接 include `resp.hpp`。

原因是：

```text
测试不直接调用 RespParser
而是把 RESP 字节发给 TcpServer
让 TcpServer 自己识别和解析
```

这比直接测 parser 更接近真实外部客户端。

---

# 3. 跨平台 socket 分支

测试需要直接创建 socket，所以保留 Windows 和类 Unix 两套 include：

```cpp
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <cerrno>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif
```

Windows 下 socket API 要先 `WSAStartup`。
Linux/macOS 下不需要。

所以测试把平台差异收进几个小工具里：

```text
TestNetworkRuntime
close_test_socket
socket_error_message
address_error_message
```

主测试逻辑就不用到处写 `#ifdef`。

---

# 4. TestNetworkRuntime

Windows 版本：

```cpp
class TestNetworkRuntime {
public:
    TestNetworkRuntime() {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            throw std::runtime_error{"WSAStartup failed"};
        }
    }

    ~TestNetworkRuntime() {
        WSACleanup();
    }
};
```

它的作用是：

```text
测试开始 socket 通信前初始化 Winsock
测试结束自动清理 Winsock
```

类 Unix 版本：

```cpp
class TestNetworkRuntime {
public:
    TestNetworkRuntime() = default;
};
```

它什么也不做。

这样 `exchange_raw` 里只需要写：

```cpp
TestNetworkRuntime runtime;
```

就同时兼容 Windows 和其他系统。

---

# 5. TestSocketGuard：socket RAII

测试里新增：

```cpp
class TestSocketGuard
```

它保存一个 socket 句柄：

```cpp
TestSocket socket_;
```

析构时自动关闭：

```cpp
~TestSocketGuard() {
    close_test_socket(socket_);
}
```

这个类的意义是：

```text
只要函数退出，socket 就会自动释放
即使中间 throw 或 assert 前出现异常，也不容易泄漏 socket
```

它禁止拷贝：

```cpp
TestSocketGuard(const TestSocketGuard&) = delete;
TestSocketGuard& operator=(const TestSocketGuard&) = delete;
```

但允许移动：

```cpp
TestSocketGuard(TestSocketGuard&& other) noexcept : socket_(other.release()) {}
```

原因是 `connect_test_socket` 需要把已经连接成功的 socket 返回给调用方。

---

# 6. release 的作用

移动构造里调用：

```cpp
other.release()
```

`release` 做两件事：

```cpp
const TestSocket socket = socket_;
socket_ = invalid_test_socket;
return socket;
```

含义是：

```text
把 socket 所有权交出去
原对象变成空句柄
```

否则移动后两个 guard 都以为自己拥有同一个 socket，析构时会重复关闭。

---

# 7. TestAddrInfoGuard

`getaddrinfo` 返回的是链表：

```cpp
addrinfo* raw_info = nullptr;
```

用完必须调用：

```cpp
freeaddrinfo(raw_info);
```

测试用：

```cpp
class TestAddrInfoGuard
```

把释放动作放进析构函数。

这和 `TestSocketGuard` 一样，都是测试里的小型 RAII：

```text
资源获取
 -> 对象持有
 -> 离开作用域自动释放
```

---

# 8. contains_log

测试需要判断日志里有没有某个事件：

```cpp
bool contains_log(const std::vector<std::string>& logs, std::string_view needle)
```

实现用：

```cpp
std::any_of(logs.begin(), logs.end(), ...)
```

只要任意一行包含目标文本，就返回 true。

比如：

```text
event=tcp_listen
event=tcp_client_accepted
event=tcp_client_closed
event=tcp_stop
```

这个函数让后面的断言更像业务语义，而不是每次手写循环。

---

# 9. make_resp_array

测试自己构造 RESP 请求：

```cpp
std::string make_resp_array(std::initializer_list<std::string_view> arguments)
```

比如传入：

```cpp
make_resp_array({"SET", "name", "resp-client"})
```

会生成：

```text
*3\r\n
$3\r\n
SET\r\n
$4\r\n
name\r\n
$11\r\n
resp-client\r\n
```

代码核心是：

```cpp
std::string encoded = "*" + std::to_string(arguments.size()) + "\r\n";
for (const auto argument : arguments) {
    encoded += "$" + std::to_string(argument.size()) + "\r\n";
    encoded += argument;
    encoded += "\r\n";
}
```

RESP array-of-bulk-strings 的格式就是：

```text
*参数数量\r\n
$参数1长度\r\n
参数1\r\n
$参数2长度\r\n
参数2\r\n
...
```

这个 helper 避免测试里手写大量 `\r\n`，也降低长度写错的概率。

---

# 10. connect_test_socket：外部客户端式连接

连接函数：

```cpp
TestSocketGuard connect_test_socket(std::string_view host, std::uint16_t port)
```

它先设置：

```cpp
addrinfo hints{};
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
```

`AF_UNSPEC` 的意思是：

```text
不预设 IPv4 或 IPv6
让系统按 host 返回可用地址
```

这延续了第十三版 hostname 兼容测试的思路。

然后调用：

```cpp
getaddrinfo(host, port_text, &hints, &raw_info)
```

系统可能返回多个候选地址。
测试逐个尝试：

```cpp
for (addrinfo* address = addresses.get(); address != nullptr; address = address->ai_next) {
    TestSocketGuard socket{::socket(...)};
    if (connect(socket.get(), address->ai_addr, ...) == 0) {
        return socket;
    }
}
```

只要有一个地址连接成功，就返回已连接 socket。

---

# 11. 为什么 connect_test_socket 返回 TestSocketGuard

返回的是：

```cpp
TestSocketGuard
```

不是裸 socket。

这样调用方拿到的仍然是自动释放资源：

```cpp
TestSocketGuard socket = connect_test_socket(host, port);
```

`exchange_raw` 结束时，socket 会自动关闭。

这让测试失败路径也更干净。

---

# 12. exchange_raw：发送原始字节

核心函数：

```cpp
std::string exchange_raw(std::string_view host, std::uint16_t port, std::string_view payload)
```

它模拟一个最小外部客户端：

```text
连接 server
 -> send payload 所有字节
 -> shutdown 写端
 -> recv 直到服务端关闭连接
 -> 返回完整响应字节
```

第一步：

```cpp
TestNetworkRuntime runtime;
TestSocketGuard socket = connect_test_socket(host, port);
```

保证 socket 环境已初始化，并连接到服务端。

---

# 13. 为什么 send 要循环

发送 payload 时不是一次 `send` 就完事：

```cpp
std::string_view remaining = payload;
while (!remaining.empty()) {
    const int sent = send(...);
    if (sent <= 0) {
        throw std::runtime_error{socket_error_message("send failed")};
    }
    remaining.remove_prefix(static_cast<std::size_t>(sent));
}
```

原因是：

```text
send 返回值只表示本次实际写入了多少字节
它可能小于 payload.size()
```

所以测试必须循环，直到所有 RESP 字节都发出去。

这比假设一次 send 成功更可靠。

---

# 14. 为什么发送后 shutdown 写端

发送完成后：

```cpp
shutdown(socket.get(), SD_SEND);
```

或类 Unix 下：

```cpp
shutdown(socket.get(), SHUT_WR);
```

意思是：

```text
客户端告诉服务端：我不会再发送新数据了
但我还要继续接收你的响应
```

这样服务端处理完 `QUIT` 后会关闭连接。
客户端就可以 `recv` 到 0，知道响应读完了。

---

# 15. recv 直到连接关闭

读取响应：

```cpp
std::string response;
char buffer[256]{};
while (true) {
    const int received = recv(...);
    if (received < 0) {
        throw std::runtime_error{socket_error_message("recv failed")};
    }
    if (received == 0) {
        break;
    }
    response.append(buffer, static_cast<std::size_t>(received));
}
```

这里不是只读一次。

原因和 send 类似：

```text
TCP 是字节流
一次 recv 不保证拿到完整响应
```

所以测试循环读，直到连接关闭。

最后返回完整的 RESP 响应字节。

---

# 16. main：创建内存 Store

测试入口：

```cpp
int main() {
    using namespace std::chrono_literals;

    minikv::Store store;
```

这里直接创建一个内存 `Store`。

没有 WAL。
没有 snapshot 文件。

因为这次关注点是：

```text
RESP-over-TCP 协议链路
```

不是持久化。

---

# 17. TcpServer 使用临时端口

测试设置：

```cpp
minikv::TcpServer::Options options;
options.host = "127.0.0.1";
options.port = 0;
options.accept_poll_interval = 10ms;
```

`port = 0` 表示：

```text
让操作系统自动分配一个空闲端口
```

这样测试不依赖固定端口，也避免端口冲突。

`accept_poll_interval = 10ms` 让测试中的 server 停止更快。

---

# 18. 捕获结构化日志

测试把日志收集到 vector：

```cpp
std::mutex logs_mutex;
std::vector<std::string> logs;
options.logger = [&](const std::string& message) {
    std::lock_guard lock{logs_mutex};
    logs.push_back(message);
};
```

加锁是因为：

```text
server 在后台线程写日志
main 测试线程读日志
```

两个线程同时访问 `logs`，必须用 mutex 保护。

---

# 19. 后台线程运行 server

测试创建：

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

`server.run()` 是阻塞函数。

所以必须放到后台线程。

如果后台线程抛异常，就保存到：

```cpp
std::exception_ptr failure;
```

测试后面会重新抛出，避免 server 线程失败被悄悄吞掉。

---

# 20. 等待 tcp_listen

测试不能一启动线程就马上连接。

因为 server 可能还没 listen 完成。

所以轮询日志：

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

最多等：

```text
100 * 10ms = 1 秒
```

看到 `event=tcp_listen` 后，说明 server 已经可以接收连接。

---

# 21. 读取真实绑定端口

server 监听成功后：

```cpp
const auto bound_port = server.bound_port();
assert(bound_port > 0);
```

因为配置里 `port = 0`，真实端口由操作系统分配。

`bound_port()` 拿到这个真实端口后，测试客户端才能连接。

---

# 22. 构造 RESP pipeline

第十六版核心请求：

```cpp
const std::string request = make_resp_array({"PING", "external"}) +
                            make_resp_array({"SET", "name", "resp-client"}) +
                            make_resp_array({"GET", "name"}) +
                            make_resp_array({"SIZE"}) +
                            make_resp_array({"QUIT"});
```

这里不是发一条、收一条。

而是把五条 RESP request 直接拼成一个 payload：

```text
PING external
SET name resp-client
GET name
SIZE
QUIT
```

这就是 pipeline。

它验证 TCP server 的 pending buffer 能连续解析多条 RESP 命令。

---

# 23. 为什么选择这五条命令

这五条覆盖了几种不同返回类型：

```text
PING external
 -> bulk string: external

SET name resp-client
 -> simple string: OK inserted

GET name
 -> bulk string: resp-client

SIZE
 -> integer: 1

QUIT
 -> simple string: BYE，并触发连接关闭
```

也就是一次测试覆盖：

```text
bulk string
simple string
integer
连接关闭
```

比只测 `PING` 更有价值。

---

# 24. 预期 RESP 响应

测试写死精确响应：

```cpp
const std::string expected = "$8\r\nexternal\r\n"
                             "+OK inserted\r\n"
                             "$11\r\nresp-client\r\n"
                             ":1\r\n"
                             "+BYE\r\n";
```

逐段拆开：

```text
$8\r\nexternal\r\n
 -> bulk string，长度 8，内容 external

+OK inserted\r\n
 -> simple string

$11\r\nresp-client\r\n
 -> bulk string，长度 11，内容 resp-client

:1\r\n
 -> integer

+BYE\r\n
 -> simple string
```

这里是精确字节比对。

所以如果 RESP 格式多一个空格、少一个 `\r\n`、长度算错，测试都会失败。

---

# 25. exchange_raw 执行请求

发送并接收：

```cpp
const auto response = exchange_raw("127.0.0.1", bound_port, request);
assert(response == expected);
```

这一步证明：

```text
raw socket 输入的 RESP pipeline
 -> server 能完整处理
 -> 输出 RESP frame 完全符合预期
```

它不是只看“看起来差不多”。

它要求响应字节完全一致。

---

# 26. 等待连接关闭日志

请求里最后一条是 `QUIT`。

所以测试接着等待：

```cpp
event=tcp_client_closed
```

代码：

```cpp
bool closed = false;
for (int attempt = 0; attempt < 100; ++attempt) {
    {
        std::lock_guard lock{logs_mutex};
        closed = contains_log(logs, "event=tcp_client_closed");
    }
    const auto stats = server.connection_stats();
    if (closed && stats.active_connections == 0 && stats.total_connections == 1) {
        break;
    }
    std::this_thread::sleep_for(10ms);
}
```

这里同时等两个条件：

```text
日志已经记录 client closed
连接统计 active_connections 归零
```

这样不会只验证协议响应，却漏掉连接生命周期问题。

---

# 27. 检查连接统计

等待结束后：

```cpp
assert(closed);
auto stats = server.connection_stats();
assert(stats.total_connections == 1);
assert(stats.active_connections == 0);
assert(stats.peak_connections == 1);
```

含义：

```text
total_connections == 1
 -> 只有一个外部客户端连接过

active_connections == 0
 -> QUIT 后连接已经关闭

peak_connections == 1
 -> 峰值同时连接数为 1
```

这复用了第十一版的 connection metrics 能力。

---

# 28. 停止 server

测试结束前：

```cpp
server.request_stop();
server_thread.join();
```

这延续第十版的生命周期设计：

```text
request_stop 设置停止标记
accept loop 醒来后退出
join 等后台线程完全结束
```

这样测试不会留下后台 server 线程。

---

# 29. 重新抛出后台线程异常

server 线程结束后：

```cpp
if (failure) {
    std::rethrow_exception(failure);
}
```

如果 `server.run()` 中抛过异常，测试主线程会重新抛出。

这样 CTest 能看到失败。

否则后台线程出错可能只导致测试异常终止或被掩盖。

---

# 30. 检查 stop_requested

测试继续断言：

```cpp
assert(server.stop_requested());
```

这证明：

```text
测试是通过正式 request_stop 机制停掉 server
```

不是靠进程结束强制销毁。

然后再次检查连接统计：

```cpp
stats = server.connection_stats();
assert(stats.total_connections == 1);
assert(stats.active_connections == 0);
assert(stats.peak_connections == 1);
```

确保停止后统计仍然稳定。

---

# 31. 最后检查日志

最后加锁读取日志：

```cpp
std::lock_guard lock{logs_mutex};
assert(contains_log(logs, "event=tcp_listen"));
assert(contains_log(logs, "event=tcp_client_accepted"));
assert(contains_log(logs, "event=tcp_client_closed"));
assert(contains_log(logs, "event=tcp_stop"));
assert(contains_log(logs, "total_connections=1"));
assert(contains_log(logs, "peak_connections=1"));
```

这些断言覆盖：

```text
server 成功 listen
客户端连接被接受
客户端连接被关闭
server 被正常停止
累计连接数为 1
峰值连接数为 1
```

也就是说，第十六版不是只测 RESP 正确，还顺手确认生命周期日志没有丢。

---

# 32. 第十六版新增测试的边界

这个测试不覆盖所有 RESP 命令。

它主要覆盖：

```text
外部 socket 连接
RESP pipeline
连续解析多条命令
simple string / bulk string / integer 响应格式
QUIT 关闭连接
连接日志和统计
```

还没有覆盖：

```text
RESP null bulk string
RESP error
DEL / EXPIRE / TTL / SAVE / LOAD 的外部 pipeline
并发多个 RESP 客户端
```

这些可以作为后续版本继续扩展。

---

# 33. 第十六版测试链路总结

整体链路是：

```text
tcp_resp_tests
 -> 创建 Store
 -> 创建 TcpServer，端口为 0
 -> 后台线程运行 server.run()
 -> 等待 event=tcp_listen
 -> bound_port() 获取真实端口
 -> make_resp_array 生成 RESP pipeline
 -> exchange_raw 用 socket 发送原始字节
 -> recv 读完整 RESP 响应
 -> assert 精确匹配响应
 -> 等待 event=tcp_client_closed
 -> 检查 connection_stats
 -> request_stop
 -> join server_thread
 -> 检查 event=tcp_stop
```

---

# 第三十四次总结

第十六版新增的 `tcp_resp_tests.cpp` 是一条外部客户端式的协议链路测试。

一句话总结：

```text
第十六版用原始 socket 向 TcpServer 发送 pipelined RESP 请求，并精确校验响应帧、连接日志和连接统计，证明 mini-kv 的 RESP-over-TCP 能被真实外部客户端按字节流方式访问。
```
