# 第二十次讲解：第九版 PING 命令和 RESP 协议加固

第十九次讲了第八版的 RESP 响应格式、客户端调整、测试和归档。

这次进入第九版核心代码：

```text
CommandProcessor::execute
CommandProcessor::help_text
RespParser::max_arguments
RespParser::max_bulk_length
RespParser::parse_command
RespParser::to_resp_response
```

第九版主要做两件事：

```text
第一，新增 Redis 风格的 PING [message] 命令
第二，给 RESP parser 增加明确的协议边界
```

它不是一次大重构。

它更像是把第八版刚接通的 RESP 网络链路往“更像真实服务”方向补了一块：

```text
客户端连上后可以 PING 探活
异常大的 RESP 声明不会让服务端一直等
RESP 响应里的 PONG 使用正确的 simple string frame
```

---

# 1. 第九版为什么先做 PING

Redis 客户端或调试工具经常用：

```text
PING
```

来确认连接还活着。

期望响应通常是：

```text
PONG
```

带消息时：

```text
PING hello
```

期望响应就是：

```text
hello
```

mini-kv 第八版已经支持 RESP 请求和响应。

但如果常见客户端一连上就发 `PING`，第八版会返回：

```text
ERR unknown command
```

这说明协议虽然接通了，但兼容性还不够自然。

所以第九版先补 `PING`。

---

# 2. `PING` 放在 CommandProcessor，而不是 TCP server

第九版没有在 `tcp_server.cpp` 里单独判断：

```text
如果收到 PING 就返回 PONG
```

而是把它放进：

```cpp
CommandProcessor::execute
```

原因是项目里所有命令都走同一个命令层：

```text
CLI 输入
 -> CommandProcessor

inline TCP 输入
 -> CommandProcessor

RESP TCP 输入
 -> RespParser
 -> to_inline_command
 -> CommandProcessor
```

所以 `PING` 放在 `CommandProcessor` 里之后，三个入口自动共享：

```text
本地 CLI 可用
inline TCP 可用
RESP TCP 可用
```

这比在 TCP server 里写特殊分支更统一。

---

# 3. `command.cpp` 中的 PING 分支

第九版在命令名转大写后新增：

```cpp
if (command == "PING") {
    std::string message;
    std::getline(input >> std::ws, message);

    return {message.empty() ? "PONG" : message};
}
```

这里先看它的位置：

```text
trim line
 -> istringstream input
 -> 读取 command
 -> command 转大写
 -> 判断 PING
 -> 再判断 SET / GET / DEL / ...
```

也就是说：

```text
ping
Ping
PING
```

都会被 `to_upper` 归一成：

```text
PING
```

---

# 4. `std::getline(input >> std::ws, message)`

这句代码和 `SET key value` 里的 value 读取方式类似。

```cpp
std::getline(input >> std::ws, message);
```

含义是：

```text
先跳过命令后面的空白
再读取这一行剩下的全部内容
```

所以：

```text
PING hello from ping
```

读出来的 `message` 是：

```text
hello from ping
```

它保留中间空格。

如果用户只输入：

```text
PING
```

那么 `message` 为空。

---

# 5. `message.empty() ? "PONG" : message`

`PING` 的返回规则写在这一行：

```cpp
return {message.empty() ? "PONG" : message};
```

它对应两种行为：

```text
PING
 -> PONG

PING hello
 -> hello
```

这里返回的是项目内部响应文本。

在不同入口里会继续被包装：

```text
CLI
 -> 直接打印 PONG

inline TCP
 -> 发送 PONG\n

RESP TCP
 -> RespParser::to_resp_response("PONG")
 -> +PONG\r\n
```

这就是第九版保持“命令层统一、协议层包装”的方式。

---

# 6. PING 不写 WAL

`PING` 是探活命令，不改变数据。

所以它不需要：

```text
Store set
Store erase
WAL append
Snapshot save
```

它只返回响应。

这和 `SIZE`、`GET`、`TTL` 类似，都属于读或查询类行为。

第九版把 `PING` 分支放在 `SET` 分支前面，也意味着：

```text
命令识别完成后立即返回
不会误触 WAL
不会改变 Store
```

---

# 7. help_text 新增 PING

第九版同步更新了帮助文本：

```cpp
std::string CommandProcessor::help_text() {
    return "Commands:\n"
           "  PING [message]\n"
           "  SET key value\n"
           "  GET key\n"
           ...
}
```

这样用户输入：

```text
HELP
```

能看到新命令。

这一步虽然不是核心逻辑，但很重要。

因为项目命令列表现在主要分布在三个地方：

```text
CommandProcessor::execute
CommandProcessor::help_text
README.md
```

新增命令时要一起更新。

