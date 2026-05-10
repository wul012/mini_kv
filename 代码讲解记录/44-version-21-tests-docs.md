# 第四十四次讲解：第二十一版测试、README、归档和整体增删改

第四十三次讲了第二十一版客户端持久化历史核心。

这次讲第二十一版配套内容：

```text
CMakeLists.txt
README.md
tests/client_history_tests.cpp
a/21/解释/说明.md
a/21/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.21.0
ClientHistory 文件加载和保存有自动化测试
真实 minikv_server + minikv_client 能跨会话复用历史文件
README 已经记录 --history-file 用法
关键命令输出已经按 a/21 归档
```

---

# 1. 第二十一版的定位

第二十版做的是工程自动化：

```text
GitHub Actions CI
Linux / macOS / Windows
CMake build
CTest
```

第二十一版回到用户体验：

```text
TCP client persistent command history
```

也就是让客户端历史从：

```text
当前进程内可用
```

推进到：

```text
指定历史文件后跨会话可用
```

---

# 2. CMakeLists.txt 版本号更新

第二十一版把版本号改成：

```cmake
project(mini_kv VERSION 0.21.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然复用已有测试目标：

```text
minikv_client_history_tests
```

所以构建目标数量不变。

---

# 3. 为什么没有新增测试 target

第二十一版主要扩展的是：

```text
ClientHistory
```

而第十五版已经有：

```text
tests/client_history_tests.cpp
```

这个测试 target 本来就负责：

```text
:history
!!
!N
history capacity
```

所以本版是在同一个测试文件里继续补：

```text
missing history file
save_to_file
load_from_file
parent directory creation
capacity trimming after load
CRLF line ending compatibility
```

不需要再建一个新测试 exe。

---

# 4. client_history_tests 原有覆盖

第十五版已有测试先验证：

```text
空历史时 :history 返回 history is empty
普通命令 PING one 会进入历史并发送
SET name mini-kv 会进入历史并发送
:history 能列出 1 / 2 两条历史
!! 能重复上一条命令
!1 能重复第一条命令
超过容量后丢掉最旧命令
!99 找不到时本地报错
!abc 当普通命令发送
ClientHistory{0} 会抛 invalid_argument
```

这些断言没有删除。

第二十一版是在这些基础上继续扩展。

---

# 5. 临时目录准备

新增测试先创建临时目录：

```cpp
const auto suffix = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
const auto temp_dir = std::filesystem::temp_directory_path() / ("minikv-client-history-tests-" + suffix);
std::filesystem::remove_all(temp_dir);
std::filesystem::create_directories(temp_dir);
```

这里用 `steady_clock` 生成后缀，是为了减少临时目录冲突。

目录位置在系统临时目录下。

测试结束会：

```cpp
std::filesystem::remove_all(temp_dir);
```

这样不会污染项目目录。

---

# 6. 缺失历史文件测试

第一段新增测试：

```cpp
minikv::ClientHistory missing_history{3};
assert(missing_history.load_from_file(temp_dir / "missing.history") == 0);
assert(missing_history.empty());
```

它验证：

```text
历史文件不存在
 -> load_from_file 返回 0
 -> history 仍然为空
 -> 不抛异常
```

这是第一次使用 `--history-file` 时最常见的场景。

如果这里设计成报错，用户第一次启动客户端就会失败。

---

# 7. 保存 bounded history 测试

接着创建容量为 3 的历史：

```cpp
minikv::ClientHistory persistent_history{3};
```

然后依次输入：

```text
PING one
SET name mini-kv
GET name
SIZE
```

因为容量是 3，四条命令进入历史后，最旧的会被裁剪。

最终应该保留：

```text
SET name mini-kv
GET name
SIZE
```

这证明保存文件时保存的是 bounded history，不是无限历史。

---

# 8. 父目录自动创建测试

保存路径是：

```cpp
const auto saved_file = temp_dir / "nested" / "client.history";
persistent_history.save_to_file(saved_file);
assert(std::filesystem::exists(saved_file));
```

这里特意把文件放在：

```text
nested/client.history
```

如果 `save_to_file()` 没有创建父目录，这一步会失败。

测试通过说明：

```text
--history-file data/client.history
```

即使 `data/` 不存在，也能自动创建。

---

# 9. 重新加载保存文件

然后：

```cpp
minikv::ClientHistory restored_history{5};
assert(restored_history.load_from_file(saved_file) == 3);
assert((restored_history.entries() ==
        std::vector<std::string>{"SET name mini-kv", "GET name", "SIZE"}));
