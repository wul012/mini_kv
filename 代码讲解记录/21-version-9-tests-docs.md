# 第二十一次讲解：第九版测试、README、运行归档和增删改

第二十次讲了第九版核心代码：

```text
PING [message]
RESP 参数数量限制
RESP bulk string 长度限制
PONG 的 RESP simple string 编码
```

这次讲第九版配套内容：

```text
tests/command_tests.cpp
tests/resp_tests.cpp
CMakeLists.txt
README.md
a/9/解释/说明.md
a/9/图片/
```

它们负责证明：

```text
新命令能执行
RESP 新边界能触发
文档能说明当前能力
运行归档能证明真实构建、测试和 TCP smoke 都通过
```

---

# 1. 第九版测试覆盖目标

第九版改动不大，但涉及两条入口：

```text
命令层：CommandProcessor::execute
协议层：RespParser::parse_command / to_resp_response
```

所以测试也分两类补：

```text
tests/command_tests.cpp
 -> 验证 PING 在命令层可用

tests/resp_tests.cpp
 -> 验证 RESP PING、PONG 编码、echo ping、协议超限错误
```

这样可以保证：

```text
不通过 TCP，也能测核心逻辑
真实 TCP smoke，再证明网络链路也能跑
```

---

# 2. command_tests 新增 PING 无参数测试

第九版在空命令测试后增加：

```cpp
result = processor.execute("PING");
assert(result.response == "PONG");
assert(!result.should_close);
```

它验证三件事：

```text
CommandProcessor 能识别 PING
返回内容是 PONG
PING 不会关闭连接
```

`should_close` 必须是 `false`。

因为：

```text
PING 是探活
QUIT / EXIT 才是关闭连接
```

如果这里误变成 `true`，TCP client 发一次 `PING` 就会断开，这显然不对。

---

# 3. command_tests 新增 PING message 测试

第二个新增测试是：

```cpp
result = processor.execute("PING hello from ping");
assert(result.response == "hello from ping");
assert(!result.should_close);
```

它验证：

```text
PING 后面的整段消息会被保留下来
中间空格不会被拆坏
响应不会触发关闭
```

这和 `command.cpp` 中的：

```cpp
std::getline(input >> std::ws, message);
```

互相对应。

如果以后有人把 `PING` 改成只读一个 token：

```cpp
input >> message;
```

这个测试就会失败，因为 `hello from ping` 会变成只返回 `hello`。

---

# 4. resp_tests 新增 PONG 编码断言

第九版在 response formatting 测试里增加：

```cpp
assert(minikv::RespParser::to_resp_response("PONG") == "+PONG\r\n");
```

它验证：

```text
内部响应 PONG
 -> RESP simple string
 -> +PONG\r\n
```

这和第八版已有断言放在一起：

```text
OK inserted -> +OK inserted\r\n
mini-kv -> $7\r\nmini-kv\r\n
(nil) -> $-1\r\n
2 -> :2\r\n
ERR unknown command -> -ERR unknown command\r\n
```

这样 `to_resp_response` 的主要分支都能被直接覆盖。

---

# 5. resp_tests 新增 RESP PING 桥接测试

第九版新增：

```cpp
const auto ping_request = make_resp_array({"PING"});
const auto ping_result = minikv::RespParser::parse_command(ping_request);
assert_complete(ping_result);
response = processor.execute(minikv::RespParser::to_inline_command(ping_result.command));
assert(response.response == "PONG");
assert(minikv::RespParser::to_resp_response(response.response) == "+PONG\r\n");
```

这段测试把完整桥接链路走了一遍：

```text
RESP bytes
 -> parse_command
 -> RespCommand arguments
 -> to_inline_command
 -> CommandProcessor::execute
 -> to_resp_response
```

它不是单纯测 parser。

它验证第九版最关心的结果：

```text
RESP PING 最终能得到 +PONG\r\n
```

---

# 6. `make_resp_array({"PING"})`

测试里没有手写：

