# 第三次讲解：`src/server_main.cpp`

这个文件是项目的 **TCP 服务端入口**。

它和 `main.cpp` 很像，都是程序入口。

但区别是：

```text
main.cpp
 -> 面向本地命令行用户
 -> 用户直接在终端输入命令

server_main.cpp
 -> 面向网络客户端
 -> 客户端通过 TCP 连接发送命令
```

可以把它理解成：

```text
server_main.cpp = 网络版 mini-kv 的启动器
```

它本身不负责处理每条命令，也不直接读写 socket 的细节。

它主要负责：

```text
解析命令行参数
 -> 创建 Store
 -> 创建 TcpServer
 -> 打印启动信息
 -> 调用 server.run()
```

---

# 1. 引入项目头文件

文件开头：

```cpp
#include "minikv/store.hpp"
#include "minikv/tcp_server.hpp"
```

这里引入两个项目模块。

第一个：

```cpp
#include "minikv/store.hpp"
```

说明服务端需要创建一个 `Store`。

这个 `Store` 是整个 TCP 服务运行期间共享的内存数据库。

第二个：

```cpp
#include "minikv/tcp_server.hpp"
```

说明这里要创建并启动 `TcpServer`。

真正的网络监听、接收连接、读写数据，都是由 `TcpServer` 实现的。

---

# 2. 引入标准库

```cpp
#include <cstdint>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
```

这些头文件分别服务于不同功能。

`cstdint` 提供固定宽度整数类型。

这里用到了：

```cpp
std::uint16_t
```

端口号范围是 `1 ~ 65535`，正好适合 16 位无符号整数。

`exception` 用来捕获标准异常：

```cpp
std::exception
```

`iostream` 用来打印启动信息和错误信息：

```cpp
std::cout
std::cerr
```

`stdexcept` 用来抛出异常：

```cpp
std::out_of_range
```

`string` 是字符串支持。

---

# 3. 匿名命名空间

```cpp
namespace {
```

这里用了匿名命名空间。

它的作用是：

```text
让里面的函数只在当前 .cpp 文件可见
```

这里面有两个辅助函数：

```text
parse_port
print_usage
```

它们只是 `server_main.cpp` 自己用，不需要暴露给别的文件。

所以放在匿名命名空间里很合适。

---

# 4. 解析端口：`parse_port`

```cpp
std::uint16_t parse_port(const char* text) {
```

这个函数负责把命令行传进来的端口字符串转换成真正的端口数字。

比如用户运行：

```powershell
.\minikv_server.exe 6379
```

那么 `argv[1]` 是：

```text
"6379"
```

注意，它不是整数，而是字符串。

所以需要解析。

第一步：

```cpp
const int port = std::stoi(text);
```

`std::stoi` 会把字符串转成 `int`。

比如：

```text
"6379" -> 6379
```

如果用户传的是：

```text
abc
```

那么 `std::stoi` 会抛异常。

这个异常会在后面的 `try/catch` 里被捕获。

第二步检查范围：

```cpp
if (port < 1 || port > 65535) {
    throw std::out_of_range{"port must be between 1 and 65535"};
}
```

TCP/UDP 端口的合法范围是：

```text
1 到 65535
```

所以：

```text
0     不合法
70000 不合法
6379  合法
```

如果端口不合法，就抛出异常。

最后：

```cpp
return static_cast<std::uint16_t>(port);
```

把 `int` 转成 `std::uint16_t` 返回。

为什么前面先用 `int`？

因为 `std::stoi` 返回的就是 `int`。

而且先用 `int` 更方便检查是否小于 1。

一句话总结：`parse_port` 把用户输入的端口字符串变成合法的端口号。

---

# 5. 打印用法：`print_usage`

```cpp
void print_usage(const char* program) {
```

这个函数负责打印程序使用方法。

内部：

```cpp
std::cerr << "Usage: " << program << " [port] [host]\n";
```

`std::cerr` 是标准错误输出。

当用户参数传错时，应该打印到 `cerr`。

