# 第十一次讲解：第四版入口、测试、文档和归档

第十次讲了 WAL 的核心实现：

```text
WriteAheadLog
CommandProcessor 接入 WAL
Store 支持绝对过期时间
```

这次讲第四版剩下的配套改动：

```text
src/main.cpp
src/server_main.cpp
include/minikv/tcp_server.hpp
src/tcp_server.cpp
tests/wal_tests.cpp
CMakeLists.txt
README.md
a/4/解释/说明.md
a/4/图片/
```

它们负责把 WAL 真正接入 CLI、TCP server、测试和项目说明。

---

# 1. 第四版入口层变化

第四版新增 WAL 后，入口层要多做两件事：

```text
启动时创建 WriteAheadLog
启动时 replay WAL 到 Store
```

然后把 WAL 传给：

```text
CommandProcessor
```

这样后续写命令才能自动追加日志。

第四版仍然保留无 WAL 模式。

也就是说：

```text
不传 wal_path
 -> 纯内存运行

传 wal_path
 -> 启动时 replay
 -> 运行时 append WAL
```

---

# 2. `main.cpp` 新增 WAL 头文件

第四版 CLI 入口新增：

```cpp
#include "minikv/wal.hpp"
```

因为 CLI 现在需要直接创建：

```cpp
minikv::WriteAheadLog
```

还新增了标准库头文件：

```cpp
#include <exception>
#include <optional>
```

`exception` 用来捕获启动过程中的异常。

`optional` 用来表示：

```text
可能有 WAL
也可能没有 WAL
```

---

# 3. CLI 用法从无参数变成可选 WAL 参数

新增用法函数：

```cpp
void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [wal_path]\n";
}
```

意思是：

```text
minikv_cli.exe
```

仍然可以直接运行。

也可以传一个 WAL 路径：

```text
minikv_cli.exe data\mini-kv.wal
```

方括号：

```text
[wal_path]
```

表示这个参数是可选的。

---

# 4. CLI 参数检查

```cpp
if (argc > 2) {
    print_usage(argv[0]);
    return 2;
}
```

CLI 最多接受一个用户参数：

```text
wal_path
```

如果传太多：

```text
minikv_cli.exe a.wal extra
```

就打印用法，并返回：

```text
2
```

这里 `2` 表示命令行参数错误。

---

# 5. CLI 创建 Store 和可选 WAL

```cpp
minikv::Store store;
std::optional<minikv::WriteAheadLog> wal;
```

`store` 仍然是内存数据库。

`wal` 是可选 WAL 对象。

为什么用 `std::optional`？

因为 WAL 只有在用户传入路径时才创建。

不传路径时：

```text
wal 没有值
```

传路径时：

```text
wal 里放一个 WriteAheadLog 对象
```

---

# 6. CLI 启动时 replay

```cpp
if (argc == 2) {
    wal.emplace(argv[1]);
    const auto replayed = wal->replay(store);
    std::cout << "WAL: " << wal->path().string() << " (" << replayed << " records replayed)\n";
}
```

如果用户传了 WAL 路径，就：

```text
创建 WriteAheadLog
replay 到 Store
打印 replay 记录数
```

例如：

```text
WAL: a\4\mini-kv-v4-cli-smoke.wal (3 records replayed)
```

这说明启动时已经从 WAL 恢复了 3 条记录。

---

# 7. CLI 创建 CommandProcessor

```cpp
minikv::CommandProcessor processor{store, wal.has_value() ? &*wal : nullptr};
```

这里把 Store 和 WAL 一起传给命令处理器。

表达式：

```cpp
wal.has_value() ? &*wal : nullptr
```

意思是：

```text
如果 wal 有值，传 WAL 指针
如果 wal 没值，传 nullptr
```

所以后面的命令处理逻辑可以统一写：

```text
wal_ == nullptr
 -> 不写 WAL

wal_ != nullptr
 -> 写命令先 append WAL
```

---

# 8. CLI 外层异常处理

第四版 CLI 把主流程包进：

```cpp
try {
    ...
} catch (const std::exception& error) {
    std::cerr << "fatal: " << error.what() << '\n';
    return 1;
}
```

这样如果 WAL 路径、文件系统或其他初始化过程抛异常，程序会打印：

```text
fatal: 具体错误
```

而不是直接崩掉。

---

# 9. `server_main.cpp` 新增 WAL

TCP server 入口新增：

```cpp
#include "minikv/wal.hpp"
#include <optional>
```

和 CLI 一样，server 也可以选择启用 WAL。

它的用法从：

```text
minikv_server.exe [port] [host]
```

变成：

```text
minikv_server.exe [port] [host] [wal_path]
```

