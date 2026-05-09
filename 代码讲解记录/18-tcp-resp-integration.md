# 第十八次讲解：第八版 TCP Server 接入 RESP

这次讲第八版最核心的改动：

```text
把第七版 RESP parser 接入 TCP server
```

第七版已经完成：

```text
RESP 字节流
 -> RespParser::parse_command
 -> arguments + consumed
 -> to_inline_command
 -> CommandProcessor
```

但第七版还没有让 TCP server 真正接收 RESP 请求。

第八版完成了这一步：

```text
TCP server 收到数据
 -> 如果 pending 以 * 开头，按 RESP 解析
 -> 否则按旧 inline 文本协议解析
```

也就是说，第八版的服务端同时支持：

```text
旧协议：
  SET name mini-kv\n

RESP 协议：
  *3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
```

---

# 1. 第八版整体链路

第八版 TCP server 的处理链路变成：

```text
recv socket 数据
 -> append 到 pending 缓冲区
 -> 看 pending.front()

如果是 '*'
 -> process_resp_commands
 -> RespParser::parse_command
 -> CommandProcessor::execute
 -> RespParser::to_resp_response
 -> send_all RESP 响应

否则
 -> process_inline_commands
 -> CommandProcessor::execute
 -> send_all 文本响应
```

这就是“自动协议识别”。

它的判断非常简单：

```text
RESP 请求以 * 开头
inline 命令一般以字母开头
```

---

# 2. 为什么保留 inline 协议

第八版没有把旧协议删掉。

原因有三个：

```text
第一，旧的 CLI / TCP client 还能继续使用
第二，前面版本的 smoke test 和文档仍然有效
第三，开发调试时 inline 命令比 RESP 更直观
```

所以第八版不是：

```text
把 inline 替换成 RESP
```

而是：

```text
inline + RESP 共存
```

这对渐进开发更安全。

---

# 3. tcp_server.cpp 新增 resp 头文件

第八版在 `src/tcp_server.cpp` 新增：

```cpp
#include "minikv/resp.hpp"
```

TCP server 现在需要直接调用：

```cpp
RespParser::parse_command
RespParser::to_inline_command
RespParser::to_resp_response
```

所以必须包含 RESP 模块接口。

---

# 4. 去掉服务端问候语

第八版删除了 `serve_client` 一开始发送的：

```text
mini-kv ready\n
```

为什么？

因为 RESP 客户端连接后，通常期待自己先发请求，然后读取 RESP 响应。

如果服务端一连接就发：

```text
mini-kv ready\n
```

这个文本不是 RESP frame，会干扰 RESP 客户端。

所以第八版让 server：

```text
连接建立后保持安静
等待客户端第一条请求
```

这更接近 Redis 协议行为。

---

# 5. `process_inline_commands`

第八版把原来写在 `serve_client` 里的 inline 行处理拆成函数：

```cpp
bool process_inline_commands(SocketHandle socket, CommandProcessor& processor, std::string& pending, bool& needs_more)
```

参数含义是：

```text
socket      当前客户端 socket
processor   当前连接的 CommandProcessor
pending     已收到但还没处理完的数据缓冲
needs_more  是否需要继续 recv 更多数据
```

返回值表示：

```text
true   连接可以继续
false  连接应该关闭
```

---

# 6. inline 处理开始时清空 needs_more

```cpp
needs_more = false;
```

每次处理前，先假设不需要更多数据。

后面如果发现：

```text
pending 里有内容但还没有 \n
```

才把它改成：

```cpp
true
```

---

# 7. inline 按换行切命令

```cpp
std::size_t newline = std::string::npos;
while ((newline = pending.find('\n')) != std::string::npos) {
    std::string line = pending.substr(0, newline);
    pending.erase(0, newline + 1);
```

inline 协议是一行一命令。

所以处理方式是：

```text
找 \n
截出一行
从 pending 删除这一行
执行命令
```

这和前几版 TCP server 的处理方式一致。

---

# 8. inline 执行命令并返回文本响应

```cpp
const auto result = processor.execute(line);
if (!result.response.empty() && !send_all(socket, result.response + "\n")) {
    return false;
}
```

inline 模式继续使用旧响应格式：

```text
OK inserted\n
mini-kv\n
1\n
ERR unknown command\n
```

也就是说：

