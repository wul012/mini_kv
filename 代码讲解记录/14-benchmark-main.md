# 第十四次讲解：第六版 Benchmark 入口

这次讲第六版新增的第一个核心文件：

```text
src/benchmark_main.cpp
```

它会构建成一个新的可执行程序：

```text
minikv_benchmark
```

这个程序不是数据库服务端，也不是客户端。

它的作用是：

```text
在本机快速跑一组 Store 和 CommandProcessor 的吞吐测试
```

可以把第六版理解成：

```text
第五版：
  功能链路已经比较完整
  支持内存 KV、TTL、WAL、Snapshot

第六版：
  增加基础性能观察入口
  增加并发压力测试
```

这篇先讲：

```text
benchmark 入口程序
```

下一篇再讲：

```text
stress_tests、CMake、README、a/6 归档
```

---

# 1. Benchmark 的整体目标

`minikv_benchmark` 做五组测试：

```text
store.set
store.get
command.set
command.get
store.erase
```

前两组直接调用：

```cpp
Store
```

后两组通过：

```cpp
CommandProcessor
```

最后一组测试删除。

这样能看出两层开销：

```text
Store 直接操作开销
CommandProcessor 解析命令后的操作开销
```

例如：

```text
store.set      直接 set key/value
command.set    先解析 "SET key value"，再调用 Store
```

`command.set` 一定会比 `store.set` 多做一些事情：

```text
字符串拼接
命令解析
大小写转换
参数校验
CommandResult 构造
```

benchmark 就是为了让这些差异有一个本地可观察的数字。

---

# 2. Benchmark 不是生产级压测

README 里专门说明：

```text
It is a lightweight local signal, not a replacement for full production-grade benchmarking.
```

意思是：

```text
这是轻量本地信号
不是严肃生产压测
```

原因是：

```text
它只在一个进程内跑
没有真实网络延迟
没有长时间预热
没有多轮统计
没有 P50 / P99 延迟
没有跨机器或多客户端压测
```

但它仍然有价值。

因为它可以快速回答：

```text
这次改动后，基本读写还能不能跑？
大概吞吐量级有没有明显异常？
命令层比 Store 直接调用多多少开销？
```

---

# 3. 引入项目头文件

```cpp
#include "minikv/command.hpp"
#include "minikv/store.hpp"
```

benchmark 需要两层对象。

第一层是：

```cpp
minikv::Store
```

用于测试直接读写。

第二层是：

```cpp
minikv::CommandProcessor
```

用于测试命令解析后的读写。

---

# 4. 引入标准库头文件

```cpp
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
```

这些头文件分工如下：

```text
chrono       计时
cstdlib      strtoull 和 exit
iomanip      setw、fixed、setprecision 格式化输出
iostream     输入输出
string       字符串
string_view  轻量读取参数文本
vector       保存预生成 key 列表
```

其中 `sstream` 当前没有实际使用。

它可以删掉，但保留它不影响编译。

以后如果 benchmark 输出或参数解析改成流式解析，也可能会用到。

---

# 5. 匿名 namespace

```cpp
namespace {
```

文件里的辅助函数都放在匿名 namespace 里。

这样它们只在当前 `.cpp` 文件内部可见。

例如：

```cpp
measure
print_result
make_key
make_value
```

都不是项目公共 API。

它们只是 benchmark 程序自己的工具函数。

---

# 6. 使用 steady_clock 计时

```cpp
using Clock = std::chrono::steady_clock;
```

benchmark 计时使用：

```text
steady_clock
```

而不是：

```text
system_clock
```

原因是：

```text
steady_clock 适合测量耗时
system_clock 适合表示现实时间点
```

`system_clock` 可能受系统时间调整影响。

比如用户改了系统时间，或者系统同步时间。

`steady_clock` 是单调递增的，更适合测：

```text
一段代码跑了多久
```

这和 Store 里 TTL 使用 `system_clock` 不冲突。

TTL 要跨进程保存绝对时间。

benchmark 要测一段代码耗时。

二者目标不同。

---

# 7. `BenchResult`

```cpp
struct BenchResult {
    std::string name;
    std::size_t operations = 0;
    std::chrono::duration<double> elapsed{};
};
```

这个结构体保存一组 benchmark 结果。

包含三部分：

```text
name        测试名称
operations  操作次数
elapsed     总耗时
```

例如：

