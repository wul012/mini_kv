# 第三十六次讲解：第十七版 RESP-over-TCP 兼容测试核心

第三十五次讲了第十六版测试、README、归档和整体增删改。

这次进入第十七版新增测试核心：

```text
tests/tcp_resp_compat_tests.cpp
```

第十七版主要补一件事：

```text
把第十六版只覆盖成功路径的 RESP pipeline，
扩展到 null bulk、integer、command error 和 protocol error。
```

第十六版证明：

```text
PING / SET / GET / SIZE / QUIT
```

可以通过外部 raw socket pipeline 正常访问。

第十七版继续往前走，验证更多真实客户端会遇到的响应类型：

```text
GET missing
 -> null bulk: $-1

TTL
 -> integer: -1 / -2

EXPIRE / DEL
 -> integer: 1

NOPE
 -> command error

GET durable extra
 -> usage error

*1\r\n$-1\r\n
 -> protocol error
```

---

# 1. 为什么需要 tcp_resp_compat_tests

第十六版 `tcp_resp_tests` 已经证明：

```text
外部 socket 可以发送 RESP pipeline
server 可以返回精确 RESP frame
```

但它主要是成功路径：

```text
bulk string
simple string
integer
QUIT
```

真实 RESP 客户端还会遇到：

```text
key 不存在
参数数量错误
未知命令
非法 RESP 数据
```

如果这些场景没有覆盖，协议层看起来能跑，但兼容性还是薄的。

所以第十七版新增：

```text
tcp_resp_compat_tests
```

专门加厚 RESP 兼容测试。

---

# 2. 文件入口

新增文件：

```text
tests/tcp_resp_compat_tests.cpp
```

开头包含：

```cpp
#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
```

和第十六版一样，它不直接 include `resp.hpp`。

原因是：

```text
测试不绕过 TCP server
不直接调用 RespParser
而是通过真实 socket 字节进入服务端
```

也就是说，它测的是：

```text
TcpServer + RespParser + CommandProcessor + Resp response formatter
```

组合起来的行为。

---

# 3. 跨平台 socket 支撑代码

文件前半部分和第十六版类似，保留一套测试 socket 工具：

```text
TestNetworkRuntime
TestSocketGuard
TestAddrInfoGuard
connect_test_socket
exchange_raw
```

它们的作用是：

```text
Windows 下初始化 Winsock
类 Unix 下使用普通 socket API
自动关闭 socket
自动释放 getaddrinfo 链表
按 AF_UNSPEC 连接 host / port
循环 send 和 recv，保证完整传输
```

这些工具让测试逻辑可以专注在 RESP 请求和响应上。

---

# 4. contains_log

测试还是需要检查结构化日志：

```cpp
bool contains_log(const std::vector<std::string>& logs, std::string_view needle)
```

它遍历日志数组，只要某行包含目标文本就返回 true。

第十七版最后会检查：

```text
event=tcp_listen
event=tcp_client_accepted
event=tcp_client_closed
event=tcp_stop
total_connections=2
peak_connections=1
```

这样协议测试不会漏掉连接生命周期。

---

# 5. make_resp_array

和第十六版一样，测试用 helper 构造 RESP array：

```cpp
std::string make_resp_array(std::initializer_list<std::string_view> arguments)
```

比如：

```cpp
make_resp_array({"EXPIRE", "durable", "120"})
```

会生成：

```text
*3\r\n
$6\r\n
EXPIRE\r\n
$7\r\n
durable\r\n
$3\r\n
120\r\n
```

这个 helper 很重要。

原因是 RESP 对长度很敏感：

```text
$7
durable
```

长度写错一个字节，服务端就会按协议错误处理。

---

# 6. exchange_raw

核心通信函数：

```cpp
std::string exchange_raw(std::string_view host, std::uint16_t port, std::string_view payload)
```

它做四步：

```text
连接 server
 -> 循环 send 完整 payload
 -> shutdown 写端
 -> 循环 recv 到连接关闭
```

这模拟的是一个非常朴素的外部客户端。

它不懂 mini-kv 内部结构，只认识：

```text
host
port
bytes in
bytes out
```