---

# 10. Server 参数检查

```cpp
if (argc > 4) {
    print_usage(argv[0]);
    return 2;
}
```

server 现在最多接受三个用户参数：

```text
port
host
wal_path
```

加上程序名，所以 `argc` 最大是 4。

如果超过，就打印：

```text
Usage: program [port] [host] [wal_path]
```

---

# 11. Server 启动时 replay

```cpp
minikv::Store store;
std::optional<minikv::WriteAheadLog> wal;

if (argc >= 4) {
    wal.emplace(argv[3]);
    const auto replayed = wal->replay(store);
    std::cout << "WAL: " << wal->path().string() << " (" << replayed << " records replayed)\n";
}
```

server 的逻辑和 CLI 一样：

```text
先创建 Store
如果传了 wal_path，就创建 WAL
replay WAL 到 Store
```

这样服务端开始监听前，内存数据已经恢复好了。

---

# 12. Server 创建 TcpServer 时传 WAL

```cpp
minikv::TcpServer server{store, options, wal.has_value() ? &*wal : nullptr};
```

这里把 WAL 指针交给 `TcpServer`。

因为每个 TCP 客户端连接里都会创建：

```cpp
CommandProcessor
```

这些 `CommandProcessor` 需要同一个 WAL 指针。

这样所有网络客户端的写命令都会写到同一个 WAL 文件。

---

# 13. `tcp_server.hpp` 新增 WAL 指针

第四版在 `tcp_server.hpp` 里新增前置声明：

```cpp
class WriteAheadLog;
```

然后构造函数新增一个重载：

```cpp
TcpServer(Store& store, Options options, WriteAheadLog* wal);
```

原来的构造函数保留：

```cpp
explicit TcpServer(Store& store, Options options);
```

这保证旧代码仍然能这样创建：

```cpp
TcpServer server{store, options};
```

不需要 WAL。

---

成员变量新增：

```cpp
WriteAheadLog* wal_ = nullptr;
```

默认是：

```text
nullptr
```

表示不启用 WAL。

---

# 14. `tcp_server.cpp` 传递 WAL 到客户端线程

第四版修改了 `serve_client`：

```cpp
void serve_client(Store& store, WriteAheadLog* wal, SocketHandle client_socket) {
    SocketGuard client{client_socket};
    CommandProcessor processor{store, wal};
```

以前每个客户端线程只拿到：

```text
Store
socket
```

现在还拿到：

```text
WriteAheadLog*
```

然后创建：

```cpp
CommandProcessor processor{store, wal};
```

所以 TCP 客户端发来的：

```text
SET
DEL
EXPIRE
```

也会写 WAL。

---

# 15. TcpServer 构造函数重载

```cpp
TcpServer::TcpServer(Store& store, Options options) : TcpServer(store, std::move(options), nullptr) {}
```

原来的构造函数现在委托给新构造函数。

意思是：

```text
不传 WAL 时，wal = nullptr
```

新构造函数：

```cpp
TcpServer::TcpServer(Store& store, Options options, WriteAheadLog* wal)
    : store_(store), options_(std::move(options)), wal_(wal) {}
```

保存：

```text
Store 引用
Options
WAL 指针
```

---

# 16. accept 后启动线程时传 WAL

第四版最后修改：

```cpp
std::thread{serve_client, std::ref(store_), wal_, client}.detach();
```

以前是：

```text
serve_client(store, client)
```

现在是：

```text
serve_client(store, wal, client)
```

这样每个连接都能共享同一个 WAL 对象。

---

# 17. `tests/wal_tests.cpp` 的角色

第四版新增测试：

```text
tests/wal_tests.cpp
```

它专门验证 WAL：

```text
写入是否成功
replay 是否能恢复数据
DEL 是否能恢复删除
EXPIRE 是否能恢复绝对过期语义
过期 key 是否不会复活
```

可以把它理解成：

```text
wal_tests.cpp = WAL 的自检程序
```

---

# 18. wal_tests 创建临时 WAL 路径

```cpp
const auto suffix = std::chrono::system_clock::now().time_since_epoch().count();
const auto path = std::filesystem::temp_directory_path() / ("minikv-wal-test-" + std::to_string(suffix) + ".wal");
std::filesystem::remove(path);
```

这里生成一个临时 WAL 文件路径。

`suffix` 用当前时间生成，避免和其他测试文件重名。

然后先：

```cpp
std::filesystem::remove(path);
```

确保测试开始时这个 WAL 文件不存在。

---

# 19. wal_tests 第一阶段：写 WAL

```cpp
{
    minikv::Store store;
    minikv::WriteAheadLog wal{path};
    minikv::CommandProcessor processor{store, &wal};
```

