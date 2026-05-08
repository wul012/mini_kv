# 第四次讲解：`tcp_server.hpp` 和 `tcp_server.cpp`

这部分是整个项目里最复杂的一层，因为它负责 **网络通信**。

前面几次讲过：

```text
Store
 -> 保存数据

CommandProcessor
 -> 解析命令并调用 Store

main.cpp
 -> 本地命令行入口

server_main.cpp
 -> TCP 服务端启动入口
```

这次的 `TcpServer` 就是：

```text
真正负责监听端口、接收客户端、读取网络命令、返回结果的模块
```

整体流程是：

```text
server_main.cpp 创建 Store
 -> 创建 TcpServer
 -> 调用 server.run()
 -> TcpServer 监听端口
 -> 客户端连接进来
 -> 为每个客户端启动一个线程
 -> 读取客户端发来的一行命令
 -> CommandProcessor 执行命令
 -> 把结果发回客户端
```

可以把它理解成：

```text
TcpServer = 网络版的 main.cpp
```

`main.cpp` 从键盘读命令，`TcpServer` 从 socket 读命令。

---

# 1. `tcp_server.hpp`：声明 TCP 服务端

先看头文件：

```cpp
class TcpServer {
```

这个类表示一个 TCP 服务端。

它的职责是：

```text
绑定地址和端口
监听客户端连接
把客户端发来的命令交给 Store/CommandProcessor
```

## 配置结构：`Options`

```cpp
struct Options {
    std::string host = "0.0.0.0";
    std::uint16_t port = 6379;
    int backlog = 64;
};
```

`Options` 保存服务端启动配置。

第一个字段：

```cpp
std::string host = "0.0.0.0";
```

`host` 表示监听地址。

默认是：

```text
0.0.0.0
```

意思是监听本机所有网络地址。

第二个字段：

```cpp
std::uint16_t port = 6379;
```

`port` 表示监听端口。

默认是：

```text
6379
```

这个端口是 Redis 默认端口，所以 mini-kv 也用它来模拟 Redis 风格。

第三个字段：

```cpp
int backlog = 64;
```

`backlog` 是监听队列长度。

简单理解就是：

```text
当很多客户端同时连接时，操作系统最多可以排队等待多少个连接
```

这里设置为 64。

## 构造函数

```cpp
explicit TcpServer(Store& store, Options options);
```

创建 `TcpServer` 时需要两个东西：

```text
Store& store
Options options
```

`Store` 是数据库。

`Options` 是监听配置。

注意这里传的是：

```cpp
Store& store
```

也就是引用。

这说明：

```text
TcpServer 不拥有 Store
它只是使用 server_main.cpp 创建的那个 Store
```

所以所有客户端共享同一个内存数据库。

## 启动函数

```cpp
void run();
```

这是最重要的函数。

`server_main.cpp` 最后调用：

```cpp
server.run();
```

然后服务端就开始长期运行。

## 私有成员

```cpp
Store& store_;
Options options_;
```

`store_` 保存外部传入的数据库引用。

`options_` 保存监听配置。

一句话总结：`tcp_server.hpp` 声明了 TCP 服务端的外壳，真正实现都在 `tcp_server.cpp`。

---

# 2. `tcp_server.cpp`：实现网络通信

这个文件负责跨平台 socket 操作。

它同时支持：

```text
Windows
Linux / macOS
```

所以里面有很多条件编译：

```cpp
#ifdef _WIN32
...
#else
...
#endif
```

因为 Windows 的 socket API 和 Linux/macOS 不完全一样。

---

# 3. 引入项目头文件

```cpp
#include "minikv/tcp_server.hpp"
#include "minikv/command.hpp"
```

第一个是当前类的声明。

第二个是命令处理器。

原因是 TCP 服务端收到客户端命令后，需要调用：

```cpp
CommandProcessor processor{store};
processor.execute(line);
```

也就是说：

```text
TcpServer 负责网络
CommandProcessor 负责命令
Store 负责数据
```

---