所以它适合验证协议边界。

---

# 7. wait_for_log

第十七版把等待日志抽成：

```cpp
void wait_for_log(const std::vector<std::string>& logs,
                  std::mutex& logs_mutex,
                  std::string_view needle)
```

它最多轮询 100 次：

```cpp
for (int attempt = 0; attempt < 100; ++attempt) {
    ...
    std::this_thread::sleep_for(10ms);
}
```

总等待时间最多约 1 秒。

它用于等待：

```text
event=tcp_listen
```

也就是确认 server 已经真正进入监听状态。

---

# 8. 为什么 wait_for_log 要加锁

日志 vector 被两个线程访问：

```text
server 线程
 -> 写入日志

main 测试线程
 -> 读取日志
```

所以读取时必须：

```cpp
std::lock_guard lock{logs_mutex};
```

否则测试可能出现数据竞争。

这点和第十六版的日志捕获逻辑一致。

---

# 9. wait_for_connection_total

第十七版新增：

```cpp
void wait_for_connection_total(const minikv::TcpServer& server, std::uint64_t total_connections)
```

它等待两个条件：

```text
stats.total_connections == total_connections
stats.active_connections == 0
```

这表示：

```text
期望数量的客户端连接已经出现过
并且当前都已经关闭
```

第十七版会建立两条连接：

```text
第一条连接
 -> compatibility pipeline

第二条连接
 -> protocol error 请求
```

所以最后期望：

```text
total_connections = 2
active_connections = 0
```

---

# 10. main：启动临时端口服务端

测试入口仍然创建内存 Store：

```cpp
minikv::Store store;
```

然后配置服务端：

```cpp
minikv::TcpServer::Options options;
options.host = "127.0.0.1";
options.port = 0;
options.accept_poll_interval = 10ms;
```

`port = 0` 的意思是：

```text
让操作系统分配一个临时空闲端口
```

这样测试不依赖固定端口，多个环境运行时更稳。

---

# 11. 后台线程运行 server

`server.run()` 是阻塞调用，所以测试把它放到后台线程：

```cpp
std::exception_ptr failure;
std::thread server_thread{[&] {
    try {
        server.run();
    } catch (...) {
        failure = std::current_exception();
    }
}};
```

如果 server 线程抛异常，会保存到：

```cpp
failure
```

测试结尾会重新抛出。

这样后台线程失败不会被悄悄吞掉。

---

# 12. 等待监听并获取端口

测试先等待：

```cpp
wait_for_log(logs, logs_mutex, "event=tcp_listen");
```

然后读取真实端口：

```cpp
const auto bound_port = server.bound_port();
assert(bound_port > 0);
```

因为前面使用 `port = 0`，只有 server 真正 bind 之后，才能知道实际端口。

---

# 13. 第一条连接：compatibility_request

第十七版第一条请求是一个 pipeline：

```cpp
const std::string compatibility_request =
    make_resp_array({"GET", "missing"}) +
    make_resp_array({"SET", "durable", "value"}) +
    make_resp_array({"TTL", "durable"}) +
    make_resp_array({"EXPIRE", "durable", "120"}) +
    make_resp_array({"DEL", "durable"}) +
    make_resp_array({"TTL", "durable"}) +
    make_resp_array({"NOPE"}) +
    make_resp_array({"GET", "durable", "extra"}) +
    make_resp_array({"QUIT"});
```

一条 TCP 连接里连续发 9 个 RESP request。

这验证的是：

```text
server pending buffer 能连续解析多条 RESP 命令
并且在不同类型响应之间切换正确
```

---

# 14. GET missing：null bulk

第一条命令：

```text
GET missing
```

key 不存在时，CommandProcessor 返回：

```text
(nil)
```

RESP formatter 会把它转换成：

```text
$-1\r\n
```

这就是 RESP null bulk string。

第十六版没有覆盖 `$-1`，第十七版补上了。

---

# 15. SET durable value：simple string

第二条命令：

```text
SET durable value
```

新 key 插入成功时返回：

```text
OK inserted
```

RESP formatter 会把 `OK ` 开头的响应转换成 simple string：

