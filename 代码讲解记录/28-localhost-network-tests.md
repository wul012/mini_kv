# 第二十八次讲解：第十三版 localhost 网络兼容测试

第二十七次讲了第十二版测试、README、运行归档和整体增删改。

这次进入第十三版核心改动：

```text
tests/tcp_server_tests.cpp
```

第十三版没有大改生产代码，而是补强网络测试。
它主要做三件事：

```text
第一，让测试客户端不再写死 IPv4 socket
第二，用 getaddrinfo(AF_UNSPEC) 连接测试服务端
第三，新增 localhost 绑定和 localhost 连接场景
```

第十二版已经让服务端有请求上限、客户端有 timeout。
第十三版继续补的是：

```text
同一个 TCP server
不只在 127.0.0.1 场景能跑
也要在 localhost 这种真实使用更常见的 hostname 场景能跑
```

---

# 1. 为什么第十三版先做测试兼容性

README 在第十二版之后的路线里写过：

```text
Add broader network compatibility tests
```

原因是 mini-kv 的网络层已经有了不少能力：

```text
inline TCP
RESP TCP
request_stop
connection metrics
request limit
client timeout
```

但第十二版之前的 `tcp_server_tests` 主要还是：

```text
server 绑定 127.0.0.1
测试客户端连接 127.0.0.1
```

这能证明 IPv4 loopback 可用。
但真实使用时，用户经常会写：

```text
localhost
```

在 Windows、Linux、macOS 上，`localhost` 可能解析出：

```text
127.0.0.1
::1
```

也就是可能走 IPv4，也可能走 IPv6。

第十三版的目标不是立刻做很大的网络重构，而是先把测试客户端改得更接近真实网络解析方式。

---

# 2. 新增 netdb.h

第十三版在非 Windows 分支新增：

```cpp
#include <netdb.h>
```

这个头文件提供：

```text
getaddrinfo
freeaddrinfo
gai_strerror
addrinfo
```

Windows 分支已经通过：

```cpp
#include <ws2tcpip.h>
```

拿到了同类 API。

这让测试代码可以在两个平台都使用：

```text
getaddrinfo
```

来解析 host 和 port。

---

# 3. address_error_message

第十三版新增了跨平台地址解析错误格式化函数。

Windows：

```cpp
std::string address_error_message(int result) {
    return "getaddrinfo failed: " + std::to_string(result);
}
```

非 Windows：

```cpp
std::string address_error_message(int result) {
    return std::string{"getaddrinfo failed: "} + gai_strerror(result);
}
```

为什么 Windows 直接输出数字？

因为 Windows 的 `getaddrinfo` 错误码通常按数字查看。
非 Windows 则有 `gai_strerror` 能转成人类可读文本。

测试里如果解析失败，会抛：

```text
getaddrinfo failed: ...
```

这比单纯 `connect failed` 更容易定位问题。

---

# 4. TestSocketGuard

第十三版新增：

```cpp
class TestSocketGuard
```

它的职责是：

```text
持有测试 socket
析构时自动 close
禁止复制
允许移动
```

核心成员是：

```cpp
TestSocket socket_;
```

析构函数是：

```cpp
~TestSocketGuard() {
    close_test_socket(socket_);
}
```

这样测试中途如果抛异常，socket 也会自动关闭。

这和生产代码里的 `SocketGuard` 思路一致：

```text
资源由对象持有
对象析构释放资源
```

第十三版把测试代码也往这个方向收拢。

---

# 5. 为什么 TestSocketGuard 要支持移动

`connect_test_socket` 会创建 socket，然后把成功连接的 socket 返回给调用方。

所以 `TestSocketGuard` 需要移动构造：

```cpp
TestSocketGuard(TestSocketGuard&& other) noexcept : socket_(other.release()) {}
```

移动赋值：

```cpp
TestSocketGuard& operator=(TestSocketGuard&& other) noexcept
```

以及内部：

```cpp
TestSocket release()
```

这条链路是：

```text
connect_test_socket 内部创建 socket
 -> connect 成功
 -> return socket
 -> 调用方接管 socket 所有权
 -> 原对象 release 后不再重复 close
```

如果不做移动所有权，返回时很容易出现：

```text
同一个 socket 被关闭两次
或者返回后 socket 已经被局部对象析构关闭
```

RAII 的关键不只是“会释放”，还包括“所有权转移清楚”。

---

# 6. TestAddrInfoGuard

第十三版新增：

```cpp
class TestAddrInfoGuard
```

它负责释放 `getaddrinfo` 返回的链表：