```text
*1\r\n$4\r\nPING\r\n
```

而是继续复用 helper：

```cpp
std::string make_resp_array(std::initializer_list<std::string_view> arguments)
```

这样测试重点放在：

```text
要表达的参数列表
```

而不是每次重复拼 RESP 长度。

第九版 `PING` 请求：

```cpp
make_resp_array({"PING"})
```

生成：

```text
*1\r\n
$4\r\n
PING\r\n
```

---

# 7. resp_tests 新增 echo ping 测试

第九版还新增了带消息的 RESP PING：

```cpp
const auto echo_ping_request = make_resp_array({"PING", "hello from resp ping"});
const auto echo_ping_result = minikv::RespParser::parse_command(echo_ping_request);
assert_complete(echo_ping_result);
response = processor.execute(minikv::RespParser::to_inline_command(echo_ping_result.command));
assert(response.response == "hello from resp ping");
assert(minikv::RespParser::to_resp_response(response.response) == "$20\r\nhello from resp ping\r\n");
```

这里验证两件事：

```text
RESP 的第二个 bulk string 能变成 PING message
message 响应会被编码成 RESP bulk string
```

`hello from resp ping` 长度是 20：

```text
hello = 5
空格 = 1
from = 4
空格 = 1
resp = 4
空格 = 1
ping = 4
总计 = 20
```

所以期望响应是：

```text
$20\r\nhello from resp ping\r\n
```

---

# 8. 为什么 echo ping 不返回 simple string

`PONG` 是固定状态响应，所以适合：

```text
+PONG\r\n
```

但用户传入的 message 是普通字符串。

普通字符串可能包含：

```text
空格
数字
更复杂的内容
```

第九版让它走 `bulk_string`：

```text
$长度\r\n内容\r\n
```

这比 simple string 更适合作为数据返回。

测试里明确断言 bulk string，是为了锁住这个行为。

---

# 9. resp_tests 新增 array 超限测试

第九版新增：

```cpp
const auto too_many_arguments =
    minikv::RespParser::parse_command("*" + std::to_string(minikv::RespParser::max_arguments + 1) + "\r\n");
assert_error(too_many_arguments);
```

它构造的请求是：

```text
*1025\r\n
```

因为当前：

```cpp
RespParser::max_arguments == 1024
```

所以 `max_arguments + 1` 就是第一个非法值。

测试没有写死 `1025`，而是引用常量：

```cpp
minikv::RespParser::max_arguments + 1
```

这样限制值以后改了，测试仍然表达：

```text
超过最大参数数量一位就应该 error
```

---

# 10. 为什么这个请求没有完整参数也应当 error

`*1025\r\n` 只声明了数组长度，没有后续 bulk string。

如果没有第九版限制，它可能会被认为是：

```text
incomplete
```

因为 parser 会继续等待 1025 个 bulk string。

但第九版的规则是：

```text
声明数量已经超出上限
 -> 不需要等后续内容
 -> 直接 error
```

所以测试使用不完整 payload 反而更能证明：

```text
限制检查发生得足够早
```

---

# 11. resp_tests 新增 bulk 超限测试

第九版还新增：

```cpp
const auto too_large_bulk =
    minikv::RespParser::parse_command("*1\r\n$" + std::to_string(minikv::RespParser::max_bulk_length + 1) + "\r\n");
assert_error(too_large_bulk);
```

它构造的是：

```text
*1\r\n
$65537\r\n
```

这表示：

```text
一个参数
但这个参数声明长度为 65537
```

当前最大 bulk 长度是：

```text
65536
```

所以它应该直接进入 `error`。

---

# 12. bulk 超限测试和 incomplete 的区别

如果请求是：

```text
*1\r\n$5\r\nhe
```

这是合法长度内的数据没收完。

状态应该是：

```text
incomplete
```

但如果请求是：

```text
*1\r\n$65537\r\n
```

这是声明本身不合法。

状态应该是：

```text
error
```

第九版测试覆盖的就是第二种。

---

