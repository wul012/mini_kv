# 第六十四次讲解：第三十一版测试、README、归档和整体增删改

第六十三次讲了第三十一版客户端行编辑核心代码。
这次讲第三十一版配套内容：

```text
CMakeLists.txt
README.md
tests/line_editor_tests.cpp
a/31/解释/说明.md
a/31/图片/
代码讲解记录/README.md
```

它们负责说明和证明：

```text
当前项目版本进入 0.31.0
line_editor 模块进入 minikv 核心库
minikv_client 已接入 read_client_line
新增 minikv_line_editor_tests
全量 CTest 从 11 个测试增长到 12 个测试
真实 server + redirected client smoke 仍然通过
README 已说明交互行编辑和非交互 fallback
关键命令输出已按 a/31 归档
```

---

# 1. 第三十一版的定位

第三十版做的是：

```text
optional periodic server metrics logs
```

第三十一版做的是：

```text
bundled TCP client line editing
```

也就是从服务端观测能力，切回客户端体验能力。

它解决的是：

```text
客户端交互时打错难改
已有历史命令不能用方向键快速调出
终端使用体验不像常规命令行工具
```

同时它保留了：

```text
stdin 重定向
管道输入
自动化 smoke test
```

这一点很重要。
行编辑增强不能破坏脚本能力。

---

# 2. CMakeLists.txt 版本号更新

第三十一版把版本号改为：

```cmake
project(mini_kv VERSION 0.31.0 LANGUAGES CXX)
```

核心库新增源文件：

```cmake
add_library(minikv
    src/client_history.cpp
    src/command.cpp
    src/line_editor.cpp
    src/resp.cpp
    src/snapshot.cpp
    src/store.cpp
    src/tcp_server.cpp
    src/wal.cpp
)
```

也就是说 `line_editor` 不是只编进客户端 executable，而是进入 `minikv` 库。

这样做的好处是：

```text
minikv_client 可以复用
minikv_line_editor_tests 也可以链接同一个库测试
后续如果 CLI 入口也想复用行编辑模块，可以直接接入
```

---

# 3. 新增 minikv_line_editor_tests 目标

CMake 新增：

```cmake
add_executable(minikv_line_editor_tests
    tests/line_editor_tests.cpp
)

target_link_libraries(minikv_line_editor_tests PRIVATE minikv)

add_test(NAME line_editor_tests COMMAND minikv_line_editor_tests)
```

这让 CTest 中新增一个测试：

```text
line_editor_tests
```

第三十版全量 CTest 是：

```text
11/11
```

第三十一版变成：

```text
12/12
```

这说明 line editor 不是只靠手动体验验证，而是核心逻辑进入自动化测试。

---

# 4. tests/line_editor_tests.cpp 的定位

`line_editor_tests` 不测试真实键盘。

它测试的是纯逻辑：

```text
LineEditorBuffer
 -> 文本和光标编辑

LineEditorHistoryNavigator
 -> Up / Down 风格历史浏览
```

为什么不在单元测试里模拟真实方向键？

```text
真实终端依赖操作系统、控制台模式和输入设备
单元测试里直接测 raw terminal 容易不稳定
```

所以 v31 分两层验证：

```text
单元测试
 -> 测可确定的编辑缓冲区和历史导航逻辑

真实 smoke
 -> 测 client 在重定向 stdin 下仍能跑通
```

交互按键本身留给真实终端使用验证。

---

# 5. LineEditorBuffer 测试插入和光标

测试开头：

```cpp
minikv::LineEditorBuffer buffer;
assert(buffer.empty());
assert(buffer.cursor() == 0);
```

证明初始状态是：

```text
文本为空
光标在 0
```

然后：

```cpp
buffer.insert('S');
buffer.insert('E');
buffer.insert('T');
assert(buffer.text() == "SET");
assert(buffer.cursor() == 3);
```

证明连续插入普通字符后：

```text
text = SET
cursor = 行尾
```

---

# 6. 测试光标中间插入

继续：

```cpp
assert(buffer.move_left());
buffer.insert('X');
assert(buffer.text() == "SEXT");
assert(buffer.cursor() == 3);
```

过程是：

```text
原始：SET|
左移：SE|T
插入 X：SEX|T
```

所以结果是：

```text
SEXT
cursor = 3
```

这证明 `insert` 不是简单追加，而是按照光标位置插入。

---

# 7. 测试 Backspace 和 Delete

Backspace：