比如程序名是：

```text
minikv_server.exe
```

打印结果类似：

```text
Usage: minikv_server.exe [port] [host]
```

这里的：

```text
[port]
[host]
```

方括号表示可选参数。

也就是说，服务端可以这样运行：

```powershell
.\minikv_server.exe
```

也可以这样运行：

```powershell
.\minikv_server.exe 6379
```

还可以这样运行：

```powershell
.\minikv_server.exe 6379 127.0.0.1
```

---

# 6. main 函数参数

```cpp
int main(int argc, char** argv) {
```

这个 `main` 和 CLI 的 `main` 不一样，它接收命令行参数。

`argc` 表示参数数量。

`argv` 表示参数内容。

比如运行：

```powershell
.\minikv_server.exe 6379 127.0.0.1
```

大致是：

```text
argc = 3

argv[0] = ".\minikv_server.exe"
argv[1] = "6379"
argv[2] = "127.0.0.1"
```

注意，`argv[0]` 是程序自身路径或名称。

所以真正用户传入的第一个参数是 `argv[1]`。

---

# 7. try/catch 包裹主逻辑

```cpp
try {
```

整个服务端启动过程被放在 `try` 里面。

原因是启动服务端可能失败。

比如：

```text
端口参数不是数字
端口超出范围
host 无法解析
端口已被占用
socket 初始化失败
```

这些错误可能以异常形式抛出。

所以这里统一捕获：

```cpp
} catch (const std::exception& error) {
```

如果出错，就打印：

```cpp
std::cerr << "fatal: " << error.what() << '\n';
```

然后返回：

```cpp
return 1;
```

表示程序异常退出。

---

# 8. 创建默认配置

```cpp
minikv::TcpServer::Options options;
```

这里创建 TCP 服务端配置。

`Options` 定义在 `tcp_server.hpp` 里。

它大概包含：

```text
host = "0.0.0.0"
port = 6379
backlog = 64
```

也就是说，如果用户什么参数都不传，服务端默认监听：

```text
0.0.0.0:6379
```

`0.0.0.0` 的意思是监听本机所有网卡地址。

---

# 9. 参数数量检查

```cpp
if (argc > 3) {
    print_usage(argv[0]);
    return 2;
}
```

这个程序最多接受两个用户参数：

```text
port
host
```

再加上 `argv[0]` 程序名，所以 `argc` 最多是 3。

如果用户这样运行：

```powershell
.\minikv_server.exe 6379 127.0.0.1 extra
```

那么 `argc = 4`。

参数太多，就打印用法并返回 2。

这里 `return 2` 通常表示：

```text
命令行参数使用错误
```

---

# 10. 读取端口参数

```cpp
if (argc >= 2) {
    options.port = parse_port(argv[1]);
}
```

如果用户传了第一个参数，就把它当成端口。

比如：

```powershell
.\minikv_server.exe 6380
```

那么：

```text
argv[1] = "6380"
```

执行后：

```text
options.port = 6380
```

如果用户没传端口，就保留默认值：

```text
6379
```

---

# 11. 读取 host 参数

```cpp
if (argc >= 3) {
    options.host = argv[2];
}
```

如果用户传了第二个参数，就把它当成监听地址。

比如：

```powershell
.\minikv_server.exe 6380 127.0.0.1
```

执行后：

```text
options.host = "127.0.0.1"
```

如果用户没传 host，就保留默认值：

```text
0.0.0.0
```

---

# 12. 创建 Store

```cpp
minikv::Store store;
```

这里和 CLI 入口一样，创建一个内存数据库。

但服务端环境下，这个 `store` 更重要。

因为 TCP 服务端可能同时连接多个客户端。

这些客户端都会共享同一个 `Store`。

比如：

```text
客户端 A 执行 SET name mini-kv
客户端 B 执行 GET name
```

如果它们连接的是同一个服务端进程，客户端 B 可以读到：

```text
mini-kv
```

因为它们共享同一个内存数据库。

---

# 13. 创建 TcpServer

