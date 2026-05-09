# 第十五次讲解：第六版压力测试、构建、文档和归档

第十四次讲了第六版新增的 benchmark 入口：

```text
src/benchmark_main.cpp
minikv_benchmark
```

这次讲第六版剩下的配套内容：

```text
tests/stress_tests.cpp
CMakeLists.txt
README.md
a/6/解释/说明.md
a/6/图片/
```

第六版的主题是：

```text
基础性能观察
并发压力验证
```

其中 benchmark 负责：

```text
跑出本地吞吐数字
```

stress test 负责：

```text
多线程同时读写 Store
验证线程安全和最终状态
```

---

# 1. stress_tests 的整体目标

第五版已经有：

```text
store_tests
command_tests
wal_tests
snapshot_tests
```

这些测试主要验证功能正确性。

第六版新增：

```text
stress_tests
```

它验证的是：

```text
多个线程共享同一个 Store 时
SET / GET / EXPIRE / TTL / erase / snapshot 是否仍然稳定
```

这和 mini-kv 的 TCP server 很相关。

因为 TCP server 会为每个客户端连接开线程。

多个客户端同时写同一个 Store 时，就需要依赖：

```cpp
Store 内部的 mutex_
```

保证数据结构不被并发破坏。

---

# 2. `tests/stress_tests.cpp` 引入头文件

```cpp
#include "minikv/command.hpp"
#include "minikv/store.hpp"
```

stress test 同时测试两层：

```text
CommandProcessor
Store
```

写入、读取、TTL 设置通过：

```cpp
CommandProcessor
```

删除和最终状态检查直接通过：

```cpp
Store
```

这样可以覆盖更多路径。

---

标准库头文件：

```cpp
#include <cassert>
#include <chrono>
#include <optional>
#include <string>
#include <thread>
#include <vector>
```

分工如下：

```text
cassert   断言测试结果
chrono    sleep_for 和时间字面量
optional  对比 get 返回值
string    拼接 key/value
thread    多线程压力测试
vector    保存线程对象
```

---

# 3. 测试规模

```cpp
constexpr std::size_t thread_count = 4;
constexpr std::size_t operations_per_thread = 1000;
```

第六版 stress test 使用：

```text
4 个写线程
每个线程写 1000 个 key
```

总 key 数：

```text
4 * 1000 = 4000
```

这个规模不算大。

它的目标不是跑很久，而是：

```text
快速制造并发读写
适合放进 CTest 每次运行
```

如果规模太大，CTest 会变慢。

所以这里做了一个折中。

---

# 4. 共享 Store

```cpp
minikv::Store store;
```

整个 stress test 只创建一个 Store。

后面所有线程都共享它。

这是测试重点。

如果每个线程都有自己的 Store，那就测不到并发安全。

共享一个 Store 才能验证：

```text
多个线程同时操作 data_
mutex_ 能不能保护住 unordered_map
```

---

# 5. writer 线程列表

```cpp
std::vector<std::thread> writers;
writers.reserve(thread_count);
```

`writers` 保存写入线程。

先调用：

```cpp
reserve(thread_count)
```

避免 vector 在插入线程对象时反复扩容。

---

# 6. 启动 writer 线程

```cpp
for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
    writers.emplace_back([&store, thread_index] {
        minikv::CommandProcessor processor{store};
```

每个线程捕获：

```text
store 引用
thread_index 值
```

注意这里 `thread_index` 是按值捕获。

这样每个线程拿到自己的编号。

如果按引用捕获，就可能所有线程看到循环结束后的同一个值。

---

每个线程内部创建自己的：

```cpp
CommandProcessor processor{store};
```

但它们共享同一个：

```cpp
Store
```

也就是说：

```text
CommandProcessor 各自独立
Store 共享
```

这很接近 TCP server 的实际使用方式。

每个客户端连接都会创建自己的命令处理器，但所有连接写同一个 Store。

---

# 7. 每个线程生成唯一 key

```cpp
const auto key = "stress:" + std::to_string(thread_index) + ":" + std::to_string(i);
const auto value = "value:" + std::to_string(i);
```

key 格式是：

```text
stress:<thread_index>:<i>
```

例如：

```text
stress:0:0
stress:0:1
stress:1:0
stress:3:999
```

这样每个线程写入的 key 不会互相覆盖。

好处是最终数量容易计算：

```text
thread_count * operations_per_thread
```

---

# 8. writer 执行 SET

```cpp
auto result = processor.execute("SET " + key + " " + value);
assert(result.response == "OK inserted");
```

每个 key 第一次写入，所以期望返回：

```text
OK inserted
```

如果返回：

```text
OK updated
```

