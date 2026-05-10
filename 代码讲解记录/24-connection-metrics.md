# 第二十四次讲解：第十一版 TCP 连接生命周期追踪和并发指标

第二十三次讲了第十版的测试、README、运行归档和整体增删改。

这次进入第十一版核心代码：

```text
include/minikv/tcp_server.hpp
src/tcp_server.cpp
```

第十一版主要做四件事：

```text
第一，给每个 TCP 连接分配 connection_id
第二，记录当前活跃连接数 active_connections
第三，记录累计连接数 total_connections
第四，记录历史峰值连接数 peak_connections
```

第十版已经让服务端能启动、停止、输出结构化生命周期日志。

第十一版继续往前走：

```text
不仅知道 server 活着
还要知道客户端连接何时接入、何时断开、当前有多少连接、历史最高并发是多少
```

---

# 1. 为什么要做连接指标

第十版的日志有：

```text
event=tcp_listen
event=tcp_client_accepted
event=tcp_stop
```

它能说明：

```text
服务启动了
有客户端接入了
服务停止了
```

但还不能回答这些问题：

```text
这是第几个连接？
当前还有多少连接没断开？
服务启动后累计接过多少连接？
最高同时在线连接数是多少？
客户端什么时候断开的？
```

第十一版补的就是这些观测能力。

---

# 2. tcp_server.hpp 新增头文件

第十一版在 `include/minikv/tcp_server.hpp` 新增：

```cpp
#include <cstddef>
#include <memory>
```

它们分别用于：

```text
cstddef
 -> std::size_t，用于连接数量

memory
 -> std::shared_ptr，用于让 server 和客户端线程共享连接统计器
```

前面第十版已经引入：

```text
atomic
chrono
functional
```

第十一版继续围绕并发运行状态扩展。

---

# 3. 前置声明 TcpServerConnectionTracker

```cpp
struct TcpServerConnectionTracker;
```

这是一个实现细节类型。

它真正定义在：

```text
src/tcp_server.cpp
```

头文件只需要知道：

```text
有这样一个类型
TcpServer 内部会持有 shared_ptr 指向它
```

这样可以减少头文件暴露的实现细节。

---

# 4. TcpServerConnectionStats

第十一版新增公开结构：

```cpp
struct TcpServerConnectionStats {
    std::uint64_t total_connections = 0;
    std::size_t active_connections = 0;
    std::size_t peak_connections = 0;
};
```

这是外部可读取的连接统计快照。

三个字段含义：

```text
total_connections
 -> server 启动以来累计接入过多少连接

active_connections
 -> 当前仍然活跃的连接数

peak_connections
 -> 历史上同时活跃连接数的最高值
```

它不包含 `connection_id`。

因为 `connection_id` 是单个连接事件的字段，而这个结构表示整体统计。

---

# 5. 为什么 total_connections 用 uint64_t

```cpp
std::uint64_t total_connections = 0;
```

累计连接数理论上会一直增长。

使用 `std::uint64_t` 比 `int` 更合适：

```text
范围更大
语义上不可能为负
更像计数器
```

`active_connections` 和 `peak_connections` 用 `std::size_t`，因为它们表示当前数量和峰值数量。

---

# 6. TcpServer 新增 bound_port()

第十一版在 `TcpServer` 公开接口新增：

```cpp
std::uint16_t bound_port() const;
```

它返回服务端实际绑定的端口。

为什么需要？

因为测试会设置：

```cpp
options.port = 0;
```

端口 0 的含义是：

```text
让操作系统自动分配一个可用端口
```

但测试要真的连接 server，就必须知道分配后的端口。

所以第十一版提供：

```cpp
server.bound_port()
```

---

# 7. TcpServer 新增 connection_stats()

第十一版还新增：

```cpp
TcpServerConnectionStats connection_stats() const;
```

它让外部可以读取当前统计：

```text
total_connections
active_connections
peak_connections
```

测试会用它断言：

```text
连接前：0 / 0 / 0
一次连接关闭后：1 / 0 / 1
```

这比只看日志更直接。

---

# 8. 新增 bound_port_ 成员

```cpp
std::atomic<std::uint16_t> bound_port_{0};
```

`run()` 在线程里绑定端口后会写入真实端口。