```cpp
assert(buffer.erase_before_cursor());
assert(buffer.text() == "SET");
assert(buffer.cursor() == 2);
```

从：

```text
SEX|T
```

删除光标前的 `X`，回到：

```text
SE|T
```

Delete：

```cpp
assert(buffer.erase_at_cursor());
assert(buffer.text() == "SE");
assert(buffer.cursor() == 2);
```

从：

```text
SE|T
```

删除光标处的 `T`，得到：

```text
SE|
```

这两个断言证明 Backspace 和 Delete 的语义区分正确。

---

# 8. 测试 Home / End 和边界移动

测试 Home：

```cpp
buffer.move_home();
assert(buffer.cursor() == 0);
assert(!buffer.move_left());
```

说明：

```text
Home 后光标在开头
开头继续左移失败
```

然后：

```cpp
buffer.insert('G');
assert(buffer.text() == "GSE");
assert(buffer.cursor() == 1);
```

证明开头插入生效。

测试 End：

```cpp
buffer.move_end();
assert(buffer.cursor() == buffer.text().size());
assert(!buffer.move_right());
```

说明：

```text
End 后光标在行尾
行尾继续右移失败
```

---

# 9. 测试 clear 和 set_text

清空：

```cpp
buffer.clear();
assert(buffer.empty());
```

设置整行：

```cpp
buffer.set_text("PING one");
assert(buffer.text() == "PING one");
assert(buffer.cursor() == buffer.text().size());
```

这对应真实场景：

```text
用户按 Up 调出历史命令
 -> set_text(history_command)
 -> 光标放到行尾
```

---

# 10. 测试历史导航 previous

构造历史：

```cpp
const std::vector<std::string> entries{"PING one", "SET name mini-kv", "GET name"};
minikv::LineEditorHistoryNavigator navigator{entries};
```

第一次 previous：

```cpp
auto value = navigator.previous("draft command");
assert(value == std::optional<std::string>{"GET name"});
```

这证明：

```text
第一次 Up 返回最近一条历史
并保存 draft command
```

继续 previous：

```cpp
value = navigator.previous("ignored");
assert(value == std::optional<std::string>{"SET name mini-kv"});
value = navigator.previous("ignored");
assert(value == std::optional<std::string>{"PING one"});
```

再继续：

```cpp
value = navigator.previous("ignored");
assert(value == std::optional<std::string>{"PING one"});
```

说明到最老历史后不会越界。

---

# 11. 测试历史导航 next 和 draft

从最老历史开始往下：

```cpp
value = navigator.next();
assert(value == std::optional<std::string>{"SET name mini-kv"});
value = navigator.next();
assert(value == std::optional<std::string>{"GET name"});
```

继续 Down：

```cpp
value = navigator.next();
assert(value == std::optional<std::string>{"draft command"});
```

说明能回到第一次按 Up 前保存的草稿。

再继续 Down：

```cpp
value = navigator.next();
assert(value == std::optional<std::string>{"draft command"});
```

说明到草稿后继续 Down 不会越界，也不会丢草稿。

---

# 12. 测试空历史

最后：

```cpp
const std::vector<std::string> empty_entries;
minikv::LineEditorHistoryNavigator empty_navigator{empty_entries};
assert(!empty_navigator.previous("draft").has_value());
assert(!empty_navigator.next().has_value());
```

这验证空历史下：

```text
Up 没有效果
Down 也没有效果
```

这能避免真实交互中空历史按方向键导致越界。

---

# 13. README 当前版本说明

README 当前版本从：

```text
Version 30 is a runnable in-memory KV service with optional periodic server metrics logs:
```

改成：

```text
Version 31 is a runnable in-memory KV service with bundled TCP client line editing:
```

关键词是：

```text
bundled TCP client line editing
```

这说明 v31 的重点不是服务端，不是存储格式，也不是 WAL。
重点是客户端交互体验。

---

# 14. README 能力列表新增 line editor

能力列表更新了测试覆盖说明：

```text
CTest coverage for store, command, WAL, snapshot, stress, RESP, TCP server, client history, and line editor behavior
```

同时新增：

```text
Bundled TCP client terminal line editing with arrow-key history navigation and cursor editing
```

这句话覆盖两类能力：

```text
arrow-key history navigation
 -> Up / Down 浏览历史

cursor editing
 -> Left / Right / Home / End / Backspace / Delete
```

---

# 15. README 运行说明新增交互行为

在 TCP client local history commands 后面新增说明：

