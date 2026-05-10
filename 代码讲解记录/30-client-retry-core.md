# 第三十次讲解：第十四版客户端连接重试核心

第二十九次讲了第十三版 README、CMake、`a/13` 归档和整体增删改。

这次进入第十四版核心代码：

```text
src/client_main.cpp
```

第十四版主要做三件事：

```text
第一，给 bundled TCP client 增加连接重试参数
第二，保持旧的 host / port / timeout_ms 用法兼容
第三，让连接失败日志保留真实 socket 错误码
```

第十三版补的是网络兼容测试。
第十四版继续往客户端可用性推进：

```text
服务端还没启动
客户端可以先启动并等待
服务端上线后客户端自动连上
```

---

# 1. 为什么要做客户端连接重试

第十二版给客户端加了：

```text
timeout_ms
```

它解决的是：

```text
连接成功之后
send / recv 不能无限等
```

但还有另一个常见场景：

```text
客户端启动得比服务端更早
```

比如：

```text
脚本同时启动 server 和 client
服务端还在初始化
客户端先 connect
connect 失败
```

第十四版补的就是这个场景：

```text
connect 失败后不要马上结束
按用户配置重试几次
```

这不是复杂的自动重连系统，只是启动连接阶段的重试。

---

# 2. 新增 thread 头文件

第十四版在 `src/client_main.cpp` 新增：

```cpp
#include <thread>
```

原因是重试之间需要等待：

```cpp
std::this_thread::sleep_for(options.retry_delay);
```

这个等待发生在客户端连接阶段。
它的含义是：

```text
本次 connect 失败
等 retry_delay
再尝试下一次 connect
```

---

# 3. ClientOptions 统一客户端配置

第十四版新增：

```cpp
struct ClientOptions {
    std::string host = "127.0.0.1";
    std::uint16_t port = 6379;
    std::optional<std::chrono::milliseconds> timeout;
    int connect_retries = 0;
    std::chrono::milliseconds retry_delay{250};
};
```

第十二版之前，`main` 里直接散着几个变量：

```text
host
port
timeout
```

第十四版继续新增：

```text
connect_retries
retry_delay
```

如果还把这些都散在 `main` 里，后面会越来越乱。

所以第十四版把客户端运行配置收进一个结构体：

```text
ClientOptions
```

默认值也集中放在这里：

```text
host = 127.0.0.1
port = 6379
connect_retries = 0
retry_delay = 250ms
```

其中：

```text
connect_retries = 0
```

表示默认不重试，保持旧行为。

---

# 4. UsageError 区分用法错误

第十四版新增：

```cpp
class UsageError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};
```

它的作用是区分两类错误：

```text
用户参数写错
 -> UsageError
 -> 打印 usage
 -> 返回 2

运行时失败
 -> std::exception
 -> 打印 fatal
 -> 返回 1
```

比如这些属于用法错误：

```text
--connect-retries 后面没值
--retry-delay-ms 后面没值
未知 --xxx 参数
位置参数太多
```

而这些属于运行时失败：

```text
getaddrinfo 失败
connect 最终失败
setsockopt 失败
server closed connection
```

把错误类型分开后，客户端行为更清楚。

---

# 5. parse_nonnegative_int

第十四版新增：

```cpp
int parse_nonnegative_int(const char* text, std::string_view name)
```

它用于解析：

```text
--connect-retries count
```

为什么是 nonnegative？

因为重试次数允许为：

```text
0
```

含义是：

```text
不重试，只尝试初始连接一次
```

但不允许：

```text
-1
abc
10abc
超出 int 范围的值
```

函数内部检查：

```text
不能为空
不能以 '-' 开头
必须完整解析
不能超过 int 最大值
```

所以 `--connect-retries` 的语义是稳定的。

---

# 6. retry-delay-ms 继续用正整数毫秒

第十四版没有给 `--retry-delay-ms` 单独写解析函数。

它复用第十二版已有的：

```cpp
parse_positive_milliseconds(argv[index], "retry-delay-ms");
```

这说明：

```text
retry-delay-ms 必须大于 0
```