# 4. 引入标准库

```cpp
#include <algorithm>
#include <array>
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
```

这些库大概负责：

```text
algorithm    用于 std::min
array        固定大小接收缓冲区
cstring      错误字符串处理
functional   std::ref
stdexcept    抛出 runtime_error
string       字符串
string_view  轻量字符串视图
thread       每个客户端一个线程
utility      std::move
```

这里最关键的是：

```cpp
#include <thread>
```

因为服务端接收客户端后，会启动新线程处理连接。

---

# 5. Windows 和非 Windows 的 socket 头文件

```cpp
#ifdef _WIN32
```

如果是 Windows，就包含：

```cpp
#include <winsock2.h>
#include <ws2tcpip.h>
```

Windows 下 socket 需要 Winsock。

而且使用前必须调用：

```cpp
WSAStartup(...)
```

结束时要调用：

```cpp
WSACleanup()
```

非 Windows 系统则包含：

```cpp
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
```

Linux/macOS 上 socket 是类 Unix 文件描述符，不需要 `WSAStartup`。

---

# 6. 匿名命名空间

```cpp
namespace {
```

这里放了一批只在当前文件内部使用的工具类和函数。

比如：

```text
SocketGuard
AddrInfoGuard
send_all
serve_client
bind_listener
```

这些函数不是公共 API，只是 `tcp_server.cpp` 自己用。

所以放在匿名命名空间里，避免暴露给其他文件。

---

# 7. Socket 类型适配

Windows 下：

```cpp
using SocketHandle = SOCKET;
constexpr SocketHandle invalid_socket = INVALID_SOCKET;
```

Windows 的 socket 类型是 `SOCKET`。

无效 socket 是：

```cpp
INVALID_SOCKET
```

非 Windows 下：

```cpp
using SocketHandle = int;
constexpr SocketHandle invalid_socket = -1;
```

Linux/macOS 的 socket 是整数文件描述符。

无效值是：

```text
-1
```

所以这里用 `using SocketHandle = ...` 把两边统一起来。

后面的代码就可以统一写：

```cpp
SocketHandle client
```

不用到处区分 Windows 和 Linux。

---

# 8. 关闭 socket：`close_socket`

Windows 下：

```cpp
void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        closesocket(socket);
    }
}
```

Windows 要用：

```cpp
closesocket(...)
```

非 Windows 下：

```cpp
void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        close(socket);
    }
}
```

Linux/macOS 要用：

```cpp
close(...)
```

这个函数统一了关闭 socket 的行为。

---

# 9. 网络运行环境：`NetworkRuntime`

Windows 下：

```cpp
class NetworkRuntime {
public:
    NetworkRuntime() {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
            throw std::runtime_error{"WSAStartup failed"};
        }
    }

    ~NetworkRuntime() {
        WSACleanup();
    }
};
```

这是一个 RAII 工具类。

RAII 的意思是：

```text
对象创建时获取资源
对象销毁时释放资源
```

在 Windows 下，使用 socket 前必须先调用：

```cpp
WSAStartup(...)
```

程序结束或不再使用 socket 时调用：

```cpp
WSACleanup()
```

所以这里把它封装成类：

```text
构造函数 -> WSAStartup
析构函数 -> WSACleanup
```

这样不容易忘记清理。

非 Windows 下：

```cpp
class NetworkRuntime {
public:
    NetworkRuntime() = default;
};
```

Linux/macOS 不需要初始化网络环境，所以这里是空实现。

---

# 10. 错误信息：`socket_error_message`

Windows 下：

```cpp
return std::string{prefix} + ": WSA error " + std::to_string(WSAGetLastError());
```

Windows 用：

```cpp
WSAGetLastError()
```

获取 socket 错误码。

非 Windows 下：

```cpp
return std::string{prefix} + ": " + std::strerror(errno);
```

Linux/macOS 用：

```cpp
errno
std::strerror(errno)
```

获取错误描述。

这个函数用于生成统一的异常信息。

---

# 11. 自动关闭 socket：`SocketGuard`

