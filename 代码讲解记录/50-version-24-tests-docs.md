# 第五十次讲解：第二十四版测试、README、归档和整体增删改

第四十九次讲了第二十四版 WAL compact 核心。

这次讲第二十四版配套内容：

```text
CMakeLists.txt
README.md
tests/wal_tests.cpp
tests/command_tests.cpp
a/24/解释/说明.md
a/24/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.24.0
COMPACT 命令可用
WAL compact 后只保留 live state
compact 后的 WAL 仍然是 WAL2 checksum 格式
TTL 可以通过 EXPIREAT 恢复
已删除 key 不会被 compact 后的 replay 恢复
关键命令输出已经按 a/24 归档
```

---

# 1. 第二十四版的定位

第二十三版做的是：

```text
WAL2 checksum
checksum mismatch 跳过
checksum_failed_records 报告
```

第二十四版继续推进 WAL：

```text
WAL compaction
```

它解决的是长日志问题：

```text
SET / DEL / EXPIRE 一直 append
WAL 文件越来越大
启动 replay 成本越来越高
```

compact 后，WAL 只表达：

```text
当前 Store 里仍然存在的数据
```

---

# 2. CMakeLists.txt 版本号更新

第二十四版把版本号改成：

```cmake
project(mini_kv VERSION 0.24.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然复用已有：

```text
minikv_wal_tests
minikv_command_tests
```

所以 CMake 的目标结构没有变。

变的是：

```text
核心库 minikv 新增 WAL compact 能力
已有测试目标新增断言覆盖
```

---

# 3. command_tests 新增 COMPACT 参数错误

`tests/command_tests.cpp` 新增：

```cpp
result = processor.execute("COMPACT now");
assert(result.response == "ERR usage: COMPACT");
```

这里的 `processor` 没有 WAL：

```cpp
minikv::CommandProcessor processor{store};
```

但测试先检查参数错误。

也就是说：

```text
COMPACT now
 -> 先命中 usage 错误
 -> 不继续检查 WAL 是否启用
```

这和其它命令的风格一致：

```text
参数格式错优先返回 usage
```

---

# 4. command_tests 新增无 WAL 错误

继续：

```cpp
result = processor.execute("COMPACT");
assert(result.response == "ERR WAL not enabled");
```

这证明：

```text
纯内存模式不能 compact
```

因为 compact 的对象是：

```text
当前 WAL 文件
```

如果程序启动时没有传入 `wal_path`，就没有文件可重写。

---

# 5. command_tests 检查 HELP

新增：

```cpp
result = processor.execute("HELP");
assert(result.response.find("COMPACT") != std::string::npos);
```

这确保用户能在帮助文本里看到新命令。

它覆盖的是：

```text
CommandProcessor::help_text()
```

而不是 compact 文件行为。

文件行为放在 `wal_tests` 和真实 CLI smoke 里验证。

---

# 6. wal_tests 新增 compact_path

`tests/wal_tests.cpp` 新增专门的测试文件：

```cpp
const auto compact_path =
    std::filesystem::temp_directory_path() / ("minikv-wal-compact-test-" + std::to_string(suffix) + ".wal");
std::filesystem::remove(compact_path);
```

它和前面几个路径分开：

```text
path
 -> 正常 WAL2 append / replay

corrupt_path
 -> 旧格式坏记录和尾部半条记录

checksum_path
 -> WAL2 checksum mismatch

compact_path
 -> WAL compact
```

测试边界更清楚。

---

# 7. wal_tests 构造 compact 场景

测试先创建带 WAL 的命令处理器：

```cpp
minikv::Store store;
minikv::WriteAheadLog wal{compact_path};
minikv::CommandProcessor processor{store, &wal};
```

然后执行：

```cpp
SET keep value
EXPIRE keep 60
SET phrase hello from compact
SET gone remove
DEL gone
COMPACT
```

这组数据覆盖三种状态：

```text
keep
 -> live key，并且有 TTL

phrase
 -> live key，没有 TTL，value 带空格

gone
 -> 曾经存在，但已删除
