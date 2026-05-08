# 第五次讲解：测试、构建和说明文件

前面已经讲完了核心运行代码：

```text
Store
CommandProcessor
main.cpp
server_main.cpp
tcp_server.cpp
```

剩下这些文件不直接参与“每条命令怎么执行”，但它们负责：

```text
证明代码是否正确
告诉 CMake 怎么编译
告诉使用者怎么运行项目
告诉 AI 助手怎么归档运行结果
```

这次讲解剩余文件：

```text
tests/store_tests.cpp
tests/command_tests.cpp
CMakeLists.txt
README.md
AGENTS.md
.gitignore
解释代码格式说明
```

---

# 1. `tests/store_tests.cpp`：测试 Store 是否正确

这个文件专门测试：

```cpp
minikv::Store
```

也就是内存数据库。

它的作用是验证：

```text
Store 初始状态是否为空
set 是否能插入
set 是否能更新
get 是否能读取
erase 是否能删除
snapshot 是否能稳定返回数据
clear 是否能清空
空 key 是否被拒绝
```

可以把它理解成：

```text
store_tests.cpp = Store 的自检程序
```

测试从这里开始：

```cpp
int main() {
    minikv::Store store;
```

它创建一个空数据库。

然后检查初始状态：

```cpp
assert(store.size() == 0);
assert(!store.contains("name"));
assert(!store.get("name").has_value());
```

意思是：

```text
刚创建的 Store 应该没有任何 key
name 不应该存在
get("name") 应该返回空 optional
```

接着测试插入：

```cpp
assert(store.set("name", "mini-kv"));
assert(store.size() == 1);
assert(store.contains("name"));
assert(store.get("name") == std::optional<std::string>{"mini-kv"});
```

这里验证：

```text
set 新 key 应该返回 true
数据库大小变成 1
name 已经存在
get("name") 能读到 mini-kv
```

然后测试更新：

```cpp
assert(!store.set("name", "redis-ish"));
assert(store.size() == 1);
assert(store.get("name") == std::optional<std::string>{"redis-ish"});
```

因为 `name` 已经存在，所以再次 `set` 是更新，不是插入。

所以返回值应该是：

```text
false
```

数据库大小仍然是：

```text
1
```

但是 value 已经从：

```text
mini-kv
```

变成：

```text
redis-ish
```

然后测试快照：

```cpp
assert(store.set("lang", "cpp20"));
const auto snapshot = store.snapshot();
assert(snapshot.size() == 2);
assert(snapshot[0].first == "lang");
assert(snapshot[1].first == "name");
```

这里插入第二个 key：

```text
lang -> cpp20
```

然后调用 `snapshot()`。

因为 `snapshot()` 内部会按 key 排序，所以返回顺序应该是：

```text
lang
name
```

这证明 `snapshot()` 不是随便返回 `unordered_map` 的无序结果，而是返回稳定排序后的数据。

然后测试删除：

```cpp
assert(store.erase("name"));
assert(!store.erase("name"));
assert(store.size() == 1);
```

第一次删除 `name`，应该成功。

第二次再删 `name`，因为已经不存在，所以应该失败。

数据库里还剩：

```text
lang
```

所以 size 是 1。

然后测试清空：

```cpp
store.clear();
assert(store.size() == 0);
```

清空后数据库大小应该回到 0。

最后测试空 key：

```cpp
assert(!store.set("", "bad"));
assert(store.size() == 0);
```

空字符串不能作为 key。

所以 `set("", "bad")` 应该失败，数据库大小仍然是 0。

一句话总结：`store_tests.cpp` 用断言验证 `Store` 的基本读写、删除、清空、快照和边界行为。

---

# 2. `tests/command_tests.cpp`：测试命令处理是否正确

这个文件测试：

```cpp
minikv::CommandProcessor
```

也就是命令解析器。

它的作用是验证：

```text
用户输入一行命令后
CommandProcessor 返回的文本是否正确
是否正确调用 Store
是否正确判断退出
是否正确处理错误命令
```

可以把它理解成：

```text
command_tests.cpp = CommandProcessor 的自检程序
```

测试开始：

```cpp
minikv::Store store;
minikv::CommandProcessor processor{store};
```

这里和 CLI、TCP Server 一样，都是：