测试主线程会读取：

```cpp
server.bound_port()
```

这是跨线程读写，所以用 atomic。

初始值是 0，表示：

```text
还没有完成 bind/listen
```

---

# 9. 新增 connection_tracker_ 成员

```cpp
std::shared_ptr<TcpServerConnectionTracker> connection_tracker_;
```

为什么用 `shared_ptr`？

因为连接统计器需要被两类线程共享：

```text
server accept 线程
 -> 新连接接入时增加 active / total / peak

client worker 线程
 -> 连接关闭时减少 active
```

客户端线程是 detach 的。

使用 `shared_ptr` 可以保证：

```text
即使 TcpServer::run() 返回了
只要客户端线程还持有 tracker，tracker 就不会提前销毁
```

---

# 10. ConnectionSnapshot

`src/tcp_server.cpp` 新增：

```cpp
struct ConnectionSnapshot {
    std::uint64_t connection_id = 0;
    std::uint64_t total_connections = 0;
    std::size_t active_connections = 0;
    std::size_t peak_connections = 0;
};
```

它表示一次连接事件发生时的瞬间状态。

和 `TcpServerConnectionStats` 相比，它多了：

```text
connection_id
```

因为 accept / close 日志都需要知道：

```text
是哪一个连接触发了这条日志
```

---

# 11. TcpServerConnectionTracker 的角色

第十一版新增内部类型：

```cpp
struct TcpServerConnectionTracker
```

它负责维护所有连接计数器：

```cpp
std::atomic<std::uint64_t> next_connection_id{1};
std::atomic<std::uint64_t> total_connections{0};
std::atomic_size_t active_connections{0};
std::atomic_size_t peak_connections{0};
```

它是第十一版连接指标的核心。

`TcpServer` 不直接散落多个计数逻辑，而是把连接统计集中到这个 tracker 里。

---

# 12. next_connection_id

```cpp
std::atomic<std::uint64_t> next_connection_id{1};
```

第一个连接 ID 从 1 开始。

原因很简单：

```text
日志里的 connection_id=1 比 connection_id=0 更像实际连接编号
0 可以保留为无效或未设置语义
```

每次接入连接时：

```cpp
snapshot.connection_id = next_connection_id.fetch_add(1);
```

这会先返回当前值，再自增。

所以连接序列是：

```text
1, 2, 3, ...
```

---

# 13. connected()

连接接入时调用：

```cpp
ConnectionSnapshot connected() {
    ConnectionSnapshot snapshot;
    snapshot.connection_id = next_connection_id.fetch_add(1);
    snapshot.total_connections = total_connections.fetch_add(1) + 1;
    snapshot.active_connections = active_connections.fetch_add(1) + 1;
    snapshot.peak_connections = update_peak(snapshot.active_connections);
    return snapshot;
}
```

它做四件事：

```text
分配 connection_id
累计连接数 +1
当前活跃连接数 +1
用新的活跃连接数更新峰值
```

最后返回一个 snapshot，用来写日志。

---

# 14. fetch_add 为什么要 +1

例如：

```cpp
snapshot.total_connections = total_connections.fetch_add(1) + 1;
```

`fetch_add(1)` 返回的是增加前的旧值。

如果旧值是 0，增加后才是 1。

所以要：

```text
旧值 + 1
```

日志里才会显示增加后的真实值：

```text
total_connections=1
active_connections=1
```

---

# 15. closed()

连接关闭时调用：

```cpp
ConnectionSnapshot closed(std::uint64_t connection_id) {
    ConnectionSnapshot snapshot;
    snapshot.connection_id = connection_id;
    snapshot.active_connections = active_connections.fetch_sub(1) - 1;
    snapshot.total_connections = total_connections.load();
    snapshot.peak_connections = peak_connections.load();
    return snapshot;
}
```

它做三件事：

```text
记录关闭的是哪个 connection_id
当前活跃连接数 -1
读取累计连接数和峰值连接数
```

关闭连接不会减少：

```text
total_connections
peak_connections
```

只会减少：

```text
active_connections
```

---

# 16. fetch_sub 为什么要 -1

```cpp
snapshot.active_connections = active_connections.fetch_sub(1) - 1;
```

和 `fetch_add` 一样，`fetch_sub(1)` 返回的是减少前的旧值。