```cpp
~TestAddrInfoGuard() {
    if (info_ != nullptr) {
        freeaddrinfo(info_);
    }
}
```

`getaddrinfo` 返回的是一串候选地址。
它们可能包含：

```text
IPv4 地址
IPv6 地址
不同协议组合
```

这块内存必须通过：

```text
freeaddrinfo
```

释放。

测试代码用 `TestAddrInfoGuard` 后，不需要在每个错误分支手动写 `freeaddrinfo`。

---

# 7. connect_test_socket 的角色

第十三版新增的关键函数是：

```cpp
TestSocketGuard connect_test_socket(std::string_view host, std::uint16_t port)
```

它取代了第十二版测试里固定 IPv4 的逻辑。

第十二版旧思路是：

```text
创建 AF_INET socket
inet_pton("127.0.0.1")
connect sockaddr_in
```

第十三版新思路是：

```text
getaddrinfo(host, port, AF_UNSPEC)
 -> 遍历所有候选地址
 -> 为每个候选地址创建 socket
 -> 尝试 connect
 -> 第一个成功的 socket 返回
```

这里的重点是：

```cpp
hints.ai_family = AF_UNSPEC;
```

`AF_UNSPEC` 的意思是：

```text
不预先限定 IPv4 或 IPv6
让系统按 host 解析结果返回可用地址
```

这正是测试 `localhost` 兼容性的基础。

---

# 8. connect_test_socket 的循环

核心循环是：

```cpp
for (addrinfo* address = addresses.get(); address != nullptr; address = address->ai_next) {
    TestSocketGuard socket{::socket(address->ai_family, address->ai_socktype, address->ai_protocol)};
    if (socket.get() == invalid_test_socket) {
        continue;
    }

    if (connect(socket.get(), address->ai_addr, static_cast<int>(address->ai_addrlen)) == 0) {
        return socket;
    }
}
```

这段代码的含义是：

```text
系统给几个候选地址
测试就依次试几个
某个地址能连上
就用这个地址
```

如果第一个候选地址不可用，不代表整个 host 不可用。
继续试后续地址，能减少平台差异带来的误判。

最后如果都连不上：

```cpp
throw std::runtime_error{socket_error_message("connect failed")};
```

也就是明确告诉测试：

```text
地址解析成功了
但所有候选地址都连接失败
```

---

# 9. exchange_inline 新增 host 参数

第十二版的 `exchange_inline` 只接收端口：

```text
exchange_inline(port, payload)
```

第十三版变成：

```cpp
std::string exchange_inline(std::string_view host, std::uint16_t port, std::string_view payload)
```

调用方式也随之变成：

```cpp
exchange_inline("127.0.0.1", bound_port, "PING\nQUIT\n");
exchange_inline("localhost", localhost_port, "PING localhost\nQUIT\n");
```

这样同一个测试发送函数可以覆盖两类场景：

```text
明确 IPv4 地址
hostname 解析地址
```

测试代码没有复制一套发送/接收逻辑，维护成本更低。

---

# 10. 原有 127.0.0.1 场景仍然保留

第十三版没有删除原来的 IPv4 loopback 场景。
原有测试仍然做：

```text
options.host = "127.0.0.1"
options.port = 0
options.max_request_bytes = 8
```

然后验证：

```text
PING / QUIT 正常
10 字节无换行请求被拒绝
event=tcp_request_rejected 出现
pending_bytes=10
max_request_bytes=8
connection metrics 正常
```

区别只是连接方式变成：

```cpp
exchange_inline("127.0.0.1", bound_port, ...)
```

也就是说，第十三版是“扩展测试面”，不是替换掉旧保障。

---

# 11. 新增 localhost 服务端

第十三版新增第二个独立 server 场景：

```cpp
minikv::Store localhost_store;
minikv::TcpServer::Options localhost_options;
localhost_options.host = "localhost";
localhost_options.port = 0;
localhost_options.accept_poll_interval = 10ms;
```

这里使用：

```text
host = localhost
port = 0
```

`port = 0` 的意思是：

```text
让系统分配一个临时可用端口
```

启动后再用：

```cpp
localhost_server.bound_port()
```

拿到真实端口。

这样测试不会和本机已有服务抢固定端口。

---

# 12. localhost 日志收集

和前面的主测试一样，localhost 场景也有独立日志数组：

```cpp
std::mutex localhost_logs_mutex;
std::vector<std::string> localhost_logs;
```

logger 写入：