```text
name = "store.set"
operations = 20000
elapsed = 10.876 ms
```

后面 `print_result` 会根据这些数据算：

```text
ops/sec
```

也就是每秒操作数。

---

# 8. `print_usage`

```cpp
void print_usage(const char* program) {
    std::cerr << "Usage: " << program << " [operations] [key_count]\n";
}
```

benchmark 支持两个可选参数：

```text
operations  总操作次数
key_count   key 的数量
```

方括号表示可选：

```text
minikv_benchmark.exe
minikv_benchmark.exe 100000
minikv_benchmark.exe 100000 10000
```

如果参数错误，就打印用法，并返回：

```text
2
```

和前面 CLI / server 的参数错误返回风格保持一致。

---

# 9. `parse_positive_size`

```cpp
bool parse_positive_size(std::string_view text, std::size_t& value) {
    std::string input{text};
    char* end = nullptr;
    const unsigned long long parsed = std::strtoull(input.c_str(), &end, 10);

    if (end == input.c_str() || *end != '\0' || parsed == 0) {
        return false;
    }

    value = static_cast<std::size_t>(parsed);
    return true;
}
```

这个函数把命令行参数解析成：

```cpp
std::size_t
```

并要求它必须是正数。

---

先把 `string_view` 转成 `string`：

```cpp
std::string input{text};
```

因为：

```cpp
std::strtoull
```

需要 C 风格字符串：

```cpp
input.c_str()
```

---

调用：

```cpp
std::strtoull(input.c_str(), &end, 10);
```

含义是：

```text
按 10 进制解析 unsigned long long
end 指向解析停止的位置
```

---

错误检查：

```cpp
if (end == input.c_str() || *end != '\0' || parsed == 0) {
    return false;
}
```

三种情况会失败：

```text
end == input.c_str()
 -> 一个数字都没解析出来

*end != '\0'
 -> 后面还有非数字字符

parsed == 0
 -> 参数不能是 0
```

所以这些输入都不合法：

```text
abc
123abc
0
```

合法输入示例：

```text
20000
100000
```

---

# 10. `measure`：统一测量一段代码

```cpp
template <typename Fn>
BenchResult measure(std::string name, std::size_t operations, Fn&& fn) {
    const auto started = Clock::now();
    fn();
    const auto ended = Clock::now();
    return BenchResult{std::move(name), operations, ended - started};
}
```

这是 benchmark 的核心小工具。

它接收三样东西：

```text
name        测试名称
operations  操作次数
fn          真正要执行的测试代码
```

流程是：

```text
记录开始时间
执行 fn()
记录结束时间
返回 BenchResult
```

因为它是模板函数，所以可以接收不同的 lambda。

例如：

```cpp
measure("store.set", operations, [&] {
    ...
});
```

或者：

```cpp
measure("command.get", operations, [&] {
    ...
});
```

---

# 11. `print_result`

```cpp
void print_result(const BenchResult& result) {
    const auto seconds = result.elapsed.count();
    const auto throughput = seconds > 0.0 ? static_cast<double>(result.operations) / seconds : 0.0;

    std::cout << std::left << std::setw(18) << result.name << std::right << std::setw(12) << result.operations
              << " ops  " << std::fixed << std::setprecision(3) << std::setw(10) << seconds * 1000.0
              << " ms  " << std::setw(14) << std::setprecision(0) << throughput << " ops/sec\n";
}
```

这个函数把一组结果打印成表格行。

先取秒数：

```cpp
const auto seconds = result.elapsed.count();
```

然后计算吞吐：

```cpp
operations / seconds
```

也就是：

```text
每秒执行多少次操作
```

---

这里还防了一下除零：

```cpp
seconds > 0.0 ? ... : 0.0
```

如果耗时太短导致秒数是 0，就把吞吐写成 0。

正常情况下不会遇到。

---

输出格式使用了：

```cpp
std::setw
std::fixed
std::setprecision
std::left
std::right
```

目的是让结果对齐，便于阅读。

输出类似：

```text
store.set                20000 ops      10.876 ms        1838878 ops/sec
store.get                20000 ops       5.697 ms        3510743 ops/sec
command.set              20000 ops      29.945 ms         667884 ops/sec
```

---

# 12. `make_key` 和 `make_value`

```cpp
std::string make_key(std::size_t index) {
    return "key:" + std::to_string(index);
}

std::string make_value(std::size_t index) {
    return "value:" + std::to_string(index);
}
```

