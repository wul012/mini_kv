# 第九次讲解：第三版测试、文档和归档

第八次讲的是 TTL 的核心实现。

这次讲第三版剩下的配套改动：

```text
tests/store_tests.cpp
tests/command_tests.cpp
CMakeLists.txt
README.md
a/3/解释/说明.md
a/3/图片/
```

它们的作用不是新增核心算法，而是负责：

```text
证明 TTL 行为正确
告诉用户第三版怎么用
记录第三版调试运行结果
```

第三版从项目层面看，可以总结为：

```text
新增：
  EXPIRE key seconds
  TTL key
  a/3 第三版验证归档

修改：
  Store 测试增加 TTL 断言
  CommandProcessor 测试增加 TTL 命令断言
  CMake 版本 0.2.0 -> 0.3.0
  README 当前版本改为 Version 3
  README 命令列表和 Roadmap 更新

删除：
  没有删除已有源码文件
  没有删除第二版客户端
  没有删除已有命令
```

---

# 1. `tests/store_tests.cpp` 新增时间头文件

第三版测试文件新增：

```cpp
#include <chrono>
#include <thread>
```

`chrono` 用来写：

```cpp
1s
1100ms
```

`thread` 用来调用：

```cpp
std::this_thread::sleep_for(...)
```

因为 TTL 测试必须真的等时间过去。

比如：

```text
设置 1 秒过期
等待 1100 毫秒
再确认 key 已经过期
```

---

测试开头新增：

```cpp
using namespace std::chrono_literals;
```

这让代码可以直接写：

```cpp
1s
1100ms
```

否则就要写得更长：

```cpp
std::chrono::seconds{1}
std::chrono::milliseconds{1100}
```

---

# 2. Store 测试：缺失 key 的 TTL 行为

新增测试：

```cpp
assert(!store.expire("missing", 1s));
assert(!store.ttl("missing").has_value());
```

第一句：

```cpp
store.expire("missing", 1s)
```

意思是：

```text
给一个不存在的 key 设置 1 秒 TTL
```

它应该失败，所以断言：

```cpp
assert(!...)
```

第二句：

```cpp
store.ttl("missing")
```

查询不存在 key 的 TTL。

Store 层返回：

```text
std::nullopt
```

所以测试写：

```cpp
assert(!store.ttl("missing").has_value());
```

对应命令层就是：

```text
TTL missing -> -2
```

---

# 3. Store 测试：SET 会重置 TTL

新增测试：

```cpp
assert(store.set("reset", "v1"));
assert(store.expire("reset", 1s));
assert(!store.set("reset", "v2"));
assert(store.get("reset") == std::optional<std::string>{"v2"});
assert(store.ttl("reset") == std::optional<std::chrono::seconds>{-1s});
```

一步步看。

先插入：

```cpp
assert(store.set("reset", "v1"));
```

这是新 key，所以返回：

```text
true
```

然后设置 TTL：

```cpp
assert(store.expire("reset", 1s));
```

说明 `reset` 这个 key 现在 1 秒后过期。

---

接着重新 SET：

```cpp
assert(!store.set("reset", "v2"));
```

因为 `reset` 已经存在，所以这次是更新，不是插入。

所以返回：

```text
false
```

但是 value 应该已经变成：

```text
v2
```

所以测试：

```cpp
assert(store.get("reset") == std::optional<std::string>{"v2"});
```

---

最后验证 TTL 被清掉：

```cpp
assert(store.ttl("reset") == std::optional<std::chrono::seconds>{-1s});
```

`-1s` 在 Store 层表示：

```text
key 存在，但没有过期时间
```

这证明第三版规定的行为成立：

```text
SET 替换 value，并清除旧 TTL
```

---

# 4. Store 测试：真正等待过期

新增测试：

```cpp
store.clear();

assert(store.set("session", "open"));
assert(store.ttl("session") == std::optional<std::chrono::seconds>{-1s});
assert(store.expire("session", 1s));
```

