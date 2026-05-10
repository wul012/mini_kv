# 第三十八次讲解：第十八版并发 RESP-over-TCP 测试核心

第三十七次讲了第十七版测试、README、归档和整体增删改。

这次进入第十八版新增测试核心：

```text
tests/tcp_resp_concurrency_tests.cpp
```

第十八版主要补一件事：

```text
让多个外部 RESP 客户端同时连接同一个 TcpServer，
验证并发连接、RESP pipeline、响应字节和连接指标都稳定。
```

第十六版验证的是：

```text
单个外部客户端成功路径 pipeline
```

第十七版验证的是：

```text
单个外部客户端的兼容边界和错误路径
```

第十八版继续往前推：

```text
多个外部客户端同时连接
 -> 服务端 active_connections 达到 4
 -> 统一放行发送 RESP pipeline
 -> 每个客户端精确校验自己的响应
 -> 最后检查 peak_connections=4
```

---

# 1. 为什么需要并发 RESP 测试

之前已经有：

```text
tcp_server_tests
 -> TCP server 生命周期、请求限制、连接指标

tcp_resp_tests
 -> 单连接 RESP pipeline 成功路径

tcp_resp_compat_tests
 -> 单连接 RESP null bulk、integer、error、protocol error
```

这些测试都很重要，但还缺一个场景：

```text
多个 RESP 客户端同时连进来
```

因为 `TcpServer` 的连接模型是：

```text
accept 一个客户端
 -> 启动一个线程 serve_client
 -> 多个客户端可以同时存在
```

所以第十八版需要确认：

```text
并发连接不会破坏 Store 线程安全
并发连接不会破坏 RESP 响应
连接指标 active / total / peak 能正确变化
```

---

# 2. 文件入口

新增文件：

```text
tests/tcp_resp_concurrency_tests.cpp
```

开头包含：

```cpp
#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
```

测试仍然不直接 include `resp.hpp`。

原因和前两版一样：

```text
测试要走真实 TCP server 路径
不绕过网络层
不直调 RespParser
```

也就是从外部客户端视角验证：

```text
socket bytes
 -> TcpServer
 -> RespParser
 -> CommandProcessor
 -> Store
 -> RESP response bytes
```

---

# 3. client_count

第十八版定义：

```cpp
constexpr std::size_t client_count = 4;
```

这表示测试会启动 4 个并发客户端。

为什么是 4：

```text
足够验证并发连接和 peak_connections
又不会让测试变慢或容易受机器负载影响
```

这个值后面会用于：

```text
创建客户端线程数量
等待 ready_clients
检查 total_connections
检查 peak_connections
```

---

# 4. 测试 socket 工具

文件前半部分继续保留跨平台 socket 工具：

```text
TestNetworkRuntime
TestSocketGuard
TestAddrInfoGuard
connect_test_socket
send_all
shutdown_write
receive_all
```

它们负责：

```text
Windows 下 WSAStartup / WSACleanup
跨平台关闭 socket
释放 getaddrinfo 返回的地址链表
使用 AF_UNSPEC 连接 127.0.0.1 和端口
循环发送所有字节
关闭写端
循环接收直到连接关闭
```

这样并发测试里的主逻辑可以更清楚。

---

# 5. TestSocketGuard

`TestSocketGuard` 是 socket 的 RAII 包装：

```cpp
class TestSocketGuard
```

析构时调用：

```cpp
close_test_socket(socket_);
```

它禁止拷贝，允许移动。

意义是：

```text
每个客户端线程退出时都会自动关闭自己的 socket
异常路径也不会泄漏 socket
```

并发测试里一旦有异常，这一点尤其重要。

---

# 6. make_resp_array

构造 RESP 请求还是用：

```cpp
std::string make_resp_array(std::initializer_list<std::string_view> arguments)
```

比如：

```cpp
make_resp_array({"SET", key, value})
```

生成：

```text
*3\r\n
$3\r\n
SET\r\n
$<key length>\r\n
key\r\n
$<value length>\r\n
value\r\n
```

这样每个客户端都能拼出自己的 RESP pipeline。

---

# 7. bulk_string

第十八版新增 helper：

```cpp
std::string bulk_string(std::string_view value)
```