```cpp
localhost_options.logger = [&](const std::string& message) {
    std::lock_guard localhost_lock{localhost_logs_mutex};
    localhost_logs.push_back(message);
};
```

为什么不用前面的 `logs`？

因为这是第二个 server。
独立日志能避免两个场景的断言互相干扰。

测试要看得清楚：

```text
前一个 server 的日志
localhost server 的日志
```

分别证明各自场景通过。

---

# 13. localhost server 线程

第十三版新建：

```cpp
minikv::TcpServer localhost_server{localhost_store, localhost_options};
std::exception_ptr localhost_failure;
std::thread localhost_thread{[&] {
    try {
        localhost_server.run();
    } catch (...) {
        localhost_failure = std::current_exception();
    }
}};
```

这和原来的 server 测试模式一致。

这样如果 server 线程里抛异常，不会直接丢失，而是保存到：

```text
localhost_failure
```

线程 join 后再：

```cpp
if (localhost_failure) {
    std::rethrow_exception(localhost_failure);
}
```

测试失败原因能被主线程重新抛出。

---

# 14. 等待 localhost listen

测试通过日志等待启动成功：

```cpp
localhost_started = contains_log(localhost_logs, "event=tcp_listen");
```

最多轮询 100 次，每次 10ms。

这说明：

```text
localhost server 真的进入监听状态
```

再拿端口：

```cpp
const auto localhost_port = localhost_server.bound_port();
assert(localhost_port > 0);
```

如果没有成功监听，`bound_port()` 就没有可靠意义。

所以顺序必须是：

```text
先等 event=tcp_listen
再读 bound_port
再发请求
```

---

# 15. localhost 请求验证

第十三版新增请求：

```cpp
const auto localhost_response =
    exchange_inline("localhost", localhost_port, "PING localhost\nQUIT\n");
```

这里 `PING localhost` 的预期响应是：

```text
localhost
```

所以断言：

```cpp
assert(localhost_response.find("localhost\n") != std::string::npos);
assert(localhost_response.find("BYE\n") != std::string::npos);
```

这同时证明：

```text
localhost 能解析
localhost 能连接
inline 协议能执行 PING message
QUIT 能关闭连接
```

这比只连上 socket 更有意义，因为它跑完了完整应用层链路。

---

# 16. localhost 停止和统计断言

请求完成后：

```cpp
localhost_server.request_stop();
localhost_thread.join();
```

然后检查：

```cpp
const auto localhost_stats = localhost_server.connection_stats();
assert(localhost_stats.total_connections == 1);
assert(localhost_stats.active_connections == 0);
assert(localhost_stats.peak_connections == 1);
```

这些断言说明：

```text
localhost 场景确实接入过一个连接
连接最终关闭了
最高并发是 1
```

这也顺便证明第十一版连接指标能力在 hostname 场景下仍然成立。

---

# 17. localhost 日志断言

最后检查 localhost server 日志：

```cpp
assert(contains_log(localhost_logs, "event=tcp_listen"));
assert(contains_log(localhost_logs, "host=localhost"));
assert(contains_log(localhost_logs, "event=tcp_client_accepted"));
assert(contains_log(localhost_logs, "event=tcp_client_closed"));
assert(contains_log(localhost_logs, "event=tcp_stop"));
```

这里的重点是：

```text
host=localhost
```

它证明这不是复用了前一个 `127.0.0.1` server 的日志。

日志链路完整说明：

```text
localhost server 启动
localhost client 接入
localhost client 关闭
localhost server 停止
```

---

# 18. 第十三版测试链路总结

第十三版新增测试链路可以概括为：

```text
exchange_inline(host, port, payload)
 -> connect_test_socket(host, port)
 -> getaddrinfo(AF_UNSPEC)
 -> 依次尝试 IPv4 / IPv6 / hostname 候选地址
 -> 连接成功后发送 inline 命令
 -> 收集响应
```

localhost server 链路是：

```text
TcpServer::Options.host = "localhost"
 -> server.run()
 -> event=tcp_listen host=localhost
 -> exchange_inline("localhost", bound_port, ...)
 -> PING localhost / QUIT
 -> connection_stats 校验
 -> event=tcp_stop
```

---

# 第二十八次总结

第十三版的核心价值不是增加一个新命令，而是让网络测试更接近真实使用方式。

一句话总结：

```text
第十三版把 tcp_server_tests 从固定 IPv4 loopback 测试推进到 hostname 解析测试，用 getaddrinfo(AF_UNSPEC) 和 localhost 场景证明 mini-kv 的 TCP 链路在更常见的本机网络配置下也能工作。
```