如果关闭前活跃连接数是 1，减少后才是 0。

所以要：

```text
旧值 - 1
```

日志里才能显示关闭后的状态：

```text
active_connections=0
```

---

# 17. stats()

```cpp
TcpServerConnectionStats stats() const {
    return {total_connections.load(), active_connections.load(), peak_connections.load()};
}
```

这个函数把内部 atomic 值读出来，组成公开的：

```text
TcpServerConnectionStats
```

它不会修改任何状态。

`TcpServer::connection_stats()` 会直接调用它。

---

# 18. update_peak()

```cpp
std::size_t update_peak(std::size_t active) {
    std::size_t current_peak = peak_connections.load();
    while (active > current_peak &&
           !peak_connections.compare_exchange_weak(current_peak, active)) {
    }
    return peak_connections.load();
}
```

它的目标是：

```text
如果当前活跃连接数超过历史峰值，就更新峰值
```

为什么不用简单赋值？

因为多个客户端可能同时接入。

两个线程都可能看到：

```text
active_connections 增加了
peak_connections 需要更新
```

所以这里用 atomic compare-exchange。

---

# 19. compare_exchange_weak 的含义

```cpp
peak_connections.compare_exchange_weak(current_peak, active)
```

大意是：

```text
如果 peak_connections 当前仍然等于 current_peak
 -> 把它改成 active
 -> 返回 true

否则
 -> 把 current_peak 更新成最新值
 -> 返回 false
```

while 条件是：

```text
active > current_peak
并且更新失败
```

这表示：

```text
只要 active 仍然比当前峰值大，就继续尝试更新
```

如果别的线程已经把峰值更新得更高了，循环自然结束。

---

# 20. connection_fields()

第十一版新增：

```cpp
std::string connection_fields(const ConnectionSnapshot& snapshot) {
    return "connection_id=" + std::to_string(snapshot.connection_id) +
           " active_connections=" + std::to_string(snapshot.active_connections) +
           " total_connections=" + std::to_string(snapshot.total_connections) +
           " peak_connections=" + std::to_string(snapshot.peak_connections);
}
```

它把连接事件快照变成日志字段。

例如：

```text
connection_id=2 active_connections=2 total_connections=2 peak_connections=2
```

这样 accept 和 close 日志都能使用统一格式。

---

# 21. stats_fields()

```cpp
std::string stats_fields(const TcpServerConnectionStats& stats) {
    return "active_connections=" + std::to_string(stats.active_connections) +
           " total_connections=" + std::to_string(stats.total_connections) +
           " peak_connections=" + std::to_string(stats.peak_connections);
}
```

它用于整体统计日志。

例如：

```text
event=tcp_stop host=127.0.0.1 port=6411 active_connections=0 total_connections=2 peak_connections=2
```

和 `connection_fields` 的区别是：

```text
stats_fields 不包含 connection_id
```

因为 `tcp_stop` 是 server 级事件，不属于某一个连接。

---

# 22. socket_bound_port()

第十一版新增：

```cpp
std::uint16_t socket_bound_port(SocketHandle socket)
```

它通过：

```cpp
getsockname(...)
```

查询 listener 实际绑定的端口。

这主要服务于：

```text
options.port = 0
```

的场景。

如果让操作系统自动分配端口，server 必须通过 `getsockname` 才知道最终端口。

---

# 23. 支持 IPv4 和 IPv6

`socket_bound_port` 里判断：

```cpp
if (address.ss_family == AF_INET) {
    const auto* ipv4 = reinterpret_cast<const sockaddr_in*>(&address);
    return ntohs(ipv4->sin_port);
}

if (address.ss_family == AF_INET6) {
    const auto* ipv6 = reinterpret_cast<const sockaddr_in6*>(&address);
    return ntohs(ipv6->sin6_port);
}
```

这表示：

```text
IPv4 listener 可以取端口
IPv6 listener 也可以取端口
```

端口从网络字节序转成本机字节序时使用：

```cpp
ntohs
```

---

# 24. ConnectionCloseLogger

第十一版新增 RAII 类型：

```cpp
class ConnectionCloseLogger
```

它的作用是：

```text
当客户端线程退出 serve_client 时，自动记录连接关闭日志
```

