# 第二十六次讲解：第十二版服务端请求上限和客户端超时

第二十五次讲了第十一版测试、README、运行归档和整体增删改。

这次进入第十二版核心代码：

```text
include/minikv/tcp_server.hpp
src/tcp_server.cpp
src/server_main.cpp
src/client_main.cpp
```

第十二版主要做三件事：

```text
第一，把 TCP 请求缓冲上限从硬编码变成 TcpServer::Options 配置
第二，让 minikv_server 可以通过命令行调整请求上限和 accept 轮询间隔
第三，让 minikv_client 可以设置 socket 接收和发送超时
```

第十一版解决的是连接生命周期观测。
第十二版继续往生产可控性走：

```text
请求不能无限堆在内存里
服务端限制要能配置
客户端等待网络响应不能只能无限等
```

---

# 1. 为什么要做请求上限配置

第八版接入 RESP 后，TCP server 有了 `pending` 缓冲区。
客户端发来的字节会先进入 `pending`，然后服务端按协议解析：

```text
inline 请求
 -> 找换行
 -> 拿出一行命令
 -> CommandProcessor 执行

RESP 请求
 -> RespParser::parse_command
 -> 判断 complete / incomplete / error
 -> complete 后执行命令
```

如果客户端一直发不完整请求，比如一直发字节但不发换行，那么 inline 协议永远不能形成完整命令。
如果没有上限，`pending` 会持续增长。

第九版已经在 RESP parser 里限制了单个 bulk string 长度。
第十二版补的是 TCP 层通用限制：

```text
不管 inline 还是 RESP
只要当前 pending 请求缓冲超过上限
就拒绝这个连接
```

这个限制属于服务端资源保护。

---

# 2. tcp_server.hpp 新增 max_request_bytes

第十二版在 `TcpServer::Options` 里新增：

```cpp
std::size_t max_request_bytes = 64 * 1024;
```

它的含义是：

```text
单个 TCP 连接里，一个尚未解析完成的请求最多允许缓存多少字节
```

默认值仍然是 `64 * 1024`，也就是 65536 字节。

这点很重要：

```text
旧行为
 -> tcp_server.cpp 内部写死 64 * 1024

新行为
 -> 默认仍然是 64 * 1024
 -> 但测试和真实运行可以通过 Options 或 CLI 调整
```

这样既不改变默认安全边界，又让配置变得可控。

---

# 3. request_limit_fields 统一日志字段

第十二版在 `src/tcp_server.cpp` 中新增辅助函数：

```cpp
std::string request_limit_fields(std::size_t pending_bytes, std::size_t max_request_bytes) {
    return "pending_bytes=" + std::to_string(pending_bytes) +
           " max_request_bytes=" + std::to_string(max_request_bytes);
}
```

这个函数只做一件事：

```text
把请求限制相关字段拼成结构化日志片段
```

最终日志类似：

```text
pending_bytes=10 max_request_bytes=8
```

这样后面如果继续扩展请求拒绝日志，比如增加协议类型或远端地址，修改点会更集中。

---

# 4. serve_client 接收 max_request_bytes

第十一版的 `serve_client` 已经接收：

```text
Store&
endpoint
client_socket
logger
tracker
connection_id
```

第十二版继续新增：

```cpp
std::size_t max_request_bytes
```

也就是每个客户端线程启动时，都会带上当前服务端配置的请求上限。

线程创建处对应变成：

```cpp
std::thread{
    serve_client,
    std::ref(store_),
    endpoint,
    client_socket,
    options_.logger,
    connection_tracker_,
    snapshot.connection_id,
    options_.max_request_bytes}
    .detach();
```

这里的链路是：

```text
TcpServer::Options::max_request_bytes
 -> TcpServer::run()
 -> std::thread 启动 serve_client
 -> serve_client 按这个上限检查 pending
```

这比在 `serve_client` 里硬编码数字更清楚。

---

# 5. pending 超限检查

第十二版核心判断在 `serve_client` 的接收循环里：

```cpp
if (pending.size() > max_request_bytes) {
    ...
}
```

注意这里是：

```text
大于 max_request_bytes
```

也就是说：

```text
pending.size() == max_request_bytes
 -> 还允许

pending.size() > max_request_bytes
 -> 拒绝
```

请求处理顺序大致是：

```text
recv 新数据
 -> 追加到 pending
 -> 尝试解析已经完整的请求
 -> 如果剩余 pending 仍然超过上限，就拒绝连接
```

这样可以避免误伤一次性发送多条完整短命令的客户端。
真正会被拒绝的是：

```text
数据已经积压成一个过大的未完成请求
```

---

# 6. 超限时返回不同协议错误

第十二版保持 inline 和 RESP 两种协议的响应风格。

如果当前 pending 看起来是 RESP：

