# 第十九次讲解：第八版响应格式、客户端调整、测试和归档

第十八次讲了第八版 TCP server 如何识别 inline / RESP，并把 RESP 请求交给 `CommandProcessor`。

这次讲第八版剩下的配套内容：

```text
RespParser::to_resp_response
src/client_main.cpp
src/server_main.cpp
tests/resp_tests.cpp
CMakeLists.txt
README.md
a/8/解释/说明.md
a/8/图片/
```

第八版不是只让 TCP server 能“读 RESP”。

它还要做到：

```text
RESP 请求返回 RESP 响应
旧 client 仍然能用
文档说清楚第八版协议状态
运行归档证明 inline 和 RESP 都跑通
```

---

# 1. 为什么需要 `to_resp_response`

`CommandProcessor` 返回的是项目内部文本响应。

例如：

```text
OK inserted
mini-kv
1
(nil)
ERR unknown command
BYE
```

inline TCP 协议可以直接把这些响应加上：

```text
\n
```

发回去。

但 RESP 客户端需要的是 RESP frame。

例如：

```text
+OK inserted\r\n
$7\r\nmini-kv\r\n
:1\r\n
$-1\r\n
-ERR unknown command\r\n
```

所以第八版新增：

```cpp
RespParser::to_resp_response
```

它负责把内部文本响应转换成 RESP 响应。

---

# 2. `starts_with`

`src/resp.cpp` 新增辅助函数：

```cpp
bool starts_with(std::string_view text, std::string_view prefix) {
    return text.size() >= prefix.size() && text.substr(0, prefix.size()) == prefix;
}
```

它用来判断响应前缀。

比如：

```text
OK inserted
ERR unknown command
```

后面 `to_resp_response` 会用它判断：

```text
ERR 开头 -> RESP error
OK 开头  -> RESP simple string
```

---

# 3. `is_integer_text`

```cpp
bool is_integer_text(std::string_view text) {
    if (text.empty()) {
        return false;
    }

    if (text.front() == '-') {
        text.remove_prefix(1);
    }

    if (text.empty()) {
        return false;
    }

    for (const char ch : text) {
        if (std::isdigit(static_cast<unsigned char>(ch)) == 0) {
            return false;
        }
    }

    return true;
}
```

这个函数判断一个文本是不是整数。

它支持：

```text
1
0
-1
-2
```

不支持：

```text
abc
1a
-
空字符串
```

RESP 里整数响应用：

```text
:数字\r\n
```

比如：

```text
:1\r\n
:-2\r\n
```

mini-kv 里很多命令返回整数文本：

```text
DEL -> 1 / 0
SIZE -> 3
TTL -> -2 / -1 / 剩余秒数
```

所以需要把这些转换成 RESP integer。

---

# 4. `bulk_string`

```cpp
std::string bulk_string(std::string_view value) {
    std::string response = "$" + std::to_string(value.size()) + "\r\n";
    response += value;
    response += "\r\n";
    return response;
}
```

普通字符串 value 用 RESP bulk string 返回。

例如：

```text
mini-kv
```

转换成：

```text
$7\r\nmini-kv\r\n
```

bulk string 的好处是：

```text
可以包含空格
可以包含任意字节
客户端按长度读取
```

第八版主要用于 `GET` 返回的 value。

---

# 5. `to_resp_response` 空响应

```cpp
if (response.empty()) {
    return {};
}
```

如果 `CommandProcessor` 返回空响应，就不发任何内容。

当前空响应主要来自：

```text
空命令
```

比如用户只发空行。

RESP 请求正常不会变成空命令，因为 parser 要求数组长度大于 0。

但函数仍然保留这个防御。

---

# 6. `(nil)` 转 null bulk string

```cpp
if (response == "(nil)") {
    return "$-1\r\n";
}
```

`CommandProcessor` 的 `GET` 在 key 不存在时返回：

```text
(nil)
```

RESP 里空值常用：

```text
$-1\r\n
```

表示 null bulk string。

所以：

```text
(nil) -> $-1\r\n
```

---

# 7. `ERR ` 转 RESP error

```cpp
if (starts_with(response, "ERR ")) {
    return "-" + std::string{response} + "\r\n";
}
```

mini-kv 的错误响应以：

```text
ERR
```

开头。

RESP error 以：

```text
-
```

开头。

所以：

```text
ERR unknown command
```

转换成：

```text
-ERR unknown command\r\n
```

---

# 8. 整数文本转 RESP integer

```cpp
if (is_integer_text(response)) {
    return ":" + std::string{response} + "\r\n";
}
```

例如：

```text
1 -> :1\r\n
0 -> :0\r\n
-2 -> :-2\r\n
```

这覆盖：