先清空前面测试留下的数据。

然后插入：

```text
session -> open
```

刚插入时没有 TTL，所以：

```cpp
store.ttl("session")
```

应该返回：

```text
-1s
```

接着设置 1 秒 TTL：

```cpp
store.expire("session", 1s)
```

---

马上查询 TTL：

```cpp
const auto ttl = store.ttl("session");
assert(ttl.has_value());
assert(ttl->count() >= 0);
assert(ttl->count() <= 1);
```

这里不直接断言等于 1。

原因是：

```text
程序从设置 TTL 到查询 TTL 之间也会消耗一点时间
```

所以返回可能是：

```text
1
```

也可能接近边界时变成：

```text
0
```

因此测试只要求：

```text
有值
并且在 0 到 1 秒之间
```

---

真正等待：

```cpp
std::this_thread::sleep_for(1100ms);
```

等待 1100 毫秒。

为什么不是刚好 1000 毫秒？

因为系统调度、计时精度、测试运行环境都会有轻微误差。

多等 100 毫秒，可以让测试更稳定。

---

等待后断言：

```cpp
assert(!store.get("session").has_value());
assert(!store.contains("session"));
assert(!store.ttl("session").has_value());
assert(store.size() == 0);
```

意思是：

```text
GET 看不到 session
CONTAINS 认为 session 不存在
TTL 返回空
SIZE 不把 session 算进去
```

这证明过期 key 在所有主要读取路径上都被当成不存在。

---

# 5. `tests/command_tests.cpp` 新增头文件

命令测试同样新增：

```cpp
#include <chrono>
#include <string>
#include <thread>
```

`chrono` 和 `thread` 用来等待 TTL 过期。

`string` 用来使用：

```cpp
std::stoll(...)
```

把 `TTL` 命令返回的字符串转成数字。

---

# 6. Command 测试：TTL 没有过期时间返回 -1

新增测试：

```cpp
result = processor.execute("SET temp keep");
assert(result.response == "OK inserted");

result = processor.execute("TTL temp");
assert(result.response == "-1");
```

先插入：

```text
temp -> keep
```

此时没有设置过期时间。

所以：

```text
TTL temp
```

应该返回：

```text
-1
```

这对应 Redis 风格语义：

```text
key 存在，但没有过期时间
```

---

# 7. Command 测试：EXPIRE 成功返回 1

```cpp
result = processor.execute("EXPIRE temp 1");
assert(result.response == "1");
```

`temp` 存在，所以设置 1 秒 TTL 应该成功。

命令层返回：

```text
1
```

---

马上查询：

```cpp
result = processor.execute("TTL temp");
const long long remaining = std::stoll(result.response);
assert(remaining >= 0);
assert(remaining <= 1);
```

`result.response` 是字符串。

比如：

```text
"1"
```

所以用：

```cpp
std::stoll(result.response)
```

转成整数。

然后只断言它在：

```text
0 到 1
```

这个范围内。

原因和 Store 测试一样：

```text
时间测试不能要求精确等于某个瞬间值
```

---

# 8. Command 测试：过期后 GET 返回 nil

```cpp
std::this_thread::sleep_for(1100ms);

result = processor.execute("GET temp");
assert(result.response == "(nil)");
```

等待超过 1 秒后，`temp` 应该过期。

所以：

```text
GET temp
```

返回：

```text
(nil)
```

这证明命令层最终调用的 `Store::get()` 已经正确处理过期 key。

---

# 9. Command 测试：过期后 TTL 返回 -2

```cpp
result = processor.execute("TTL temp");
assert(result.response == "-2");
```

过期后的 key 对外等于不存在。

所以：

```text
TTL temp
```

返回：

```text
-2
```

也就是：

```text
key 不存在
```

---

# 10. Command 测试：不存在 key 的 EXPIRE 返回 0