```cpp
send_all(client.get(), "-ERR request too long\r\n");
```

如果是 inline：

```cpp
send_all(client.get(), "ERR line too long\n");
```

所以同一个保护机制，对外表现是：

```text
RESP
 -> -ERR request too long\r\n

inline
 -> ERR line too long\n
```

这样做的好处是：

```text
RESP 客户端仍然收到 RESP error frame
inline 客户端仍然收到普通文本错误
```

协议边界没有混在一起。

---

# 7. event=tcp_request_rejected

第十二版新增结构化拒绝日志：

```text
event=tcp_request_rejected host=127.0.0.1 port=6412 connection_id=2 reason=request_too_long pending_bytes=10 max_request_bytes=8
```

字段含义：

```text
event=tcp_request_rejected
 -> 这不是普通断开，而是服务端主动拒绝请求

connection_id
 -> 对应第十一版新增的连接编号

reason=request_too_long
 -> 拒绝原因是请求缓冲过大

pending_bytes
 -> 拒绝时 pending 里已经积压的字节数

max_request_bytes
 -> 当前服务端配置的上限
```

第十一版有连接生命周期日志。
第十二版在这个基础上能解释：

```text
某个连接为什么被服务端提前关掉
```

这对排查异常客户端很有用。

---

# 8. ConnectionCloseLogger 的小坑

第十二版调试时有一个关键点：

```cpp
ConnectionCloseLogger close_logger{logger, endpoint, tracker, connection_id};
```

这里不能把 `logger` move 走。

原因是 `serve_client` 后面还要用 `logger` 记录：

```text
event=tcp_request_rejected
```

如果把 `logger` 移动给 `ConnectionCloseLogger`，后面的拒绝日志就可能没有 logger 可用。

这一版最后选择：

```text
close_logger 保存一份 logger
serve_client 本地继续保留 logger
```

代价只是复制一个可调用对象，换来拒绝日志稳定可用。

---

# 9. tcp_listen 日志输出 max_request_bytes

第十二版还改了监听启动日志：

```cpp
log_event(options_, "event=tcp_listen " + endpoint + " max_request_bytes=" +
                        std::to_string(options_.max_request_bytes));
```

以前只能知道：

```text
服务监听在哪个 host / port
```

现在还能知道：

```text
服务以多大的请求上限启动
```

比如：

```text
event=tcp_listen host=127.0.0.1 port=6412 max_request_bytes=8
```

这和拒绝日志能对应起来：

```text
启动时 max_request_bytes=8
拒绝时 max_request_bytes=8
```

排查时不用猜服务当时用了什么配置。

---

# 10. server_main 新增解析 size 的函数

第十二版在 `src/server_main.cpp` 中新增：

```cpp
std::size_t parse_positive_size(const char* value, std::string_view name)
```

它负责解析：

```text
--max-request-bytes 后面的 bytes
```

这个函数做了几层校验：

```text
不能为空
不能是负数
必须完整解析成数字
不能是 0
不能超过 std::size_t 最大值
```

也就是说这些都应该失败：

```text
--max-request-bytes 0
--max-request-bytes -1
--max-request-bytes abc
--max-request-bytes 12abc
```

服务端配置属于边界参数，不能用宽松解析。
越早拒绝错误参数，后面越少走弯路。

---

# 11. server_main 新增 parse_positive_milliseconds

第十二版还新增：

```cpp
std::chrono::milliseconds parse_positive_milliseconds(const char* value, std::string_view name)
```

它复用正整数解析逻辑，然后包装成：

```cpp
std::chrono::milliseconds
```

目前用于：

```text
--accept-poll-ms
```

这个参数控制 accept loop 的轮询间隔。

第十版引入 `accept_poll_interval` 时，代码层已经支持配置。
第十二版把它暴露给命令行：

```text
minikv_server.exe 6379 127.0.0.1 --accept-poll-ms 50
```

含义是：

```text
accept loop 最多约 50ms 醒来一次检查 should_stop
```

间隔越小，停止响应越快，但空闲轮询更频繁。

---

# 12. server_main 参数布局

第十二版用法变成：

```text
minikv_server.exe [port] [host] [wal_path] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

它继续保留前三个位置参数：

```text
port
host
wal_path
```

同时支持两个 flag：

```text
--max-request-bytes bytes
--accept-poll-ms ms
```

解析规则是：

```text
遇到 --max-request-bytes
 -> 读取下一个参数作为 bytes
 -> 写入 options.max_request_bytes

遇到 --accept-poll-ms
 -> 读取下一个参数作为 ms
 -> 写入 options.accept_poll_interval

遇到其他 '-' 开头参数
 -> 认为是未知选项
 -> 打印错误并返回 2

非 '-' 开头参数
 -> 按 port / host / wal_path 的位置顺序处理
