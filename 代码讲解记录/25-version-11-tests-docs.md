# 第二十五次讲解：第十一版测试、README、运行归档和增删改

第二十四次讲了第十一版核心代码：

```text
TcpServerConnectionStats
TcpServerConnectionTracker
ConnectionSnapshot
ConnectionCloseLogger
bound_port()
connection_stats()
event=tcp_client_accepted / event=tcp_client_closed 指标字段
```

这次讲第十一版配套内容：

```text
tests/tcp_server_tests.cpp
CMakeLists.txt
README.md
a/11/解释/说明.md
a/11/图片/
```

它们负责证明：

```text
真实 TCP 连接能跑通
连接 ID 能记录
active / total / peak 指标能变化
两个重叠连接能跑出 peak_connections=2
文档和运行归档能说明第十一版状态
```

---

# 1. 第十一版为什么继续改 tcp_server_tests

第十版的 `tcp_server_tests` 主要验证：

```text
server 能启动
request_stop 能让 run() 退出
event=tcp_listen / event=tcp_stop 会出现
```

第十一版新增连接指标后，测试目标变成：

```text
不仅要启动和停止
还要真实连一次 TCP
验证连接接入日志
验证连接关闭日志
验证 connection_stats()
```

所以 `tcp_server_tests` 从纯生命周期测试升级成：

```text
生命周期 + 真实 TCP 请求 + 连接指标测试
```

---

# 2. 测试新增 socket 头文件

第十一版测试新增跨平台 socket 头文件。

Windows 分支：

```cpp
#include <winsock2.h>
#include <ws2tcpip.h>
```

非 Windows 分支：

```cpp
#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
```

原因是：

```text
第十一版测试不再只看 logger
而是真的创建 TCP socket 连接本地 server
```

---

# 3. TestSocket 类型别名

Windows：

```cpp
using TestSocket = SOCKET;
constexpr TestSocket invalid_test_socket = INVALID_SOCKET;
```

非 Windows：

```cpp
using TestSocket = int;
constexpr TestSocket invalid_test_socket = -1;
```

这和 `src/tcp_server.cpp` 里的跨平台 socket 处理一致。

原因是：

```text
Windows socket 不是普通 int fd
Unix-like socket 是 int fd
```

测试代码也要尊重这个差异。

---

# 4. TestNetworkRuntime

Windows 下：

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

非 Windows 下：

```cpp
class TestNetworkRuntime {
public:
    TestNetworkRuntime() = default;
};
```

Windows socket 使用前必须初始化 Winsock。

测试自己创建 socket，所以测试也需要自己的 `TestNetworkRuntime`。

---

# 5. close_test_socket

Windows：

```cpp
closesocket(socket);
```

非 Windows：

```cpp
close(socket);
```

测试用一个统一函数：

```cpp
void close_test_socket(TestSocket socket)
```

隐藏平台差异。

这样后面的测试流程不用到处写 `#ifdef`。

---

# 6. exchange_inline 的角色

第十一版新增测试辅助函数：

```cpp
std::string exchange_inline(std::uint16_t port, std::string_view payload)
```

它负责：

```text
连接 127.0.0.1:port
发送 inline 文本命令
关闭写方向
读取所有响应
返回响应字符串
```

测试调用它：

```cpp
const auto response = exchange_inline(bound_port, "PING\nQUIT\n");
```

这是真实 TCP 请求，不是直接调用 `CommandProcessor`。

---

# 7. exchange_inline 创建 socket

```cpp
TestSocket socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
if (socket == invalid_test_socket) {
    throw std::runtime_error{socket_error_message("socket failed")};
}
```

这里创建 IPv4 TCP socket。

如果创建失败，直接抛异常。

这样测试不会静默失败。

---

# 8. 连接 127.0.0.1

```cpp
sockaddr_in address{};
address.sin_family = AF_INET;
address.sin_port = htons(port);
if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) != 1) {
    close_test_socket(socket);
    throw std::runtime_error{"inet_pton failed"};
}
```

这段把：

```text
127.0.0.1
port
```

填进 `sockaddr_in`。

`htons` 把端口从本机字节序转成网络字节序。

---

# 9. connect