说明 key 被重复写了。

如果返回错误，说明命令解析或 Store 写入有问题。

---

# 9. writer 立即执行 GET

```cpp
result = processor.execute("GET " + key);
assert(result.response == value);
```

写入后马上读取。

期望读到刚才写入的 value。

这样每个线程内部都验证：

```text
SET 后 GET 能读到数据
```

即使其他线程也在同时操作 Store，因为 key 唯一，所以不会互相覆盖。

---

# 10. 每三个 key 设置 TTL

```cpp
if (i % 3 == 0) {
    result = processor.execute("EXPIRE " + key + " 60");
    assert(result.response == "1");
```

每三个 key 里，有一个会设置：

```text
EXPIRE key 60
```

这样 stress test 不只测普通 key，也测 TTL 路径。

`EXPIRE` 返回：

```text
1
```

表示设置成功。

---

然后读取 TTL：

```cpp
result = processor.execute("TTL " + key);
const auto ttl = std::stoll(result.response);
assert(ttl > 0);
assert(ttl <= 60);
```

TTL 应该满足：

```text
大于 0
小于等于 60
```

为什么不是必须等于 60？

因为设置和读取之间会有一点时间流逝。

所以返回 59 或 60 都可能是正常的。

---

# 11. 等待 writer 完成

```cpp
for (auto& writer : writers) {
    writer.join();
}
```

`join()` 的意思是：

```text
等待线程执行完
```

只有所有 writer 都结束后，后面才检查总数量。

如果不 join，主线程可能提前检查，看到的数据还没写完。

---

# 12. 检查总 key 数

```cpp
const auto total_keys = thread_count * operations_per_thread;
assert(store.size() == total_keys);
```

理论上总 key 数是：

```text
4 * 1000 = 4000
```

因为每个线程的 key 都唯一。

所以写入结束后：

```cpp
store.size()
```

必须等于：

```cpp
total_keys
```

这能发现一些并发问题。

比如如果 `Store` 没有锁，`unordered_map` 在并发写入时可能损坏，最终 size 可能不对，甚至程序崩溃。

---

# 13. 验证 snapshot 导出

```cpp
const auto items = store.snapshot_items();
assert(items.size() == total_keys);
```

这一步复用第五版新增的：

```cpp
snapshot_items()
```

它会：

```text
加锁
清理过期 key
导出 key/value/expires_at
按 key 排序
```

因为 TTL 都设置为 60 秒，这时不应该过期。

所以导出的条数仍然应该是：

```text
total_keys
```

---

# 14. 验证 snapshot 恢复

```cpp
minikv::Store restored;
restored.restore_snapshot(items);
assert(restored.size() == total_keys);
```

这里创建新的 Store：

```cpp
restored
```

然后用：

```cpp
restore_snapshot(items)
```

恢复。

这不是文件层测试。

它直接测 Store 的快照导出/恢复接口。

意义是：

```text
并发写入后的数据
仍然可以被 snapshot_items 导出
并且可以 restore_snapshot 到另一个 Store
```

---

# 15. eraser 线程列表

```cpp
std::vector<std::thread> erasers;
erasers.reserve(thread_count);
```

写入和快照验证完成后，测试开始第二阶段：

```text
并发删除
```

`erasers` 保存删除线程。

---

# 16. 启动 eraser 线程

```cpp
for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
    erasers.emplace_back([&store, thread_index] {
        for (std::size_t i = 0; i < operations_per_thread; i += 2) {
            const auto key = "stress:" + std::to_string(thread_index) + ":" + std::to_string(i);
            assert(store.erase(key));
        }
    });
}
```

每个 eraser 删除自己线程编号下的偶数 key：

```text
i = 0, 2, 4, ...
```

也就是每个线程删除一半 key。

所有 eraser 共享同一个 Store。

这里直接调用：

```cpp
store.erase(key)
```

而不是走 `CommandProcessor`。

这是为了直接压 Store 的并发删除路径。

---

断言：

```cpp
assert(store.erase(key));
```

表示这些 key 应该存在，并且删除成功。

如果返回 false，说明：

```text
key 不存在
或者已经过期
或者并发状态异常
```

这里 TTL 是 60 秒，不会在测试期间过期。

所以 false 就是不符合预期。

---

# 17. 等待 eraser 完成

```cpp
for (auto& eraser : erasers) {
    eraser.join();
}
```

和 writer 一样，主线程等待所有删除线程完成。

---

# 18. 删除后一半 key 保留

```cpp
assert(store.size() == total_keys / 2);
```

每个线程写入 1000 个 key。

删除偶数编号的 500 个。

所以每个线程剩 500 个。