```text
+OK inserted\r\n
```

这延续了前面版本的响应规则。

---

# 16. TTL durable：无过期时间

第三条命令：

```text
TTL durable
```

刚 SET 的 key 没有过期时间。

Store 返回的语义是：

```text
-1
```

RESP formatter 看到整数字符串，会转换成：

```text
:-1\r\n
```

这验证了：

```text
负整数也要按 RESP integer 返回
```

---

# 17. EXPIRE durable 120

第四条命令：

```text
EXPIRE durable 120
```

key 存在，设置过期成功。

CommandProcessor 返回：

```text
1
```

RESP formatter 转成：

```text
:1\r\n
```

这说明写命令的 integer reply 也在 RESP TCP 路径上工作。

---

# 18. DEL durable

第五条命令：

```text
DEL durable
```

key 存在，删除成功。

返回：

```text
1
```

RESP frame：

```text
:1\r\n
```

这补上了第十六版没测到的删除命令。

---

# 19. 删除后的 TTL durable

第六条命令再次执行：

```text
TTL durable
```

这时 key 已经被删除。

TTL 对不存在 key 返回：

```text
-2
```

RESP frame：

```text
:-2\r\n
```

这验证了另一个负整数响应。

---

# 20. NOPE：未知命令错误

第七条命令：

```text
NOPE
```

CommandProcessor 不认识它，返回：

```text
ERR unknown command
```

RESP formatter 看到 `ERR ` 开头，会返回 error frame：

```text
-ERR unknown command\r\n
```

这覆盖 command error。

---

# 21. GET durable extra：usage error

第八条命令：

```text
GET durable extra
```

`GET` 只允许一个 key。

多了 `extra` 后，CommandProcessor 返回：

```text
ERR usage: GET key
```

RESP frame：

```text
-ERR usage: GET key\r\n
```

这说明参数数量错误也会在 RESP TCP 路径里变成 error frame。

---

# 22. QUIT：关闭连接

最后一条命令：

```text
QUIT
```

返回：

```text
BYE
```

RESP frame：

```text
+BYE\r\n
```

同时 `CommandResult.should_close` 为 true，服务端会关闭这条连接。

---

# 23. compatibility_expected

第十七版把完整预期响应写死：

```cpp
const std::string compatibility_expected = "$-1\r\n"
                                           "+OK inserted\r\n"
                                           ":-1\r\n"
                                           ":1\r\n"
                                           ":1\r\n"
                                           ":-2\r\n"
                                           "-ERR unknown command\r\n"
                                           "-ERR usage: GET key\r\n"
                                           "+BYE\r\n";
```

这不是模糊匹配。

它要求：

```text
响应类型正确
响应顺序正确
CRLF 正确
错误文本正确
整数符号正确
```

只要有一个字节不一致，测试就会失败。

---

# 24. 第一条连接的断言

执行：

```cpp
const auto compatibility_response = exchange_raw("127.0.0.1", bound_port, compatibility_request);
assert(compatibility_response == compatibility_expected);
wait_for_connection_total(server, 1);
```

这里验证两件事：

```text
第一，RESP 响应字节完全匹配
第二，第一条客户端连接已经关闭并计入 total_connections
```

所以它同时覆盖协议层和连接生命周期。

---

# 25. 第二条连接：非法 null bulk

第十七版又发起第二条连接：

```cpp
const std::string protocol_error_response = exchange_raw("127.0.0.1", bound_port, "*1\r\n$-1\r\n");
```

这个请求表示：

```text
一个 RESP array
里面有一个 null bulk string
```

mini-kv 的 RESP parser 当前不支持 null bulk string 作为命令参数。

所以 parser 返回 error：

```text
null RESP bulk strings are not supported
```

TCP server 会包成 RESP error：

```text
-ERR null RESP bulk strings are not supported\r\n
```

---

# 26. protocol_error_response 断言

测试断言：

```cpp
assert(protocol_error_response == "-ERR null RESP bulk strings are not supported\r\n");
wait_for_connection_total(server, 2);
```

这证明：

```text
协议错误不会让 server 卡住
协议错误会返回 RESP error
协议错误连接会正常关闭
总连接数变成 2
```