```

这样既兼容旧启动方式，也允许新参数出现在后面。

---

# 13. server_start 日志新增配置字段

第十二版启动日志变成：

```text
event=server_start host=127.0.0.1 port=6379 protocol=inline,resp max_request_bytes=65536 accept_poll_ms=200
```

字段分成两类：

```text
服务身份
 -> host
 -> port
 -> protocol

运行配置
 -> max_request_bytes
 -> accept_poll_ms
```

这和 `event=tcp_listen` 互相补充：

```text
server_start
 -> server_main 已经解析好的启动配置

tcp_listen
 -> TcpServer 真正监听成功后的配置和端口
```

---

# 14. client_main 为什么要加 timeout_ms

第十二版之前，`minikv_client` 连接服务端后，socket 读写没有用户可调的超时参数。

这在本地调试通常没问题，但真实网络里可能遇到：

```text
服务端卡住
网络中断
对端不返回数据
连接半开
```

如果没有超时，客户端体验会很差。

第十二版新增第三个可选参数：

```text
minikv_client.exe [host] [port] [timeout_ms]
```

示例：

```text
minikv_client.exe 127.0.0.1 6379 5000
```

含义是：

```text
socket 接收和发送都设置为 5000ms 超时
```

---

# 15. client_main 解析 timeout_ms

客户端也新增：

```cpp
parse_positive_milliseconds(argv[3], "timeout_ms")
```

它和服务端一样坚持正整数：

```text
timeout_ms 必须大于 0
```

这些输入会被拒绝：

```text
0
-1
abc
10abc
```

原因很直接：

```text
0ms timeout 的跨平台语义容易混乱
负数和非数字没有意义
```

所以第十二版只接受清晰的正整数毫秒。

---

# 16. set_socket_timeout 的跨平台处理

第十二版在 `src/client_main.cpp` 中新增：

```cpp
void set_socket_timeout(SocketHandle socket, std::chrono::milliseconds timeout)
```

Windows 分支使用：

```text
DWORD timeout_ms
SO_RCVTIMEO
SO_SNDTIMEO
```

POSIX 分支使用：

```text
timeval
SO_RCVTIMEO
SO_SNDTIMEO
```

两个平台做的是同一件事：

```text
设置接收超时
设置发送超时
```

Windows 这里还做了一个保护：

```text
把 timeout.count() clamp 到 DWORD 最大值
```

避免传入特别大的毫秒数时发生整数截断。

---

# 17. timeout 在 connect 前设置

第十二版的连接函数变成：

```cpp
SocketGuard connect_to_server(std::string_view host,
                              std::uint16_t port,
                              std::optional<std::chrono::milliseconds> timeout)
```

创建 socket 后，如果用户传入 timeout，就先调用：

```cpp
set_socket_timeout(socket.get(), *timeout);
```

再执行：

```cpp
connect(...)
```

这样做的好处是：

```text
后续 send / recv 一开始就受 timeout 保护
```

第十二版没有额外实现非阻塞 connect 超时。
它先把最直接的客户端收发超时补上，范围清楚，风险较小。

---

# 18. 客户端启动输出 timeout

连接成功后，如果设置了 timeout，客户端会输出：

```text
socket timeout: 2000 ms
```

这个输出不是给协议用的，而是给人看的运行提示。

它能确认：

```text
用户输入的第三个参数确实被解析并应用了
```

第十二版 smoke test 就用它验证 bundled client 参数链路。

---

# 19. 第十二版核心链路总结

服务端请求上限链路：

```text
minikv_server 参数
 -> --max-request-bytes bytes
 -> parse_positive_size
 -> TcpServer::Options::max_request_bytes
 -> TcpServer::run()
 -> serve_client(..., max_request_bytes)
 -> pending.size() > max_request_bytes
 -> 返回协议错误
 -> event=tcp_request_rejected
```

客户端 timeout 链路：

```text
minikv_client 第三个参数
 -> parse_positive_milliseconds
 -> connect_to_server(..., timeout)
 -> set_socket_timeout
 -> SO_RCVTIMEO / SO_SNDTIMEO
 -> send / recv 受超时影响
```

日志观测链路：

```text
event=server_start
 -> max_request_bytes
 -> accept_poll_ms

event=tcp_listen
 -> max_request_bytes

event=tcp_request_rejected
 -> connection_id
 -> reason=request_too_long
 -> pending_bytes
 -> max_request_bytes
```

---

# 第二十六次总结

第十二版核心代码把 TCP 层从“能处理请求”推进到“能限制请求、能解释拒绝、能调整客户端等待行为”。

一句话总结：

```text
第十二版让服务端请求边界从硬编码变成可配置参数，同时让客户端具备 socket timeout，mini-kv 的网络层开始具备更清楚的运行控制能力。
```