这比在 `serve_client` 的每个 return 前手写关闭逻辑更稳。

因为 `serve_client` 里有很多退出路径：

```text
recv <= 0
send 失败
QUIT
请求过长
RESP 错误
```

RAII 可以保证：

```text
函数怎么 return，析构函数都会执行
```

---

# 25. ConnectionCloseLogger 构造函数

```cpp
ConnectionCloseLogger(TcpServer::Options::LogHandler logger,
                      std::string endpoint,
                      std::shared_ptr<TcpServerConnectionTracker> tracker,
                      std::uint64_t connection_id)
    : logger_(std::move(logger)),
      endpoint_(std::move(endpoint)),
      tracker_(std::move(tracker)),
      connection_id_(connection_id) {}
```

它保存四个信息：

```text
logger
 -> 用于写日志

endpoint
 -> host / port 字段

tracker
 -> 用于减少 active_connections

connection_id
 -> 表示关闭的是哪条连接
```

---

# 26. ConnectionCloseLogger 析构函数

```cpp
~ConnectionCloseLogger() {
    if (!tracker_) {
        return;
    }

    const auto snapshot = tracker_->closed(connection_id_);
    if (logger_) {
        logger_("event=tcp_client_closed " + endpoint_ + " " + connection_fields(snapshot));
    }
}
```

析构时：

```text
调用 tracker.closed(connection_id)
 -> active_connections 减一
 -> 得到关闭后的快照

如果 logger 存在
 -> 输出 event=tcp_client_closed
```

日志形如：

```text
event=tcp_client_closed host=127.0.0.1 port=6411 connection_id=1 active_connections=0 total_connections=2 peak_connections=2
```

---

# 27. serve_client 参数变化

第十一版把 `serve_client` 从：

```cpp
void serve_client(Store& store, WriteAheadLog* wal, SocketHandle client_socket)
```

扩展为：

```cpp
void serve_client(Store& store,
                  WriteAheadLog* wal,
                  SocketHandle client_socket,
                  TcpServer::Options::LogHandler logger,
                  std::string endpoint,
                  std::shared_ptr<TcpServerConnectionTracker> tracker,
                  std::uint64_t connection_id)
```

新增参数都是为了连接关闭日志：

```text
logger
endpoint
tracker
connection_id
```

---

# 28. serve_client 内创建 close_logger

```cpp
SocketGuard client{client_socket};
ConnectionCloseLogger close_logger{std::move(logger), std::move(endpoint), std::move(tracker), connection_id};
```

这两行都是 RAII：

```text
SocketGuard
 -> 函数结束时关闭 socket

ConnectionCloseLogger
 -> 函数结束时记录连接关闭并更新 active_connections
```

顺序也有意义。

两个对象都在函数开头创建，函数退出时都会析构。

---

# 29. 构造函数初始化 connection_tracker_

第十一版构造函数变成：

```cpp
TcpServer::TcpServer(Store& store, Options options, WriteAheadLog* wal)
    : store_(store),
      options_(std::move(options)),
      wal_(wal),
      connection_tracker_(std::make_shared<TcpServerConnectionTracker>()) {}
```

每个 `TcpServer` 拥有一份连接统计器。

它会被：

```text
server accept loop
client worker threads
connection_stats()
```

共同使用。

---

# 30. bound_port() 实现

```cpp
std::uint16_t TcpServer::bound_port() const {
    return bound_port_.load();
}
```

它只是读取 atomic。

在 `run()` 调用 `bind_listener` 成功后会写入真实端口。

测试等待：

```text
event=tcp_listen
```

之后再读取 `bound_port()`，这时端口已经可用。

---

# 31. connection_stats() 实现

```cpp
TcpServerConnectionStats TcpServer::connection_stats() const {
    return connection_tracker_->stats();
}
```

它把内部 tracker 的统计暴露出去。

外部不直接接触：

```text
TcpServerConnectionTracker
```

这样 public API 保持简洁。

---

# 32. run() 里记录实际端口

第十一版在 `run()` 里新增：

```cpp
const auto actual_port = socket_bound_port(listener.get());
bound_port_.store(actual_port);
const std::string endpoint = endpoint_fields(options_, actual_port);
log_event(options_, "event=tcp_listen " + endpoint);
```