```text
先创建 Store
再创建 CommandProcessor
```

先测试空命令：

```cpp
auto result = processor.execute("");
assert(result.response.empty());
assert(!result.should_close);
```

空字符串不应该报错，也不应该退出。

结果应该是：

```text
response = ""
should_close = false
```

测试 `SET`：

```cpp
result = processor.execute("SET name mini-kv");
assert(result.response == "OK inserted");
assert(!result.should_close);
```

第一次插入 `name`，所以返回：

```text
OK inserted
```

并且不退出。

测试 `GET`：

```cpp
result = processor.execute("GET name");
assert(result.response == "mini-kv");
```

刚才已经设置了：

```text
name -> mini-kv
```

所以现在应该能读出来。

测试小写命令和更新：

```cpp
result = processor.execute("set name redis-ish");
assert(result.response == "OK updated");
```

这里命令是小写：

```text
set
```

但 `CommandProcessor` 会转成大写。

所以它仍然识别为：

```text
SET
```

而且 `name` 已经存在，所以返回：

```text
OK updated
```

再次读取：

```cpp
result = processor.execute("GET name");
assert(result.response == "redis-ish");
```

证明 value 已经被更新。

测试 `SIZE`：

```cpp
result = processor.execute("SIZE");
assert(result.response == "1");
```

现在只有一个 key：

```text
name
```

所以 size 是 1。

测试 `DEL`：

```cpp
result = processor.execute("DEL name");
assert(result.response == "1");
```

删除存在的 key，返回：

```text
1
```

然后再读：

```cpp
result = processor.execute("GET name");
assert(result.response == "(nil)");
```

因为已经删除，所以返回：

```text
(nil)
```

测试错误用法：

```cpp
result = processor.execute("GET name extra");
assert(result.response == "ERR usage: GET key");
```

`GET` 只允许一个参数。

这里多了：

```text
extra
```

所以应该返回用法错误。

测试未知命令：

```cpp
result = processor.execute("UNKNOWN");
assert(result.response == "ERR unknown command");
```

`UNKNOWN` 不在支持命令列表里，所以返回未知命令错误。

测试退出命令：

```cpp
result = processor.execute("QUIT");
assert(result.response == "BYE");
assert(result.should_close);
```

`QUIT` 应该返回：

```text
BYE
```

并且：

```text
should_close = true
```

这样 CLI 或 TCP Server 才知道应该关闭当前会话。

一句话总结：`command_tests.cpp` 验证命令解析层是否正确连接了文本命令和 Store 操作。

---

# 3. `CMakeLists.txt`：告诉 CMake 怎么编译项目

这个文件是项目的构建脚本。

它的作用是告诉 CMake：

```text
项目叫什么
使用什么 C++ 标准
哪些 .cpp 编译成库
哪些 .cpp 编译成可执行程序
测试程序有哪些
Windows 下需要链接什么系统库
```

可以把它理解成：

```text
CMakeLists.txt = 项目的编译说明书
```

开头：

```cmake
cmake_minimum_required(VERSION 3.20)
project(mini_kv VERSION 0.1.0 LANGUAGES CXX)
```

意思是：

```text
最低需要 CMake 3.20
项目名叫 mini_kv
版本是 0.1.0
语言是 C++
```

设置 C++ 标准：