```text
DEL
SIZE
TTL
EXPIRE
```

等返回数字的命令。

---

# 9. OK / BYE 转 simple string

```cpp
if (starts_with(response, "OK ") || response == "BYE") {
    return "+" + std::string{response} + "\r\n";
}
```

RESP simple string 以：

```text
+
```

开头。

第八版把这些响应作为 simple string：

```text
OK inserted
OK updated
OK saved 1
OK loaded 1
BYE
```

例如：

```text
OK inserted -> +OK inserted\r\n
BYE         -> +BYE\r\n
```

---

# 10. 其他响应转 bulk string

```cpp
return bulk_string(response);
```

如果不是 nil、错误、整数、OK、BYE，就按普通 value 处理。

典型例子是：

```text
GET name -> mini-kv
GET phrase -> hello from resp wire
HELP -> Commands:\n...
```

这些用 bulk string 可以保留完整内容。

---

# 11. resp_tests 新增响应格式断言

第八版在 `tests/resp_tests.cpp` 里新增了这些断言：

```cpp
assert(minikv::RespParser::to_resp_response("OK inserted") == "+OK inserted\r\n");
assert(minikv::RespParser::to_resp_response("mini-kv") == "$7\r\nmini-kv\r\n");
assert(minikv::RespParser::to_resp_response("(nil)") == "$-1\r\n");
assert(minikv::RespParser::to_resp_response("2") == ":2\r\n");
assert(minikv::RespParser::to_resp_response("-2") == ":-2\r\n");
assert(minikv::RespParser::to_resp_response("ERR unknown command") == "-ERR unknown command\r\n");
```

它们覆盖了第八版响应转换的主要分支：

```text
OK simple string
普通 value bulk string
nil null bulk string
正整数 integer
负整数 integer
错误 error
```

---

# 12. bundled client 为什么要改

第八版修改了：

```text
src/client_main.cpp
```

以前 client 连接后会先读服务端问候语：

```cpp
std::string greeting;
if (!read_line(socket.get(), greeting)) {
    std::cerr << "server closed connection before greeting\n";
    return 1;
}

std::cout << greeting << '\n';
```

这是因为旧 server 一连接就发：

```text
mini-kv ready
```

---

第八版 server 为了兼容 RESP，去掉了连接问候语。

所以 client 也必须改。

否则 client 会卡在：

```text
等待 greeting
```

服务端又在等待客户端发命令。

双方互等，连接就无法继续。

---

# 13. client 新连接提示

第八版 client 改成：

```cpp
NetworkRuntime runtime;
SocketGuard socket = connect_to_server(host, port);
std::cout << "connected to mini-kv at " << host << ':' << port << '\n';
```

也就是说：

```text
连接成功后，客户端自己打印提示
不再等待服务端先发东西
```

这样：

```text
inline client 可继续使用
RESP 客户端也不会被问候语干扰
```

---

# 14. server_main 提示更新

第八版把服务端启动提示从：

```text
Protocol: one command per line. Try: SET name mini-kv
```

改成：

```text
Protocol: inline text or RESP array requests. Try: SET name mini-kv
```

这只是文案变化。

它提醒用户：

```text
现在 TCP server 支持两种协议输入
```

---

# 15. CMake 版本更新

第八版项目版本改成：

```cmake
project(mini_kv VERSION 0.8.0 LANGUAGES CXX)
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
0.8.0 = TCP RESP integration
```

---

# 16. README 当前版本更新

第八版 README 当前版本变成：

```text
Version 8 is a runnable in-memory KV service with TCP RESP protocol support:
```

功能列表里把：

```text
TCP server with one-command-per-line protocol
```

更新成：

```text
TCP server with inline text and RESP request support
```

也把 RESP 描述从 parser 升级为：

```text
Redis RESP parser and TCP response formatting
```

---

# 17. README TCP protocol 章节更新

第八版说明 inline 用法仍然存在：

```text
Connect with a TCP client, then send one inline command per line:
```

