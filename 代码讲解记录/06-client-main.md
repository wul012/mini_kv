# 第六次讲解：`src/client_main.cpp`

这个文件是第二版新增的 **TCP 客户端入口**。

前面第四次讲过：

```text
tcp_server.cpp = 网络服务端
```

那么这次新增的：

```text
client_main.cpp = 网络客户端
```

它的作用不是保存数据，也不是解析 `SET / GET / DEL` 这些业务命令。

它负责的是：

```text
连接正在运行的 minikv_server
 -> 读取服务端 greeting
 -> 从终端或管道读取用户命令
 -> 把命令通过 TCP 发给服务端
 -> 读取服务端响应
 -> 打印到屏幕
 -> 遇到 QUIT / EXIT 后退出
```

可以把它理解成：

```text
main.cpp 是本地 CLI
client_main.cpp 是远程 CLI
```

它们都给用户一个命令行窗口。

区别是：

```text
main.cpp
 -> 直接在本进程里创建 Store
 -> 直接调用 CommandProcessor

client_main.cpp
 -> 不创建 Store
 -> 不调用 CommandProcessor
 -> 只负责通过 socket 把文本命令发给服务端
```

所以第二版的整体链路变成：

```text
用户输入命令
 -> minikv_client
 -> TCP socket
 -> minikv_server
 -> CommandProcessor
 -> Store
 -> CommandProcessor 返回文本
 -> minikv_server 通过 TCP 发回
 -> minikv_client 打印响应
```

---

# 1. 引入头文件

文件开头：

```cpp
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
```

这些是标准库头文件。

它们分别负责：

```text
algorithm     使用 std::ranges::all_of、std::min
cctype        判断空白字符、大小写转换
cstdint       使用 std::uint16_t 保存端口
cstring       Linux / macOS 下把 errno 转成错误文本
exception     捕获 std::exception
iostream      输入输出
stdexcept     抛出 runtime_error / out_of_range
string        保存 host、命令行、响应
string_view   轻量传递字符串片段
```

这里没有包含项目自己的头文件，比如：

```cpp
#include "minikv/command.hpp"
```

这是刻意的。

因为客户端不应该直接执行命令。

客户端只知道：

```text
我把一行文本发出去
然后等服务端回一行或多行文本
```

真正的命令解析仍然在服务端。

---

# 2. 区分 Windows 和非 Windows 网络 API

接下来是平台相关代码：

```cpp
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
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

这和 `tcp_server.cpp` 的思路一样。

Windows 下 socket API 来自 Winsock：

```text
winsock2.h
ws2tcpip.h
```

非 Windows 下 socket API 来自 POSIX：

```text
netdb.h
sys/socket.h
unistd.h
```

为什么要分开？

因为 Windows 和 Linux / macOS 的 socket 很像，但并不完全一样：

```text
Windows socket 类型是 SOCKET
Linux socket 类型是 int

Windows 关闭 socket 用 closesocket
Linux 关闭 socket 用 close

Windows 使用 socket 前需要 WSAStartup
Linux 不需要
```

所以这里用：

```cpp
#ifdef _WIN32
```

把两套差异包起来。

---

# 3. 匿名 namespace

```cpp
namespace {
```

这里进入匿名命名空间。

意思是：

```text
下面这些辅助函数和类只在 client_main.cpp 内部可见
```

比如：

```cpp
SocketGuard
connect_to_server
read_line
send_all
```

这些东西不是 mini-kv 的公共 API。

它们只是客户端入口文件的内部工具。

放进匿名 namespace，可以避免和别的文件里的同名函数冲突。

---

# 4. Windows 下的 SocketHandle

Windows 分支：

```cpp
using SocketHandle = SOCKET;
constexpr SocketHandle invalid_socket = INVALID_SOCKET;
```

这里做了一个类型别名：

```text
SocketHandle = SOCKET
```

后面代码就可以统一写：

```cpp
SocketHandle socket
```

而不用到处写 `SOCKET`。

`invalid_socket` 表示无效 socket。

Windows 下无效 socket 是：

```cpp
INVALID_SOCKET
```

---

关闭 socket 的函数：

```cpp
void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        closesocket(socket);
    }
}
```

它先判断 socket 是否有效。

如果有效，就调用：

```cpp
closesocket(socket);
```

这里不要用 `close()`，因为 Windows socket 必须用 `closesocket()` 关闭。

---

# 5. Windows 网络运行时 `NetworkRuntime`

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
```