```cpp
result = processor.execute("EXPIRE temp 1");
assert(result.response == "0");
```

前面 `temp` 已经过期，并且在 `GET` 或 `TTL` 时被懒清理掉。

所以现在再设置过期时间，等价于：

```text
给不存在的 key 设置 TTL
```

结果应该是：

```text
0
```

---

# 11. Command 测试：非法 EXPIRE 用法

```cpp
result = processor.execute("EXPIRE temp 0");
assert(result.response == "ERR usage: EXPIRE key seconds");
```

第三版规定 `seconds` 必须是正整数。

所以：

```text
EXPIRE temp 0
```

不是合法命令。

返回：

```text
ERR usage: EXPIRE key seconds
```

这证明 `parse_positive_seconds()` 和命令用法检查生效。

---

# 12. Command 测试：TTL 多余参数报错

```cpp
result = processor.execute("TTL temp extra");
assert(result.response == "ERR usage: TTL key");
```

`TTL` 只接受一个参数：

```text
TTL key
```

多出来的：

```text
extra
```

应该被识别为用法错误。

返回：

```text
ERR usage: TTL key
```

---

# 13. `CMakeLists.txt` 版本更新

第三版把项目版本从：

```cmake
project(mini_kv VERSION 0.2.0 LANGUAGES CXX)
```

改成：

```cmake
project(mini_kv VERSION 0.3.0 LANGUAGES CXX)
```

含义是：

```text
0.1.0 = 第一版内存 KV 服务
0.2.0 = 第二版新增 TCP 客户端
0.3.0 = 第三版新增 TTL
```

这次没有新增 CMake target。

因为 TTL 是核心库 `minikv` 内部能力。

已有目标都会自动获得这个能力：

```text
minikv_cli
minikv_server
minikv_client
minikv_store_tests
minikv_command_tests
```

---

# 14. `README.md` 当前版本更新

README 当前版本从第二版：

```text
Version 2 is a runnable in-memory KV service with a TCP client:
```

改成第三版：

```text
Version 3 is a runnable in-memory KV service with TTL support:
```

这告诉用户：

```text
当前重点能力已经变成 TTL
```

功能列表新增：

```text
Expiring keys with `EXPIRE` and `TTL`
```

意思是：

```text
现在 key 可以设置过期时间
```

---

# 15. README 命令列表新增 TTL 命令

第三版命令列表新增：

```text
EXPIRE key seconds
TTL key
```

完整命令集合变成：

```text
SET key value
GET key
DEL key
EXPIRE key seconds
TTL key
SIZE
HELP
EXIT
```

这里 `QUIT` 虽然也支持，但 HELP 文本里仍然保留 `EXIT` 作为主要退出命令。

---

# 16. README TCP 示例新增 TTL 流程

TCP protocol 示例里新增：

```text
EXPIRE name 10
TTL name
```

完整示例变成：

```text
SET name mini-kv
GET name
EXPIRE name 10
TTL name
SIZE
DEL name
QUIT
```

这说明不管是 CLI 还是 TCP 协议，TTL 命令都是一样的文本命令。

原因是：

```text
CLI 和 TCP Server 最终都调用 CommandProcessor
```

---

# 17. README 增加 TTL 语义说明

第三版 README 新增：

```text
EXPIRE key seconds  Set a positive second-based expiration for an existing key.
TTL key             Return -2 when the key is missing, -1 when it has no expiration, or remaining seconds otherwise.
```

这段解释了两个关键点。

第一，`EXPIRE` 只给已经存在的 key 设置 TTL：

```text
key 不存在 -> 返回 0
```

第二，`TTL` 有三个返回语义：

```text
-2  key 不存在
-1  key 存在但没有过期时间
>=0 剩余秒数
```

README 还补充：

```text
SET replaces the value and clears any existing TTL for that key.
```

也就是：

```text
SET 会清除旧 TTL
```

---

# 18. README Roadmap 更新