4 个线程总共剩：

```text
4 * 500 = 2000
```

也就是：

```cpp
total_keys / 2
```

---

# 19. 检查典型 key 状态

```cpp
for (std::size_t thread_index = 0; thread_index < thread_count; ++thread_index) {
    const auto remaining_key = "stress:" + std::to_string(thread_index) + ":1";
    const auto erased_key = "stress:" + std::to_string(thread_index) + ":0";

    assert(store.get(remaining_key) == std::optional<std::string>{"value:1"});
    assert(!store.get(erased_key).has_value());
}
```

对每个线程编号，检查两个 key。

奇数 key：

```text
stress:n:1
```

应该还在，value 是：

```text
value:1
```

偶数 key：

```text
stress:n:0
```

应该已经被删除。

这比只检查 size 更细。

因为 size 正确不代表具体 key 状态一定正确。

---

# 20. 最后再检查一次 size

```cpp
std::this_thread::sleep_for(1ms);
assert(store.size() == total_keys / 2);
```

这里短暂等待 1 毫秒后，再检查 size。

这不是为了等 TTL 过期。

因为 TTL 是 60 秒。

它只是给测试末尾多一个轻量稳定性检查：

```text
短暂等待后 Store 状态仍然一致
```

---

# 21. stress_tests 的覆盖范围

第六版 stress test 覆盖了：

```text
多线程 CommandProcessor::execute("SET ...")
多线程 CommandProcessor::execute("GET ...")
多线程 CommandProcessor::execute("EXPIRE ...")
多线程 CommandProcessor::execute("TTL ...")
Store::size()
Store::snapshot_items()
Store::restore_snapshot()
多线程 Store::erase()
Store::get()
```

这对当前项目很有价值。

因为 `Store` 是 TCP server 多线程共享的核心对象。

---

# 22. stress_tests 不覆盖什么

它没有覆盖：

```text
真实 TCP 网络并发
WAL 多线程写入
snapshot 文件并发 SAVE / LOAD
长时间压力测试
随机操作序列
```

这些可以作为后续增强。

第六版先覆盖最基础的：

```text
共享 Store 并发读写
```

---

# 23. CMake 版本更新

第六版把版本改成：

```cmake
project(mini_kv VERSION 0.6.0 LANGUAGES CXX)
```

版本节奏现在是：

```text
0.1.0 = 内存 KV + CLI + TCP server
0.2.0 = TCP client
0.3.0 = TTL
0.4.0 = WAL persistence
0.5.0 = Manual snapshots
0.6.0 = Benchmark and stress tests
```

---

# 24. CMake 新增 benchmark 目标

```cmake
add_executable(minikv_benchmark
    src/benchmark_main.cpp
)

target_link_libraries(minikv_benchmark PRIVATE minikv)
```

这会生成：

```text
minikv_benchmark.exe
```

它链接：

```text
minikv
```

所以可以使用：

```text
Store
CommandProcessor
```

---

# 25. CMake 新增 stress test 目标

```cmake
add_executable(minikv_stress_tests
    tests/stress_tests.cpp
)

target_link_libraries(minikv_stress_tests PRIVATE minikv)

add_test(NAME stress_tests COMMAND minikv_stress_tests)
```

这会生成：

```text
minikv_stress_tests.exe
```

并注册到 CTest：

```text
stress_tests
```

第六版运行 CTest 时会看到：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
```

---

# 26. README 当前版本更新

第六版 README 当前版本变成：

```text
Version 6 is a runnable in-memory KV service with benchmarks and stress tests:
```

功能列表新增：

```text
Benchmark executable for quick local throughput checks
CTest coverage for store, command, WAL, snapshot, and stress behavior
```

这说明第六版的重点不是新增数据命令，而是新增：

```text
性能观察
压力验证
```

---

# 27. README 新增 benchmark 运行方式

README 新增：

```powershell
.\build\Debug\minikv_benchmark.exe 100000 10000
```

以及单配置生成器路径：

```powershell
.\cmake-build-debug\minikv_benchmark.exe 100000 10000
```

参数格式是：

```text
minikv_benchmark.exe [operations] [key_count]
```

其中：

```text
operations  每组测试操作次数
key_count   key 池大小
```

---

# 28. README 新增 CLion 目标

CLion 目标列表新增：

```text
minikv_benchmark
minikv_stress_tests
```

这样在 CLion 里可以直接运行：

```text
benchmark 程序
stress 测试程序
```

---

# 29. README 新增 Benchmarks and stress tests 章节

新增章节：

```text
## Benchmarks and stress tests
```

它说明 benchmark 输出：

```text
elapsed time
operations per second
Store direct operations
CommandProcessor SET / GET operations
```

也说明：

```text
stress_tests runs multiple writer and eraser threads against one shared Store
```

这和代码里的设计对应：

```text
writers 多线程写入
erasers 多线程删除
snapshot_items / restore_snapshot 验证快照接口
```

---

# 30. Roadmap 更新

第五版 Roadmap 是：

```text
1. Add benchmarking and stress tests.
2. Add a Redis RESP parser.
```

第六版完成 benchmark 和 stress test 后，Roadmap 变成：

```text
1. Add a Redis RESP parser.
```

也就是说：

```text
benchmarking and stress tests 已经完成第一版
```

下一项是 RESP 协议解析。

---

# 31. 第六版归档 `a/6`

第六版按项目规则新增：

```text
a/6/图片/
a/6/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-benchmark-smoke.png
05-stress-test.png
```

说明文档记录：

```text
第六版做了什么
每张截图对应什么命令
配置、构建、测试、benchmark、stress test 的结果
是否启动后台进程
是否有临时文件需要清理
```

---

# 32. `01-cmake-configure.png`

对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
配置成功
```