Windows 使用 socket 前必须先调用：

```cpp
WSAStartup(...)
```

程序结束时再调用：

```cpp
WSACleanup()
```

这里把这两个操作放进一个类里。

创建对象时：

```text
构造函数调用 WSAStartup
```

对象销毁时：

```text
析构函数调用 WSACleanup
```

这样可以保证：

```text
只要 NetworkRuntime 对象存在
Winsock 就是初始化好的
```

如果初始化失败：

```cpp
throw std::runtime_error{"WSAStartup failed"};
```

程序会进入最外层 `catch`，打印 fatal 错误。

---

下面两行禁止拷贝：

```cpp
NetworkRuntime(const NetworkRuntime&) = delete;
NetworkRuntime& operator=(const NetworkRuntime&) = delete;
```

因为网络运行时这种对象不应该被复制。

它代表的是一段成对的资源管理：

```text
构造 = 初始化
析构 = 清理
```

复制它没有意义，还容易导致多次清理。

---

# 6. Windows 错误信息

```cpp
std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": WSA error " + std::to_string(WSAGetLastError());
}
```

如果 socket 操作失败，Windows 用：

```cpp
WSAGetLastError()
```

取得错误码。

这个函数会把错误码拼成类似：

```text
connect failed: WSA error 10061
```

方便用户知道是哪一步失败了。

---

# 7. 非 Windows 下的 SocketHandle

非 Windows 分支：

```cpp
using SocketHandle = int;
constexpr SocketHandle invalid_socket = -1;
```

Linux / macOS 下 socket 本质上是文件描述符，所以类型是：

```text
int
```

无效 socket 通常是：

```text
-1
```

关闭 socket：

```cpp
void close_socket(SocketHandle socket) {
    if (socket != invalid_socket) {
        close(socket);
    }
}
```

这里用 POSIX 的：

```cpp
close(socket);
```

---

非 Windows 的 `NetworkRuntime` 很简单：

```cpp
class NetworkRuntime {
public:
    NetworkRuntime() = default;
};
```

因为 Linux / macOS 不需要 `WSAStartup`。

为了让后面的主流程跨平台统一，仍然保留这个类。

这样 `main()` 里可以始终写：

```cpp
NetworkRuntime runtime;
```

不用在主流程里到处写平台判断。

---

非 Windows 错误信息：

```cpp
std::string socket_error_message(std::string_view prefix) {
    return std::string{prefix} + ": " + std::strerror(errno);
}
```

POSIX socket 失败时，一般通过：

```cpp
errno
```

拿到错误码，再用：

```cpp
std::strerror(errno)
```

转成人能看懂的错误文本。

---

# 8. `SocketGuard`：自动关闭 socket

```cpp
class SocketGuard {
public:
    explicit SocketGuard(SocketHandle socket = invalid_socket) : socket_(socket) {}
```

`SocketGuard` 是一个 RAII 包装类。

RAII 的意思是：

```text
资源的生命周期和对象生命周期绑定
```

这里的资源就是：

```text
socket
```

创建 `SocketGuard` 时，把 socket 存进去。

---

析构函数：

```cpp
~SocketGuard() {
    close_socket(socket_);
}
```

对象销毁时，自动关闭 socket。

这很重要。

因为客户端里有很多可能提前退出的路径：

```text
连接失败
读取失败
发送失败
用户输入结束
服务端关闭连接
```

如果每个分支都手动写 `close_socket()`，很容易漏。

有了 `SocketGuard`，只要对象离开作用域，socket 就会自动关闭。

---

禁止拷贝：

```cpp
SocketGuard(const SocketGuard&) = delete;
SocketGuard& operator=(const SocketGuard&) = delete;
```

因为一个 socket 只能由一个 guard 负责关闭。

如果允许复制，就可能出现：

```text
两个 SocketGuard 指向同一个 socket
第一个析构关闭一次
第二个析构又关闭一次
```

这会造成资源管理错误。

---

允许移动：

```cpp
SocketGuard(SocketGuard&& other) noexcept : socket_(other.release()) {}
```

移动构造的意思是：

```text
把 other 管理的 socket 转移给当前对象
```

`other.release()` 会把原对象里的 socket 取出来，并把原对象置为无效。

这样 socket 仍然只会被关闭一次。

---

移动赋值：

