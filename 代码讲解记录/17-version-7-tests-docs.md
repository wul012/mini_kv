# 第十七次讲解：第七版测试、构建、文档和归档

第十六次讲了第七版 RESP parser 核心：

```text
RespParseStatus
RespCommand
RespParseResult
RespParser::parse_command
RespParser::to_inline_command
```

这次讲第七版剩下的配套内容：

```text
tests/resp_tests.cpp
CMakeLists.txt
README.md
a/7/解释/说明.md
a/7/图片/
```

它们负责证明：

```text
RESP parser 不只是写了代码
而是能编译、能测试、能和现有 CommandProcessor 桥接
并且不破坏原有 inline CLI 行为
```

---

# 1. `tests/resp_tests.cpp` 的角色

第七版新增文件：

```text
tests/resp_tests.cpp
```

它专门验证 RESP parser。

测试范围包括：

```text
合法 RESP SET 请求
带空格 value 的 RESP SET 请求
pipeline 输入中的 consumed 字节数
incomplete 输入
非 RESP 输入
非 bulk string 参数
null bulk string
空数组
桥接到 CommandProcessor 执行
```

这样既测协议解析，也测第七版和旧命令层之间的衔接。

---

# 2. 引入头文件

```cpp
#include "minikv/command.hpp"
#include "minikv/resp.hpp"
#include "minikv/store.hpp"
```

测试需要三层对象：

```text
resp.hpp
 -> 被测试的 RESP parser

command.hpp
 -> 验证 to_inline_command 后能执行

store.hpp
 -> CommandProcessor 背后的内存数据库
```

---

标准库头文件：

```cpp
#include <cassert>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
```

其中：

```text
cassert           用 assert 做测试断言
initializer_list  方便构造 RESP 参数列表
string_view       轻量传参数
vector            对比解析出的 arguments
```

`optional` 当前没有实际使用。

它可以后续清理掉，但不影响测试通过。

---

# 3. `make_resp_array`

```cpp
std::string make_resp_array(std::initializer_list<std::string_view> arguments) {
    std::string encoded = "*" + std::to_string(arguments.size()) + "\r\n";
    for (const auto argument : arguments) {
        encoded += "$" + std::to_string(argument.size()) + "\r\n";
        encoded += argument;
        encoded += "\r\n";
    }
    return encoded;
}
```

这个函数把参数列表编码成 RESP array-of-bulk-strings。

例如：

```cpp
make_resp_array({"SET", "name", "mini-kv"})
```

会生成：

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
```

这样测试不用手写一长串 `\r\n`。

可读性更好，也不容易写错长度。

---

# 4. `assert_complete`

```cpp
void assert_complete(const minikv::RespParseResult& result) {
    assert(result.status == minikv::RespParseStatus::complete);
    assert(result.error.empty());
}
```

这个辅助函数验证解析成功。

成功时应该满足：

```text
status == complete
error 为空
```

后面再单独检查：

```text
arguments
consumed
```

---

# 5. `assert_error`

```cpp
void assert_error(const minikv::RespParseResult& result) {
    assert(result.status == minikv::RespParseStatus::error);
    assert(!result.error.empty());
}
```

这个辅助函数验证解析失败。

失败时应该满足：

```text
status == error
error 非空
```

这样可以保证 parser 不只是说失败，还给了可用于调试的错误消息。

---

# 6. 测试普通 SET 请求

```cpp
const auto set_request = make_resp_array({"SET", "name", "mini-kv"});
const auto set_result = minikv::RespParser::parse_command(set_request);
assert_complete(set_result);
```

这里构造一个 RESP SET 请求。

解析后必须是：

```text
complete
```

---

检查参数：

```cpp
const std::vector<std::string> expected_set{"SET", "name", "mini-kv"};
assert(set_result.command.arguments == expected_set);
```

解析出的 arguments 应该是：

```text
SET
name
mini-kv
```

---

检查 consumed：

```cpp
assert(set_result.command.consumed == set_request.size());
```

因为输入只有一个命令，所以 consumed 应该等于整个请求长度。

---

检查 inline 桥接：

```cpp
assert(minikv::RespParser::to_inline_command(set_result.command) == "SET name mini-kv");
```

这说明 RESP 参数能被转换成当前命令层使用的文本格式。

---

# 7. 桥接到 CommandProcessor

```cpp
minikv::Store store;
minikv::CommandProcessor processor{store};
auto response = processor.execute(minikv::RespParser::to_inline_command(set_result.command));
assert(response.response == "OK inserted");
```

这里不是只测 parser。

它还把 parser 输出交给：

```cpp
CommandProcessor
```

执行。

这证明：

```text
RESP parser -> inline command -> CommandProcessor -> Store
```

这条桥接链路能跑通。

---

# 8. 测试带空格 value

```cpp
const auto spaced_request = make_resp_array({"SET", "phrase", "hello from resp wire"});
const auto spaced_result = minikv::RespParser::parse_command(spaced_request);
assert_complete(spaced_result);
```

这里的 value 是：

```text
hello from resp wire
```

包含空格。

RESP bulk string 本身支持空格，因为它用长度标记。

解析出的参数应该是：

```cpp
const std::vector<std::string> expected_spaced{"SET", "phrase", "hello from resp wire"};
assert(spaced_result.command.arguments == expected_spaced);
```

---

然后执行：

```cpp
response = processor.execute(minikv::RespParser::to_inline_command(spaced_result.command));
assert(response.response == "OK inserted");
response = processor.execute("GET phrase");
assert(response.response == "hello from resp wire");
```

这证明：

```text
带空格的 RESP bulk string value
 -> to_inline_command 后
 -> 仍然能被 SET 命令正确保存