```

---

# 8. 为什么 phrase value 带空格

测试里有：

```cpp
result = processor.execute("SET phrase hello from compact");
```

这是为了继续覆盖：

```text
SET 的 value 可以包含空格
WAL compact 写回后 replay 仍然能恢复完整 value
```

compact 写出的记录是：

```text
SET phrase hello from compact
```

replay 时 `SET` 会把 key 后面的剩余整行作为 value。

所以恢复结果应该是：

```text
hello from compact
```

---

# 9. COMPACT 返回 live key 数量

测试断言：

```cpp
result = processor.execute("COMPACT");
assert(result.response == "OK compacted 2");
```

这里 compacted 是：

```text
当前 live key 数量
```

当时 Store 里只有：

```text
keep
phrase
```

`gone` 已经被删除。

所以返回：

```text
2
```

---

# 10. compact 后 WAL 行数是 3

测试读回 WAL：

```cpp
std::ifstream input{compact_path};
std::vector<std::string> lines;
std::string line;
while (std::getline(input, line)) {
    lines.push_back(line);
}

assert(lines.size() == 3);
```

为什么是 3 行？

```text
keep
 -> SET keep value
 -> EXPIREAT keep ...

phrase
 -> SET phrase hello from compact
```

所以：

```text
2 个 live key
3 条 WAL 记录
```

这也说明 `OK compacted 2` 不是 WAL 行数。

---

# 11. 每一行都必须是 WAL2

测试循环里：

```cpp
for (const auto& record : lines) {
    assert(record.rfind("WAL2 ", 0) == 0);
    contents += record;
    contents += '\n';
}
```

这确认 compact 没有写旧格式裸记录。

第二十四版 compact 继续复用第二十三版：

```cpp
encode_wal_record(...)
```

所以压缩后的 WAL 仍然有 checksum。

---

# 12. compact 文件必须包含 live key

测试检查：

```cpp
assert(contents.find("SET keep value") != std::string::npos);
assert(contents.find("EXPIREAT keep ") != std::string::npos);
assert(contents.find("SET phrase hello from compact") != std::string::npos);
```

这三条对应：

```text
keep 的值
keep 的 TTL
phrase 的值
```

注意 `EXPIREAT keep ` 后面没有写死具体时间。

原因是：

```text
epoch_millis 每次运行都会变化
```

测试只需要确认有这条记录。

---

# 13. compact 文件不能包含 deleted key

测试还检查：

```cpp
assert(contents.find("DEL gone") == std::string::npos);
assert(contents.find("SET gone remove") == std::string::npos);
```

这证明 compact 后：

```text
删除历史没有保留
已删除 key 的旧值没有保留
```

如果这里失败，说明 compact 仍然在保存历史，而不是保存当前状态。

---

# 14. replay compact 后的 WAL

下一段测试：

```cpp
minikv::Store restored;
minikv::WriteAheadLog wal{compact_path};
const auto report = wal.replay_with_report(restored);
```

这是模拟：

```text
程序重启
从 compact 后的 WAL 恢复 Store
```

断言：

```cpp
assert(report.applied_records == 3);
assert(report.skipped_records == 0);
assert(report.truncated_records == 0);
assert(report.checksum_failed_records == 0);
```

说明 compact 后的 WAL：

```text
3 条记录全部 replay 成功
没有坏记录
没有尾部半条
没有 checksum mismatch
```

---

# 15. replay 后 Store 状态

测试继续：

```cpp
assert(restored.get("keep") == std::optional<std::string>{"value"});
assert(restored.ttl("keep").has_value());
assert(restored.get("phrase") == std::optional<std::string>{"hello from compact"});
assert(!restored.get("gone").has_value());
```

这验证四件事：

```text
keep 的 value 恢复
keep 的 TTL 恢复
phrase 的带空格 value 恢复
gone 不会恢复
```

这就是 compact 行为的核心验收。

---

# 16. README 当前版本更新

README 当前版本从：

```text
Version 23 is a runnable in-memory KV service with checksummed WAL records:
```

改成：

```text
Version 24 is a runnable in-memory KV service with manual WAL compaction:
```

关键词是：

```text
manual WAL compaction
```

说明本版重点是手动压缩 WAL。

---

# 17. README 能力列表新增 compact

能力列表新增：

```text
Manual WAL compaction rewrites the log from the current live store state
```

这句话表达两个重点：

```text
manual
 -> 用户主动执行 COMPACT

current live store state
 -> 压缩依据当前 Store，不保留旧历史