# 13. CMakeLists.txt 版本号更新

第九版把项目版本从：

```cmake
project(mini_kv VERSION 0.8.0 LANGUAGES CXX)
```

更新为：

```cmake
project(mini_kv VERSION 0.9.0 LANGUAGES CXX)
```

这个版本号目前没有参与复杂发布流程。

但它有两个作用：

```text
第一，标记当前源码已经进入第九版
第二，和 README、a/9 归档保持一致
```

每一版开发完成后同步版本号，后续回看会更清楚。

---

# 14. README Current version 更新

第九版 README 把当前版本描述改为：

```text
Version 9 is a runnable in-memory KV service with TCP RESP compatibility hardening
```

关键词是：

```text
RESP compatibility hardening
```

意思是：

```text
不是重新做数据库核心
而是在第八版 RESP 网络协议基础上加强兼容性和边界处理
```

这和本轮真实代码改动一致。

---

# 15. README 功能列表新增 PING 和限制说明

第九版 README 的能力列表新增：

```text
Redis-style PING [message] command and RESP parser size limits
```

这句话覆盖了本版两个重点：

```text
PING [message]
 -> 兼容常见 Redis 探活命令

RESP parser size limits
 -> 限制 array 参数数量和 bulk string 长度
```

README 是给人快速看项目状态的地方。

所以这里不展开细节，只点明能力。

---

# 16. README CLI commands 新增 PING

第九版命令列表新增：

```text
PING [message]
SET key value
GET key
...
```

注意它放在 `SET` 前面。

原因是：

```text
PING 是最基础的连接探活命令
通常比读写数据命令更先使用
```

这和 Redis 客户端习惯也更接近。

---

# 17. README TCP protocol 示例新增 PING

inline TCP 示例新增：

```text
SET name mini-kv
PING
GET name
...
```

RESP 示例新增：

```text
*1\r\n$4\r\nPING\r\n
```

这样 README 同时告诉读者：

```text
inline 可以 PING
RESP 也可以 PING
```

这很重要，因为第九版的一个设计点就是：

```text
新增命令不绑定某一种入口
```

---

# 18. README RESP protocol 新增加固说明

第九版 README 新增：

```text
Maximum RESP arguments: 1024
Maximum RESP bulk string length: 65536 bytes
```

并说明：

```text
Oversized RESP requests return a RESP error instead of waiting indefinitely for huge payloads.
```

这句话把实现意图说清楚了：

```text
超限不是 incomplete
超限是 error
```

这和测试、代码逻辑一致。

---

# 19. README Roadmap 调整

第八版 roadmap 是：

```text
1. Add broader RESP compatibility tests and protocol hardening.
2. Add graceful shutdown and structured logging.
```

第九版已经完成一部分：

```text
RESP compatibility hardening
```

所以 roadmap 更新为：

```text
1. Add graceful shutdown and structured logging.
2. Add broader network compatibility tests.
```

也就是说，下一阶段重点从：

```text
RESP parser 基础加固
```

转向：

```text
服务运行管理
更广的网络兼容验证
```

---

# 20. a/9 运行归档

第九版新增归档目录：

```text
a/9/图片/
a/9/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-inline-tcp-smoke.png
05-resp-tcp-smoke.png
```

这延续了项目规则：

```text
关键命令输出需要保存为 PNG 截图
说明文档解释每张截图对应的命令、结果和意义
```

---

# 21. 01-cmake-configure.png

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
CMake 能识别第九版源码和测试目标
```

---

# 22. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug
```

结果：

```text
minikv 库编译成功
minikv_cli / minikv_server / minikv_client / minikv_benchmark 链接成功
所有测试 exe 链接成功
```

意义：

```text
PING 代码、RESP 限制代码和测试代码都能通过 C++20 编译
```

---

# 23. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
6/6 tests passed
```

包括：

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
第九版新增行为通过测试
旧功能没有被破坏
```

---

# 24. 04-inline-tcp-smoke.png

对应验证：