第一段作用域里创建：

```text
一个空 Store
一个 WAL
一个带 WAL 的 CommandProcessor
```

然后执行命令。

---

写入普通 key：

```cpp
auto result = processor.execute("SET name mini-kv");
assert(result.response == "OK inserted");
```

这会写入 WAL：

```text
SET name mini-kv
```

---

写入带空格的 value：

```cpp
result = processor.execute("SET phrase hello from wal");
assert(result.response == "OK inserted");
```

这专门验证 WAL 的 `SET` replay 能处理：

```text
value 里有空格
```

WAL 记录是：

```text
SET phrase hello from wal
```

---

设置 TTL：

```cpp
result = processor.execute("EXPIRE name 60");
assert(result.response == "1");
```

这会写入内部记录：

```text
EXPIREAT name ...
```

---

删除 phrase：

```cpp
result = processor.execute("DEL phrase");
assert(result.response == "1");
```

这会写入：

```text
DEL phrase
```

后面 replay 时，`phrase` 应该不存在。

---

写入即将过期的 stale：

```cpp
result = processor.execute("SET stale old");
assert(result.response == "OK inserted");

result = processor.execute("EXPIRE stale 1");
assert(result.response == "1");
```

然后等待：

```cpp
std::this_thread::sleep_for(1100ms);
```

再确认：

```cpp
result = processor.execute("GET stale");
assert(result.response == "(nil)");
```

这证明运行时已经过期。

更重要的是，后面 replay 时也不应该复活。

---

# 20. wal_tests 第二阶段：replay WAL

第二个作用域：

```cpp
{
    minikv::Store restored;
    minikv::WriteAheadLog wal{path};
    const auto applied = wal.replay(restored);
```

这里创建一个新的空 Store：

```text
restored
```

然后用同一个 WAL 路径 replay。

---

断言 replay 条数：

```cpp
assert(applied == 6);
```

为什么是 6？

前面成功写入了 6 条 WAL 记录：

```text
SET name mini-kv
SET phrase hello from wal
EXPIREAT name ...
DEL phrase
SET stale old
EXPIREAT stale ...
```

---

验证 name 被恢复：

```cpp
assert(restored.get("name") == std::optional<std::string>{"mini-kv"});
assert(restored.ttl("name").has_value());
```

说明：

```text
SET name 被恢复
EXPIREAT name 也被恢复
```

---

验证 phrase 被删除：

```cpp
assert(!restored.get("phrase").has_value());
```

虽然 WAL 里有：

```text
SET phrase hello from wal
```

但后面还有：

```text
DEL phrase
```

按顺序 replay 后，最终状态应该是不存在。

---

验证 stale 不复活：

```cpp
assert(!restored.get("stale").has_value());
```

`stale` 的 EXPIREAT 在 replay 时已经过期。

所以 replay 逻辑会：

```text
store.erase("stale")
```

最终它不存在。

---

最后清理临时文件：

```cpp
std::filesystem::remove(path);
```

测试结束后不留下 WAL 临时文件。

---

# 21. CMake 新增 wal.cpp 和 wal_tests

第四版把 `wal.cpp` 加入核心库：

```cmake
add_library(minikv
    src/command.cpp
    src/store.cpp
    src/tcp_server.cpp
    src/wal.cpp
)
```

这样 CLI、server、tests 只要链接 `minikv`，就能使用 WAL 实现。

---

新增测试目标：

```cmake
add_executable(minikv_wal_tests
    tests/wal_tests.cpp
)

target_link_libraries(minikv_wal_tests PRIVATE minikv)

add_test(NAME wal_tests COMMAND minikv_wal_tests)
```

这会生成：

```text
minikv_wal_tests
```

并注册到 CTest：

```text
wal_tests
```

所以第四版运行 CTest 时会看到：

```text
store_tests
command_tests
wal_tests
```

---

# 22. CMake 版本更新

第四版项目版本改成：

```cmake
project(mini_kv VERSION 0.4.0 LANGUAGES CXX)
```

版本节奏现在是：

```text
0.1.0 = 内存 KV + CLI + TCP server
0.2.0 = TCP client
0.3.0 = TTL
0.4.0 = WAL persistence
```

---

# 23. README 当前版本更新

README 当前版本变成：

```text
Version 4 is a runnable in-memory KV service with WAL persistence:
```

功能列表新增：

```text
Optional write-ahead log persistence for SET, DEL, and EXPIRE
```

这里强调：

```text
WAL 是可选的
记录的是写命令
```

---

# 24. README 新增 CLI with WAL