```

---

# 18. README CLI commands 新增 COMPACT

命令列表新增：

```text
COMPACT
```

位置在：

```text
LOAD path
COMPACT
HELP
EXIT
```

这说明它和 `SAVE` / `LOAD` 一样属于用户主动触发的维护命令。

---

# 19. README TCP 示例也加入 COMPACT

TCP protocol 示例中也新增：

```text
COMPACT
```

原因是 `CommandProcessor` 是共享命令层。

所以新命令不只 CLI 能用：

```text
CLI
inline TCP
RESP TCP
```

都可以走到同一个 `CommandProcessor::execute()`。

---

# 20. README WAL persistence 章节更新

WAL 章节新增：

```text
`COMPACT` is available when a WAL path is configured;
it rewrites the log to the current live key set using checksummed `SET` and `EXPIREAT` records,
dropping overwritten, deleted, and expired history.
```

这句话说明：

```text
必须启用 WAL
压缩后仍然是 checksummed 记录
只保留 SET / EXPIREAT
丢弃覆盖、删除、过期历史
```

---

# 21. README 测试说明更新

测试说明里新增：

```text
`wal_tests` verifies checksummed WAL records,
older plain-record replay compatibility,
checksum mismatch skipping,
truncated-tail detection,
and compacted WAL replay.
```

这把 `wal_tests` 当前覆盖范围讲完整：

```text
WAL2 checksum
旧格式兼容
checksum mismatch
尾部半条记录
compact replay
```

---

# 22. README Roadmap 调整

第二十三版 Roadmap 里还有：

```text
Add WAL compaction or startup repair tooling for long-running logs.
```

第二十四版已经完成 compaction。

所以 Roadmap 改成：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL startup repair tooling for damaged logs.
```

WAL 后续方向从：

```text
compaction or repair
```

收窄为：

```text
startup repair
```

---

# 23. a/24 归档目录

第二十四版新增归档：

```text
a/24/图片/
a/24/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-wal-tests.png
05-cli-wal-compact-smoke.png
06-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 24. 01-cmake-configure.png

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
CMake 能识别 0.24.0
当前构建目录可以继续生成
```

---

# 25. 02-cmake-build.png

第二张图对应命令：

```text
cmake --build cmake-build-debug
```

结果：

```text
23 个 Ninja 构建步骤全部完成
```

意义：

```text
WriteAheadLog::compact 能编译
COMPACT 命令能编译
新增测试能编译
旧 CLI / server / client / benchmark / tests 目标没有构建回归
```

---

# 26. 03-ctest.png

第三张图对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

测试列表是：

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
WAL compact 改动通过
前面二十三版积累的能力没有回归
```

---

# 27. 04-wal-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_wal_tests.exe
```

结果：

```text
退出码 0
minikv_wal_tests.exe: PASS
```

它重点证明：

```text
compact 后 WAL 只保留 live state
compact WAL 可以 replay
TTL 可以恢复
已删除 key 不会恢复
checksum mismatch 旧覆盖仍然正常
```

---

# 28. 05-cli-wal-compact-smoke.png

第五张图是真实 CLI smoke。

它真实运行两次：

```text
.\cmake-build-debug\minikv_cli.exe tmp-v24-run\compact-smoke.wal
```

第一次输入：

```text
SET keep value
SET phrase hello from compact
EXPIRE keep 60
SET gone remove
DEL gone
COMPACT
EXIT
```

第一次结果包括：

```text
OK compacted 2
```

---

# 29. CLI smoke 检查 compact 后 WAL 文件

脚本检查 compact 后 WAL：

```text
只有 3 行
每行都以 WAL2 开头
包含 SET keep value
包含 EXPIREAT keep ...
包含 SET phrase hello from compact
不包含 gone
不包含 DEL
```

这证明：

```text
文件层面确实压缩成功
压缩后仍然是 WAL2
删除历史已被移除
```

---

# 30. CLI smoke 第二次 replay

第二次输入：

```text
GET keep
GET phrase
GET gone
TTL keep
EXIT
```

启动输出：

```text
3 records replayed, 0 skipped, 0 truncated, 0 checksum failed
```

命令结果：

```text
GET keep
 -> value

GET phrase
 -> hello from compact

GET gone
 -> (nil)

TTL keep
 -> 60 附近的剩余秒数
```

这是真实可执行程序路径的验收。

---

# 31. 06-version-docs-check.png

第六张图对应版本和文档一致性检查。

检查项包括：

```text
git diff --check: PASS
CMakeLists.txt 版本号为 0.24.0
README 当前版本说明为 Version 24
README CLI 命令和 WAL persistence 包含 COMPACT
include/minikv/wal.hpp 暴露 WriteAheadLog::compact
src/command.cpp 包含 COMPACT 命令和错误处理
tests/wal_tests.cpp / tests/command_tests.cpp 包含 compact 覆盖
```

意义是：

```text
代码版本、用户文档、API 入口、命令入口和测试覆盖同步
```

---

# 32. 本次代码增删改总览