```cpp
SocketGuard& operator=(SocketGuard&& other) noexcept {
    if (this != &other) {
        close_socket(socket_);
        socket_ = other.release();
    }
    return *this;
}
```

如果当前对象已经持有一个 socket，先关闭它。

然后从 `other` 接管新的 socket。

---

取得 socket：

```cpp
SocketHandle get() const {
    return socket_;
}
```

后面调用：

```cpp
send(socket.get(), ...)
recv(socket.get(), ...)
```

就靠这个函数拿到底层 socket。

---

释放所有权：

```cpp
SocketHandle release() {
    const SocketHandle socket = socket_;
    socket_ = invalid_socket;
    return socket;
}
```

`release()` 的意思是：

```text
把 socket 交出去
当前 guard 不再负责关闭它
```

这个函数主要用于移动构造和移动赋值。

---

# 9. `AddrInfoGuard`：自动释放地址列表

```cpp
class AddrInfoGuard {
public:
    explicit AddrInfoGuard(addrinfo* info) : info_(info) {}
```

`getaddrinfo()` 会返回一个 `addrinfo` 链表。

用完后必须调用：

```cpp
freeaddrinfo(info);
```

所以这里也做了一个 RAII 包装。

---

析构函数：

```cpp
~AddrInfoGuard() {
    if (info_ != nullptr) {
        freeaddrinfo(info_);
    }
}
```

只要 `AddrInfoGuard` 离开作用域，就自动释放地址列表。

---

同样禁止拷贝：

```cpp
AddrInfoGuard(const AddrInfoGuard&) = delete;
AddrInfoGuard& operator=(const AddrInfoGuard&) = delete;
```

因为这份地址列表也只能释放一次。

---

取出原始指针：

```cpp
addrinfo* get() const {
    return info_;
}
```

后面遍历地址列表时会用到。

---

# 10. 解析端口 `parse_port`

```cpp
std::uint16_t parse_port(const char* text) {
    const int port = std::stoi(text);
    if (port < 1 || port > 65535) {
        throw std::out_of_range{"port must be between 1 and 65535"};
    }
    return static_cast<std::uint16_t>(port);
}
```

客户端启动时可以传端口：

```powershell
.\minikv_client.exe 127.0.0.1 6379
```

命令行参数进来时是字符串：

```text
"6379"
```

`std::stoi(text)` 把它转成整数。

然后检查范围：

```text
1 到 65535
```

这是 TCP / UDP 端口的合法范围。

如果端口不合法，就抛出异常：

```cpp
throw std::out_of_range{"port must be between 1 and 65535"};
```

最后转成：

```cpp
std::uint16_t
```

因为端口号本来就是 16 位无符号整数。

---

# 11. 命令名转大写 `to_upper`

```cpp
std::string to_upper(std::string_view text) {
    std::string result{text};
    for (char& ch : result) {
        ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    }
    return result;
}
```

这个函数把字符串转成大写。

比如：

```text
quit -> QUIT
exit -> EXIT
help -> HELP
```

客户端需要判断用户是不是输入了：

```text
QUIT
EXIT
HELP
```

用户可能输入小写，所以这里要统一成大写再判断。

这里写法比较严谨：

```cpp
std::toupper(static_cast<unsigned char>(ch))
```

因为 `std::toupper` 要求传入的值能表示成 `unsigned char` 或 EOF。

直接传 `char` 在某些平台上可能有未定义行为。

---

# 12. 取得第一个单词 `first_token`

```cpp
std::string first_token(std::string_view line) {
```

这个函数负责从一整行命令里取出命令名。

比如：

```text
SET name mini-kv
```

第一个 token 是：

```text
SET
```

---

先跳过开头空白：

```cpp
while (!line.empty() && std::isspace(static_cast<unsigned char>(line.front())) != 0) {
    line.remove_prefix(1);
}
```

如果用户输入：

```text
   quit
```

前面的空格会被去掉。

---

找第一个空白分隔符：

```cpp
const std::size_t end = line.find_first_of(" \t\r\n");
```

也就是找：

```text
空格
tab
回车
换行
```

如果找不到，说明整行只有一个单词：

```cpp
if (end == std::string_view::npos) {
    return to_upper(line);
}
```

比如：

```text
QUIT
```

整行就是命令名。

如果找到了，就只取前半段：

```cpp
return to_upper(line.substr(0, end));
```

比如：

```text
SET name mini-kv
```

返回：

