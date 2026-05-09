# 第十六次讲解：第七版 RESP Parser 核心实现

这次讲第七版最核心的新增能力：

```text
Redis RESP parser
```

RESP 是：

```text
REdis Serialization Protocol
```

也就是 Redis 客户端和服务端之间常用的协议格式。

第七版先实现的是：

```text
RESP array-of-bulk-strings 请求解析
```

比如 Redis 客户端发送的：

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
```

表示的命令其实是：

```text
SET name mini-kv
```

第七版还没有把 RESP 接入 TCP server。

它先做一件事：

```text
把 RESP 字节流解析成参数列表
```

然后提供一个桥接函数：

```text
参数列表 -> 当前 CommandProcessor 能执行的 inline 命令
```

可以把第七版理解成：

```text
第六版：
  有 benchmark 和 stress test

第七版：
  新增 RESP parser
  为第八版 TCP server 支持 RESP 做准备
```

---

# 1. 为什么先做独立 Parser

当前 TCP server 使用的是一行一命令协议：

```text
SET name mini-kv\n
GET name\n
```

这种协议简单，适合前几版快速开发。

但 Redis 标准客户端一般发的是 RESP。

比如：

```text
*2\r\n$3\r\nGET\r\n$4\r\nname\r\n
```

如果想让 mini-kv 更接近 Redis，就需要支持 RESP。

---

第七版没有直接改 TCP server，而是先新增：

```text
include/minikv/resp.hpp
src/resp.cpp
tests/resp_tests.cpp
```

这样做的好处是：

```text
协议解析可以单独测试
不会影响现有 CLI / TCP inline 功能
下一版接入 TCP server 时有稳定模块可用
```

这符合前面几版的节奏：

```text
先把核心模块做稳
再接入口层
```

---

# 2. 第七版 RESP 支持范围

第七版支持的是 Redis 请求里最常见的一种结构：

```text
Array of Bulk Strings
```

也就是：

```text
*参数数量\r\n
$第一个参数长度\r\n
第一个参数\r\n
$第二个参数长度\r\n
第二个参数\r\n
...
```

例如：

```text
*3\r\n
$3\r\n
SET\r\n
$4\r\n
name\r\n
$7\r\n
mini-kv\r\n
```

解析后得到：

```text
["SET", "name", "mini-kv"]
```

---

当前不支持：

```text
RESP simple string
RESP error
RESP integer
RESP null bulk string
RESP nested array
RESP response formatting
```

因为第七版的目标是：

```text
先解析客户端请求命令
```

而不是一次性实现完整 Redis 协议栈。

---

# 3. `resp.hpp` 的角色

第七版新增头文件：

```text
include/minikv/resp.hpp
```

它定义了 RESP parser 对外接口。

主要有四个东西：

```text
RespParseStatus
RespCommand
RespParseResult
RespParser
```

---

# 4. `RespParseStatus`

```cpp
enum class RespParseStatus {
    complete,
    incomplete,
    error,
};
```

这个枚举表示解析结果状态。

三种状态分别是：

```text
complete
 -> 已经成功解析出一个完整命令

incomplete
 -> 当前输入还不完整，需要继续读 socket

error
 -> 输入格式错误，不能继续当作合法 RESP 命令