也就是不接受：

```text
0
-1
abc
```

为什么不允许 0？

因为 0ms 重试会变成很紧的循环。
对一个本地小工具来说，明确要求正数更稳。

默认值是：

```text
250ms
```

既不会太慢，也不会在服务端没启动时疯狂重试。

---

# 7. connect_to_server 保存 last_connect_error

第十四版对 `connect_to_server` 做了一个细节修正：

```cpp
std::string last_connect_error;
```

每次 `connect` 失败后马上保存：

```cpp
last_connect_error = socket_error_message("connect failed");
```

为什么要马上保存？

因为 socket 错误码是一个易变状态。
尤其在 Windows 上，如果后续又执行了其他 socket API，最后再读错误码，可能拿不到真正的 connect 失败原因。

第十四版真实 smoke 里能看到：

```text
connect failed: WSA error 10061
```

`10061` 表示连接被拒绝。

这比第十四版调试中一开始出现过的：

```text
WSA error 0
```

有用得多。

---

# 8. connect_to_server 最终抛出保存的错误

循环结束后：

```cpp
if (last_connect_error.empty()) {
    last_connect_error = socket_error_message("connect failed");
}
throw std::runtime_error{last_connect_error};
```

这里有两种情况：

```text
有候选地址，并且 connect 失败
 -> last_connect_error 已经记录真实错误

没有成功创建/尝试任何 socket
 -> last_connect_error 为空
 -> 用 socket_error_message 补一个通用错误
```

这样 `connect_to_server` 对外仍然只做一件事：

```text
成功返回 SocketGuard
失败抛 std::runtime_error
```

但错误信息更可靠。

---

# 9. connect_with_retries 的角色

第十四版新增核心函数：

```cpp
SocketGuard connect_with_retries(const ClientOptions& options)
```

它包在 `connect_to_server` 外面。

职责分工是：

```text
connect_to_server
 -> 做一次连接尝试
 -> 成功返回 socket
 -> 失败抛异常

connect_with_retries
 -> 决定要尝试多少次
 -> 失败时打印日志
 -> 失败后 sleep
 -> 最后仍失败就抛总结错误
```

这样单次连接和重试策略没有混在一起。

---

# 10. total_attempts 的含义

重试函数里先算：

```cpp
const int total_attempts = options.connect_retries + 1;
```

这里有一个容易混淆的点：

```text
connect_retries
 -> 失败后的额外重试次数

total_attempts
 -> 初始尝试 + 额外重试
```

所以如果用户传：

```text
--connect-retries 20
```

真实最多尝试：

```text
21 次
```

日志里就会出现：

```text
connect attempt 1/21 failed
```

这比只写“retry 1”更清楚。

---

# 11. 重试循环

核心循环是：

```cpp
for (int attempt = 1; attempt <= total_attempts; ++attempt) {
    try {
        return connect_to_server(options.host, options.port, options.timeout);
    } catch (const std::exception& error) {
        ...
    }
}
```

如果连接成功：

```text
立即 return socket
```

如果连接失败：

```text
保存 last_error
如果还有机会，就打印日志并等待
如果没有机会，就跳出循环
```

最后抛：

```cpp
throw std::runtime_error{"connect failed after " + std::to_string(total_attempts) +
                         " attempt(s): " + last_error};
```

这条错误会告诉用户：

```text
总共试了多少次
最后一次失败原因是什么
```

---

# 12. 重试日志

第十四版连接失败但还会继续重试时，会输出到 stderr：

```text
connect attempt 1/21 failed: connect failed: WSA error 10061
retrying in 200 ms
```

这里的字段含义：

```text
1/21
 -> 当前是第 1 次，总共最多 21 次

WSA error 10061
 -> 当前连接失败原因

200 ms
 -> 下一次重试前等待多久
```

这些日志放在 stderr。
原因是：

```text
它们是运行诊断信息
不是服务端响应
```

正常连接成功后的交互仍然走 stdout。

---

# 13. print_usage 更新

第十四版 usage 变成：

```cpp
Usage: minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
```

它保留了前三个位置参数：