```cpp
minikv::TcpServer server{store, options};
```

这里创建服务端对象。

它需要两个东西：

```text
store   数据库
options 监听配置
```

也就是说：

```text
TcpServer 负责网络通信
Store 负责数据保存
```

`TcpServer` 内部收到客户端命令后，会创建或使用 `CommandProcessor` 来执行命令。

---

# 14. 打印启动信息

```cpp
std::cout << "mini-kv server listening on " << options.host << ':' << options.port << '\n';
```

这里告诉用户服务端准备监听哪个地址和端口。

比如：

```text
mini-kv server listening on 0.0.0.0:6379
```

再打印协议提示：

```cpp
std::cout << "Protocol: one command per line. Try: SET name mini-kv" << '\n';
```

意思是：

```text
协议是一行一个命令
```

比如客户端发送：

```text
SET name mini-kv\n
GET name\n
```

每一行都是一条命令。

---

# 15. 启动服务端

```cpp
server.run();
```

这是整个文件最关键的一行。

执行到这里后，控制权交给 `TcpServer`。

后面会发生：

```text
初始化网络环境
 -> 绑定 host:port
 -> listen 监听端口
 -> accept 等待客户端连接
 -> 每个客户端交给一个线程处理
```

注意：

```cpp
server.run();
```

正常情况下不会返回。

因为服务端是一个长期运行的程序。

它会一直监听，直到：

```text
进程被手动停止
发生致命错误
```

---

# 16. 异常处理

如果 `server.run()` 或前面的配置解析发生异常，就会进入：

```cpp
catch (const std::exception& error) {
```

打印错误：

```cpp
std::cerr << "fatal: " << error.what() << '\n';
```

比如端口不合法：

```text
fatal: port must be between 1 and 65535
```

或者端口被占用：

```text
fatal: bind/listen failed: ...
```

然后：

```cpp
return 1;
```

表示异常退出。

---

# 17. 正常返回

```cpp
return 0;
```

理论上，如果服务端正常结束，会返回 0。

不过当前 `TcpServer::run()` 是无限循环，所以正常服务运行时一般不会走到这里。

只有未来如果给服务端加上优雅停止机制，才更可能执行到这个 `return 0`。

---

# 18. 服务端启动示例

如果运行：

```powershell
.\build\Debug\minikv_server.exe 6379
```

流程是：

```text
main(argc=2, argv)
 -> 创建默认 options
 -> argc >= 2，解析 argv[1] 为 6379
 -> host 保持默认 0.0.0.0
 -> 创建 Store
 -> 创建 TcpServer
 -> 打印监听信息
 -> server.run()
```

输出类似：

```text
mini-kv server listening on 0.0.0.0:6379
Protocol: one command per line. Try: SET name mini-kv
```

然后服务端开始等待 TCP 客户端连接。

---

# 19. 和 CLI 入口的区别

`main.cpp` 的流程是：

```text
自己读取键盘输入
 -> 自己调用 processor.execute()
 -> 自己打印结果
```

`server_main.cpp` 的流程是：

```text
只负责启动 TcpServer
 -> TcpServer 接收网络输入
 -> TcpServer 调用 processor.execute()
 -> TcpServer 把结果发回客户端
```

也就是说：

```text
main.cpp 是本地单用户外壳
server_main.cpp 是网络服务启动器
```

它们共享同一套核心逻辑：

```text
Store + CommandProcessor
```

这也是项目结构清晰的地方。

命令逻辑只写一遍，CLI 和 TCP Server 都能复用。

---

# 第三次总结

`server_main.cpp` 是 mini-kv 的 TCP 服务端入口。

它主要负责：

```text
解析命令行参数
检查端口合法性
创建 Store
创建 TcpServer
打印监听信息
调用 server.run()
捕获并打印启动异常
```

一句话总结：

```text
server_main.cpp 不处理具体命令，也不直接管理客户端连接；它只是把 Store 和 TcpServer 组装起来，然后启动网络版 mini-kv。
```