```text
inline 请求 -> inline 响应
```

不会突然变成 RESP。

---

# 9. inline 遇到 QUIT

```cpp
if (result.should_close) {
    return false;
}
```

`CommandProcessor` 遇到：

```text
QUIT
EXIT
```

会返回：

```cpp
should_close = true
```

TCP server 看到后返回 false，让连接关闭。

---

# 10. inline 中途切到 RESP

```cpp
if (!pending.empty() && pending.front() == '*') {
    return true;
}
```

这一句用于支持同一批 pending 里混合出现：

```text
inline 命令
RESP 命令
```

比如：

```text
SET a b\n*2\r\n$3\r\nGET\r\n$1\r\na\r\n
```

处理完 inline 的 `SET a b` 后，如果剩余 pending 以 `*` 开头，就先返回，让外层循环切到：

```cpp
process_resp_commands
```

---

# 11. inline 输入不完整

```cpp
needs_more = !pending.empty();
return true;
```

如果 pending 里还有内容，但没有换行，说明 inline 命令还没收完整。

例如只收到：

```text
SET name
```

这时不能执行。

应该等待下一次 `recv` 收到：

```text
 mini-kv\n
```

所以设置：

```cpp
needs_more = true;
```

---

# 12. `process_resp_commands`

第八版新增 RESP 处理函数：

```cpp
bool process_resp_commands(SocketHandle socket, CommandProcessor& processor, std::string& pending, bool& needs_more)
```

它和 inline 处理函数参数类似。

区别是：

```text
inline 按 \n 切行
RESP 按 RespParser::parse_command 切 frame
```

---

# 13. RESP 循环解析

```cpp
while (!pending.empty() && pending.front() == '*') {
    const auto parsed = RespParser::parse_command(pending);
```

只要 pending 以 `*` 开头，就尝试解析一个 RESP 命令。

这里会支持 pipeline。

比如 pending 里一次有多个 RESP 请求：

```text
*3\r\n...\r\n*2\r\n...\r\n*1\r\n...\r\n
```

每次解析一个，执行一个，然后通过 `consumed` 移除已处理部分。

---

# 14. RESP incomplete

```cpp
if (parsed.status == RespParseStatus::incomplete) {
    needs_more = true;
    return true;
}
```

如果 parser 返回 incomplete，说明请求还没收完整。

例如：

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmi
```

这时 TCP server 不能关闭连接。

也不能继续 while 空转。

它应该：

```text
设置 needs_more = true
返回外层
等待下一次 recv
```

这就是 `needs_more` 参数存在的原因。

---

# 15. RESP error

```cpp
if (parsed.status == RespParseStatus::error) {
    send_all(socket, "-ERR " + parsed.error + "\r\n");
    return false;
}
```

如果 parser 返回 error，说明协议格式错误。

服务端返回 RESP error：

```text
-ERR ...
```

然后关闭连接。

比如客户端发：

```text
*1\r\n+PING\r\n
```

第七版 parser 会说：

```text
RESP command arguments must be bulk strings
```

第八版 TCP server 返回：

```text
-ERR RESP command arguments must be bulk strings\r\n
```

---

# 16. RESP complete 后执行命令

```cpp
const auto result = processor.execute(RespParser::to_inline_command(parsed.command));
```

parser 返回 complete 后，先用：

```cpp
RespParser::to_inline_command(parsed.command)
```

把参数列表转成 inline 命令。

例如：

```text
["SET", "resp", "v8-wire"]
```

变成：

```text
SET resp v8-wire
```

然后交给现有：

```cpp
CommandProcessor
```

这样第八版不用重写所有命令逻辑。

---

# 17. RESP 响应编码

```cpp
if (!result.response.empty() && !send_all(socket, RespParser::to_resp_response(result.response))) {
    return false;
}
```

RESP 请求不能返回旧的：

```text
OK inserted\n
```

它要返回 RESP frame。

所以第八版新增：

```cpp
RespParser::to_resp_response
```

把 `CommandProcessor` 的文本响应转换成 RESP 响应。

例如：

```text
OK inserted -> +OK inserted\r\n
v8-wire     -> $7\r\nv8-wire\r\n
1           -> :1\r\n
(nil)       -> $-1\r\n
```

---

# 18. RESP 消费 pending

```cpp
pending.erase(0, parsed.command.consumed);
```

第七版 parser 返回：

```cpp
consumed
```

第八版在这里真正用上它。

每处理完一个 RESP 命令，就从 pending 里删掉已消费字节。

如果 pending 后面还有 pipeline 的第二个命令，while 会继续解析。

---

# 19. RESP QUIT

```cpp
if (result.should_close) {
    return false;
}
```

RESP 请求也可以发送：

```text
["QUIT"]
```

`CommandProcessor` 会返回：

```text
BYE
should_close = true
```

第八版会先发送：

```text
+BYE\r\n
```

然后关闭连接。

---

# 20. `serve_client` 的主循环

第八版 `serve_client` 仍然是每个客户端一个线程。

核心结构：

```cpp
while (true) {
    recv(...)
    pending.append(...)

    while (!pending.empty()) {
        bool needs_more = false;
        const bool ok = pending.front() == '*'
            ? process_resp_commands(...)
            : process_inline_commands(...);
        ...
    }
}
```

也就是说：

```text
收数据
放进 pending
根据 pending 开头选择协议处理器
如果一批数据里有多个命令，就尽量处理完
如果命令不完整，就等下一次 recv
```

---

# 21. 外层为什么看 pending.front()

```cpp
pending.front() == '*'
    ? process_resp_commands(...)
    : process_inline_commands(...)