```cpp
class SocketGuard {
```

这是一个非常重要的资源管理类。

它的作用是：

```text
确保 socket 最后一定会被关闭
```

构造函数：

```cpp
explicit SocketGuard(SocketHandle socket = invalid_socket) : socket_(socket) {}
```

创建时保存 socket。

析构函数：

```cpp
~SocketGuard() {
    close_socket(socket_);
}
```

对象销毁时自动关闭 socket。

这避免了手动管理 socket 时常见的问题：

```text
某个分支 return 了，忘记 close
发生异常了，忘记 close
代码变复杂后资源泄漏
```

有了 `SocketGuard`，只要对象离开作用域，socket 就自动关闭。

它禁止复制：

```cpp
SocketGuard(const SocketGuard&) = delete;
SocketGuard& operator=(const SocketGuard&) = delete;
```

原因是 socket 是独占资源。

如果两个 `SocketGuard` 管理同一个 socket，就可能重复关闭。

但它允许移动：

```cpp
SocketGuard(SocketGuard&& other) noexcept : socket_(other.release()) {}
```

移动的意思是：

```text
把 socket 所有权从一个对象转移到另一个对象
```

`release()` 函数：

```cpp
SocketHandle release() {
    const SocketHandle socket = socket_;
    socket_ = invalid_socket;
    return socket;
}
```

它把当前 socket 取出来，并把自己内部置为无效。

这样析构时就不会再关闭这个 socket。

一句话总结：`SocketGuard` 是 socket 的自动清理器。

---

# 12. 自动释放地址信息：`AddrInfoGuard`

```cpp
class AddrInfoGuard {
```

这个类负责管理 `getaddrinfo` 返回的地址链表。

`getaddrinfo` 会分配一块地址信息，使用完必须调用：

```cpp
freeaddrinfo(...)
```

所以这里也用 RAII 封装。

构造函数保存地址信息：

```cpp
explicit AddrInfoGuard(addrinfo* info) : info_(info) {}
```

析构函数自动释放：

```cpp
~AddrInfoGuard() {
    if (info_ != nullptr) {
        freeaddrinfo(info_);
    }
}
```

它同样禁止复制，避免重复释放。

一句话总结：`AddrInfoGuard` 确保 `getaddrinfo` 的结果一定被释放。

---

# 13. 发送完整数据：`send_all`

```cpp
bool send_all(SocketHandle socket, std::string_view data) {
```

这个函数负责把一整段字符串发送给客户端。

为什么不能直接调用一次 `send`？

因为 TCP 的 `send` 不保证一次把所有数据都发出去。

比如你想发送 10000 字节，`send` 可能只发送了 4096 字节。

所以需要循环发送，直到全部发完。

核心循环：

```cpp
while (!data.empty()) {
```

只要还有没发送的数据，就继续。

每次最多发送 4096 字节：

```cpp
const std::size_t chunk_size = std::min<std::size_t>(data.size(), 4096);
```

然后调用 `send`。

如果发送失败：

```cpp
if (sent <= 0) {
    return false;
}
```

如果发送成功，就把已经发送的部分从 `data` 前面移除：

```cpp
data.remove_prefix(static_cast<std::size_t>(sent));
```

最后全部发完：

```cpp
return true;
```

一句话总结：`send_all` 保证响应文本尽量完整发送给客户端。

---

# 14. 服务单个客户端：`serve_client`

```cpp
void serve_client(Store& store, SocketHandle client_socket) {
```

这个函数负责处理一个客户端连接。

它接收两个参数：

```text
Store& store
client_socket
```

`store` 是共享数据库。

`client_socket` 是当前客户端的 socket。

第一步：

```cpp
SocketGuard client{client_socket};
```

用 `SocketGuard` 接管客户端 socket。

这样函数结束时，客户端连接会自动关闭。

第二步：

```cpp
CommandProcessor processor{store};
```

为这个客户端创建一个命令处理器。

注意，它使用的是同一个共享 `Store`。