这两个函数用来生成测试数据。

例如：

```text
make_key(0)    -> key:0
make_key(10)   -> key:10
make_value(10) -> value:10
```

这样 benchmark 不需要写死大量 key。

---

# 13. `make_keys`

```cpp
std::vector<std::string> make_keys(std::size_t key_count) {
    std::vector<std::string> keys;
    keys.reserve(key_count);

    for (std::size_t i = 0; i < key_count; ++i) {
        keys.push_back(make_key(i));
    }

    return keys;
}
```

这个函数提前生成 key 列表。

为什么不在每次操作里都生成 key？

因为 benchmark 想观察的是：

```text
Store / CommandProcessor 的操作成本
```

如果每次都重新生成 key，会把更多字符串构造成本混进去。

不过 command benchmark 仍然会构造完整命令字符串：

```text
SET key value
GET key
```

这是合理的。

因为它要模拟用户命令进入 `CommandProcessor` 的成本。

---

# 14. main 默认参数

```cpp
std::size_t operations = 100000;
std::size_t key_count = 10000;
```

如果用户不传参数，默认跑：

```text
100000 次操作
10000 个 key
```

这种规模足够快速跑完，也能有一个可读的吞吐结果。

---

# 15. main 参数检查

```cpp
if (argc > 3) {
    print_usage(argv[0]);
    return 2;
}
```

最多允许两个用户参数：

```text
operations
key_count
```

如果传太多，就打印用法。

---

解析第一个参数：

```cpp
if (argc >= 2 && !parse_positive_size(argv[1], operations)) {
    print_usage(argv[0]);
    return 2;
}
```

解析第二个参数：

```cpp
if (argc >= 3 && !parse_positive_size(argv[2], key_count)) {
    print_usage(argv[0]);
    return 2;
}
```

这样可以运行：

```text
minikv_benchmark.exe 20000 2000
```

意思是：

```text
每组读写跑 20000 次
key 池大小是 2000
```

---

# 16. 限制 key_count 不超过 operations

```cpp
if (key_count > operations) {
    key_count = operations;
}
```

如果用户传：

```text
minikv_benchmark.exe 100 10000
```

操作数只有 100，key 数却是 10000。

那就没有必要生成 10000 个 key。

所以这里把 `key_count` 限制到：

```text
不超过 operations
```

---

# 17. 创建 Store 和 CommandProcessor

```cpp
const auto keys = make_keys(key_count);
minikv::Store store;
minikv::CommandProcessor processor{store};
```

先生成 key 列表。

然后创建一个共享的：

```cpp
Store store;
```

再创建一个命令处理器：

```cpp
CommandProcessor processor{store};
```

这里没有传 WAL。

所以：

```text
processor 是纯内存模式
```

benchmark 主要测内存 Store 和命令解析。

如果未来要测 WAL 或 snapshot，可以再新增专门场景。

---

# 18. 打印 benchmark 头部

```cpp
std::cout << "mini-kv benchmark\n";
std::cout << "operations: " << operations << '\n';
std::cout << "key_count:  " << key_count << "\n\n";
```

输出当前参数。

这样截图和日志里能看出这次 benchmark 的规模。

例如：

```text
mini-kv benchmark
operations: 20000
key_count:  2000
```

---

# 19. `store.set`

```cpp
const auto store_set = measure("store.set", operations, [&] {
    for (std::size_t i = 0; i < operations; ++i) {
        store.set(keys[i % key_count], make_value(i));
    }
});
```

这一组直接调用：

```cpp
store.set(...)
```

key 从预生成的 `keys` 中取：

```cpp
keys[i % key_count]
```

这样操作次数可以大于 key 数量。

比如：

```text
operations = 20000
key_count = 2000
```

每个 key 大约会被写 10 次。

后写的值会覆盖前面的值。

---

# 20. `store.get`

```cpp
std::size_t hits = 0;
const auto store_get = measure("store.get", operations, [&] {
    for (std::size_t i = 0; i < operations; ++i) {
        if (store.get(keys[i % key_count]).has_value()) {
            ++hits;
        }
    }
});
```

这一组直接调用：

```cpp
store.get(...)
```

因为上一组已经写入了所有 key，所以这里正常应该全部命中。

命中时：

```cpp
++hits;
```

最后会打印：

```text
store.get hits: 20000
```