```

RESP array 请求以：

```text
*
```

开头。

inline 命令一般是：

```text
SET
GET
DEL
...
```

所以这个判断可以自动选择协议。

当前第八版没有单独的协议协商。

它使用的是：

```text
按请求首字节识别
```

---

# 22. needs_more 控制外层循环

```cpp
if (pending.empty() || needs_more) {
    break;
}
```

如果 pending 空了，说明这一批数据处理完了。

如果 needs_more 为 true，说明当前命令不完整。

这两种情况都要跳出内层 while。

然后回到外层 `recv` 等更多数据。

---

# 23. 请求长度限制

```cpp
if (pending.size() > 64 * 1024) {
    if (!pending.empty() && pending.front() == '*') {
        send_all(client.get(), "-ERR request too long\r\n");
    } else {
        send_all(client.get(), "ERR line too long\n");
    }
    return;
}
```

第八版保留 64 KiB 的 pending 限制。

如果 RESP 请求太长，返回：

```text
-ERR request too long\r\n
```

如果 inline 行太长，返回：

```text
ERR line too long\n
```

这仍然保持：

```text
RESP 请求 -> RESP 响应
inline 请求 -> inline 响应
```

---

# 24. 第八版协议共存的效果

第八版以后，TCP server 可以这样用。

inline：

```text
SET inline v8-ok\n
GET inline\n
QUIT\n
```

响应：

```text
OK inserted\n
v8-ok\n
BYE\n
```

RESP：

```text
*3\r\n$3\r\nSET\r\n$4\r\nresp\r\n$7\r\nv8-wire\r\n
*2\r\n$3\r\nGET\r\n$4\r\nresp\r\n
*1\r\n$4\r\nQUIT\r\n
```

响应：

```text
+OK inserted\r\n
$7\r\nv8-wire\r\n
+BYE\r\n
```

---

# 25. 当前实现的边界

第八版已经能跑 RESP 请求，但还不是完整 Redis 兼容。

当前边界包括：

```text
只支持 RESP array-of-bulk-strings 请求
响应格式是根据 CommandProcessor 文本结果推断的
还没有专门的 RESP PING / INFO / COMMAND 等 Redis 命令
还没有认证、选择数据库、事务、pub/sub 等 Redis 能力
错误分类还比较粗
```

但是作为第八版，它已经完成了最关键一步：

```text
TCP server 能接收 RESP 请求并返回 RESP frame
```

---

# 第十八次总结

第八版 TCP server 的核心变化是：

```text
pending.front() == '*'
 -> RESP 模式
 -> RespParser::parse_command
 -> CommandProcessor
 -> RespParser::to_resp_response

否则
 -> inline 模式
 -> 按 \n 切行
 -> CommandProcessor
 -> 文本响应
```

一句话总结：

```text
第八版把 RESP parser 从“可单独测试的协议模块”接成了“TCP server 可用的网络协议能力”，同时保留旧 inline 协议，让项目从自定义文本服务进一步靠近 Redis 风格服务。
```