```text
SET
```

一句话总结：`first_token()` 用来让客户端知道用户输入的是哪条命令。

---

# 13. 判断空行 `is_blank`

```cpp
bool is_blank(std::string_view line) {
    return std::ranges::all_of(line, [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
}
```

这个函数判断一行是不是全是空白字符。

比如：

```text
""
"   "
"\t"
```

都算空白行。

客户端遇到空白行时，不会发给服务端。

原因是：

```text
用户只是按了一下回车
不需要占用一次网络往返
也不需要服务端返回空响应
```

---

# 14. 完整发送数据 `send_all`

```cpp
bool send_all(SocketHandle socket, std::string_view data) {
```

这个函数负责把一段字符串完整发到 socket。

为什么不直接调用一次 `send()`？

因为 `send()` 不保证一次把所有内容都发完。

比如你想发：

```text
SET name mini-kv\n
```

理论上 `send()` 可能只发出去前几个字节。

所以要循环发送，直到 `data` 为空。

---

每次最多发 4096 字节：

```cpp
const std::size_t chunk_size = std::min<std::size_t>(data.size(), 4096);
```

如果数据很长，就分块发。

---

平台分支：

```cpp
#ifdef _WIN32
const int sent = send(socket, data.data(), static_cast<int>(chunk_size), 0);
#else
const auto sent = send(socket, data.data(), chunk_size, 0);
#endif
```

Windows 的 `send()` 长度参数是 `int`。

非 Windows 通常接受 `size_t`。

所以 Windows 下做了：

```cpp
static_cast<int>(chunk_size)
```

---

如果发送失败：

```cpp
if (sent <= 0) {
    return false;
}
```

返回 `false`，交给主流程打印错误。

---

如果发送成功，就把已经发送的部分从 `data` 前面移除：

```cpp
data.remove_prefix(static_cast<std::size_t>(sent));
```

循环继续，直到所有数据都发完。

最后：

```cpp
return true;
```

一句话总结：`send_all()` 保证一整条命令都会被完整写入 socket。

---

# 15. 按行读取响应 `read_line`

```cpp
bool read_line(SocketHandle socket, std::string& line) {
    line.clear();
```

服务端协议是：

```text
一行一个响应
```

所以客户端需要一直读取，直到遇到换行符：

```text
\n
```

---

进入循环：

```cpp
while (true) {
    char ch = '\0';
```

一次读一个字符。

虽然这不是最高性能的做法，但这个项目当前是练习版客户端，协议也很简单。

按字符读的优点是：

```text
逻辑直接
不会多读下一条响应
很容易按行处理 HELP 这种多行输出
```

---

调用 `recv()`：

```cpp
#ifdef _WIN32
const int received = recv(socket, &ch, 1, 0);
#else
const auto received = recv(socket, &ch, 1, 0);
#endif
```

这里每次最多读 1 个字节。

如果返回值小于等于 0：

```cpp
if (received <= 0) {
    return false;
}
```

说明连接关闭或读取失败。

---

遇到换行就结束：

```cpp
if (ch == '\n') {
    return true;
}
```

如果是回车：

```cpp
if (ch != '\r') {
    line.push_back(ch);
}
```

就忽略掉。

这样无论服务端返回：

```text
\n
```

还是：

```text
\r\n
```

客户端拿到的 `line` 都不带换行和回车。

---

最后有一个保护：

```cpp
if (line.size() > 64 * 1024) {
    throw std::runtime_error{"response line too long"};
}
```

如果服务端一直不发换行，客户端不能无限吃内存。

所以单行响应超过 64KB 就抛异常。

一句话总结：`read_line()` 从 socket 里读取一整行服务端响应。

---

# 16. 连接服务端 `connect_to_server`

```cpp
SocketGuard connect_to_server(std::string_view host, std::uint16_t port) {
```

这个函数负责根据：

```text
host
port
```

连接服务端。

比如：

```text
host = 127.0.0.1
port = 6379
```

---

先准备 `addrinfo` 查询条件：

```cpp
addrinfo hints{};
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
```

`AF_UNSPEC` 表示：

```text
IPv4 或 IPv6 都可以
```

`SOCK_STREAM` 表示：

```text
TCP
```

---

准备端口字符串：

```cpp
addrinfo* raw_info = nullptr;
const std::string port_text = std::to_string(port);
```

`getaddrinfo()` 要求端口是字符串形式。

所以把：