它生成 RESP bulk string：

```cpp
std::string response = "$" + std::to_string(value.size()) + "\r\n";
response += value;
response += "\r\n";
```

它用于构造预期响应。

比如：

```text
value-0
```

会变成：

```text
$7\r\nvalue-0\r\n
```

---

# 8. send_all

并发测试把发送拆成独立函数：

```cpp
void send_all(TestSocket socket, std::string_view payload)
```

循环发送：

```cpp
while (!remaining.empty()) {
    const auto sent = send(...);
    remaining.remove_prefix(static_cast<std::size_t>(sent));
}
```

原因是：

```text
TCP send 不保证一次写完所有字节
```

所以测试必须循环，直到 payload 全部发出去。

---

# 9. shutdown_write

发送完成后调用：

```cpp
shutdown_write(socket.get());
```

Windows 下是：

```cpp
shutdown(socket, SD_SEND);
```

类 Unix 下是：

```cpp
shutdown(socket, SHUT_WR);
```

含义是：

```text
客户端告诉服务端：我已经不再发送数据
但还要继续接收响应
```

这样服务端处理到 `QUIT` 后关闭连接，客户端 `receive_all` 就能读到 EOF。

---

# 10. receive_all

接收函数：

```cpp
std::string receive_all(TestSocket socket)
```

循环 `recv` 到返回 0：

```cpp
if (received == 0) {
    break;
}
```

这符合 TCP 字节流特性：

```text
一次 recv 不保证拿到完整 RESP 响应
```

所以每个客户端都要一直读到连接关闭。

---

# 11. wait_for_log

测试启动服务端后要等待监听完成：

```cpp
wait_for_log(logs, logs_mutex, "event=tcp_listen");
```

它最多轮询 100 次，每次睡 10ms。

也就是最多约：

```text
1 秒
```

看到 `event=tcp_listen` 后，测试才读取：

```cpp
server.bound_port()
```

并启动客户端线程。

---

# 12. failures 收集

并发测试里不能简单让客户端线程直接 `assert`。

原因是：

```text
assert 发生在子线程里时，定位和收尾都不方便
```

所以第十八版用：

```cpp
std::vector<std::string> failures;
std::mutex failures_mutex;
```

配合两个函数：

```cpp
bool has_failures(...)
void add_failure(...)
```

客户端线程如果出错，就把错误文本放进 `failures`。

主线程最后统一断言：

```cpp
assert(!has_failures(failures, failures_mutex));
```

---

# 13. wait_for_concurrent_clients

第十八版关键等待函数：

```cpp
bool wait_for_concurrent_clients(const minikv::TcpServer& server,
                                 const std::atomic_size_t& ready_clients,
                                 const std::vector<std::string>& failures,
                                 std::mutex& failures_mutex)
```

它等待三个条件同时成立：

```cpp
ready_clients.load() == client_count
stats.total_connections == client_count
stats.active_connections == client_count
```

含义是：

```text
4 个客户端线程都已经连上
服务端已经接受 4 个连接
这 4 个连接都还保持 active
```

只有这个条件成立，测试才认为：

```text
真的形成了并发连接状态
```

---

# 14. 为什么要先连上再统一发送

如果 4 个客户端各自创建连接后马上发送、马上关闭，那么可能发生：

```text
第一个客户端已经关闭
第二个客户端才刚连接
```

这样 `peak_connections` 不一定能稳定到 4。

第十八版用：

```text
客户端先连接
 -> ready_clients +1
 -> 等待 start 信号
 -> 主线程确认 active_connections=4
 -> start=true
 -> 4 个客户端一起发请求
```

这样并发峰值是稳定可验证的，不靠碰运气。

---

# 15. wait_for_closed_clients

请求执行完后，测试等待：

```cpp
stats.total_connections == client_count
stats.active_connections == 0
```

这表示：

```text
4 个连接都出现过
4 个连接都已经关闭
```

如果等待超时，函数最后会断言当前统计。

这样可以抓到连接没有正确关闭的问题。

---

# 16. client_request

每个客户端发送自己的 pipeline：

```cpp
std::string client_request(std::size_t index)
```

里面先生成：