```text
host
port
timeout_ms
```

新增两个 flag：

```text
--connect-retries count
--retry-delay-ms ms
```

这样旧命令仍然可用：

```text
minikv_client.exe 127.0.0.1 6379 5000
```

新命令也可用：

```text
minikv_client.exe 127.0.0.1 6379 5000 --connect-retries 10 --retry-delay-ms 250
```

---

# 14. parse_options 统一参数解析

第十四版新增：

```cpp
ClientOptions parse_options(int argc, char** argv)
```

它把 `main` 里原来的参数解析移出来。

解析规则是：

```text
遇到 --connect-retries
 -> 读取下一个参数
 -> parse_nonnegative_int
 -> 写入 options.connect_retries

遇到 --retry-delay-ms
 -> 读取下一个参数
 -> parse_positive_milliseconds
 -> 写入 options.retry_delay

遇到其他 '-' 开头参数
 -> UsageError unknown option

非 '-' 开头参数
 -> 依次作为 host / port / timeout_ms
```

这个结构和第十二版 `server_main` 的 flag 解析风格保持一致。

---

# 15. parse_options 保持旧用法

位置参数仍然按老顺序：

```text
第 1 个位置参数 -> host
第 2 个位置参数 -> port
第 3 个位置参数 -> timeout_ms
```

所以这些旧命令都还能用：

```text
minikv_client.exe
minikv_client.exe 127.0.0.1
minikv_client.exe 127.0.0.1 6379
minikv_client.exe 127.0.0.1 6379 5000
```

第十四版新增的是后面的可选 flag。

这点很重要：

```text
新功能不能让旧教程和旧脚本突然失效
```

---

# 16. main 变得更短

第十四版 `main` 开头变成：

```cpp
const ClientOptions options = parse_options(argc, argv);

NetworkRuntime runtime;
SocketGuard socket = connect_with_retries(options);
```

这比以前更清楚：

```text
先解析所有配置
再初始化网络运行时
再按配置连接服务端
```

`main` 不再关心：

```text
怎么解析 --connect-retries
怎么循环重试
怎么记录失败原因
```

这些都被拆到小函数里。

---

# 17. 连接成功后的提示

连接成功后仍然输出：

```text
connected to mini-kv at 127.0.0.1:6415
```

如果设置了 timeout：

```text
socket timeout: 2000 ms
```

第十四版新增：

```text
connect retries: 20, retry delay: 200 ms
```

这个提示只在：

```text
options.connect_retries > 0
```

时输出。

所以默认旧用法不会多出这行。

---

# 18. catch 顺序

第十四版最后的异常处理顺序是：

```cpp
} catch (const UsageError& error) {
    ...
    return 2;
} catch (const std::exception& error) {
    ...
    return 1;
}
```

`UsageError` 必须放在 `std::exception` 前面。

原因是：

```text
UsageError 继承自 std::runtime_error
std::runtime_error 又是 std::exception
```

如果先 catch `std::exception`，`UsageError` 就会被提前截住，usage 就不会打印。

第十四版调试时专门修了这个顺序。

---

# 19. 第十四版客户端链路总结

参数链路：

```text
命令行参数
 -> parse_options
 -> ClientOptions
 -> connect_with_retries
```

单次连接链路：

```text
connect_to_server
 -> getaddrinfo(AF_UNSPEC)
 -> 遍历候选地址
 -> socket
 -> set_socket_timeout
 -> connect
 -> 成功返回 SocketGuard
```

重试链路：

```text
connect_with_retries
 -> connect_to_server 失败
 -> 记录 last_error
 -> 打印 connect attempt current/total
 -> sleep retry_delay
 -> 再次 connect_to_server
 -> 成功后进入正常交互
```

---

# 第三十次总结

第十四版把客户端从“连接失败就结束”推进到“可以按配置等待服务端上线”。

一句话总结：

```text
第十四版通过 ClientOptions、parse_options 和 connect_with_retries，把 bundled TCP client 的启动连接阶段做成可配置重试，同时保持旧参数兼容和真实错误码可观察。
```