---

# 8. 第九版为什么要加 RESP parser 限制

第八版 RESP parser 已经能解析：

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
```

但它有一个隐患。

如果客户端发送：

```text
*999999999\r\n
```

这表示：

```text
我要发送一个有 999999999 个参数的数组
```

服务端会认为：

```text
这是不完整请求
继续等后续参数
```

类似地，如果客户端发送：

```text
*1\r\n$999999999\r\n
```

这表示：

```text
我要发送一个超大的 bulk string
```

如果没有明确边界，服务端很难判断这是：

```text
正常但还没发完
```

还是：

```text
异常请求
```

第九版给 parser 增加了明确上限。

---

# 9. resp.hpp 中新增两个常量

第九版在 `RespParser` 类里新增：

```cpp
static constexpr std::size_t max_arguments = 1024;
static constexpr std::size_t max_bulk_length = 64 * 1024;
```

这两个值是协议层限制：

```text
max_arguments
 -> 一个 RESP command array 最多 1024 个参数

max_bulk_length
 -> 单个 bulk string 最多 64 KiB
```

它们放在头文件里，有两个好处：

```text
第一，parse_command 可以直接使用
第二，tests/resp_tests.cpp 可以直接引用同一份限制
```

测试不需要自己写死：

```text
1024
65536
```

而是使用：

```cpp
RespParser::max_arguments
RespParser::max_bulk_length
```

这样以后调整限制时，测试仍然跟着同一个定义走。

---

# 10. 为什么用 `constexpr`

```cpp
static constexpr std::size_t max_arguments = 1024;
```

这里的意思是：

```text
这是编译期常量
属于 RespParser 类
不需要创建 RespParser 对象
```

所以可以这样访问：

```cpp
RespParser::max_arguments
```

当前 `RespParser` 本来就是一个纯静态工具类：

```text
parse_command 是 static
to_inline_command 是 static
to_resp_response 是 static
```

限制常量也放成 static，风格一致。

---

# 11. array 参数数量限制

在 `RespParser::parse_command` 里，原来解析 array count 后直接进入循环：

```cpp
const auto count = static_cast<std::size_t>(count_value);
std::vector<std::string> arguments;
```

第九版改成：

```cpp
const auto count = static_cast<std::size_t>(count_value);
if (count > RespParser::max_arguments) {
    return error_result("RESP command array has too many arguments");
}

std::vector<std::string> arguments;
```

执行顺序是：

```text
读到 * 后面的数组长度
 -> 必须是整数
 -> 必须大于 0
 -> 必须不超过 max_arguments
 -> 才开始解析每个 bulk string
```

这样如果客户端只发：

```text
*1025\r\n
```

parser 会直接返回：

```text
error
```

而不是：

```text
incomplete
```

这能让 TCP server 立即返回错误并关闭当前连接。

---

# 12. 为什么 array count 小于等于 0 仍然是错误

原来的逻辑保留了：

```cpp
if (count_value <= 0) {
    return error_result("RESP command array must contain at least one bulk string");
}
```

这意味着：

```text
*0\r\n
```

仍然是错误。

原因是 mini-kv 解析的是：

```text
RESP command array
```

而不是所有 RESP 数据类型。

一个命令至少需要命令名。

例如：

```text
*1\r\n$4\r\nPING\r\n
```

第一个 bulk string 就是命令名。

空数组没有命令名，无法桥接到 `CommandProcessor`，所以继续按错误处理。

---

# 13. bulk string 长度限制

第九版在解析 bulk length 后新增：

```cpp
const auto length = static_cast<std::size_t>(length_value);
if (length > RespParser::max_bulk_length) {
    return error_result("RESP bulk string is too large");
}

cursor = *length_end + 2;
```

注意它的位置。

它发生在：

```text
长度文本解析成功之后
cursor 移动到 bulk 内容之前
检查 input 里是否已经有足够数据之前
```

这样做的效果是：

```text
如果声明的 bulk string 已经超限
 -> 直接 error
 -> 不再等待后续内容
```

例如：

```text
*1\r\n$65537\r\n
```

不会被认为是：

```text
还缺 65537 字节，所以 incomplete
```

而是：

```text
RESP bulk string is too large
```

---

# 14. 为什么先拒绝超限，再判断 incomplete

第九版这段代码顺序是刻意的：

```cpp
if (length > RespParser::max_bulk_length) {
    return error_result("RESP bulk string is too large");
}

cursor = *length_end + 2;

if (length > input.size() - cursor) {
    return incomplete_result();
}
```

它表达了一个规则：

```text
合法范围内但数据还没到齐
 -> incomplete

声明本身已经超过服务端允许范围
 -> error
