# 第七次讲解：第二版构建和说明文件改动

这次讲解第二版里除了 `src/client_main.cpp` 之外的增删改。

第二版代码层面的变化可以概括成：

```text
新增：
  src/client_main.cpp
  minikv_client 构建目标
  README 里的 TCP client 用法

修改：
  CMakeLists.txt 项目版本从 0.1.0 到 0.2.0
  CMakeLists.txt 增加 minikv_client 可执行程序
  README 当前版本说明从 Version 1 改成 Version 2
  README CLion target 列表增加 minikv_client
  README Roadmap 移除“添加小客户端程序”

删除：
  没有删除源码文件
  没有删除已有命令
  没有删除已有测试
```

可以把第二版理解成：

```text
第一版：
  有 CLI
  有 TCP Server
  但没有正式 TCP Client

第二版：
  保留第一版能力
  增加一个能连接 TCP Server 的 minikv_client
```

---

# 1. `CMakeLists.txt`：项目版本升级

原来是：

```cmake
project(mini_kv VERSION 0.1.0 LANGUAGES CXX)
```

第二版改成：

```cmake
project(mini_kv VERSION 0.2.0 LANGUAGES CXX)
```

这行的作用是声明项目：

```text
项目名：mini_kv
版本号：0.2.0
语言：C++
```

为什么要从 `0.1.0` 改到 `0.2.0`？

因为这次不只是文档调整，而是新增了一个可运行程序：

```text
minikv_client
```

这代表项目功能从第一版继续前进了一步。

按照当前项目节奏：

```text
0.1.0 = 第一版可运行内存 KV 服务
0.2.0 = 第二版增加 TCP 客户端
```

---

# 2. `CMakeLists.txt`：新增客户端目标

这次新增了：

```cmake
add_executable(minikv_client
    src/client_main.cpp
)

target_link_libraries(minikv_client PRIVATE minikv)
```

这段和前面的 CLI、Server 目标是同一种结构。

先看：

```cmake
add_executable(minikv_client
    src/client_main.cpp
)
```

意思是：

```text
把 src/client_main.cpp 编译成一个可执行程序
程序名叫 minikv_client
```

在 Windows + Ninja 构建目录里，最终产物是：

```text
cmake-build-debug/minikv_client.exe
```

---

再看：

```cmake
target_link_libraries(minikv_client PRIVATE minikv)
```

意思是：

```text
minikv_client 链接 minikv 核心库
```

这里有一个细节。

`client_main.cpp` 当前没有直接包含：

```cpp
#include "minikv/command.hpp"
#include "minikv/store.hpp"
```

它也没有直接调用 `Store` 或 `CommandProcessor`。

那为什么仍然链接 `minikv`？

原因是项目已有的 Windows socket 链接设置在 `minikv` 目标上：

```cmake
if(WIN32)
    target_link_libraries(minikv PUBLIC ws2_32)
endif()
```

这里的 `PUBLIC ws2_32` 会传递给链接 `minikv` 的目标。

所以 `minikv_client` 链接 `minikv` 后，也能拿到 Windows 下需要的：

```text
ws2_32
```

否则客户端在 Windows 上使用：

```cpp
WSAStartup
socket
connect
send
recv
closesocket
```

时，链接阶段可能会找不到 Winsock 符号。

---

# 3. CMake 目标之间的关系

第二版之后，`CMakeLists.txt` 里的可执行目标变成：

```text
minikv_cli
minikv_server
minikv_client
minikv_store_tests
minikv_command_tests
```

它们和核心库的关系是：

```text
minikv
 -> command.cpp
 -> store.cpp
 -> tcp_server.cpp

minikv_cli
 -> main.cpp
 -> link minikv

minikv_server
 -> server_main.cpp
 -> link minikv

minikv_client
 -> client_main.cpp
 -> link minikv

minikv_store_tests
 -> store_tests.cpp
 -> link minikv

minikv_command_tests
 -> command_tests.cpp
 -> link minikv
```

从运行职责看：

```text
minikv_cli
 -> 本地交互模式

minikv_server
 -> TCP 服务端

minikv_client
 -> TCP 客户端
```

这样第二版就形成了三种运行入口：

```text
本地入口
网络服务端入口
网络客户端入口
```

---

# 4. `README.md`：当前版本说明更新

原来 README 写的是：

```text
Version 1 is a runnable in-memory KV service:
```

第二版改成：

```text
Version 2 is a runnable in-memory KV service with a TCP client:
```

这句话告诉读者：

```text
当前已经不是只有服务端的第一版
现在还包含正式 TCP 客户端
```

然后功能列表新增一项：

```text
TCP client for connecting to a running server
```

意思是：

```text
项目现在有一个客户端程序，可以连接正在运行的服务端
```

这里的 “running server” 很关键。

因为 `minikv_client` 自己不保存数据。

它必须连接到一个已经启动的：

```text
minikv_server
```

所以正确使用顺序是：

```text
先启动 server
再启动 client
```

---

# 5. `README.md`：Run 部分新增 TCP client

原来 Run 里只有：

```text
CLI
TCP server
```