```text
When the bundled TCP client runs in an interactive terminal, it supports Up/Down history navigation, Left/Right cursor movement, Home/End, Backspace, and Delete. When stdin is redirected or piped, it keeps the original line-by-line input behavior.
```

这段很关键，因为它同时告诉用户：

```text
交互终端能编辑
脚本输入不受影响
```

后面 TCP client options 段也补充了：

```text
On an interactive terminal, the bundled client edits the current line locally with arrow keys, Home/End, Backspace, and Delete. Up/Down walk the same in-memory or loaded history that powers :history, !!, and !N. Redirected stdin still uses normal line-by-line reading for scripts and smoke tests.
```

这里明确了 Up/Down 使用的是同一套历史：

```text
ClientHistory entries
```

而不是另一份独立历史。

---

# 16. README 测试目标列表更新

CLion 可运行目标列表新增：

```text
minikv_line_editor_tests
```

测试说明段落新增：

```text
line_editor_tests verifies the line editing buffer operations and Up/Down history navigation used by the interactive bundled TCP client.
```

这让用户知道：

```text
行编辑不是只能手测
核心编辑逻辑可以直接跑测试
```

---

# 17. README Roadmap 更新

v30 的 Roadmap 是：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add command execution counters and error counters.
```

v31 完成行编辑后，Roadmap 改成：

```text
1. Add command execution counters and error counters.
2. Add interactive command completion for the bundled TCP client.
```

这说明当前客户端体验下一步可以从：

```text
编辑当前行
```

继续推进到：

```text
命令补全
```

比如未来可以补：

```text
PING / SET / GET / DEL / EXPIRE / TTL / SIZE / SAVE / LOAD / COMPACT / WALINFO / STATS / HEALTH / HELP / QUIT
```

---

# 18. a/31 归档目录

第三十一版新增归档：

```text
a/31/图片/
a/31/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-line-editor-tests.png
05-client-line-editor-smoke.png
06-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

继续符合项目规则：

```text
关键命令输出保存为 PNG
解释文档说明每张截图对应命令、结果和意义
```

---

# 19. 01-cmake-configure.png

对应命令：

```text
cmake -S . -B cmake-build-debug
```

结果：

```text
配置成功
构建文件写入 cmake-build-debug
```

意义：

```text
CMake 能识别 0.31.0
新增 line_editor 源文件、头文件和测试目标后，工程仍可生成
```

---

# 20. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
干净构建通过
```

意义：

```text
src/line_editor.cpp
include/minikv/line_editor.hpp
src/client_main.cpp 接入
tests/line_editor_tests.cpp
```

都可以完整编译链接。

---

# 21. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
12 个测试全部通过
```

意义：

```text
v31 新增 line_editor_tests
同时没有破坏已有 Store / Command / WAL / Snapshot / RESP / TCP server / TCP RESP / client history 测试
```

这是 v31 最重要的回归信号。

---

# 22. 04-line-editor-tests.png

对应命令：

```text
.\cmake-build-debug\minikv_line_editor_tests.exe
```

结果：

```text
退出码 0
```

重点覆盖：

```text
插入字符
光标左右移动
Backspace 删除光标前字符
Delete 删除光标处字符
Home / End 移动到首尾
Up / Down 历史导航
浏览历史后回到当前未发送草稿
空历史下方向键无效果
```

---

# 23. 05-client-line-editor-smoke.png

这张图对应真实 server/client smoke。

流程是：

```text
启动真实 minikv_server.exe
启动真实 minikv_client.exe
client stdin 使用重定向输入文件
client 使用 --history-file
```

输入命令：

```text
PING
SET lineedit ok
GET lineedit
:history
QUIT
```

关键结果：

```text
PONG
OK inserted
ok
1  PING
2  SET lineedit ok
3  GET lineedit
BYE
```

历史文件保存：

```text
PING
SET lineedit ok
GET lineedit
QUIT
```

意义是：

```text
stdin 被重定向时，read_client_line 会走 fallback_read_line
客户端脚本化输入仍然可用
历史命令和 --history-file 仍然正常
```

---

# 24. 06-version-docs-check.png

这张图是版本和文档一致性检查。

检查项包括：

```text
git diff --check
CMakeLists.txt 版本为 0.31.0
README 当前版本为 Version 31
README 包含 terminal line editing
line_editor.hpp 包含 LineEditorBuffer
line_editor.cpp 包含 read_client_line
client_main.cpp 调用 read_client_line
CMakeLists.txt 构建 src/line_editor.cpp
CMakeLists.txt 注册 line_editor_tests
line_editor_tests.cpp 覆盖 LineEditorHistoryNavigator
```