这既是 benchmark 指标，也是一个简单正确性检查。

---

# 21. `command.set`

```cpp
const auto command_set = measure("command.set", operations, [&] {
    for (std::size_t i = 0; i < operations; ++i) {
        const auto result = processor.execute("SET " + keys[i % key_count] + " " + make_value(i));
        if (result.response.empty()) {
            std::cerr << "unexpected empty SET response\n";
            std::exit(1);
        }
    }
});
```

这一组不直接调 Store。

它拼出命令字符串：

```text
SET key:n value:n
```

然后调用：

```cpp
processor.execute(...)
```

这会经过完整命令层：

```text
trim
读取命令名
to_upper
读取 key
读取 value
调用 Store::set
返回 CommandResult
```

---

这里检查：

```cpp
if (result.response.empty())
```

正常 `SET` 应该返回：

```text
OK inserted
OK updated
```

如果返回空字符串，说明命令执行有异常。

此时 benchmark 直接：

```cpp
std::exit(1);
```

避免继续打印误导性数据。

---

# 22. `command.get`

```cpp
std::size_t command_hits = 0;
const auto command_get = measure("command.get", operations, [&] {
    for (std::size_t i = 0; i < operations; ++i) {
        const auto result = processor.execute("GET " + keys[i % key_count]);
        if (result.response != "(nil)") {
            ++command_hits;
        }
    }
});
```

这一组通过命令层执行：

```text
GET key:n
```

如果结果不是：

```text
(nil)
```

就算命中。

因为前面 `command.set` 已经写过这些 key，所以正常应该全部命中。

最后会打印：

```text
command.get hits: 20000
```

---

# 23. `store.erase`

```cpp
const auto store_delete = measure("store.erase", key_count, [&] {
    for (const auto& key : keys) {
        store.erase(key);
    }
});
```

这一组直接删除所有 key。

操作次数是：

```cpp
key_count
```

不是：

```cpp
operations
```

因为 key 列表里只有 `key_count` 个唯一 key。

每个删除一次就够了。

---

# 24. 打印结果

```cpp
print_result(store_set);
print_result(store_get);
print_result(command_set);
print_result(command_get);
print_result(store_delete);
```

每组结果打印一行。

格式统一是：

```text
名称  操作次数  毫秒耗时  ops/sec
```

例如：

```text
store.set                20000 ops      10.876 ms        1838878 ops/sec
```

---

# 25. 打印正确性摘要

```cpp
std::cout << "\nstore.get hits:    " << hits << '\n';
std::cout << "command.get hits:  " << command_hits << '\n';
std::cout << "final store size:  " << store.size() << '\n';
```

这三行很重要。

它们说明：

```text
store.get 命中了多少次
command.get 命中了多少次
最终 Store 里还剩多少 key
```

正常 smoke test 里希望看到：

```text
final store size: 0
```

因为最后已经把所有 key 删除了。

---

# 26. 第六版 benchmark smoke test

第六版调试时运行过：

```powershell
.\cmake-build-debug\minikv_benchmark.exe 20000 2000
```

输出中包含：

```text
mini-kv benchmark
operations: 20000
key_count:  2000
store.set
store.get
command.set
command.get
store.erase
final store size:  0
```

这说明：

```text
benchmark 可执行程序能启动
参数能解析
五组测试都跑完
最终状态正确
```

---

# 27. 后续可以怎么扩展

第六版 benchmark 还很轻量。

后面可以扩展：

```text
WAL 写入 benchmark
Snapshot 保存加载 benchmark
TCP client/server 端到端 benchmark
多线程 benchmark
多轮平均值
P50 / P95 / P99 延迟统计
CSV / JSON 输出
```

但第六版先保持简单。

它的目标是：

```text
让项目有一个能快速运行的本地性能观察入口
```

---

# 第十四次总结

第六版新增的 `benchmark_main.cpp` 负责构建：

```text
minikv_benchmark
```

它用统一的 `measure` 函数计时，分别测试：

```text
Store 直接 set/get/erase
CommandProcessor 的 SET/GET
```

并输出：

```text
总操作数
总耗时
ops/sec
命中次数
最终 Store 大小
```

一句话总结：

```text
第六版 benchmark 给 mini-kv 增加了一个轻量但实用的性能观察入口，让后续每次改动后都能快速看一眼基础读写和命令层吞吐是否正常。
```