```cpp
if (connect(socket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) != 0) {
    close_test_socket(socket);
    throw std::runtime_error{socket_error_message("connect failed")};
}
```

连接失败时：

```text
先关闭 socket
再抛异常
```

这样测试资源不会泄漏。

---

# 10. 发送 payload

```cpp
std::string_view remaining = payload;
while (!remaining.empty()) {
    const auto sent = send(...);
    ...
    remaining.remove_prefix(static_cast<std::size_t>(sent));
}
```

即使 payload 很短，也用循环发送。

原因是：

```text
send 不保证一次发送完所有字节
```

这是网络代码里更稳的写法。

---

# 11. shutdown 写方向

发送完命令后：

Windows：

```cpp
shutdown(socket, SD_SEND);
```

非 Windows：

```cpp
shutdown(socket, SHUT_WR);
```

这表示：

```text
客户端不再发送更多数据
但仍然继续接收服务端响应
```

服务端收到 `QUIT` 后会返回 `BYE` 并关闭连接。

---

# 12. 读取响应

```cpp
std::string response;
char buffer[256]{};
while (true) {
    const auto received = recv(...);
    if (received < 0) {
        ...
    }
    if (received == 0) {
        break;
    }
    response.append(buffer, static_cast<std::size_t>(received));
}
```

循环读到：

```text
recv 返回 0
```

表示对端关闭连接。

最终返回完整响应。

---

# 13. 等待 server 启动

第十一版保留第十版逻辑：

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

看到 `event=tcp_listen` 后，说明：

```text
server 已经 bind/listen 成功
```

这时可以读取真实端口并发起连接。

---

# 14. 读取 bound_port

```cpp
const auto bound_port = server.bound_port();
assert(bound_port > 0);
```

测试设置的是：

```cpp
options.port = 0;
```

所以不能直接连接 `0`。

必须等 server 启动后读取：

```text
操作系统分配的真实端口
```

`bound_port > 0` 证明端口已经写入。

---

# 15. 连接前统计为 0

```cpp
auto stats = server.connection_stats();
assert(stats.total_connections == 0);
assert(stats.active_connections == 0);
assert(stats.peak_connections == 0);
```

在发起任何客户端连接前，三个指标都应该是 0。

这验证：

```text
TcpServerConnectionTracker 初始状态正确
```

---

# 16. 发送 PING / QUIT

```cpp
const auto response = exchange_inline(bound_port, "PING\nQUIT\n");
assert(response.find("PONG\n") != std::string::npos);
assert(response.find("BYE\n") != std::string::npos);
```

这条真实连接会触发：

```text
event=tcp_client_accepted
event=tcp_client_closed
```

同时验证服务端仍然能正确处理 inline 命令：

```text
PING -> PONG
QUIT -> BYE
```

---

# 17. 等待连接关闭日志

```cpp
bool closed = false;
for (int attempt = 0; attempt < 100; ++attempt) {
    {
        std::lock_guard lock{logs_mutex};
        closed = contains_log(logs, "event=tcp_client_closed");
    }
    stats = server.connection_stats();
    if (closed && stats.active_connections == 0 && stats.total_connections == 1) {
        break;
    }
    std::this_thread::sleep_for(10ms);
}

assert(closed);
```

连接关闭日志由客户端线程析构 `ConnectionCloseLogger` 时写出。

因为客户端线程是 detach 的，主测试线程不能立刻假设日志已经出现。

所以这里轮询等待。

---

# 18. 连接后统计

```cpp
stats = server.connection_stats();
assert(stats.total_connections == 1);
assert(stats.active_connections == 0);
assert(stats.peak_connections == 1);
```

一次连接完整结束后：

```text
累计连接数 = 1
当前活跃连接数 = 0
峰值连接数 = 1
```

这正好验证：

```text
connected() 增加计数
closed() 减少 active
peak 保留历史峰值
```

---

# 19. 最终日志断言

第十一版新增多条日志断言：

```cpp
assert(contains_log(logs, "event=tcp_client_accepted"));
assert(contains_log(logs, "event=tcp_client_closed"));
assert(contains_log(logs, "connection_id=1"));
assert(contains_log(logs, "active_connections=1"));
assert(contains_log(logs, "active_connections=0"));
assert(contains_log(logs, "total_connections=1"));
assert(contains_log(logs, "peak_connections=1"));
```