第三版完成 TTL 后，Roadmap 从：

```text
Add TTL support
Add WAL persistence
Add snapshots
...
```

更新为：

```text
1. Add WAL persistence.
2. Add snapshots.
3. Add benchmarking and stress tests.
4. Add a Redis RESP parser.
```

也就是说：

```text
TTL 已经从待办项变成已完成能力
```

下一版最自然的方向变成：

```text
WAL persistence
```

也就是写前日志持久化。

---

# 19. 第三版归档 `a/3`

按照项目规则，第三版调试运行结果归档到：

```text
a/3/图片/
a/3/解释/说明.md
```

其中图片包括：

```text
01-cmake-configure.png
02-build-all-targets.png
03-run-ctest.png
04-cli-ttl-smoke-test.png
05-tcp-ttl-smoke-test.png
```

说明文档是：

```text
a/3/解释/说明.md
```

它记录了：

```text
第三版做了什么
每张截图对应什么命令
命令结果说明什么
服务端进程是否停止
临时文件是否清理
```

---

# 20. 第三版验证链路

第三版验证链路是：

```text
cmake configure
 -> clean build all targets
 -> ctest
 -> CLI TTL smoke test
 -> TCP TTL smoke test
```

CMake configure 证明：

```text
项目配置可生成
```

Clean build 证明：

```text
所有目标可以从干净状态重新编译链接
```

CTest 证明：

```text
Store 层和 CommandProcessor 层的 TTL 行为都通过断言
```

CLI smoke test 证明：

```text
用户在本地 CLI 能使用 EXPIRE / TTL
```

TCP smoke test 证明：

```text
通过网络连接 minikv_server 时，TTL 行为也正确
```

---

# 21. TCP TTL smoke test 的意义

第三版 TCP smoke test 发送：

```text
SET temp ttl-value
EXPIRE temp 1
TTL temp
等待 1200ms
GET temp
TTL temp
QUIT
```

关键结果是：

```text
EXPIRE temp 1 -> 1
TTL temp -> 1
GET temp -> (nil)
TTL temp -> -2
```

这说明：

```text
设置 TTL 成功
过期前 TTL 能查到剩余秒数
过期后 GET 看不到 key
过期后 TTL 把 key 当作不存在
```

而且测试结束后记录：

```text
server stopped
```

说明服务端后台进程没有残留。

---

# 22. 第三版文件变化总表

第三版相关增删改可以整理成：

```text
新增：
  a/3/图片/
  a/3/解释/说明.md

修改：
  include/minikv/store.hpp
    Store 增加 Entry、expire、ttl、过期判断和懒清理声明

  src/store.cpp
    实现带 TTL 的 set/get/erase/contains/size/snapshot
    实现 expire 和 ttl

  src/command.cpp
    新增 EXPIRE / TTL 命令解析
    HELP 文本加入新命令

  tests/store_tests.cpp
    增加 Store TTL 行为测试

  tests/command_tests.cpp
    增加命令层 TTL 行为测试

  CMakeLists.txt
    版本 0.2.0 -> 0.3.0

  README.md
    当前版本、命令列表、TTL 说明和 Roadmap 更新

删除：
  无源码文件删除
```

---

# 第九次总结

第三版除了核心 TTL 实现，还补齐了测试、说明和运行归档。

测试负责证明：

```text
TTL 语义正确
过期 key 会被当作不存在
SET 会清除旧 TTL
命令错误用法能被识别
```

README 负责告诉用户：

```text
第三版支持 EXPIRE / TTL
TTL 的 -2 / -1 / 剩余秒数分别是什么意思
下一步 Roadmap 是 WAL persistence
```

归档负责证明：

```text
第三版确实构建过、测试过、通过 CLI 和 TCP 真实运行验证过
```

一句话总结：

```text
第三版不是只把 TTL 写进代码里，还把 TTL 的测试、使用说明和调试运行证据一起补齐了。
```