结果：

```text
全部 PASS
```

这类检查能防止：

```text
版本号没升级
README 没同步
测试目标没注册
新增源码没进 CMake
文档说有功能但代码没接入
```

---

# 25. a/31/解释/说明.md 的内容

说明文档记录了：

```text
本轮版本目标
v31 重点验证项
每张截图对应命令
每条命令的结果
每条命令的意义
真实 smoke 的输入流程
本轮 server 进程已停止
tmp-v31-run 最终会删除
```

以后回看 v31 时，可以直接知道：

```text
这版为什么做
跑了哪些命令
每张截图证明了什么
临时文件和进程是否清理
```

---

# 26. 第三十一版整体增删改

第三十一版新增：

```text
include/minikv/line_editor.hpp
 -> LineEditorBuffer
 -> LineEditorHistoryNavigator
 -> read_client_line

src/line_editor.cpp
 -> 行编辑缓冲区实现
 -> 历史导航实现
 -> Windows / Unix-like 按键读取
 -> 终端重绘
 -> 非交互 fallback

tests/line_editor_tests.cpp
 -> 编辑缓冲区测试
 -> 历史导航测试

a/31/图片/
 -> v31 关键命令截图

a/31/解释/说明.md
 -> v31 运行调试归档说明
```

第三十一版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.31.0
 -> minikv 库新增 src/line_editor.cpp
 -> 新增 minikv_line_editor_tests
 -> CTest 注册 line_editor_tests

src/client_main.cpp
 -> include minikv/line_editor.hpp
 -> 主循环用 read_client_line 读取输入
 -> 传入 history.entries()

README.md
 -> 当前版本更新为 Version 31
 -> 能力列表新增 terminal line editing
 -> CLion target 列表新增 minikv_line_editor_tests
 -> TCP client 说明新增交互终端按键能力和非交互 fallback
 -> 测试说明新增 line_editor_tests
 -> Roadmap 移除 line editing，新增 command completion
```

第三十一版删除：

```text
无源码文件删除
无功能删除
```

---

# 27. 第三十一版之后当前能力总览

到第三十一版为止，mini-kv 已经具备：

```text
内存 key-value 存储
TTL
CLI
TCP server
TCP client
TCP client line editing
TCP client local history
TCP client persistent history
RESP-over-TCP
WAL append / replay
WAL2 checksum
WAL repair
manual COMPACT
WALINFO
startup WAL stats
auto compact
configurable compact thresholds
compact counters
STATS
HEALTH
TCP connection stats through commands
periodic server metrics logs
Snapshot
Benchmark
Stress tests
GitHub Actions CI
运行归档
代码讲解记录
```

客户端体验链路现在是：

```text
read_client_line
 -> 交互终端下行编辑
 -> 非交互下 getline fallback

ClientHistory
 -> :history / !! / !N
 -> --history-file

client_main
 -> 把 line_editor 得到的一行交给 resolve_client_input
 -> 再发给 server
```

---

# 28. 第三十一版之后适合继续做什么

运行观测方向：

```text
命令执行计数
命令错误计数
按命令类型统计成功 / 失败
把 counters 接入 STATS / HEALTH / server_metrics
```

客户端体验方向：

```text
命令补全
历史搜索
更好的错误提示
```

工程质量方向：

```text
格式化检查
Sanitizer CI
更细粒度 benchmark 输出
```

v31 完成后，最自然的下一步是：

```text
command execution counters and error counters
```

因为它能继续增强运行期观测，并和 v29 / v30 的 `STATS`、`HEALTH`、`server_metrics` 连起来。

---

# 29. 第三十一版清理结论

本轮运行后清理结果是：

```text
tmp-v31-run 已删除
client line editor smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/31/图片/
归档说明保留在 a/31/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第六十四次总结

第三十一版配套工作证明了三件事：

```text
line_editor_tests
 -> 行编辑核心逻辑和历史导航逻辑可自动化验证

真实 server/client smoke
 -> read_client_line 在 stdin 重定向时能回退到 getline，不破坏脚本化客户端输入

README 和 a/31 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第三十一版把 bundled TCP client 的交互体验推进到可编辑、可用方向键浏览历史的阶段，并用单元测试、全量 CTest、真实 client smoke、README 和 a/31 归档证明新输入链路可用且不破坏非交互脚本输入。
```