第二版新增了 TCP client 的运行命令：

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379
```

这说明多配置生成器下，客户端可能位于：

```text
build/Debug/minikv_client.exe
```

命令里的两个参数是：

```text
127.0.0.1
6379
```

分别表示：

```text
host = 本机
port = 6379
```

如果使用 Ninja 这类单配置生成器，README 后面也提醒：

```text
For single-config generators, executables may be directly under build.
```

也就是说可执行文件可能在：

```text
build/minikv_client.exe
```

或当前项目实际使用的：

```text
cmake-build-debug/minikv_client.exe
```

---

# 6. `README.md`：CLion target 列表新增客户端

原来 CLion 可运行目标是：

```text
minikv_cli
minikv_server
minikv_store_tests
minikv_command_tests
```

第二版新增：

```text
minikv_client
```

完整列表变成：

```text
minikv_cli
minikv_server
minikv_client
minikv_store_tests
minikv_command_tests
```

这对用 CLion 打开项目的人很有帮助。

因为他们可以直接在 IDE 的 target 下拉框里找到：

```text
minikv_client
```

然后运行客户端。

---

# 7. `README.md`：TCP protocol 部分新增 bundled client

原来 TCP protocol 部分只说：

```text
启动 server
连接一个 TCP client
发送一行一条命令
```

但第一版没有正式客户端，所以 “TCP client” 可以是：

```text
telnet
nc
PowerShell TcpClient 脚本
```

第二版新增了项目自带客户端的用法：

```powershell
.\cmake-build-debug\minikv_client.exe 127.0.0.1 6379
```

这里的 bundled client 指的是：

```text
项目自带客户端
```

也就是本次新增的：

```text
minikv_client
```

这样 README 里的运行方式就更完整了：

```text
先启动 minikv_server
再用 minikv_client 连接它
然后输入 SET / GET / SIZE / DEL / QUIT
```

---

# 8. `README.md`：Roadmap 更新

原来的 Roadmap 第一项是：

```text
1. Add a small client program.
```

第二版已经完成这件事，所以 Roadmap 改成：

```text
1. Add TTL support.
2. Add WAL persistence.
3. Add snapshots.
4. Add benchmarking and stress tests.
5. Add a Redis RESP parser.
```

也就是说：

```text
“添加小客户端程序” 已经从待办变成已完成
```

后续最自然的第三版方向就变成：

```text
TTL support
```

也就是给 key 增加过期时间。

---

# 9. 第二版没有删除什么

这次代码改动没有删除已有功能。

没有删除：

```text
Store
CommandProcessor
CLI main.cpp
TCP server
测试文件
已有命令
```

也没有修改服务端协议。

服务端仍然是：

```text
一行一个命令
一行或多行响应
```

已有命令仍然是：

```text
SET key value
GET key
DEL key
SIZE
HELP
EXIT / QUIT
```

所以第二版是一个向后兼容的功能增加：

```text
旧功能继续可用
新增一个正式客户端
```

---

# 10. 第二版和第一版的运行链路对比

第一版有两条链路。

本地 CLI：

```text
minikv_cli
 -> main.cpp
 -> CommandProcessor
 -> Store
```

TCP 服务端：

```text
外部 TCP 工具
 -> minikv_server
 -> tcp_server.cpp
 -> CommandProcessor
 -> Store
```

第二版新增了一条更完整的网络链路：

```text
minikv_client
 -> client_main.cpp
 -> TCP socket
 -> minikv_server
 -> tcp_server.cpp
 -> CommandProcessor
 -> Store
 -> tcp_server.cpp
 -> TCP socket
 -> client_main.cpp
```

这样用户不再需要自己找 `telnet`、`nc` 或写临时脚本。

项目自己就能完成：

```text
服务端启动
客户端连接
命令发送
响应打印
```

---

# 11. 第二版验证结果对应的意义

第二版做完后，执行过几类关键验证。

CMake configure：

```text
确认新增 minikv_client 目标能被 CMake 识别
```

Build：

```text
确认 client_main.cpp 可以编译
确认 minikv_client.exe 可以链接
```

CTest：

```text
确认新增客户端没有破坏 Store 和 CommandProcessor 的既有行为
```

Client smoke test：

```text
启动 minikv_server
 -> 用 minikv_client 连接
 -> 发送 SET / GET / SIZE / HELP extra / HELP / QUIT
 -> 确认响应正确
 -> 停止 server
```

其中 `HELP extra` 这个用例专门验证：

```text
客户端不会把 HELP 的错误响应误当成多行帮助响应
```

这对应 `client_main.cpp` 里的判断：

```cpp
if (first_token(command) == "HELP" && response == "Commands:") {
```

只有第一行确实是：

```text
Commands:
```

才继续读取多行帮助文本。

---

# 12. 第二版文件变化总表

这次和代码相关的增删改可以整理成：

```text
新增文件：
  src/client_main.cpp
    TCP 客户端入口

修改文件：
  CMakeLists.txt
    版本 0.1.0 -> 0.2.0
    新增 minikv_client 可执行目标

  README.md
    当前版本改为 Version 2
    功能列表增加 TCP client
    运行方式增加 minikv_client
    CLion target 增加 minikv_client
    TCP protocol 增加 bundled client 用法
    Roadmap 移除已完成的小客户端任务

删除文件：
  无
```

---

# 第七次总结

第二版的构建和说明文件改动，核心目的是让新增的 TCP 客户端成为项目的一等公民。

`CMakeLists.txt` 负责让它能被编译出来：

```text
src/client_main.cpp -> minikv_client.exe
```

`README.md` 负责让使用者知道它已经存在，并知道怎么运行：

```text
先启动 minikv_server
再运行 minikv_client host port
```

一句话总结：

```text
第二版没有推翻第一版，而是在原来的内存 KV 服务和 TCP 服务端旁边，补上了一个正式的 TCP 客户端入口。
```