```cmake
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

意思是：

```text
使用 C++20
必须满足 C++20
不启用编译器私有扩展
```

这个项目里用了 C++20 特性，比如：

```cpp
std::ranges::sort
```

所以需要 C++20。

定义核心库：

```cmake
add_library(minikv
    src/command.cpp
    src/store.cpp
    src/tcp_server.cpp
)
```

这里把三个核心实现文件编译成一个库：

```text
minikv
```

这三个文件分别对应：

```text
command.cpp     命令解析
store.cpp       数据存储
tcp_server.cpp  网络服务
```

设置头文件路径：

```cmake
target_include_directories(minikv
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
```

这样代码里才能写：

```cpp
#include "minikv/command.hpp"
```

而不需要写很长的相对路径。

设置编译警告：

```cmake
if(MSVC)
    target_compile_options(minikv PRIVATE /W4 /permissive-)
else()
    target_compile_options(minikv PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

如果是 Visual Studio 编译器，就用：

```text
/W4 /permissive-
```

如果是 GCC/Clang，就用：

```text
-Wall -Wextra -Wpedantic
```

这些选项会让编译器更严格，帮助发现潜在问题。

Windows 下链接 Winsock：

```cmake
if(WIN32)
    target_link_libraries(minikv PUBLIC ws2_32)
endif()
```

因为 `tcp_server.cpp` 在 Windows 下用了 Winsock API：

```cpp
WSAStartup
socket
bind
listen
accept
send
recv
```

所以必须链接：

```text
ws2_32
```

否则会出现链接错误。

定义 CLI 程序：

```cmake
add_executable(minikv_cli
    src/main.cpp
)

target_link_libraries(minikv_cli PRIVATE minikv)
```

这会生成命令行版本程序：

```text
minikv_cli
```

它只需要编译 `main.cpp`，然后链接前面定义好的 `minikv` 核心库。

定义服务端程序：

```cmake
add_executable(minikv_server
    src/server_main.cpp
)

target_link_libraries(minikv_server PRIVATE minikv)
```

这会生成 TCP 服务端程序：

```text
minikv_server
```

它使用 `server_main.cpp` 作为入口，也链接同一个 `minikv` 核心库。

启用测试：

```cmake
enable_testing()
```

这行开启 CTest 支持。

定义 Store 测试：

```cmake
add_executable(minikv_store_tests
    tests/store_tests.cpp
)

target_link_libraries(minikv_store_tests PRIVATE minikv)

add_test(NAME store_tests COMMAND minikv_store_tests)
```

这里做了三件事：

```text
生成 minikv_store_tests 测试程序
让它链接 minikv 核心库
把它注册成 CTest 测试，名字叫 store_tests
```

定义 Command 测试：

```cmake
add_executable(minikv_command_tests
    tests/command_tests.cpp
)

target_link_libraries(minikv_command_tests PRIVATE minikv)

add_test(NAME command_tests COMMAND minikv_command_tests)
```

同理，这里注册了：

```text
command_tests
```

所以执行：

```powershell
ctest --test-dir build --output-on-failure
```

时，CTest 会运行：

```text
store_tests
command_tests
```

一句话总结：`CMakeLists.txt` 把项目拆成核心库、CLI 程序、TCP 服务端和两个测试目标。

---

# 4. `README.md`：给使用者看的项目说明

`README.md` 是项目说明文档。

它不是给编译器看的，而是给人看的。

它说明：

```text
这个项目是什么
当前版本有什么功能
怎么构建
怎么运行 CLI
怎么运行 TCP Server
支持哪些命令
TCP 协议是什么
后续计划是什么
```

可以把它理解成：

```text
README.md = 项目使用手册
```

它说明当前版本是：

```text
Version 1 is a runnable in-memory KV service
```

也就是：

```text
第一版是一个可运行的内存键值服务
```

当前功能包括：

```text
CMake 项目结构
线程安全内存 KV Store
交互式命令行客户端
TCP 服务端
Store 和 Command 的基础测试
```

构建命令是：

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

含义是：

```text
cmake -S . -B build
 -> 根据当前目录源码生成 build 构建目录

cmake --build build
 -> 编译项目

ctest --test-dir build --output-on-failure
 -> 运行测试，失败时输出详细信息
```

CLI 运行方式：

```powershell
.\build\Debug\minikv_cli.exe
```

TCP 服务端运行方式：

```powershell
.\build\Debug\minikv_server.exe 6379
```

README 也提醒：

```text
如果是 single-config generator，可执行文件可能直接在 build 目录下
```

这和 CMake 生成器有关。

Visual Studio 这类多配置生成器通常会有：

```text
build/Debug/
build/Release/
```

而 Ninja、Makefile 这类单配置生成器可能直接输出到：

```text
build/
```

README 里列出了 CLI 命令：

```text
SET key value
GET key
DEL key
SIZE
HELP
EXIT
```

也列出了 TCP 协议：

```text
一行一个命令
```

比如：

```text
SET name mini-kv
GET name
SIZE
DEL name
QUIT
```

最后 Roadmap 写了后续方向：

```text
添加小客户端程序
添加 TTL
添加 WAL 持久化
添加快照
添加 benchmark 和 stress test
添加 Redis RESP parser
```

这说明当前版本只是第一版，还没有：

```text
数据持久化
过期时间
标准 Redis RESP 协议
性能压测
```

一句话总结：`README.md` 是给开发者和使用者看的入口说明。

---

# 5. `AGENTS.md`：给 AI 助手看的项目规则

`AGENTS.md` 是当前项目对 AI 助手的额外约束。

它要求：

```text
每次执行关键命令后，需要把重要结果归档
```

关键命令包括：

```text
配置
构建
测试
运行验证
服务端 smoke test
```

归档目录是：

```text
a/1/图片/
a/1/解释/说明.md
```

也就是说，如果后续运行：

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

或者启动服务端做 smoke test，都需要把命令结果截图保存到：

```text
a/1/图片/
```

并在：

```text
a/1/解释/说明.md
```

解释每张截图对应的命令、结果和意义。

如果启动了后台服务，还需要记录：

```text
服务是否已停止
是否有清理残留进程
```

一句话总结：`AGENTS.md` 是项目里的操作规程，主要约束调试和验证时的归档行为。

---

# 6. `.gitignore`：告诉 Git 忽略哪些文件

`.gitignore` 的作用是告诉 Git：

```text
哪些文件不需要提交到版本库
```

常见被忽略的内容包括：

```text
build/
cmake-build-debug/
临时文件
IDE 生成文件
编译产物
```

这个项目里能看到：

```text
cmake-build-debug/
```

通常就是 CLion 或 CMake 生成的构建目录。

这种目录可以重新生成，不应该作为源码提交。

一句话总结：`.gitignore` 用来避免把构建产物和临时文件提交进仓库。

---

# 7. `解释代码格式说明`：讲解风格参考

这个文件是讲解模板。

它的特点是：

```text
先说文件或类的角色
再给核心流程
然后逐段解释代码
最后一句话总结
```

前面几次讲解基本就是按这个风格来的。

比如讲 `Store` 时，用的是：

```text
Store = 一个带锁的 unordered_map
```

讲 `CommandProcessor` 时，用的是：

```text
CommandProcessor = 命令翻译器
```

讲 `TcpServer` 时，用的是：

```text
TcpServer = 网络版 main.cpp
```

这样方便把代码和实际职责联系起来。

一句话总结：`解释代码格式说明` 是当前代码讲解的格式参考，不是项目运行必需文件。

---

# 8. 整个项目最后串起来

现在所有文件可以串成一条完整链路：

```text
README.md
 -> 告诉用户怎么构建和运行

CMakeLists.txt
 -> 告诉 CMake 怎么编译项目

main.cpp
 -> 启动 CLI 模式

server_main.cpp
 -> 启动 TCP Server 模式

tcp_server.cpp
 -> 接收网络客户端命令

command.cpp
 -> 解析 SET / GET / DEL / SIZE / HELP / EXIT

store.cpp
 -> 真正读写内存数据

tests/
 -> 验证 Store 和 CommandProcessor 是否工作正确

AGENTS.md
 -> 约束调试运行结果如何归档
```

如果从用户操作角度看：

```text
用户运行 minikv_cli
 -> main.cpp 读取输入
 -> command.cpp 解析命令
 -> store.cpp 保存或读取数据
 -> main.cpp 打印结果
```

如果从网络服务角度看：

```text
用户运行 minikv_server
 -> server_main.cpp 创建服务
 -> tcp_server.cpp 监听端口
 -> 客户端发送命令
 -> command.cpp 解析命令
 -> store.cpp 保存或读取数据
 -> tcp_server.cpp 发回结果
```

如果从测试角度看：

```text
ctest
 -> 运行 store_tests
 -> 运行 command_tests
 -> 验证核心逻辑是否正确
```

---

# 第五次总结

这次讲解的是剩余辅助文件：

```text
tests/store_tests.cpp
tests/command_tests.cpp
CMakeLists.txt
README.md
AGENTS.md
.gitignore
解释代码格式说明
```

它们分别负责：

```text
测试核心逻辑
配置项目构建
说明项目用法
约束调试归档
忽略构建产物
提供讲解格式参考
```

一句话总结：

```text
前面的 src 和 include 是 mini-kv 的运行主体；tests、CMakeLists.txt、README.md 和 AGENTS.md 则负责保证项目能被正确构建、验证、使用和记录。
```