```

这里验证两件事。

第一：

```text
load_from_file 返回 3
```

说明保存文件里有 3 条非空命令。

第二：

```text
entries 顺序正确
```

历史应该按照原顺序恢复：

```text
1  SET name mini-kv
2  GET name
3  SIZE
```

这样 `!1`、`!2`、`!3` 才能符合用户预期。

---

# 10. 手写历史文件测试

测试还手写了一个文件：

```cpp
const auto manual_file = temp_dir / "manual.history";
{
    std::ofstream manual{manual_file};
    manual << "PING old\n";
    manual << "\n";
    manual << "SET loaded value\r\n";
    manual << "GET loaded\n";
    manual << "SIZE\n";
}
```

这个文件故意包含：

```text
正常行
空行
CRLF 行
超过容量的多条命令
```

这样一次测试多个边界。

---

# 11. 空行和 CRLF 的预期

手写文件里有：

```text
PING old

SET loaded value\r\n
GET loaded
SIZE
```

加载时：

```text
空行跳过
SET loaded value 后面的 \r 去掉
```

所以非空命令数量是 4：

```text
PING old
SET loaded value
GET loaded
SIZE
```

测试写的是：

```cpp
assert(clipped_history.load_from_file(manual_file) == 4);
```

---

# 12. 加载时容量裁剪

`clipped_history` 容量是 2：

```cpp
minikv::ClientHistory clipped_history{2};
```

它加载 4 条命令后，只保留最近 2 条：

```cpp
assert((clipped_history.entries() == std::vector<std::string>{"GET loaded", "SIZE"}));
```

这证明 `load_from_file()` 没有绕过 `add()`。

它仍然遵守：

```text
超过 max_entries_
 -> 删除最旧记录
```

---

# 13. README 当前版本更新

README 当前版本从：

```text
Version 20 is a runnable in-memory KV service with cross-platform CMake and CTest CI:
```

改成：

```text
Version 21 is a runnable in-memory KV service with persistent TCP client command history:
```

关键词是：

```text
persistent TCP client command history
```

这说明第二十一版重点是客户端命令历史持久化。

---

# 14. README 能力列表新增项

能力列表新增：

```text
Optional bundled TCP client persistent history via --history-file
```

这里强调：

```text
Optional
```

说明这个功能是可选的。

不传 `--history-file` 时，客户端行为仍然和第十五版一样：

```text
只有当前会话内历史
不会读写历史文件
```

---

# 15. README Run 章节新增示例

新增运行示例：

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379 --history-file data\client.history
```

这个例子告诉用户：

```text
历史文件路径可以自己指定
```

比如：

```text
data\client.history
```

后续客户端会：

```text
启动时读这个文件
发送命令后写这个文件
```

---

# 16. README TCP client options 更新

客户端 options 从：

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
```

更新成：

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
                  [--history-file path]
```

并补充说明：

```text
When --history-file is set,
the client loads existing commands from that file at startup
and saves the bounded history after each successful sent command.
```

这里有两个关键词。

第一个是：

```text
at startup
```

启动时加载。

第二个是：

```text
after each successful sent command
```

成功发送并收到响应后保存。

---

# 17. README 测试说明更新

`client_history_tests` 的说明从：

```text
verifies the bundled client's local :history, !!, and !N session history behavior
```

扩展为：

```text
verifies the bundled client's local :history, !!, and !N behavior plus persistent history file load/save and capacity trimming
```

这让 README 的测试描述跟真实测试覆盖同步。

---

# 18. README Roadmap 调整

第二十版 Roadmap 第一项是：

```text
Add persistent client history or interactive line editing.
```

第二十一版完成了其中的：

```text
persistent client history
```

所以 Roadmap 调整为：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL checksums or atomic snapshot writes for stronger recovery guarantees.
```

含义是：

```text
持久化历史已经落地
客户端体验下一步可以继续做交互式行编辑
```

---

# 19. a/21 归档目录

第二十一版新增归档：

```text
a/21/图片/
a/21/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-client-history-tests.png
05-client-history-smoke.png
06-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 20. 01-cmake-configure.png

第一张图对应命令：

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
CMake 能识别 0.21.0
现有构建目标都能正常生成
```

---

# 21. 02-cmake-build.png

第二张图对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完整编译链接
38 个构建步骤成功
```

意义：

```text
std::filesystem 能编译链接
ClientHistory 文件读写能编译
client_main 新参数解析能编译
client_history_tests 新断言能编译
旧目标没有构建回归
```

---

# 22. 为什么还是 38 个构建步骤

第二十一版没有新增 target。

只是修改：

```text
include/minikv/client_history.hpp
src/client_history.cpp
src/client_main.cpp
tests/client_history_tests.cpp
CMakeLists.txt
README.md
```

所以构建步骤仍然是：