```text
6379
```

转成：

```text
"6379"
```

---

调用 `getaddrinfo()`：

```cpp
const int result = getaddrinfo(std::string{host}.c_str(), port_text.c_str(), &hints, &raw_info);
```

它会把 host 和 port 解析成一个地址列表。

比如：

```text
127.0.0.1:6379
localhost:6379
```

都可以通过它解析。

如果失败，就抛出错误。

Windows 下：

```cpp
throw std::runtime_error{"getaddrinfo failed: " + std::to_string(result)};
```

非 Windows 下：

```cpp
throw std::runtime_error{std::string{"getaddrinfo failed: "} + gai_strerror(result)};
```

---

用 `AddrInfoGuard` 接管地址列表：

```cpp
AddrInfoGuard addresses{raw_info};
```

这样函数退出时会自动调用 `freeaddrinfo()`。

---

遍历地址列表：

```cpp
for (addrinfo* addr = addresses.get(); addr != nullptr; addr = addr->ai_next) {
```

一个 host 可能对应多个地址。

比如：

```text
IPv4 地址
IPv6 地址
```

客户端会逐个尝试。

---

创建 socket：

```cpp
SocketGuard socket{::socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol)};
```

这里用了：

```cpp
::socket
```

前面的 `::` 表示调用全局命名空间里的 socket 函数。

创建出来后立刻交给 `SocketGuard` 管理。

如果创建失败：

```cpp
if (socket.get() == invalid_socket) {
    continue;
}
```

就尝试下一个地址。

---

尝试连接：

```cpp
if (connect(socket.get(), addr->ai_addr, static_cast<int>(addr->ai_addrlen)) == 0) {
    return socket;
}
```

`connect()` 返回 0 表示成功。

成功后直接返回这个 `SocketGuard`。

因为 `SocketGuard` 支持移动，所以这里可以安全返回。

如果连接失败，就进入下一轮。

当前这个 `socket` 离开循环作用域时会自动关闭。

---

如果所有地址都连接失败：

```cpp
throw std::runtime_error{socket_error_message("connect failed")};
```

比如服务端没启动，就可能看到类似：

```text
fatal: connect failed: WSA error 10061
```

一句话总结：`connect_to_server()` 把 host 和 port 变成一个已经连接好的 TCP socket。

---

# 17. 打印用法 `print_usage`

```cpp
void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [host] [port]\n";
}
```

如果用户传了太多参数，就打印用法。

客户端支持：

```text
不传参数
 -> 默认连接 127.0.0.1:6379

传 host
 -> 连接指定 host，端口仍然默认 6379

传 host 和 port
 -> 连接指定 host:port
```

所以用法是：

```text
minikv_client [host] [port]
```

---

# 18. 打印服务端响应 `print_response`

```cpp
bool print_response(SocketHandle socket, std::string_view command) {
```

这个函数负责：

```text
读取服务端响应
打印服务端响应
必要时读取多行响应
```

---

先读第一行：

```cpp
std::string response;
if (!read_line(socket, response)) {
    return false;
}
```

如果读不到，说明连接断了，返回 `false`。

---

打印第一行：

```cpp
std::cout << response << '\n';
```

比如：

```text
OK inserted
mini-kv
(nil)
BYE
```

---

特殊处理 `HELP`：

```cpp
if (first_token(command) == "HELP" && response == "Commands:") {
```

为什么需要特殊处理？

因为服务端的普通响应都是一行。

比如：

```text
GET name -> mini-kv
SIZE -> 1
```

但 `HELP` 返回多行：

```text
Commands:
  SET key value
  GET key
  DEL key
  SIZE
  HELP
  EXIT
```

如果客户端只读一行，就只能看到：

```text
Commands:
```

后面的行会留在 socket 缓冲区，影响下一条命令。

---

这里还有一个细节：

```cpp
response == "Commands:"
```

客户端不只判断命令名是 `HELP`。

还要确认第一行响应确实是：

```text
Commands:
```

这是为了处理错误用法：

```text
HELP extra
```

服务端会返回：

```text
ERR usage: HELP
```

这是一行错误响应，不是多行帮助文本。

如果客户端只看到命令名 `HELP` 就继续读多行，就会卡住等待不存在的后续行。

所以这里写成：

```text
命令名是 HELP
并且第一行响应是 Commands:
才按多行 HELP 处理
```

---

读取多行 HELP：