所以多个客户端虽然各自有自己的 `CommandProcessor`，但背后的数据库是同一个。

第三步：

```cpp
std::string pending;
std::array<char, 4096> buffer{};
```

`buffer` 是每次从网络读取数据的临时缓冲区。

`pending` 是还没处理完的文本。

为什么需要 `pending`？

因为 TCP 是字节流，不是消息流。

客户端发送：

```text
SET name mini-kv\n
```

服务端一次 `recv` 不一定刚好收到完整一行。

可能收到：

```text
SET name
```

下一次才收到：

```text
 mini-kv\n
```

也可能一次收到多行：

```text
SET name mini-kv\nGET name\n
```

所以服务端需要把收到的数据先放进 `pending`，然后按换行符 `\n` 切分命令。

---

# 15. 发送欢迎语

```cpp
if (!send_all(client.get(), "mini-kv ready\n")) {
    return;
}
```

客户端连接成功后，服务端先发：

```text
mini-kv ready
```

如果发送失败，说明客户端可能已经断开，就直接返回。

---

# 16. 循环接收客户端数据

```cpp
while (true) {
```

只要客户端不断开，这个函数就一直读取数据。

`recv` 返回收到的字节数。

如果：

```cpp
if (received <= 0) {
    return;
}
```

说明连接关闭或出错，直接结束当前客户端处理。

---

# 17. 把收到的数据追加到 pending

```cpp
pending.append(buffer.data(), static_cast<std::size_t>(received));
```

这里把刚刚收到的字节追加到 `pending` 后面。

比如之前 `pending` 是：

```text
SET name
```

这次收到：

```text
 mini-kv\n
```

追加后变成：

```text
SET name mini-kv\n
```

这样就能组成完整命令。

---

# 18. 按换行符提取命令

```cpp
std::size_t newline = std::string::npos;
while ((newline = pending.find('\n')) != std::string::npos) {
```

这段逻辑会不断查找 `\n`。

只要找到一行完整命令，就处理一次。

取出一行：

```cpp
std::string line = pending.substr(0, newline);
```

删除已经处理的内容：

```cpp
pending.erase(0, newline + 1);
```

比如：

```text
pending = "SET name mini-kv\nGET name\n"
```

第一次取出：

```text
line = "SET name mini-kv"
```

剩下：

```text
pending = "GET name\n"
```

第二次取出：

```text
line = "GET name"
```

剩下：

```text
pending = ""
```

---

# 19. 执行命令

```cpp
const auto result = processor.execute(line);
```

这里就和 CLI 的逻辑一样了。

区别只是来源不同：

```text
CLI:
  line 来自 std::getline(std::cin, line)

TCP:
  line 来自 socket recv
```

但后面的命令处理完全复用：

```text
CommandProcessor::execute
 -> Store::set/get/erase/size
```

---

# 20. 返回执行结果给客户端

```cpp
if (!result.response.empty()) {
    if (!send_all(client.get(), result.response + "\n")) {
        return;
    }
}
```

如果命令有响应，就把响应加上换行符发回客户端。

比如客户端发：

```text
GET name\n
```

服务端返回：

```text
mini-kv\n
```

如果发送失败，就结束当前客户端连接。

---

# 21. 处理退出命令

```cpp
if (result.should_close) {
    return;
}
```

如果客户端发送：

```text
QUIT
```

`CommandProcessor` 返回：

```text
response = BYE
should_close = true
```

服务端先发送：

```text
BYE
```

然后这里 `return`。

`serve_client` 结束后，`SocketGuard` 自动关闭客户端 socket。

注意，这只关闭当前客户端连接，不会关闭整个服务端。

---

# 22. 防止一行太长

```cpp
if (pending.size() > 64 * 1024) {
    send_all(client.get(), "ERR line too long\n");
    return;
}
```

如果客户端一直发送数据但不发送换行符，`pending` 会越来越大。

为了避免内存无限增长，这里限制单行最大约：

```text
64 KB
```

超过就返回错误：

```text
ERR line too long
```