它们验证：

```text
接入日志存在
关闭日志存在
连接 ID 正确
active 从 1 回到 0
total 是 1
peak 是 1
```

这比只验证 `connection_stats()` 更完整。

---

# 20. CMakeLists.txt 版本更新

第十一版把版本号从：

```cmake
project(mini_kv VERSION 0.10.0 LANGUAGES CXX)
```

更新为：

```cmake
project(mini_kv VERSION 0.11.0 LANGUAGES CXX)
```

这表示当前源码进入第十一版。

测试目标没有新增文件。

第十一版继续复用：

```text
minikv_tcp_server_tests
```

只是增强了测试内容。

---

# 21. README 当前版本更新

第十一版 README 改为：

```text
Version 11 is a runnable in-memory KV service with TCP connection lifecycle tracking and concurrency metrics
```

关键词是：

```text
connection lifecycle tracking
concurrency metrics
```

它对应第十一版新增能力：

```text
连接接入
连接关闭
连接 ID
活跃连接数
累计连接数
峰值连接数
```

---

# 22. README 功能列表新增连接指标

第十一版功能列表新增：

```text
TCP connection IDs plus active, total, and peak connection metrics in lifecycle logs
```

这句话把新增字段说清楚：

```text
connection_id
active_connections
total_connections
peak_connections
```

它们都出现在结构化日志里。

---

# 23. README 日志示例更新

第十版示例：

```text
event=tcp_client_accepted host=127.0.0.1 port=6379
```

第十一版变成：

```text
event=tcp_client_accepted host=127.0.0.1 port=6379 connection_id=1 active_connections=1 total_connections=1 peak_connections=1
event=tcp_client_closed host=127.0.0.1 port=6379 connection_id=1 active_connections=0 total_connections=1 peak_connections=1
event=tcp_stop host=127.0.0.1 port=6379 active_connections=0 total_connections=1 peak_connections=1
```

文档现在能直接展示：

```text
连接接入时 active +1
连接关闭时 active -1
server 停止时带最终统计
```

---

# 24. README 测试说明更新

第十一版把 `tcp_server_tests` 描述更新为：

```text
starts a server on an ephemeral port, sends a real inline TCP request, requests stop through the server API, and checks the structured listen/accept/close/stop log events plus active, total, and peak connection metrics
```

这句话说明测试范围已经扩大：

```text
临时端口
真实 inline TCP 请求
server API 停止
listen / accept / close / stop 日志
active / total / peak 指标
```

---

# 25. README Roadmap 调整

第十版 Roadmap 是：

```text
1. Add broader network compatibility tests.
2. Add connection lifecycle tracking and client concurrency metrics.
```

第十一版完成了第二项。

所以 Roadmap 更新为：

```text
1. Add broader network compatibility tests.
2. Add configurable server limits and client timeout tuning.
```

下一阶段可以继续补：

```text
更多网络兼容测试
可配置 server 限制
客户端超时参数
```

---

# 26. a/11 运行归档

第十一版新增归档目录：

```text
a/11/图片/
a/11/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-connection-metrics-test.png
05-concurrent-tcp-smoke.png
```

这延续项目规则：

```text
关键命令输出保存为 PNG
说明文档解释每张图的命令、结果和意义
```

---

# 27. 01-cmake-configure.png

对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
Configuring done
Generating done
Build files have been written to cmake-build-debug
```

意义：

```text
CMake 能识别第十一版版本号、TCP server 指标接口和测试目标
```

---

# 28. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完整编译并链接所有目标
```

意义：

```text
TcpServerConnectionTracker、连接关闭日志、真实端口查询、跨平台测试 socket 和新增指标测试都能通过编译
```

---

# 29. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
7/7 tests passed
```

测试包括：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
resp_tests
tcp_server_tests
```

意义：

```text
第十一版连接生命周期指标通过自动化测试
旧功能测试也全部通过
```

---

# 30. 04-connection-metrics-test.png

对应命令：

```text
.\cmake-build-debug\minikv_tcp_server_tests.exe
```

结果：

```text
Exit code: 0
Result: PASS
```

意义：