```

这对 TCP server 很重要。

因为 TCP 是流协议。

一次 `recv` 不一定刚好收到完整命令。

可能出现：

```text
只收到 *3\r\n$3\r\nSET\r\n$4\r\nna
```

这时不能报错。

应该返回：

```text
incomplete
```

等下一次收到剩余字节。

---

# 5. `RespCommand`

```cpp
struct RespCommand {
    std::vector<std::string> arguments;
    std::size_t consumed = 0;
};
```

`arguments` 保存解析出的参数列表。

例如：

```text
["SET", "name", "mini-kv"]
```

`consumed` 表示这个命令消耗了输入缓冲区多少字节。

这个字段是为 pipeline 准备的。

比如输入里一次来了两个命令：

```text
*2\r\n$3\r\nGET\r\n$4\r\nname\r\n*1\r\n$4\r\nSIZE\r\n
```

第一次解析只消费前面的 `GET name`。

然后 TCP server 可以用：

```text
buffer.erase(0, consumed)
```

继续解析后面的 `SIZE`。

---

# 6. `RespParseResult`

```cpp
struct RespParseResult {
    RespParseStatus status = RespParseStatus::incomplete;
    RespCommand command;
    std::string error;
};
```

这是 parser 的返回值。

它同时包含：

```text
status  解析状态
command 成功时的命令
error   失败时的错误信息
```

如果状态是：

```text
complete
```

就看：

```cpp
result.command.arguments
result.command.consumed
```

如果状态是：

```text
error
```

就看：

```cpp
result.error
```

如果状态是：

```text
incomplete
```

说明输入还不够，继续接收数据。

---

# 7. `RespParser`

```cpp
class RespParser {
public:
    static RespParseResult parse_command(std::string_view input);
    static std::string to_inline_command(const RespCommand& command);
};
```

`RespParser` 当前只有两个静态函数。

第一个：

```cpp
parse_command(input)
```

负责解析 RESP 输入。

第二个：

```cpp
to_inline_command(command)
```

负责把解析后的参数列表转换成当前项目已有的 inline 命令。

例如：

```text
["SET", "name", "mini-kv"]
```

转换成：

```text
SET name mini-kv
```

这样第七版不用改 `CommandProcessor`。

它先让 RESP parser 能桥接到现有命令层。

---

# 8. `resp.cpp` 的辅助函数

`src/resp.cpp` 是 RESP parser 的具体实现。

开头有几个匿名 namespace 内的辅助函数：

```text
incomplete_result
error_result
find_crlf
parse_integer
```

这些函数不暴露给外部，只服务于当前 `.cpp` 文件。

---

# 9. `incomplete_result`

```cpp
RespParseResult incomplete_result() {
    return {RespParseStatus::incomplete, {}, {}};
}
```

这个函数统一返回：

```text
解析未完成
```

它用于这些场景：

```text
输入为空
找不到 CRLF
bulk string 内容还没收全
bulk string 后面的 CRLF 还没收全
```

这类情况都不应该立刻当成错误。

因为网络里很可能只是：

```text
数据还没全部到达
```

---

# 10. `error_result`

```cpp
RespParseResult error_result(std::string message) {
    return {RespParseStatus::error, {}, std::move(message)};
}
```

这个函数统一返回：

```text
解析错误
```

并保存错误消息。

例如：

```text
RESP command must start with an array
invalid RESP array length
RESP command arguments must be bulk strings
```

和 `incomplete` 不同，`error` 表示：

```text
当前输入格式不合法
继续等更多字节也不能修复
```

---

# 11. `find_crlf`

```cpp
std::optional<std::size_t> find_crlf(std::string_view input, std::size_t start) {
    const auto pos = input.find("\r\n", start);
    if (pos == std::string_view::npos) {
        return std::nullopt;
    }

    return pos;
}
```

RESP 每一段长度行都以：

```text
\r\n
```

结尾。

所以 parser 经常需要找 CRLF。

如果找到了，返回位置。

如果没找到，返回：

```cpp
std::nullopt
```

表示输入还不完整。

---

# 12. `parse_integer`

```cpp
bool parse_integer(std::string_view text, long long& value) {
    if (text.empty()) {
        return false;
    }

    const auto* first = text.data();
    const auto* last = text.data() + text.size();
    long long parsed = 0;
    const auto [ptr, ec] = std::from_chars(first, last, parsed);
    if (ec != std::errc{} || ptr != last) {
        return false;
    }

    value = parsed;
    return true;
}
```

RESP 里有两种地方要解析数字：

```text
数组长度
bulk string 长度
```

比如：

```text
*3
$7
```

这里用：

```cpp
std::from_chars
```

而不是：

```cpp
std::stoi
```

好处是：

```text
不抛异常
可以明确检查是否整段都被解析
适合协议解析这种热路径
```

---

这段检查很关键：

```cpp
if (ec != std::errc{} || ptr != last) {
    return false;
}
```

`ec` 检查解析是否出错。

`ptr != last` 检查是不是有多余字符。

比如：

```text
123abc
```

不应该被当成合法数字。

---

# 13. `parse_command` 起点

```cpp
RespParseResult RespParser::parse_command(std::string_view input) {
    if (input.empty()) {
        return incomplete_result();
    }
```

如果输入为空，返回：

```text
incomplete
```

因为空输入可能只是还没收到数据。

---

然后检查第一个字符：

```cpp
if (input.front() != '*') {
    return error_result("RESP command must start with an array");
}
```

Redis 命令请求通常是数组。

所以必须以：

```text
*
```

开头。

如果用户传：

```text
GET name\r\n
```

这不是 RESP array。

parser 返回：

```text
error
```

---

# 14. 解析数组长度

```cpp
const auto count_end = find_crlf(input, 1);
if (!count_end.has_value()) {
    return incomplete_result();
}
```

数组长度在 `*` 后面，一直到 CRLF 前。

比如：

```text
*3\r\n
```

如果找不到 CRLF，说明长度行还没收完整。

返回：

```text
incomplete
```

---

解析数字：

```cpp
long long count_value = 0;
if (!parse_integer(input.substr(1, *count_end - 1), count_value)) {
    return error_result("invalid RESP array length");
}
```

这里解析的是：

```text
3
```

如果是：

```text
*abc\r\n
```

就返回：

```text
invalid RESP array length
```

---

数组长度必须大于 0：

```cpp
if (count_value <= 0) {
    return error_result("RESP command array must contain at least one bulk string");
}
```

因为一个命令至少要有命令名。

例如：

```text
["GET", "name"]
```

第一个参数是：

```text
GET
```

如果数组长度是 0，就没有命令名。

当前项目不接受这种请求。

---

# 15. 为什么不提前 reserve 超大 count

代码里是：

```cpp
const auto count = static_cast<std::size_t>(count_value);
std::vector<std::string> arguments;
```

注意这里没有写：

```cpp
arguments.reserve(count);
```

这是有意的。

如果客户端发送：

```text
*999999999999\r\n
```

然后数据还没发完，提前 `reserve` 可能造成巨大内存分配。

第七版选择：

```text
边解析
边 push_back
```

这样对恶意或异常的大数组更保守。

---

# 16. cursor 的含义

```cpp
std::size_t cursor = *count_end + 2;
```

`cursor` 表示当前解析到输入缓冲区的哪个位置。

`count_end` 是数组长度行里 CRLF 的位置。

所以：

```text
*3\r\n
   ^
   count_end 指向 \r
```

加 2 是跳过：

```text
\r\n
```

让 cursor 指向第一个 bulk string 的 `$`。

---

# 17. 循环解析每个参数

```cpp
for (std::size_t index = 0; index < count; ++index) {
```

RESP 数组长度是多少，就解析多少个参数。

每个参数都必须是 bulk string。

例如：

```text
$3\r\nSET\r\n
```

---

如果 cursor 已经到输入末尾：

```cpp
if (cursor >= input.size()) {
    return incomplete_result();
}
```

说明还需要继续读数据。

---

检查类型标记：

```cpp
if (input[cursor] != '$') {
    return error_result("RESP command arguments must be bulk strings");
}
```

第七版要求每个参数都是 bulk string。

如果遇到：

```text
+PING\r\n
```

这是 simple string，不是 bulk string。

返回：

```text
error
```

---

# 18. 解析 bulk string 长度

```cpp
const auto length_end = find_crlf(input, cursor + 1);
if (!length_end.has_value()) {
    return incomplete_result();
}
```

bulk string 的长度在 `$` 后面，到 CRLF 前。

例如：

```text
$7\r\n
```

如果找不到 CRLF，说明长度行没收完整。

---

解析长度数字：

```cpp
long long length_value = 0;
if (!parse_integer(input.substr(cursor + 1, *length_end - (cursor + 1)), length_value)) {
    return error_result("invalid RESP bulk length");
}
```

如果长度不是数字，返回：

```text
invalid RESP bulk length
```

---

当前不支持 null bulk string：

```cpp
if (length_value < 0) {
    return error_result("null RESP bulk strings are not supported");
}
```

Redis RESP 里：

```text
$-1\r\n
```

表示 null bulk string。

但 mini-kv 的命令参数目前需要真实字符串。

所以第七版先把它当成错误。

---

# 19. 读取 bulk string 内容

```cpp
const auto length = static_cast<std::size_t>(length_value);
cursor = *length_end + 2;
```

先把长度转成 `size_t`。

然后把 cursor 移到内容开始位置。

---

检查内容是否收完整：

```cpp
if (length > input.size() - cursor) {
    return incomplete_result();
}
```

如果声明长度是 7，但当前缓冲区只剩 3 个字节，就说明数据还没收完。

返回：

```text
incomplete
```

---

计算内容结束位置：

```cpp
const auto terminator = cursor + length;
```

bulk string 内容后面还必须跟：

```text
\r\n
```

所以继续检查：

```cpp
if (input.size() - terminator < 2) {
    return incomplete_result();
}
```

如果连结尾 CRLF 都还没收到，就返回 incomplete。

---

确认结尾确实是 CRLF：

```cpp
if (input[terminator] != '\r' || input[terminator + 1] != '\n') {
    return error_result("RESP bulk string must end with CRLF");
}
```

如果不是 CRLF，就说明格式错误。

---

# 20. 保存参数并推进 cursor

```cpp
arguments.emplace_back(input.substr(cursor, length));
cursor = terminator + 2;
```

先把 bulk string 内容复制成 `std::string`，加入参数列表。

然后 cursor 跳过内容后的：

```text
\r\n
```

继续解析下一个参数。

---

# 21. 返回 complete

```cpp
return {RespParseStatus::complete, RespCommand{std::move(arguments), cursor}, {}};
```

如果所有参数都解析成功，返回：

```text
complete
```

其中：

```text
arguments = 参数列表
consumed  = cursor
error     = 空字符串
```

`consumed` 是当前命令消耗的字节数。

如果输入后面还有 pipeline 的第二个命令，`consumed` 会小于输入总长度。

---

# 22. `to_inline_command`

```cpp
std::string RespParser::to_inline_command(const RespCommand& command) {
    if (command.arguments.empty()) {
        return {};
    }

    std::string line = command.arguments.front();
    for (std::size_t index = 1; index < command.arguments.size(); ++index) {
        line.push_back(' ');
        line += command.arguments[index];
    }

    return line;
}
```

这个函数把 RESP 参数列表转成当前命令层能执行的 inline 命令。

例如：

```text
["SET", "name", "mini-kv"]
```

变成：

```text
SET name mini-kv
```

---

如果参数为空：

```cpp
return {};
```

正常 `parse_command` 不会返回空参数的 complete 命令。

但这个函数单独对外公开，所以仍然做防御。

---

从第一个参数开始：

```cpp
std::string line = command.arguments.front();
```

后面的参数前面加空格：

```cpp
line.push_back(' ');
line += command.arguments[index];
```

这样就能交给：

```cpp
CommandProcessor::execute(line)
```

---

# 23. 当前桥接方式的限制

`to_inline_command` 很实用，但也有一个限制。

它用空格拼参数。

所以如果参数本身包含特殊结构，未来可能需要更强的命令层接口。

当前项目里 `SET` 的 value 是用：

```cpp
getline(input >> std::ws, value)
```

读取剩余整行。

所以这种 RESP 请求：

```text
["SET", "phrase", "hello from resp wire"]
```

转换成：

```text
SET phrase hello from resp wire
```

仍然能被当前 `CommandProcessor` 正确处理。

但如果未来要支持更复杂参数，最好新增：

```text
CommandProcessor::execute_arguments(vector<string>)
```

让 RESP 不再绕回 inline 字符串。

第七版先用桥接函数保持改动范围小。

---

# 24. 第七版 parser 的安全边界

第七版 parser 做了这些边界处理：

```text
空输入 -> incomplete
不是 * 开头 -> error
数组长度不是数字 -> error
数组长度 <= 0 -> error
找不到 CRLF -> incomplete
参数不是 bulk string -> error
bulk 长度不是数字 -> error
bulk 长度 < 0 -> error
bulk 内容没收完 -> incomplete
bulk 结尾不是 CRLF -> error
```

这些状态区分很重要。

因为下一版接 TCP server 时，server 需要根据状态做不同动作：

```text
complete
 -> 执行命令并移除 consumed 字节

incomplete
 -> 继续读取 socket

error
 -> 返回协议错误或关闭连接
```

---

# 第十六次总结

第七版 RESP parser 的核心是：

```text
RespParser::parse_command
 -> 解析 RESP array-of-bulk-strings
 -> 返回 complete / incomplete / error
 -> complete 时给出 arguments 和 consumed

RespParser::to_inline_command
 -> 把 arguments 桥接成当前 CommandProcessor 能执行的文本命令
```

一句话总结：

```text
第七版没有急着改 TCP server，而是先把 RESP 请求解析成稳定的独立模块，为下一版真正接入 Redis 风格协议打好地基。
```