```

原因是 `CommandProcessor` 的 `SET` 会用：

```cpp
getline(input >> std::ws, value)
```

读取剩余整行作为 value。

---

# 9. 测试 pipeline consumed

```cpp
const auto pipelined = make_resp_array({"GET", "name"}) + make_resp_array({"SIZE"});
const auto first = minikv::RespParser::parse_command(pipelined);
assert_complete(first);
```

这里把两个 RESP 命令拼在一起。

这模拟 Redis pipeline：

```text
客户端一次发送多个命令
服务端逐个解析执行
```

---

第一个命令应该是：

```cpp
const std::vector<std::string> expected_get{"GET", "name"};
assert(first.command.arguments == expected_get);
```

然后检查：

```cpp
assert(first.command.consumed < pipelined.size());
```

因为输入里还有第二个命令，所以第一个命令消耗的字节数小于总长度。

---

解析第二个命令：

```cpp
const auto second = minikv::RespParser::parse_command(pipelined.substr(first.command.consumed));
assert_complete(second);
const std::vector<std::string> expected_size{"SIZE"};
assert(second.command.arguments == expected_size);
assert(second.command.consumed == make_resp_array({"SIZE"}).size());
```

这里用：

```cpp
pipelined.substr(first.command.consumed)
```

跳过第一个命令，继续解析剩余数据。

这证明 `consumed` 字段可以支持 pipeline。

---

# 10. 测试 incomplete 输入

```cpp
const auto incomplete = set_request.substr(0, set_request.size() - 2);
const auto incomplete_result = minikv::RespParser::parse_command(incomplete);
assert(incomplete_result.status == minikv::RespParseStatus::incomplete);
```

这里故意把 SET 请求最后两个字节砍掉。

最后两个字节通常是：

```text
\r\n
```

所以请求不完整。

parser 应该返回：

```text
incomplete
```

而不是：

```text
error
```

因为 TCP server 遇到这种情况时，应该继续读 socket。

---

# 11. 测试非 RESP 输入

```cpp
const auto not_resp = minikv::RespParser::parse_command("GET name\r\n");
assert_error(not_resp);
```

`GET name\r\n` 是 inline 文本命令。

RESP parser 要求请求以：

```text
*
```

开头。

所以这里应该返回：

```text
error
```

---

# 12. 测试参数不是 bulk string

```cpp
const auto invalid_bulk = minikv::RespParser::parse_command("*1\r\n+PING\r\n");
assert_error(invalid_bulk);
```

这里数组里放的是：

```text
+PING
```

这是 RESP simple string。

但第七版只支持：

```text
bulk string 参数
```

所以返回 error。

---

# 13. 测试 null bulk string

```cpp
const auto null_bulk = minikv::RespParser::parse_command("*1\r\n$-1\r\n");
assert_error(null_bulk);
```

RESP 中：

```text
$-1\r\n
```

表示 null bulk string。

第七版 parser 不支持 null 参数。

所以返回 error。

---

# 14. 测试空数组

```cpp
const auto empty_array = minikv::RespParser::parse_command("*0\r\n");
assert_error(empty_array);
```

空数组没有命令名。

mini-kv 无法执行它。

所以第七版把它当作协议错误。

---

# 15. CMake 新增 resp.cpp

第七版把 RESP 实现加入核心库：

```cmake
add_library(minikv
    src/command.cpp
    src/resp.cpp
    src/snapshot.cpp
    src/store.cpp
    src/tcp_server.cpp
    src/wal.cpp
)
```

这样只要链接：

```text
minikv
```

就可以使用：

```text
RespParser
```

包括：

```text
未来的 TCP server
resp_tests
其他工具程序
```

---

# 16. CMake 新增 resp_tests

```cmake
add_executable(minikv_resp_tests
    tests/resp_tests.cpp
)

target_link_libraries(minikv_resp_tests PRIVATE minikv)

add_test(NAME resp_tests COMMAND minikv_resp_tests)
```

这会生成：

```text
minikv_resp_tests.exe
```

并注册到 CTest：

```text
resp_tests
```

第七版运行 CTest 时会看到：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
resp_tests
```

---

# 17. CMake 版本更新

第七版项目版本改成：

```cmake
project(mini_kv VERSION 0.7.0 LANGUAGES CXX)
```

版本节奏现在是：

```text
0.1.0 = 内存 KV + CLI + TCP server
0.2.0 = TCP client
0.3.0 = TTL
0.4.0 = WAL persistence
0.5.0 = Manual snapshots
0.6.0 = Benchmark and stress tests
0.7.0 = RESP parser
```