然后新增 RESP 请求示例：

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
*2\r\n$3\r\nGET\r\n$4\r\nname\r\n
```

并说明：

```text
RESP mode returns simple strings, integers, bulk strings, null bulk strings, or errors using RESP framing.
Inline mode keeps the original one-line text responses.
```

这句话把协议边界说清楚了。

---

# 18. README RESP protocol 章节更新

第七版章节叫：

```text
RESP parser
```

第八版更新成：

```text
RESP protocol
```

因为 parser 已经接进 TCP server。

README 说明：

```text
The server auto-detects RESP requests when a buffered request starts with `*`;
otherwise it keeps using the inline text protocol.
```

这对应 `tcp_server.cpp` 里的：

```cpp
pending.front() == '*'
```

---

# 19. Roadmap 更新

第七版 Roadmap 是：

```text
1. Wire the RESP parser into the TCP server.
2. Add RESP response formatting and protocol hardening.
```

第八版完成 TCP 接入和基础响应格式后，Roadmap 变成：

```text
1. Add broader RESP compatibility tests and protocol hardening.
2. Add graceful shutdown and structured logging.
```

也就是说：

```text
RESP 接入已完成第一版
下一步是兼容性、健壮性和服务治理
```

---

# 20. 第八版归档 `a/8`

第八版按项目规则新增：

```text
a/8/图片/
a/8/解释/说明.md
```

截图包括：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-inline-tcp-smoke.png
05-resp-tcp-smoke.png
```

说明文档记录：

```text
第八版做了什么
每张截图对应什么命令
配置、构建、测试、inline TCP smoke、RESP TCP smoke 的结果
服务端进程是否停止
是否有临时文件需要清理
```

---

# 21. `01-cmake-configure.png`

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
第八版更新后的 CMake 版本和源码都能被识别
```

---

# 22. `02-cmake-build.png`

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
RESP response formatting、TCP 协议自动识别、client 去问候语依赖等改动能成功编译链接
```

---

# 23. `03-ctest.png`

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
第八版改动没有破坏原有功能、压力测试和 RESP parser 测试
```

---

# 24. `04-inline-tcp-smoke.png`

验证流程：

```text
启动 minikv_server.exe 6398
用 minikv_client.exe 127.0.0.1 6398 连接
发送 inline 命令
```

输入：

```text
SET inline v8-ok
GET inline
QUIT
```

输出：

```text
OK inserted
v8-ok
BYE
```

意义：

```text
去掉服务端问候语后，旧 inline TCP 协议和 bundled client 仍然可用
```

---

# 25. `05-resp-tcp-smoke.png`

验证流程：

```text
启动 minikv_server.exe 6398
用 Python socket 发送原始 RESP 字节
```

发送的是 RESP pipeline：

```text
SET resp v8-wire
GET resp
SIZE
QUIT
```

输出：

```text
+OK inserted\r\n
$7\r\nv8-wire\r\n
:1\r\n
+BYE\r\n
```

意义：

```text
TCP server 已经能接收 RESP 请求，并返回 RESP simple string、bulk string、integer 和关闭响应
```

---

# 26. 为什么 smoke test 用 Python socket

第八版验证时试过 PowerShell `TcpClient` 发送 RESP 字节。

在当前环境里它偶发：

```text
拒绝访问
```

而 Python `socket` 可以稳定发送原始字节。

所以最终归档采用：

```text
Python socket
```

这和项目逻辑无关，只是本机验证工具选择。

---

# 27. 第八版文件变化总表

第八版相关增删改可以整理成：

```text
新增：
  a/8/图片/
  a/8/解释/说明.md

修改：
  include/minikv/resp.hpp
    新增 to_resp_response 声明

  src/resp.cpp
    新增 RESP 响应格式化逻辑

  src/tcp_server.cpp
    自动识别 inline / RESP
    新增 process_inline_commands
    新增 process_resp_commands
    RESP 请求返回 RESP 响应
    去掉连接问候语

  src/client_main.cpp
    不再等待服务端问候语
    连接成功后由客户端自己打印 connected 提示

  src/server_main.cpp
    更新协议提示文案

  tests/resp_tests.cpp
    新增 to_resp_response 断言

  CMakeLists.txt
    版本 0.8.0

  README.md
    Version 8
    TCP RESP 协议说明
    RESP protocol 章节
    Roadmap 更新

删除：
  无源码文件删除
```

---

# 28. 第八版当前能力总览

到第八版为止，mini-kv 已经具备：

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
CTest 自动化覆盖
运行归档
```

协议层现在是：

```text
inline 文本协议
 -> CLI / bundled client / TCP server 可用

RESP 协议
 -> TCP server 可识别 RESP 请求
 -> 返回 RESP frame
 -> 支持基本 pipeline
```

---

# 第十九次总结

第八版不仅接入了 RESP parser，还补上了完整配套：

```text
to_resp_response
 -> 把内部文本响应编码成 RESP frame

client_main.cpp
 -> 去掉对服务端问候语的依赖

README.md
 -> 说明 inline 和 RESP 共存

a/8
 -> 用 inline TCP smoke 和 RESP TCP smoke 证明两种协议都能跑
```

一句话总结：

```text
第八版让 mini-kv 的 TCP server 从“只会自定义文本协议”升级为“同时支持 inline 和 RESP 的网络服务”，并用运行归档证明旧客户端和 RESP 字节流都能正常工作。
```