这比只测正常 pipeline 更接近真实网络环境。

---

# 27. 停止服务端

两条连接验证完成后：

```cpp
server.request_stop();
server_thread.join();
```

这沿用第十版的 graceful stop 机制。

测试不是靠进程退出强杀 server，而是：

```text
设置停止标记
等待 accept loop 退出
join 后台线程
```

---

# 28. 重新抛出后台异常

server 线程结束后：

```cpp
if (failure) {
    std::rethrow_exception(failure);
}
```

这保证：

```text
如果 server.run() 内部异常
CTest 能看到失败
```

不会出现“测试主线程看起来正常，但 server 线程已经失败”的情况。

---

# 29. 检查连接统计

最终统计：

```cpp
const auto stats = server.connection_stats();
assert(stats.total_connections == 2);
assert(stats.active_connections == 0);
assert(stats.peak_connections == 1);
```

含义：

```text
total_connections == 2
 -> compatibility pipeline 一条连接，protocol error 一条连接

active_connections == 0
 -> 两条连接都已经关闭

peak_connections == 1
 -> 两条连接是先后执行，不是并发执行
```

---

# 30. 检查日志

最后检查日志：

```cpp
assert(contains_log(logs, "event=tcp_listen"));
assert(contains_log(logs, "event=tcp_client_accepted"));
assert(contains_log(logs, "event=tcp_client_closed"));
assert(contains_log(logs, "event=tcp_stop"));
assert(contains_log(logs, "total_connections=2"));
assert(contains_log(logs, "peak_connections=1"));
```

这证明：

```text
server 启动过
客户端连接被接受过
客户端连接被关闭过
server 正常停止过
累计连接数记录到 2
峰值连接数记录到 1
```

日志和统计是第十一版之后的重要观测能力。
第十七版继续保证它们没有被协议错误路径破坏。

---

# 31. 第十七版测试覆盖了什么

`tcp_resp_compat_tests` 覆盖：

```text
RESP null bulk reply
 -> GET missing 返回 $-1

RESP integer reply
 -> TTL / EXPIRE / DEL 返回 :-1 / :1 / :-2

RESP command error
 -> NOPE 返回 -ERR unknown command

RESP usage error
 -> GET durable extra 返回 -ERR usage: GET key

RESP protocol error
 -> null bulk 参数返回 -ERR null RESP bulk strings are not supported

连接生命周期
 -> 两条连接都 accepted / closed
```

---

# 32. 第十七版仍然没有覆盖什么

这一版仍然没有做：

```text
并发多个 RESP 客户端
长时间 RESP 压测
SAVE / LOAD 的 RESP 外部测试
请求超限在 RESP compatibility pipeline 中的扩展
完整 Redis 命令兼容
```

所以它不是“完整 Redis 兼容”。

它的定位更准确地说是：

```text
mini-kv 自己支持的命令，在 RESP-over-TCP 上有更多响应类型覆盖。
```

---

# 33. 第十七版测试链路总结

整体链路是：

```text
tcp_resp_compat_tests
 -> 创建 Store
 -> 创建 TcpServer，端口为 0
 -> 后台线程运行 server.run()
 -> wait_for_log 等待 event=tcp_listen
 -> bound_port() 获取真实端口
 -> 第一条连接发送 compatibility pipeline
 -> 精确比对 null bulk / integer / error / BYE 响应
 -> 等待 total_connections=1 且 active_connections=0
 -> 第二条连接发送非法 RESP null bulk
 -> 精确比对 protocol error 响应
 -> 等待 total_connections=2 且 active_connections=0
 -> request_stop
 -> join server_thread
 -> 检查最终 stats 和日志
```

---

# 第三十六次总结

第十七版新增的 `tcp_resp_compat_tests.cpp` 是第十六版 RESP pipeline 测试的加厚版。

一句话总结：

```text
第十七版用两条 raw socket 连接验证 RESP-over-TCP 的 null bulk、integer、command error、usage error 和 protocol error，让 mini-kv 的外部 RESP 客户端兼容测试从成功路径扩展到更多真实边界。
```