```cpp
const std::string key = "worker:" + std::to_string(index);
const std::string value = "value-" + std::to_string(index);
const std::string ping_message = "client-" + std::to_string(index);
```

每个客户端都有独立 key：

```text
worker:0
worker:1
worker:2
worker:3
```

这样多个客户端不会互相覆盖同一个 key。

---

# 17. 每个客户端的 pipeline

请求内容是：

```cpp
SET key value
GET key
PING client-N
TTL key
DEL key
GET key
QUIT
```

它覆盖：

```text
写入
读取
PING bulk response
TTL 无过期时间
删除
删除后 null bulk
关闭连接
```

每个客户端都走同样流程，但 key 和 value 不同。

---

# 18. client_expected_response

对应的预期响应：

```cpp
return "+OK inserted\r\n" +
       bulk_string(value) +
       bulk_string(ping_message) +
       ":-1\r\n"
       ":1\r\n"
       "$-1\r\n"
       "+BYE\r\n";
```

逐段看：

```text
+OK inserted
 -> SET 成功

bulk_string(value)
 -> GET key 返回自己的 value

bulk_string(ping_message)
 -> PING client-N 返回 message

:-1
 -> TTL key 没有过期时间

:1
 -> DEL key 删除成功

$-1
 -> 删除后 GET key 返回 null bulk

+BYE
 -> QUIT 关闭连接
```

每个客户端会把收到的字节和这段预期精确比较。

---

# 19. run_client

客户端线程入口：

```cpp
void run_client(std::uint16_t port,
                std::size_t index,
                const std::atomic_bool& start,
                std::atomic_size_t& ready_clients,
                std::vector<std::string>& failures,
                std::mutex& failures_mutex)
```

它做这些事：

```text
初始化网络运行时
连接 127.0.0.1:port
ready_clients +1
等待 start=true
发送自己的 RESP pipeline
关闭写端
读取完整响应
精确比对响应
失败则写入 failures
```

这样每个客户端线程是一个完整的外部 RESP 客户端模拟。

---

# 20. start 原子变量

主线程创建：

```cpp
std::atomic_bool start{false};
```

客户端线程连接成功后：

```cpp
while (!start.load()) {
    std::this_thread::sleep_for(1ms);
}
```

主线程确认 4 个连接都 active 后：

```cpp
start.store(true);
```

这就是第十八版并发测试的“发令枪”。

---

# 21. main：启动 server

测试入口创建：

```cpp
minikv::Store store;
minikv::TcpServer::Options options;
options.host = "127.0.0.1";
options.port = 0;
options.accept_poll_interval = 10ms;
```

`port = 0` 仍然表示：

```text
让操作系统分配临时空闲端口
```

然后把日志收集到 vector：

```cpp
options.logger = [&](const std::string& message) {
    std::lock_guard lock{logs_mutex};
    logs.push_back(message);
};
```

测试内部日志收集本身也是线程安全的。

---

# 22. 后台线程运行 server

和前几版一样：

```cpp
std::thread server_thread{[&] {
    try {
        server.run();
    } catch (...) {
        failure = std::current_exception();
    }
}};
```

server 在线程中运行。

如果 `server.run()` 抛异常，保存到：

```cpp
failure
```

测试结尾会重新抛出。

---

# 23. 启动 4 个客户端线程

主线程创建：

```cpp
std::vector<std::thread> clients;
clients.reserve(client_count);
```

然后循环：

```cpp
for (std::size_t index = 0; index < client_count; ++index) {
    clients.emplace_back(run_client, ...);
}
```

每个线程拿到自己的 index。

所以每个客户端使用不同 key：

```text
worker:0
worker:1
worker:2
worker:3
```

---

# 24. 等待并发状态成立

主线程调用：

```cpp
const bool concurrent_clients_ready =
    wait_for_concurrent_clients(server, ready_clients, failures, failures_mutex);
```

这个返回值会被保存下来。

后面断言：

```cpp
assert(concurrent_clients_ready);
```

如果 4 个客户端没有同时 active，测试失败。

这能防止测试退化成普通的串行连接测试。

---

# 25. 放行客户端并 join

主线程放行：

```cpp
start.store(true);
```

然后等待所有客户端结束：