---

# 18. README 当前版本更新

第七版 README 当前版本变成：

```text
Version 7 is a runnable in-memory KV service with benchmarks, stress tests, and a RESP parser:
```

功能列表新增：

```text
Redis RESP parser for array-of-bulk-string requests
```

这里强调：

```text
第七版支持的是请求解析器
不是 TCP server 已经全面切换到 RESP
```

---

# 19. README 新增 CLion 目标

第七版 CLion 目标列表新增：

```text
minikv_resp_tests
```

这样可以在 CLion 里单独运行 RESP parser 测试。

---

# 20. README 新增 RESP parser 章节

README 新增：

```text
## RESP parser
```

里面给出示例：

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
```

说明 parser 当前处理：

```text
Redis-style request arrays made of bulk strings
```

并说明：

```text
It returns the parsed arguments and the number of bytes consumed
```

这对应代码里的：

```cpp
RespCommand::arguments
RespCommand::consumed
```

---

README 还明确说明：

```text
The current TCP server still uses the inline text protocol;
the RESP parser is ready for the next integration step.
```

这句话很重要。

它避免用户误解：

```text
第七版已经能用 Redis 客户端直接连 TCP server
```

实际情况是：

```text
第七版 parser 已完成
第八版才适合接入 TCP server
```

---

# 21. Roadmap 更新

第六版 Roadmap 是：

```text
1. Add a Redis RESP parser.
```

第七版完成 parser 后，Roadmap 变成：

```text
1. Wire the RESP parser into the TCP server.
2. Add RESP response formatting and protocol hardening.
```

也就是说：

```text
RESP parser 已经完成第一步
下一步是 TCP server 集成
```

---

# 22. 第七版归档 `a/7`

第七版按项目规则新增：

```text
a/7/图片/
a/7/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-resp-tests-verbose.png
05-cli-regression-smoke.png
```

说明文档记录：

```text
第七版做了什么
每张截图对应什么命令
配置、构建、全量 CTest、RESP 单项测试、CLI 回归验证的结果
是否启动后台进程
是否有临时文件需要清理
```

---

# 23. `01-cmake-configure.png`

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
CMake 能识别 resp.cpp、resp_tests.cpp 和 minikv_resp_tests 目标
```

---

# 24. `02-cmake-build.png`

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
RESP parser 模块、测试目标和原有可执行目标都能通过 C++20 编译链接
```

---

# 25. `03-ctest.png`

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
6 个测试全部通过
```

测试列表：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
resp_tests
```

意义：

```text
新增 RESP parser 后，原有功能和压力测试都没有被破坏
```

---

# 26. `04-resp-tests-verbose.png`

对应命令：

```text
ctest --test-dir cmake-build-debug -R resp_tests --verbose
```

结果：

```text
resp_tests passed
```

意义：

```text
单独证明 RESP parser 测试目标可以运行，并且 parser 核心行为通过断言
```

---

# 27. `05-cli-regression-smoke.png`

对应命令：

```text
通过管道向 minikv_cli.exe 输入 inline 命令
```

输入：

```text
SET resp seventh-version
GET resp
QUIT
```

结果：

```text
OK inserted
seventh-version
BYE
```

意义：

```text
第七版新增 RESP parser 后，原来的 CLI inline 命令仍然正常
```

这是一条回归验证。

因为第七版没有改 CLI 协议，但仍然要确认新增模块没有影响旧入口。

---

# 28. 第七版文件变化总表

第七版相关增删改可以整理成：

```text
新增：
  include/minikv/resp.hpp
  src/resp.cpp
  tests/resp_tests.cpp
  a/7/图片/
  a/7/解释/说明.md

修改：
  CMakeLists.txt
    版本 0.7.0
    minikv 加入 resp.cpp
    新增 minikv_resp_tests
    新增 resp_tests CTest

  README.md
    Version 7
    RESP parser 章节
    CLion 目标列表
    Roadmap 更新

删除：
  无源码文件删除
```

---

# 29. 第七版当前能力总览

到第七版为止，mini-kv 已经具备：

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
CTest 自动化覆盖
运行归档
```

协议层现在是：

```text
inline 文本协议
 -> 已接入 CLI 和 TCP server

RESP parser
 -> 已实现并测试
 -> 尚未接入 TCP server
```

---

# 第十七次总结

第七版把 RESP parser 从代码、测试、文档、归档四个层面补齐：

```text
resp.hpp / resp.cpp
 -> 定义并实现 RESP 请求解析

resp_tests.cpp
 -> 验证完整请求、不完整请求、错误请求、pipeline consumed 和 CommandProcessor 桥接

CMakeLists.txt
 -> 新增 resp.cpp 和 resp_tests

README.md
 -> 说明 RESP parser 当前能力和下一步接入 TCP server

a/7
 -> 保存配置、构建、CTest、RESP 单测和 CLI 回归验证截图
```

一句话总结：

```text
第七版完成了 Redis RESP 请求解析的第一块地基：协议字节能被解析成命令参数，测试证明它能桥接到现有命令层，但真正让 TCP server 接收 RESP 还留到下一版。
```