README 新增：

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal
```

意思是：

```text
CLI 启动时传入 WAL 路径
```

如果这个文件存在，会 replay。

如果不存在，会从空 Store 启动，并在后续写命令时创建它。

---

# 25. README 新增 TCP server with WAL

README 新增：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal
```

参数含义是：

```text
6379          端口
127.0.0.1     监听地址
data\mini-kv.wal  WAL 文件路径
```

server 启动后会：

```text
replay WAL
开始监听端口
之后所有客户端写命令都会追加 WAL
```

---

# 26. README 新增 WAL persistence 章节

README 新增章节说明：

```text
Pass a WAL path to minikv_cli or minikv_server to enable persistence.
```

意思是：

```text
只有传入 WAL 路径才启用持久化
```

不传 WAL 路径时：

```text
仍然是纯内存服务
```

---

README 还说明会记录：

```text
SET key value
DEL key
EXPIRE key seconds
```

并特别说明：

```text
EXPIRE is persisted as an internal absolute expiration record
```

也就是第十次讲过的：

```text
EXPIRE -> EXPIREAT
```

这样过期 key 重启后不会复活。

---

# 27. Roadmap 更新

第四版完成 WAL 后，Roadmap 改成：

```text
1. Add snapshots.
2. Add benchmarking and stress tests.
3. Add a Redis RESP parser.
```

也就是说：

```text
WAL persistence 已经完成
下一步是 snapshots
```

---

# 28. 第四版归档 `a/4`

第四版按项目规则新增：

```text
a/4/图片/
a/4/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-build-all-targets.png
03-run-ctest.png
04-cli-wal-smoke-test.png
05-tcp-wal-smoke-test.png
```

说明文档记录：

```text
第四版做了什么
每张截图对应什么命令
CLI WAL 和 TCP WAL 如何验证
服务端进程是否停止
临时文件是否清理
```

---

# 29. CLI WAL smoke test 的意义

CLI smoke test 分两次运行。

第一次：

```text
SET persisted cli-v4
EXPIRE persisted 60
SET phrase hello from wal
EXIT
```

这会写入 WAL：

```text
SET persisted cli-v4
EXPIREAT persisted ...
SET phrase hello from wal
```

第二次用同一个 WAL 路径启动。

验证：

```text
GET persisted -> cli-v4
TTL persisted -> 60
GET phrase -> hello from wal
```

这说明：

```text
CLI 能写 WAL
CLI 启动时能 replay WAL
TTL 也能通过 EXPIREAT 恢复
```

---

# 30. TCP WAL smoke test 的意义

TCP smoke test 分两次启动 server。

第一次 server 写入：

```text
SET tcp persisted
SET stale gone
EXPIRE stale 1
```

然后停止 server，等待超过 1 秒。

第二次 server 使用同一个 WAL 路径启动。

验证：

```text
GET tcp -> persisted
GET stale -> (nil)
TTL stale -> -2
```

这说明：

```text
未过期数据能恢复
已经过期的数据不会复活
TCP server 的 WAL 接入可用
```

测试结束后两个 server 进程都停止。

---

# 31. 第四版文件变化总表

第四版相关增删改可以整理成：

```text
新增：
  include/minikv/wal.hpp
  src/wal.cpp
  tests/wal_tests.cpp
  a/4/图片/
  a/4/解释/说明.md

修改：
  include/minikv/command.hpp
    CommandProcessor 接收可选 WriteAheadLog*

  src/command.cpp
    SET / DEL / EXPIRE 先 append WAL 再改 Store

  include/minikv/store.hpp
  src/store.cpp
    Clock 改为 system_clock
    新增 expire_at 支持绝对过期时间

  src/main.cpp
    CLI 支持 [wal_path]

  src/server_main.cpp
    server 支持 [port] [host] [wal_path]

  include/minikv/tcp_server.hpp
  src/tcp_server.cpp
    TcpServer 保存 WAL 指针并传给每个客户端 CommandProcessor

  CMakeLists.txt
    版本 0.4.0
    minikv 加入 wal.cpp
    新增 wal_tests

  README.md
    Version 4、WAL 用法、Roadmap 更新

删除：
  无源码文件删除
```

---

# 第十一次总结

第四版不仅新增了 WAL 类，还把它接进了实际入口：

```text
CLI 可传 wal_path
TCP server 可传 wal_path
CommandProcessor 能写 WAL
TcpServer 能把 WAL 传给每个连接
CTest 有 wal_tests
README 有 WAL 使用说明
a/4 有完整运行归档
```

一句话总结：

```text
第四版把 WAL 从一个底层模块接成了真正可用的持久化功能：命令能写日志，程序能重启恢复，测试和归档也证明了这条链路跑通。
```