```cpp
while (response != "  EXIT") {
    if (!read_line(socket, response)) {
        return false;
    }
    std::cout << response << '\n';
}
```

服务端帮助文本最后一行是：

```text
  EXIT
```

所以客户端一直读到这一行为止。

每读一行，就打印一行。

一句话总结：`print_response()` 让客户端既能处理普通单行响应，也能正确处理 `HELP` 的多行响应。

---

# 19. 程序入口 `main`

```cpp
int main(int argc, char** argv) {
```

这是客户端程序入口。

当运行：

```powershell
.\cmake-build-debug\minikv_client.exe 127.0.0.1 6379
```

程序就从这里开始。

---

外层包了一层异常处理：

```cpp
try {
```

后面如果出现：

```text
端口解析失败
WSAStartup 失败
getaddrinfo 失败
connect 失败
响应过长
```

都会抛异常，然后被最后的 `catch` 统一处理。

---

# 20. 检查参数数量

```cpp
if (argc > 3) {
    print_usage(argv[0]);
    return 2;
}
```

客户端最多接受两个用户参数：

```text
host
port
```

再加上程序名本身，所以 `argc` 最多是 3。

如果参数太多：

```text
minikv_client 127.0.0.1 6379 extra
```

就打印用法，并返回：

```text
2
```

这里 `2` 常用来表示命令行参数错误。

---

# 21. 设置默认地址和端口

```cpp
std::string host = "127.0.0.1";
std::uint16_t port = 6379;
```

如果用户直接运行：

```powershell
.\minikv_client.exe
```

客户端默认连接：

```text
127.0.0.1:6379
```

这和 README 里的服务端默认端口保持一致。

---

如果传了 host：

```cpp
if (argc >= 2) {
    host = argv[1];
}
```

比如：

```powershell
.\minikv_client.exe localhost
```

就连接：

```text
localhost:6379
```

如果也传了端口：

```cpp
if (argc >= 3) {
    port = parse_port(argv[2]);
}
```

比如：

```powershell
.\minikv_client.exe 127.0.0.1 6381
```

就连接：

```text
127.0.0.1:6381
```

---

# 22. 初始化网络并连接服务端

```cpp
NetworkRuntime runtime;
SocketGuard socket = connect_to_server(host, port);
```

第一行：

```cpp
NetworkRuntime runtime;
```

在 Windows 下会调用 `WSAStartup`。

在非 Windows 下什么也不用做。

第二行：

```cpp
connect_to_server(host, port)
```

会创建 socket，并连接服务端。

返回的 socket 交给 `SocketGuard` 管理。

如果连接失败，程序会抛异常并打印：

```text
fatal: connect failed: ...
```

---

# 23. 读取服务端 greeting

```cpp
std::string greeting;
if (!read_line(socket.get(), greeting)) {
    std::cerr << "server closed connection before greeting\n";
    return 1;
}

std::cout << greeting << '\n';
```

`minikv_server` 接受客户端连接后，会先发送：

```text
mini-kv ready
```

所以客户端连接成功后，第一件事就是读这一行。

如果服务端还没发 greeting 就关闭了连接，说明连接异常。

客户端打印：

```text
server closed connection before greeting
```

并返回 1。

如果读取成功，就打印：

```text
mini-kv ready
```

---

# 24. 进入命令循环

```cpp
std::string line;
while (true) {
```

这里和本地 CLI 的 `main.cpp` 很像。

客户端也需要不断读取用户输入。

整体流程是：

```text
打印提示符
 -> 读取一行输入
 -> 空行就跳过
 -> 发送到服务端
 -> 读取响应并打印
 -> 如果是 QUIT / EXIT 就退出
```

---

打印提示符：

```cpp
std::cout << "mini-kv@" << host << ':' << port << "> ";
```

输出类似：

```text
mini-kv@127.0.0.1:6379>
```

和本地 CLI 的：

```text
mini-kv>
```

相比，这里多了：

```text
host:port
```

这样用户知道自己连接的是哪个服务端。

---

读取用户输入：

```cpp
if (!std::getline(std::cin, line)) {
    std::cout << '\n';
    break;
}
```

这里同样用 `std::getline` 读取完整一行。

这让客户端既支持交互输入，也支持管道输入。

交互输入：

```powershell
.\minikv_client.exe 127.0.0.1 6379
```

然后手动输入命令。

管道输入：