```cpp
for (auto& client : clients) {
    client.join();
}
```

这保证：

```text
所有客户端请求都已经完成
所有响应都已经校验过
```

才进入后面的服务端停止流程。

---

# 26. 停止 server

客户端结束后：

```cpp
wait_for_closed_clients(server);

server.request_stop();
server_thread.join();
```

顺序是：

```text
先确认客户端连接都关闭
再请求 server 停止
最后 join server 线程
```

这样测试不会留下后台线程。

---

# 27. 最终断言

最后断言：

```cpp
assert(concurrent_clients_ready);
assert(!has_failures(failures, failures_mutex));
assert(server.stop_requested());
```

含义：

```text
确实达成过 4 个 active 连接
没有任何客户端失败
server 收到过停止请求
```

---

# 28. 检查连接统计

连接统计：

```cpp
const auto stats = server.connection_stats();
assert(stats.total_connections == client_count);
assert(stats.active_connections == 0);
assert(stats.peak_connections == client_count);
```

也就是：

```text
total_connections == 4
 -> 总共 4 个客户端连接过

active_connections == 0
 -> 测试结束时没有连接残留

peak_connections == 4
 -> 峰值并发连接确实达到 4
```

这是第十八版最关键的指标断言。

---

# 29. 检查 Store 回到空

测试还断言：

```cpp
assert(store.size() == 0);
```

原因是每个客户端都会：

```text
SET worker:N value-N
DEL worker:N
```

所以全部结束后，内存数据库应该没有残留 key。

这顺手验证了并发 SET / DEL 后的最终状态。

---

# 30. 检查日志

最后检查：

```cpp
assert(contains_log(logs, "event=tcp_listen"));
assert(contains_log(logs, "event=tcp_client_accepted"));
assert(contains_log(logs, "event=tcp_client_closed"));
assert(contains_log(logs, "event=tcp_stop"));
assert(contains_log(logs, "total_connections=" + std::to_string(client_count)));
assert(contains_log(logs, "peak_connections=" + std::to_string(client_count)));
```

这证明：

```text
server 正常 listen
客户端连接被接受
客户端连接被关闭
server 正常 stop
日志里出现 total_connections=4
日志里出现 peak_connections=4
```

---

# 31. 第十八版测试覆盖了什么

`tcp_resp_concurrency_tests` 覆盖：

```text
4 个 raw socket RESP 客户端同时连接
4 个连接同时保持 active
每个客户端发送独立 RESP pipeline
每个客户端精确校验响应
Store 并发读写后回到空
连接统计 total / active / peak 正确
结构化日志包含并发峰值
```

它把 RESP-over-TCP 测试从：

```text
单连接正确
```

推进到：

```text
多连接并发正确
```

---

# 32. 第十八版仍然没有覆盖什么

这一版还不是完整压力测试。

它没有覆盖：

```text
长时间运行
上百连接
随机命令混合
网络半关闭异常
客户端突然断开
WAL 开启时的并发写入
```

所以它的定位是：

```text
稳定、可重复的小规模并发 smoke / regression test。
```

---

# 33. 第十八版测试链路总结

整体链路是：

```text
tcp_resp_concurrency_tests
 -> 创建 Store
 -> 创建 TcpServer，端口为 0
 -> 后台线程运行 server.run()
 -> 等待 event=tcp_listen
 -> 读取 bound_port
 -> 启动 4 个客户端线程
 -> 每个客户端连接后等待 start
 -> 主线程等待 active_connections=4
 -> start=true
 -> 4 个客户端一起发送 RESP pipeline
 -> 每个客户端精确比对响应
 -> 等待 active_connections=0
 -> request_stop
 -> join server_thread
 -> 检查 total=4 / active=0 / peak=4 / store.size=0
```

---

# 第三十八次总结

第十八版新增的 `tcp_resp_concurrency_tests.cpp` 是 RESP-over-TCP 从单连接走向多连接的关键测试。

一句话总结：

```text
第十八版用 4 个 raw socket 客户端同时连接同一个 TcpServer，并在 active_connections=4 后统一发送 RESP pipeline，证明 mini-kv 的 RESP TCP 链路在小规模并发场景下响应正确、连接统计正确、最终数据状态也正确。
```