```text
单独证明 TcpServer 可以在临时端口启动，真实处理一次 PING / QUIT，并记录 connection_id=1、total=1、active 从 1 回 0、peak=1
```

---

# 31. 05-concurrent-tcp-smoke.png

对应验证：

```text
启动 minikv_server 6411 127.0.0.1
用 Python socket 建立两个重叠 TCP 连接
```

流程是：

```text
连接 1:
  PING 后保持连接

连接 2:
  PING
  QUIT
  关闭

连接 1:
  QUIT
  关闭
```

结果：

```text
connection_id=1
connection_id=2
active_connections=2
total_connections=2
peak_connections=2
active_connections=0
```

意义：

```text
真实 server 能追踪连接生命周期，并能记录两个重叠连接造成的并发峰值
```

---

# 32. 第十一版文件变化总表

第十一版相关增删改可以整理成：

```text
新增：
  a/11/图片/
  a/11/解释/说明.md
  代码讲解记录/24-connection-metrics.md
  代码讲解记录/25-version-11-tests-docs.md

修改：
  CMakeLists.txt
    版本 0.11.0

  README.md
    Version 11
    新增 TCP connection IDs
    新增 active / total / peak connection metrics
    更新结构化日志示例
    更新 tcp_server_tests 说明
    更新 Roadmap

  include/minikv/tcp_server.hpp
    新增 TcpServerConnectionStats
    新增 TcpServerConnectionTracker 前置声明
    新增 bound_port()
    新增 connection_stats()
    新增 bound_port_
    新增 connection_tracker_

  src/tcp_server.cpp
    新增 ConnectionSnapshot
    新增 TcpServerConnectionTracker
    新增 connection_fields
    新增 stats_fields
    新增 socket_bound_port
    新增 ConnectionCloseLogger
    serve_client 新增 connection_id / tracker / logger 参数
    accept 成功时记录 event=tcp_client_accepted 指标
    client 线程退出时记录 event=tcp_client_closed 指标
    tcp_stop 日志新增最终连接统计

  tests/tcp_server_tests.cpp
    新增跨平台测试 socket
    新增 exchange_inline
    验证 bound_port()
    验证 connection_stats()
    验证 PING / QUIT 响应
    验证 accept / close / stop 日志和指标字段

删除：
  无源码文件删除
```

---

# 33. 第十一版之后当前能力总览

到第十一版为止，mini-kv 已经具备：

```text
内存 key-value 存储
命令解析
CLI 模式
TCP server
TCP client
TTL 过期
WAL 增量持久化
Snapshot 手动完整状态保存和加载
Benchmark 本地吞吐观察
Stress test 并发压力验证
RESP 请求解析器
TCP RESP 请求/响应
PING 探活命令
RESP parser 参数数量限制
RESP parser bulk string 长度限制
TcpServer 可请求停止
server_main 支持 Ctrl+C / SIGTERM 停止标记
结构化生命周期日志
TCP connection_id
TCP active / total / peak connection metrics
CTest 自动化覆盖
运行归档
```

服务观测层现在是：

```text
server_start
tcp_listen
tcp_client_accepted + connection metrics
tcp_client_closed + connection metrics
tcp_stop + final metrics
server_stopped
```

---

# 34. 第十一版和前几版的关系

第十版：

```text
让 TcpServer 能停止
让生命周期日志结构化
```

第十一版：

```text
让连接生命周期可观察
让并发连接数量可统计
```

这两版是一条连续路线：

```text
服务可管理
 -> 服务可观察
```

下一步再做：

```text
连接限制
超时配置
更广网络兼容测试
```

就会顺很多。

---

# 第二十五次总结

第十一版配套工作证明了四件事：

```text
tcp_server_tests
 -> 真实 TCP 请求、连接 ID、active / total / peak 指标都正确

concurrent smoke
 -> 两个重叠连接能跑出 peak_connections=2

README
 -> 第十一版连接生命周期日志和指标字段说明清楚

a/11
 -> 构建、测试、连接指标测试、并发 smoke 都有截图和说明
```

一句话总结：

```text
第十一版不仅增加了连接指标代码，还用单元测试和真实双连接 smoke 证明这些指标会随连接接入、断开和并发峰值正确变化。
```