```powershell
@('SET name mini-kv', 'GET name', 'QUIT') | .\minikv_client.exe 127.0.0.1 6379
```

这次 smoke test 就使用了这种方式。

---

# 25. 跳过空行

```cpp
if (is_blank(line)) {
    continue;
}
```

如果用户只按回车，客户端不发送任何东西。

直接进入下一轮循环。

---

# 26. 发送命令

```cpp
if (!send_all(socket.get(), line + "\n")) {
    std::cerr << "failed to send command\n";
    return 1;
}
```

客户端把用户输入的命令加上换行：

```text
line + "\n"
```

因为服务端协议是：

```text
一行一个命令
```

服务端在 `tcp_server.cpp` 里是通过寻找 `\n` 来判断一条命令结束。

如果发送失败，客户端打印：

```text
failed to send command
```

并返回 1。

---

# 27. 读取并打印响应

```cpp
if (!print_response(socket.get(), line)) {
    std::cerr << "server closed connection\n";
    return 1;
}
```

这一步会：

```text
读服务端响应
打印响应
如果是 HELP 成功响应，就继续读完整多行帮助
```

如果读不到响应，说明服务端关闭了连接。

客户端打印：

```text
server closed connection
```

并返回 1。

---

# 28. 判断是否退出

```cpp
const std::string command = first_token(line);
if (command == "QUIT" || command == "EXIT") {
    break;
}
```

发送并读取响应之后，客户端判断用户输入的命令是不是：

```text
QUIT
EXIT
```

如果是，就跳出循环。

注意这里是在读取响应之后才退出。

也就是说用户输入：

```text
QUIT
```

客户端仍然会先收到并打印服务端返回的：

```text
BYE
```

然后才退出。

---

# 29. 异常处理

```cpp
} catch (const std::exception& error) {
    std::cerr << "fatal: " << error.what() << '\n';
    return 1;
}
```

如果前面的任何地方抛出标准异常，就在这里统一处理。

比如：

```text
端口不是数字
端口超出范围
连接失败
响应行太长
```

都会输出：

```text
fatal: 具体错误
```

并返回 1。

---

最后：

```cpp
return 0;
```

表示客户端正常结束。

---

# 30. 一次完整运行示例

先启动服务端：

```powershell
.\cmake-build-debug\minikv_server.exe 6379
```

再启动客户端：

```powershell
.\cmake-build-debug\minikv_client.exe 127.0.0.1 6379
```

客户端连接成功后：

```text
mini-kv ready
mini-kv@127.0.0.1:6379>
```

用户输入：

```text
SET name mini-kv-v2
```

内部流程：

```text
client_main.cpp 读取整行
 -> send_all 发送 "SET name mini-kv-v2\n"
 -> tcp_server.cpp 收到命令
 -> CommandProcessor 执行 SET
 -> Store 保存 name = mini-kv-v2
 -> 服务端返回 "OK inserted\n"
 -> client_main.cpp read_line 读取响应
 -> 打印 OK inserted
```

用户输入：

```text
GET name
```

内部流程：

```text
client_main.cpp 发送 "GET name\n"
 -> 服务端读取 Store
 -> 返回 "mini-kv-v2\n"
 -> 客户端打印 mini-kv-v2
```

用户输入：

```text
HELP
```

内部流程：

```text
client_main.cpp 发送 "HELP\n"
 -> 服务端返回多行帮助文本
 -> 客户端先读到 Commands:
 -> 判断这是 HELP 成功响应
 -> 继续读到最后一行 "  EXIT"
 -> 全部打印出来
```

用户输入：

```text
QUIT
```

内部流程：

```text
client_main.cpp 发送 "QUIT\n"
 -> 服务端返回 BYE
 -> 客户端打印 BYE
 -> 客户端发现命令是 QUIT
 -> 退出循环
 -> return 0
```

---

# 第六次总结

`client_main.cpp` 是第二版新增的 TCP 客户端。

它本身不保存数据，也不解析 KV 命令。

它只负责：

```text
解析 host 和 port
初始化网络环境
连接 minikv_server
读取用户输入
把命令完整发送到 socket
按行读取服务端响应
处理 HELP 多行输出
遇到 QUIT / EXIT 后退出
```

一句话总结：

```text
client_main.cpp 是 mini-kv 的远程命令行外壳，它把用户输入的一行文本通过 TCP 交给 minikv_server，再把服务端返回的文本显示给用户。
```