然后关闭连接。

这是一种基础的安全保护。

---

# 23. 绑定监听地址：`bind_listener`

```cpp
SocketGuard bind_listener(const TcpServer::Options& options) {
```

这个函数负责创建监听 socket。

它会根据：

```text
options.host
options.port
options.backlog
```

完成：

```text
解析地址
创建 socket
设置端口复用
bind
listen
```

最后返回一个监听 socket。

第一步准备地址查询参数：

```cpp
addrinfo hints{};
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;
```

含义是：

```text
AF_UNSPEC
 -> IPv4 或 IPv6 都可以

SOCK_STREAM
 -> TCP

AI_PASSIVE
 -> 用于服务端监听地址
```

第二步把端口转成字符串：

```cpp
const std::string port = std::to_string(options.port);
```

`getaddrinfo` 需要字符串形式的端口，所以把数字转成字符串。

第三步解析地址：

```cpp
addrinfo* raw_info = nullptr;
const int result = getaddrinfo(options.host.c_str(), port.c_str(), &hints, &raw_info);
```

这里根据 host 和 port 获取可用地址列表。

如果失败，就抛出异常。

第四步用 `AddrInfoGuard` 管理地址结果：

```cpp
AddrInfoGuard addresses{raw_info};
```

这样后面函数结束时，会自动调用 `freeaddrinfo`。

---

# 24. 尝试每个地址

```cpp
for (addrinfo* addr = addresses.get(); addr != nullptr; addr = addr->ai_next) {
```

`getaddrinfo` 可能返回多个地址。

服务端逐个尝试，直到某个地址可以成功：

```text
socket
 -> bind
 -> listen
```

创建 socket：

```cpp
SocketGuard listener{socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)};
```

如果创建失败，就尝试下一个地址。

设置端口复用：

```cpp
int reuse = 1;
setsockopt(listener.get(), SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&reuse), sizeof(reuse));
```

`SO_REUSEADDR` 的作用是让端口在一些情况下可以更快重新绑定。

比如服务刚关闭后马上重启，端口可能还处于系统保留状态。

设置这个选项可以减少“端口暂时不可用”的情况。

绑定地址：

```cpp
if (bind(listener.get(), addr->ai_addr, static_cast<int>(addr->ai_addrlen)) != 0) {
    continue;
}
```

`bind` 的意思是：

```text
把 socket 绑定到 host:port
```

比如：

```text
0.0.0.0:6379
```

如果失败，可能是：

```text
端口被占用
地址不可用
权限不足
```

这里会继续尝试下一个地址。

开始监听：

```cpp
if (listen(listener.get(), options.backlog) != 0) {
    continue;
}
```

`listen` 之后，这个 socket 就变成监听 socket。

它可以接收客户端连接。

如果成功：

```cpp
return listener;
```

把监听 socket 返回出去。

如果所有地址都失败：

```cpp
throw std::runtime_error{socket_error_message("bind/listen failed")};
```

说明服务端启动失败。

---

# 25. 构造函数

```cpp
TcpServer::TcpServer(Store& store, Options options) : store_(store), options_(std::move(options)) {}
```

这个构造函数很短。

它做两件事：

```text
把外部 Store 引用保存到 store_
把 options 移动保存到 options_
```

`std::move(options)` 表示把配置对象移动进成员变量，避免不必要复制。

---

# 26. 启动服务端：`TcpServer::run`

```cpp
void TcpServer::run() {
```

这是 `TcpServer` 的核心入口。

`server_main.cpp` 调用：

```cpp
server.run();
```

最终就进入这里。

第一步：

```cpp
NetworkRuntime runtime;
```

初始化网络环境。

Windows 下会调用 `WSAStartup`。

非 Windows 下什么也不做。

这个对象存在于 `run()` 的整个生命周期中。

第二步：

```cpp
SocketGuard listener = bind_listener(options_);
```

创建监听 socket。

这里会完成：

```text
getaddrinfo
socket
setsockopt
bind
listen
```