第二十四版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.24.0

README.md
 -> 当前版本说明更新为 Version 24
 -> 能力列表新增 manual WAL compaction
 -> CLI / TCP 命令列表新增 COMPACT
 -> WAL persistence 章节说明 COMPACT 行为
 -> 测试说明新增 compacted WAL replay
 -> Roadmap 移除 WAL compaction，保留 startup repair

include/minikv/wal.hpp
 -> 新增 WriteAheadLog::compact 声明

src/wal.cpp
 -> 新增 TempWalFile
 -> 新增 make_temp_wal_path
 -> 新增 replace_file_atomically
 -> 新增 WAL compact 用 format_expires_at
 -> 新增 WriteAheadLog::compact 实现

src/command.cpp
 -> 新增 WAL compact 错误响应
 -> 新增 wal_command_mutex
 -> SET / DEL / EXPIRE 的 WAL 模式进入命令层互斥
 -> 新增 COMPACT 命令
 -> HELP 新增 COMPACT

tests/command_tests.cpp
 -> 覆盖 COMPACT 参数错误
 -> 覆盖无 WAL 模式错误
 -> 覆盖 HELP 包含 COMPACT

tests/wal_tests.cpp
 -> 覆盖 COMPACT 返回 live key 数量
 -> 覆盖 compact 后 WAL 行内容
 -> 覆盖 compact WAL replay report
 -> 覆盖 TTL 恢复和 deleted key 不恢复
```

第二十四版新增：

```text
a/24/图片/01-cmake-configure.png
a/24/图片/02-cmake-build.png
a/24/图片/03-ctest.png
a/24/图片/04-wal-tests.png
a/24/图片/05-cli-wal-compact-smoke.png
a/24/图片/06-version-docs-check.png
a/24/解释/说明.md
代码讲解记录/49-wal-compaction-core.md
代码讲解记录/50-version-24-tests-docs.md
```

第二十四版删除：

```text
无源码文件删除
```

---

# 33. 第二十四版之后当前能力总览

到第二十四版为止，mini-kv 已经具备：

```text
内存 key-value 存储
命令解析
CLI 模式
TCP server
TCP client
TTL 过期
WAL 增量持久化
WAL2 checksum 记录格式
旧格式 WAL replay 兼容
WAL replay applied / skipped / truncated / checksum_failed 报告
WAL 坏记录跳过
WAL checksum mismatch 跳过
WAL 尾部疑似部分写入记录跳过
手动 WAL compact
compact 后 WAL 保留 live SET / EXPIREAT
compact 后删除 overwritten / deleted / expired 历史
Snapshot 手动完整状态保存和加载
Snapshot SAVE 临时文件写完后替换目标文件
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

# 34. 第二十四版和前几版的关系

第四版：

```text
加入 WAL
```

第十九版：

```text
WAL replay report
坏记录跳过
尾部半条记录跳过
```

第二十三版：

```text
WAL2 checksum
checksum mismatch 跳过
checksum_failed 报告
```

第二十四版：

```text
WAL compact
用当前 live state 重写 WAL
减少长期 replay 成本
```

也就是从：

```text
能持久化
```

推进到：

```text
能可靠恢复并报告问题
```

再推进到：

```text
能维护 WAL 文件体积
```

---

# 35. 第二十四版之后适合继续做什么

WAL 方向：

```text
启动时检测坏 WAL 并生成 repair report
WAL repair 工具
按文件大小自动提示 COMPACT
compact 前后文件大小统计
```

客户端体验方向：

```text
交互式行编辑
方向键历史浏览
命令补全
```

工程质量方向：

```text
Sanitizer CI
格式检查
静态分析
更细的 benchmark 输出
```

README 当前 Roadmap 选择：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL startup repair tooling for damaged logs.
```

---

# 36. 第二十四版归档的清理结论

本轮运行后清理结果是：

```text
临时目录 tmp-v24-run 已删除
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/24/图片/
归档说明保留在 a/24/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第五十次总结

第二十四版配套工作证明了三件事：

```text
wal_tests
 -> COMPACT 后 WAL 文件内容、WAL2 格式、TTL 恢复、deleted key 不恢复都有自动化测试

真实 CLI smoke
 -> minikv_cli 可以真实执行 COMPACT，重启后 replay compact WAL 成功

README 和 a/24 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十四版把 WAL 从“可靠追加和校验”推进到“可手动压缩为当前 live state”，并通过单元测试、全量 CTest、真实 CLI smoke 和 a/24 归档证明新能力能稳定运行。
```