意义：

```text
CMake 能识别 benchmark_main.cpp、stress_tests.cpp 和新增目标
```

---

# 33. `02-cmake-build.png`

对应命令：

```text
cmake --build cmake-build-debug
```

结果：

```text
编译成功
```

意义：

```text
minikv_benchmark 和 minikv_stress_tests 能通过 C++20 编译并链接
```

---

# 34. `03-ctest.png`

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
5 个测试全部通过
```

测试列表：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
```

意义：

```text
第六版新增 stress_tests 后，原有功能测试仍然全部通过
```

---

# 35. `04-benchmark-smoke.png`

对应命令：

```text
.\cmake-build-debug\minikv_benchmark.exe 20000 2000
```

结果包含：

```text
store.set
store.get
command.set
command.get
store.erase
final store size: 0
```

意义：

```text
第六版新增 benchmark 可执行入口真实可运行
输出了耗时和 ops/sec
最终 Store 被清理到空状态
```

---

# 36. `05-stress-test.png`

对应命令：

```text
.\cmake-build-debug\minikv_stress_tests.exe
```

结果：

```text
Exit code: 0
```

stress test 本身不打印详细日志。

它依赖：

```cpp
assert
```

如果任何并发读写、TTL、snapshot、删除结果不符合预期，程序会断言失败并返回非 0。

退出码 0 表示所有断言通过。

---

# 37. 本机工具链规则继续适用

第六版运行验证仍然使用：

```powershell
$env:PATH = 'D:\CLION\CLion 2025.1.1\bin\mingw\bin;' + $env:PATH
```

原因和第五版一样：

```text
直接运行 ctest 或 exe 前，需要能找到 MinGW 运行时 DLL
```

否则可能出现：

```text
0xc0000135
```

这是本机运行环境问题，不是代码失败。

---

# 38. 第六版文件变化总表

第六版相关增删改可以整理成：

```text
新增：
  src/benchmark_main.cpp
  tests/stress_tests.cpp
  a/6/图片/
  a/6/解释/说明.md

修改：
  CMakeLists.txt
    版本 0.6.0
    新增 minikv_benchmark
    新增 minikv_stress_tests
    新增 stress_tests CTest

  README.md
    Version 6
    Benchmark 运行方式
    Benchmarks and stress tests 章节
    CLion 目标列表
    Roadmap 更新

删除：
  无源码文件删除
```

---

# 39. 第六版当前能力总览

到第六版为止，mini-kv 已经具备：

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
CTest 自动化覆盖
运行归档
```

从验证角度看，现在有三类验证：

```text
功能测试：
  store_tests
  command_tests
  wal_tests
  snapshot_tests

压力测试：
  stress_tests

运行观察：
  minikv_benchmark
  a/6 截图归档
```

---

# 第十五次总结

第六版把 mini-kv 的验证体系往前推进了一步：

```text
benchmark_main.cpp
 -> 给出 Store 和 CommandProcessor 的本地吞吐观察

stress_tests.cpp
 -> 多线程共享 Store，验证并发写读、TTL、snapshot、删除后的状态

CMakeLists.txt
 -> 新增 benchmark 和 stress test 目标

README.md
 -> 写清楚第六版使用方式

a/6
 -> 保存配置、构建、CTest、benchmark、stress test 的运行证据
```

一句话总结：

```text
第六版没有新增用户命令，而是给项目补上了“看性能”和“压并发”的基础工具，让后续开发不只看功能是否能跑，也能更早发现性能或线程安全异常。
```