它做三件事：

```text
查询真实绑定端口
保存到 bound_port_
用真实端口写 tcp_listen 日志
```

这让端口 0 场景不再模糊。

---

# 33. accept 成功后更新连接指标

第十版 accept 后只是写日志：

```text
event=tcp_client_accepted
```

第十一版改成：

```cpp
const auto snapshot = connection_tracker_->connected();
log_event(options_, "event=tcp_client_accepted " + endpoint + " " + connection_fields(snapshot));
```

也就是说：

```text
先更新连接统计
再把统计快照写入日志
```

接入第一个连接时：

```text
connection_id=1 active_connections=1 total_connections=1 peak_connections=1
```

接入第二个重叠连接时：

```text
connection_id=2 active_connections=2 total_connections=2 peak_connections=2
```

---

# 34. 创建客户端线程时传入 tracker

第十一版创建线程时：

```cpp
std::thread{serve_client,
            std::ref(store_),
            wal_,
            client,
            options_.logger,
            endpoint,
            connection_tracker_,
            snapshot.connection_id}
    .detach();
```

关键是传入：

```text
connection_tracker_
snapshot.connection_id
```

这样客户端线程退出时知道：

```text
要关闭哪一个 connection_id
要更新哪一份连接统计器
```

---

# 35. tcp_stop 新增最终统计

第十一版停止日志变成：

```cpp
log_event(options_, "event=tcp_stop " + endpoint + " " + stats_fields(connection_tracker_->stats()));
```

它会输出：

```text
event=tcp_stop host=127.0.0.1 port=6411 active_connections=0 total_connections=2 peak_connections=2
```

这表示 server 停止时的整体连接状态。

如果停止时还有连接没退出，也能从：

```text
active_connections
```

看出来。

---

# 36. 第十一版真实日志示例

双连接 smoke 里实际看到：

```text
event=tcp_client_accepted host=127.0.0.1 port=6411 connection_id=1 active_connections=1 total_connections=1 peak_connections=1
event=tcp_client_accepted host=127.0.0.1 port=6411 connection_id=2 active_connections=2 total_connections=2 peak_connections=2
event=tcp_client_closed host=127.0.0.1 port=6411 connection_id=2 active_connections=1 total_connections=2 peak_connections=2
event=tcp_client_closed host=127.0.0.1 port=6411 connection_id=1 active_connections=0 total_connections=2 peak_connections=2
```

这四行说明：

```text
第一个连接接入，活跃数为 1
第二个连接接入，活跃数为 2，峰值为 2
第二个连接先关闭，活跃数回到 1
第一个连接再关闭，活跃数回到 0
```

这就是第十一版最直观的成果。

---

# 37. 第十一版核心链路总览

连接接入链路：

```text
accept 成功
 -> connection_tracker_->connected()
 -> 分配 connection_id
 -> total_connections +1
 -> active_connections +1
 -> 更新 peak_connections
 -> event=tcp_client_accepted
 -> 启动 serve_client 线程
```

连接关闭链路：

```text
serve_client 返回
 -> ConnectionCloseLogger 析构
 -> connection_tracker_->closed(connection_id)
 -> active_connections -1
 -> event=tcp_client_closed
```

统计读取链路：

```text
TcpServer::connection_stats()
 -> connection_tracker_->stats()
 -> 返回 total / active / peak
```

---

# 38. 第十一版没有改变的地方

第十一版没有改变：

```text
命令语义
RESP parser 行为
WAL / Snapshot 格式
客户端交互格式
第十版 request_stop 停止机制
```

它只是增强：

```text
连接观测能力
测试连接真实端口能力
结构化日志字段
```

所以从用户命令角度看：

```text
PING / SET / GET / QUIT 行为不变
```

从运维和调试角度看：

```text
现在能看见连接 ID 和连接数量变化
```

---

# 第二十四次总结

第十一版把 TCP server 从：

```text
知道有客户端接入
```

推进到：

```text
知道哪个连接接入、哪个连接关闭、当前多少连接、累计多少连接、峰值多少连接
```

一句话总结：

```text
第十一版让 mini-kv 的网络服务具备了基础连接观测能力，为后续连接限制、超时控制和并发压测打下了地基。
```