```text
38
```

---

# 23. 03-ctest.png

第三张图对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

测试列表仍是：

```text
store_tests
command_tests
wal_tests
snapshot_tests
stress_tests
resp_tests
tcp_server_tests
tcp_resp_tests
tcp_resp_compat_tests
tcp_resp_concurrency_tests
client_history_tests
```

意义：

```text
客户端持久化历史新增行为通过
前面二十版积累的功能没有回归
```

---

# 24. 04-client-history-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_client_history_tests.exe
```

结果：

```text
退出码 0
Result: PASS
```

这张图专门单独复跑客户端历史测试。

原因是第二十一版主要改动就在：

```text
ClientHistory
client_main history-file
```

---

# 25. client_history_tests 的新增意义

单独测试确认：

```text
缺失历史文件
 -> 返回 0，不报错

保存历史文件
 -> 能创建父目录
 -> 能写入 bounded history

加载历史文件
 -> 能恢复命令顺序
 -> 能跳过空行
 -> 能去掉 CRLF 的 \r
 -> 能按容量裁剪
```

这覆盖了文件历史最容易出错的边界。

---

# 26. 05-client-history-smoke.png

第五张图是真实进程 smoke。

它不是只跑单元测试，而是启动真实：

```text
minikv_server.exe
minikv_client.exe
```

流程是：

```text
启动 server
 -> 第一次 client 使用 --history-file
 -> 发送 PING first / SET smoke history / QUIT
 -> 生成历史文件
 -> 第二次 client 使用同一个 --history-file
 -> :history 查看旧命令
 -> !2 重复旧的 SET smoke history
 -> GET smoke 验证服务端值
 -> QUIT
```

---

# 27. 第一次 client 输出

第一次会话启动时显示：

```text
history file: ... (0 entries available, 0 loaded)
```

这说明：

```text
历史文件第一次使用
文件不存在或为空
加载 0 条
当前可用 0 条
```

然后输入：

```text
PING first
SET smoke history
QUIT
```

服务端返回：

```text
first
OK inserted
BYE
```

---

# 28. 第二次 client 输出

第二次会话启动时显示：

```text
history file: ... (3 entries available, 3 loaded)
```

三条历史是：

```text
PING first
SET smoke history
QUIT
```

然后 `:history` 输出：

```text
1  PING first
2  SET smoke history
3  QUIT
```

这证明旧会话历史已经加载进当前 `ClientHistory`。

---

# 29. !2 跨会话复用

第二次会话输入：

```text
!2
```

输出：

```text
repeating: SET smoke history
OK updated
```

这里 `!2` 引用的不是当前会话新输入的第二条命令。

它引用的是从历史文件加载出来的第 2 条：

```text
SET smoke history
```

这就是第二十一版最核心的用户价值。

---

# 30. GET smoke 验证执行效果

第二次会话继续输入：

```text
GET smoke
```

返回：

```text
history
```

这证明 `!2` 不只是打印了 repeating。

它确实把：

```text
SET smoke history
```

发送给了真实服务端，并更新了 Store。

---

# 31. smoke 保存后的历史文件

smoke 里保存后的历史文件是：

```text
PING first
SET smoke history
QUIT
SET smoke history
GET smoke
QUIT
```

前 3 条来自第一次会话。

后 3 条来自第二次会话。

其中第二次的 `SET smoke history` 是由：

```text
!2
```

解析出来并发送的命令。

---

# 32. smoke 检查项

smoke 最后检查：

```text
first session created history file: True
first session saved SET command: True
second session loaded previous entries: True
history command shows saved SET as entry 2: True
bang-index repeats saved SET command: True
GET smoke returns persisted server value: True
server accepted both clients: True
server stderr empty: True
```

最终：

```text
Result: PASS
```

这说明真实进程路径没有问题：

```text
server 启动正常
client 参数解析正常
history 文件读写正常
跨会话 !N 正常
server 没有 stderr
```

---

# 33. 06-version-docs-check.png

第六张图对应版本和文档一致性检查。

检查项包括：

```text
CMake version is 0.21.0
README current version is Version 21
README documents --history-file
README roadmap moved past persistent history
client_main parses --history-file
ClientHistory loads from file
ClientHistory saves to file
```

结果：

```text
Result: PASS
```

意义是：

```text
代码版本、README 描述、客户端参数和 ClientHistory 实现互相对齐
```

---

# 34. 本次代码增删改总览

第二十一版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.21.0

include/minikv/client_history.hpp
 -> 新增 <filesystem>
 -> 新增 load_from_file
 -> 新增 save_to_file

src/client_history.cpp
 -> 新增文件存在性检查
 -> 新增历史文件读取
 -> 新增 CRLF 尾部 \r 处理
 -> 新增空行跳过
 -> 新增父目录创建
 -> 新增历史文件覆盖保存

src/client_main.cpp
 -> ClientOptions 新增 history_file
 -> parse_options 新增 --history-file
 -> usage 新增 --history-file path
 -> 启动后加载历史文件
 -> 发送命令成功后保存历史文件

tests/client_history_tests.cpp
 -> 新增缺失文件加载验证
 -> 新增保存和重新加载验证
 -> 新增父目录创建验证
 -> 新增 CRLF 和空行验证
 -> 新增加载容量裁剪验证

README.md
 -> 当前版本说明更新为 Version 21
 -> 能力列表新增 --history-file
 -> Run 章节新增持久化历史示例
 -> TCP client options 新增 --history-file path
 -> 测试说明补充 persistent history file load/save
 -> Roadmap 移除 persistent client history
```