```

这两个状态不能混在一起。

`incomplete` 是正常网络分包。

`error` 是请求不合法。

第九版把这条边界划清楚了。

---

# 15. `to_resp_response` 新增 PONG

第八版的 RESP response formatting 已经支持：

```text
(nil) -> $-1\r\n
ERR ... -> -ERR ...\r\n
整数文本 -> :数字\r\n
OK ... / BYE -> +...\r\n
普通字符串 -> bulk string
```

第九版补上：

```cpp
if (starts_with(response, "OK ") || response == "BYE" || response == "PONG") {
    return "+" + std::string{response} + "\r\n";
}
```

也就是：

```text
PONG
 -> +PONG\r\n
```

这符合 RESP simple string 的常见用法。

---

# 16. 为什么 PONG 是 simple string，而 echo message 是 bulk string

`PING` 有两种响应：

```text
PING
 -> PONG

PING hello from resp ping
 -> hello from resp ping
```

第九版的编码结果是：

```text
PONG
 -> +PONG\r\n

hello from resp ping
 -> $20\r\nhello from resp ping\r\n
```

原因是 `to_resp_response` 的判断顺序：

```text
先判断 PONG
再走普通字符串 fallback
```

`hello from resp ping` 不是：

```text
(nil)
ERR ...
整数
OK ...
BYE
PONG
```

所以最后进入：

```cpp
return bulk_string(response);
```

bulk string 更适合承载用户传入的普通消息。

---

# 17. 第九版没有改 TCP server 的原因

第九版没有动 `src/tcp_server.cpp`。

这是一个好信号。

因为第八版已经把 TCP server 设计成：

```text
inline 请求
 -> CommandProcessor

RESP 请求
 -> RespParser
 -> CommandProcessor
 -> RespParser::to_resp_response
```

第九版只是增强：

```text
CommandProcessor 能识别 PING
RespParser 能拒绝超限 RESP
to_resp_response 能格式化 PONG
```

于是 TCP server 自然继承这些能力。

这说明第八版拆出来的边界是有价值的。

---

# 18. 超限错误如何到达客户端

RESP TCP 请求链路是：

```text
process_resp_commands
 -> RespParser::parse_command
```

如果 parser 返回 `error`：

```cpp
if (parsed.status == RespParseStatus::error) {
    send_all(socket, "-ERR " + parsed.error + "\r\n");
    return false;
}
```

第九版新增的两个错误：

```text
RESP command array has too many arguments
RESP bulk string is too large
```

都会走这条链路。

客户端收到的是 RESP error：

```text
-ERR RESP command array has too many arguments\r\n
-ERR RESP bulk string is too large\r\n
```

然后当前连接关闭。

---

# 19. 第九版对安全性的意义

第九版不是完整安全系统。

但它补上了协议入口很关键的一层：

```text
对参数数量有限制
对单个字符串长度有限制
对超限声明立即返回错误
```

这能减少两类问题：

```text
异常客户端声明巨大 array，服务端一直等
异常客户端声明巨大 bulk string，服务端一直积累 pending
```

虽然 TCP server 里本来还有：

```text
pending.size() > 64 * 1024
```

的整体请求保护，但第九版 parser 限制更早、更明确。

它看到声明已经不合法时，不需要等 pending 真正涨到 64 KiB。

---

# 20. 第九版核心链路总览

第九版之后，`PING` 的 inline 链路是：

```text
客户端发送：
PING\n

TCP server:
process_inline_commands
 -> CommandProcessor::execute("PING")
 -> "PONG"
 -> send_all("PONG\n")
```

RESP 链路是：

```text
客户端发送：
*1\r\n$4\r\nPING\r\n

TCP server:
process_resp_commands
 -> RespParser::parse_command
 -> arguments = ["PING"]
 -> to_inline_command = "PING"
 -> CommandProcessor::execute
 -> "PONG"
 -> to_resp_response
 -> +PONG\r\n
```

RESP 超限链路是：

```text
客户端发送：
*1025\r\n

RespParser::parse_command
 -> count = 1025
 -> count > max_arguments
 -> error

TCP server:
 -> -ERR RESP command array has too many arguments\r\n
 -> close connection
```

---

# 第二十次总结

第九版核心不是增加很多命令，而是让协议层更稳：

```text
PING [message]
 -> 给 CLI、inline TCP、RESP TCP 都补上探活命令

RESP parser limits
 -> 给 array 参数数量和 bulk string 长度划边界

to_resp_response("PONG")
 -> 让 RESP 客户端拿到正确的 +PONG\r\n
```

一句话总结：

```text
第九版把第八版刚打通的 RESP 服务从“能通信”推进到“更像可被客户端正常探活、并且能拒绝明显异常请求”的阶段。
```