```text
启动 minikv_server 6399 127.0.0.1
用 minikv_client 连接
发送 PING
发送 PING inline v9 smoke
发送 QUIT
```

结果：

```text
PONG
inline v9 smoke
BYE
```

意义：

```text
第九版 PING 在 inline TCP 协议和 bundled client 中可用
```

---

# 25. 05-resp-tcp-smoke.png

对应验证：

```text
启动 minikv_server 6399 127.0.0.1
用 Python socket 发送 RESP 原始字节
```

验证内容：

```text
RESP PING
RESP PING hello from resp ping
RESP QUIT
RESP 超限 array: *1025\r\n
```

结果：

```text
+PONG\r\n
$20\r\nhello from resp ping\r\n
+BYE\r\n
-ERR RESP command array has too many arguments\r\n
```

意义：

```text
第九版 RESP 正常路径和超限错误路径都能在真实 TCP 连接上工作
```

---

# 26. 为什么第九版 smoke 继续用 Python socket

第八版已经记录过：

```text
当前环境里 PowerShell TcpClient 发送原始 RESP 字节不够稳定
Python socket 更适合做原始字节级 smoke test
```

第九版继续沿用这个方式。

因为第九版要验证的是：

```text
准确发送 \r\n
准确接收 RESP frame
准确构造超限请求
```

Python socket 对这种测试更直接。

---

# 27. 第九版文件变化总表

第九版相关增删改可以整理成：

```text
新增：
  a/9/图片/
  a/9/解释/说明.md
  代码讲解记录/20-ping-resp-hardening.md
  代码讲解记录/21-version-9-tests-docs.md

修改：
  CMakeLists.txt
    版本 0.9.0

  README.md
    Version 9
    新增 PING [message]
    新增 RESP parser size limits
    更新 TCP / RESP 示例
    更新 Roadmap

  include/minikv/resp.hpp
    新增 max_arguments
    新增 max_bulk_length

  src/command.cpp
    新增 PING [message] 命令
    HELP 文本新增 PING [message]

  src/resp.cpp
    parse_command 新增 array 参数数量限制
    parse_command 新增 bulk string 长度限制
    to_resp_response 新增 PONG simple string 编码

  tests/command_tests.cpp
    新增 PING 和 PING message 测试

  tests/resp_tests.cpp
    新增 PONG 编码测试
    新增 RESP PING 桥接测试
    新增 RESP echo ping 测试
    新增 array 超限测试
    新增 bulk string 超限测试

删除：
  无源码文件删除
```

---

# 28. 第九版之后当前能力总览

到第九版为止，mini-kv 已经具备：

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
CTest 自动化覆盖
运行归档
```

协议层现在是：

```text
inline 文本协议
 -> SET / GET / DEL / EXPIRE / TTL / SIZE / SAVE / LOAD / HELP / PING / QUIT

RESP 协议
 -> array-of-bulk-strings 请求
 -> RESP simple string / integer / bulk string / null bulk / error 响应
 -> 支持基本 pipeline
 -> 对明显超限请求返回错误
```

---

# 29. 第九版和前几版的关系

第七版：

```text
做出 RESP parser
```

第八版：

```text
把 RESP parser 接入 TCP server
```

第九版：

```text
让 RESP 服务更像可被常见客户端探活的服务
并给 parser 加边界
```

所以这三版是一条连续路线：

```text
能解析
 -> 能联网收发
 -> 能更稳地处理常见客户端行为和异常输入
```

---

# 第二十一次总结

第九版配套工作证明了四件事：

```text
command_tests
 -> PING 在命令层正确

resp_tests
 -> RESP PING、PONG、echo、超限错误正确

README
 -> 当前版本能力和协议限制说清楚

a/9
 -> 构建、测试、inline TCP smoke、RESP TCP smoke 都有运行截图和说明
```

一句话总结：

```text
第九版不是只把代码写进去，而是把 PING 兼容和 RESP 协议加固从单元测试、网络 smoke、README 到运行归档都闭环了。
```