第二十一版新增：

```text
a/21/图片/01-cmake-configure.png
a/21/图片/02-cmake-build.png
a/21/图片/03-ctest.png
a/21/图片/04-client-history-tests.png
a/21/图片/05-client-history-smoke.png
a/21/图片/06-version-docs-check.png
a/21/解释/说明.md
代码讲解记录/43-client-history-persistence.md
代码讲解记录/44-version-21-tests-docs.md
```

第二十一版删除：

```text
无源码文件删除
```

---

# 35. 第二十一版之后当前能力总览

到第二十一版为止，mini-kv 已经具备：

```text
内存 key-value 存储
命令解析
CLI 模式
TCP server
TCP client
TTL 过期
WAL 增量持久化
WAL replay applied / skipped / truncated 报告
WAL 坏记录跳过
WAL 尾部疑似部分写入记录跳过
Snapshot 手动完整状态保存和加载
坏 Snapshot 不替换当前 Store
Benchmark 本地吞吐观察
Stress test 并发压力验证
RESP 请求解析器
TCP RESP 请求/响应
PING 探活命令
RESP parser 参数数量限制
RESP parser bulk string 长度限制
TcpServer 可请求停止
server_main 支持 Ctrl+C / SIGTERM 停止标记
结构化生命周期日志
并发连接事件下 stdout 日志按行输出
TCP connection_id
TCP active / total / peak connection metrics
TCP request byte limit
event=tcp_request_rejected 请求拒绝日志
server_main 可配置 max_request_bytes 和 accept_poll_ms
client_main 可配置 socket timeout
localhost / hostname 网络兼容性测试
client_main 可配置连接重试和重试间隔
client_main 本地会话历史 :history / !! / !N
client_main 可选 --history-file 跨会话持久化历史
外部客户端式 RESP-over-TCP pipeline 验证
RESP-over-TCP null bulk / integer / error / protocol error 验证
并发 RESP-over-TCP 客户端验证
CTest 自动化覆盖
GitHub Actions 跨平台 CI
运行归档
代码讲解记录
```

---

# 36. 第二十一版和前几版的关系

第十五版：

```text
加入客户端本地历史
```

第二十一版：

```text
让客户端历史可以保存到文件并跨会话复用
```

它们是一条连续演进：

```text
无历史
 -> 当前会话历史
 -> 可选持久化历史
```

这也是 Roadmap 中“persistent client history”的完成。

---

# 37. 第二十一版之后适合继续做什么

客户端体验方向：

```text
交互式行编辑
方向键浏览历史
历史搜索
命令补全
```

持久化安全方向：

```text
WAL checksum
Snapshot 原子写
WAL compact
```

工程质量方向：

```text
CI 上传测试日志
Sanitizer job
格式检查或静态分析
```

README 当前 Roadmap 选择：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL checksums or atomic snapshot writes for stronger recovery guarantees.
```

---

# 38. 第二十一版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v21-run 已删除
真实 smoke 启动的 minikv_server 已停止
检查后没有 minikv_server / minikv_client 残留进程
关键截图保留在 a/21/图片/
归档说明保留在 a/21/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第四十四次总结

第二十一版配套工作证明了三件事：

```text
client_history_tests
 -> 历史文件缺失、保存、加载、父目录创建、CRLF 兼容和容量裁剪都有自动化测试

真实 server/client smoke
 -> --history-file 在真实 minikv_client 两次会话之间可以复用历史

README 和 a/21 归档
 -> 用户用法、版本说明、运行截图和验证解释已经同步
```

一句话总结：

```text
第二十一版把客户端历史从“单次会话内便利”推进到“可选文件持久化、跨会话可复用”，并用单元测试、全量 CTest 和真实客户端 smoke 做了复核。
```