如果失败，会抛出异常，让 `server_main.cpp` 的 `catch` 捕获。

第三步进入无限循环：

```cpp
while (true) {
```

服务端要一直等待客户端连接。

接受连接：

```cpp
SocketHandle client = accept(listener.get(), nullptr, nullptr);
```

`accept` 会阻塞等待客户端连接。

当有客户端连接进来，它返回一个新的 socket。

注意有两个 socket：

```text
listener socket
 -> 只负责监听新连接

client socket
 -> 负责和某一个客户端通信
```

如果 accept 失败：

```cpp
if (client == invalid_socket) {
    throw std::runtime_error{socket_error_message("accept failed")};
}
```

服务端认为这是致命错误，抛出异常。

为客户端创建线程：

```cpp
std::thread{serve_client, std::ref(store_), client}.detach();
```

这行很重要。

它的意思是：

```text
启动一个新线程
在线程里调用 serve_client(store_, client)
然后 detach，让线程独立运行
```

`std::ref(store_)` 的作用是：

```text
把 store_ 按引用传给线程函数
```

否则线程可能会尝试复制 `Store`。

但 `Store` 内部有 `mutex`，不适合复制。

`detach()` 表示：

```text
主线程不等待这个客户端线程结束
客户端线程自己处理完自己退出
```

所以服务端主线程可以继续回到 `accept`，接收下一个客户端。

这就是：

```text
一个客户端一个线程
```

的并发模型。

---

# 27. 多客户端共享数据

因为每个客户端线程都拿到同一个：

```cpp
store_
```

所以数据是共享的。

比如：

```text
客户端 A:
SET name mini-kv

客户端 B:
GET name
```

客户端 B 可以得到：

```text
mini-kv
```

这也解释了为什么 `Store` 里面必须有：

```cpp
std::mutex mutex_;
```

因为多个线程可能同时读写同一个 `unordered_map`。

---

# 28. TCP 服务端完整流程

一次完整运行可以理解成：

```text
server_main.cpp
 -> 创建 Store
 -> 创建 TcpServer
 -> 调用 run()

TcpServer::run
 -> 初始化网络环境
 -> bind_listener 创建监听 socket
 -> while true 等待客户端

客户端连接
 -> accept 返回 client socket
 -> 创建线程 serve_client

serve_client
 -> 发送 mini-kv ready
 -> recv 接收字节
 -> pending 按 \n 切成命令
 -> processor.execute(line)
 -> send_all 返回结果
 -> QUIT 或断开时关闭连接
```

---

# 29. 和前面代码的关系

`tcp_server.cpp` 不直接实现 `SET`、`GET`、`DEL`。

它只是把网络收到的文本命令交给：

```cpp
CommandProcessor
```

而 `CommandProcessor` 再调用：

```cpp
Store
```

所以职责非常清楚：

```text
tcp_server.cpp
 -> 负责网络输入输出

command.cpp
 -> 负责命令解析

store.cpp
 -> 负责数据存储
```

这样的好处是：

```text
CLI 和 TCP Server 可以复用同一套命令逻辑
```

比如：

```text
SET name mini-kv
```

无论是在 CLI 输入，还是通过 TCP 发送，最终都会走：

```cpp
CommandProcessor::execute(...)
```

---

# 第四次总结

本次讲解的是 mini-kv 的网络层。

`tcp_server.hpp` 声明了 `TcpServer` 类和 `Options` 配置。

`tcp_server.cpp` 实现了完整 TCP 服务端逻辑：

```text
跨平台 socket 适配
Windows 网络初始化
socket 自动关闭管理
地址信息自动释放
完整发送响应
按行读取客户端命令
每个客户端一个线程
绑定和监听端口
accept 接收连接
调用 CommandProcessor 执行命令
```

一句话总结：

```text
tcp_server.cpp 是 mini-kv 的网络外壳，它把 TCP 客户端发来的一行文本命令，转换成对 CommandProcessor 的调用，再把执行结果通过 socket 发回客户端。
```
