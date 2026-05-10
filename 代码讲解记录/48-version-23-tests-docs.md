# 第四十八次讲解：第二十三版测试、README、归档和整体增删改

第四十七次讲了第二十三版 WAL checksum 核心。

这次讲第二十三版配套内容：

```text
CMakeLists.txt
README.md
tests/wal_tests.cpp
a/23/解释/说明.md
a/23/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.23.0
新写入 WAL 记录使用 WAL2 checksum 格式
旧格式 WAL 仍然兼容 replay
checksum mismatch 会被跳过并报告
真实 minikv_cli 能识别 WAL2、坏 checksum 和旧格式裸记录
关键命令输出已经按 a/23 归档
```

---

# 1. 第二十三版的定位

第二十二版做的是 Snapshot 保存端可靠性：

```text
临时文件写完整
再替换目标 snapshot
避免直接截断旧 snapshot
```

第二十三版转向 WAL 恢复可靠性：

```text
WAL checksum
```

它解决的是：

```text
WAL 记录被改坏但仍然像合法命令
```

这种情况。

---

# 2. CMakeLists.txt 版本号更新

第二十三版把版本号改成：

```cmake
project(mini_kv VERSION 0.23.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然复用已有：

```text
minikv_wal_tests
```

所以构建步骤仍然是：

```text
38
```

---

# 3. wal_tests 原有覆盖

`tests/wal_tests.cpp` 原来已经覆盖：

```text
正常 SET / EXPIRE / DEL / 过期 key replay
坏 WAL 记录跳过
坏 EXPIREAT 跳过
尾部无换行半条记录跳过
WalReplayReport applied / skipped / truncated
```

第二十三版保留这些测试。

这表示：

```text
WAL2 checksum 改动不能破坏旧格式兼容和第十九版恢复保护
```

---

# 4. 检查新 WAL 第一行是 WAL2

正常 WAL 写入后，新增测试：

```cpp
{
    std::ifstream input{path};
    std::string first_line;
    assert(std::getline(input, first_line));
    assert(first_line.rfind("WAL2 ", 0) == 0);
}
```

这里验证：

```text
CommandProcessor 调用 wal.append
 -> WriteAheadLog::append
 -> 实际写入 WAL2 格式
```

`rfind("WAL2 ", 0) == 0` 的意思是：

```text
first_line 从位置 0 开始就是 WAL2 
```

也就是检查前缀。

---

# 5. 正常 replay 仍然是 6 条

原测试里正常 replay：

```cpp
const auto applied = wal.replay(restored);
assert(applied == 6);
```

为什么是 6？

前面成功写入的 WAL 操作是：

```text
SET name mini-kv
SET phrase hello from wal
EXPIREAT name ...
DEL phrase
SET stale old
EXPIREAT stale ...
```

共 6 条。

虽然新文件格式变成 WAL2，但 replay 结果仍然应该是：

```text
applied = 6
```

---

# 6. 旧坏 WAL 不计 checksum_failed

第十九版的坏 WAL fixture 保留：

```text
SET kept value
GARBAGE record
EXPIREAT kept not-a-number
SET partial value
```

最后一行没有换行。

第二十三版新增断言：

```cpp
assert(report.checksum_failed_records == 0);
```

原因是这几条都是旧格式裸记录。

它们不带 WAL2 checksum。

所以坏记录还是计入：

```text
skipped_records
truncated_records
```

但不计入：

```text
checksum_failed_records
```

---

# 7. 新增 checksum_path

第二十三版新增专门的 checksum 测试文件：

```cpp
const auto checksum_path =
    std::filesystem::temp_directory_path() / ("minikv-wal-checksum-test-" + std::to_string(suffix) + ".wal");
std::filesystem::remove(checksum_path);
```

它独立于正常 WAL 测试和旧坏 WAL 测试。

这样测试边界更清楚：

```text
path
 -> 正常 WAL2 replay

corrupt_path
 -> 旧格式坏记录和尾部半条记录

checksum_path
 -> WAL2 checksum mismatch
```

---

# 8. 写入两条 WAL2

新增测试先写：

```cpp
{
    minikv::WriteAheadLog wal{checksum_path};
    assert(wal.append("SET checked ok"));
    assert(wal.append("SET another value"));
}
```

这两条都会通过 `WriteAheadLog::append()` 变成 WAL2 格式：

```text
WAL2 <checksum1> SET checked ok
WAL2 <checksum2> SET another value
```

---

# 9. 手动篡改第一条 payload

然后测试手动读文件：

```cpp
std::ifstream input{checksum_path, std::ios::binary};
std::string contents{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
const auto position = contents.find("SET checked ok");
assert(position != std::string::npos);
contents.replace(position, std::string{"SET checked ok"}.size(), "SET checked tampered");
```

这一步只改 payload。

不改 checksum。

所以文件会从：

```text
WAL2 <checksum-of-SET-checked-ok> SET checked ok
```

变成：

```text
WAL2 <checksum-of-SET-checked-ok> SET checked tampered
```

checksum 和 payload 不再匹配。

---

# 10. 为什么使用 binary 读写

测试里读写使用：

```cpp
std::ios::binary
```

这是为了减少文本模式对换行的自动转换。

WAL 测试要精确控制文件内容。

特别是：

```text
替换 payload
保留原 checksum
保留换行结构
```

二进制模式更直接。

---

# 11. 写回篡改后的文件

写回：

```cpp
std::ofstream output{checksum_path, std::ios::binary | std::ios::trunc};
output << contents;
```

这会覆盖原 WAL 文件。

覆盖后的文件包含：

```text
第一条 WAL2 checksum mismatch
第二条 WAL2 正常
```

---

# 12. replay checksum_path

然后：

```cpp
minikv::Store restored;
minikv::WriteAheadLog wal{checksum_path};
const auto report = wal.replay_with_report(restored);
```

预期是：

```text
第一条被跳过
第二条被应用
```

所以断言：

```cpp
assert(report.applied_records == 1);
assert(report.skipped_records == 1);
assert(report.truncated_records == 0);
assert(report.checksum_failed_records == 1);
```

---

# 13. Store 状态断言

接着：

```cpp
assert(!restored.get("checked").has_value());
assert(restored.get("another") == std::optional<std::string>{"value"});
```

第一条被篡改的是：

```text
SET checked tampered
```

因为 checksum 不匹配，它不应该进入 Store。

所以：

```text
checked 不存在
```

第二条没被改：

```text
SET another value
```

所以：

```text
another = value
```

---

# 14. replay 旧接口仍然返回 1

测试还保留：

```cpp
assert(wal.replay(restored) == 1);
```

这确认旧接口仍然只返回：

```text
applied_records
```

不暴露 skipped / checksum_failed 细节。

兼容旧调用方式。

---

# 15. CLI 输出更新测试靠 smoke 覆盖

`wal_tests` 直接测试 WAL 模块。

CLI 输出：

```text
checksum failed
```

不在 `wal_tests` 里断言。

它通过真实 CLI smoke 验证。

这样分工清楚：

```text
wal_tests
 -> 模块行为

CLI smoke
 -> 用户入口输出和真实文件 replay
```

---

# 16. README 当前版本更新

README 当前版本从：

```text
Version 22 is a runnable in-memory KV service with atomic snapshot saves:
```

改成：

```text
Version 23 is a runnable in-memory KV service with checksummed WAL records:
```

关键词是：

```text
checksummed WAL records
```

这说明第二十三版重点是 WAL 记录校验。

---

# 17. README 能力列表更新

能力列表把旧的：

```text
WAL replay reports applied, skipped, and truncated records
```

扩展成两条：

```text
WAL records include checksums while replay remains compatible with older plain records
WAL replay reports applied, skipped, truncated, and checksum-failed records
```

第一条说明：

```text
新格式有 checksum
旧格式仍兼容
```

第二条说明：

```text
启动 replay 报告更细
```

---

# 18. README WAL persistence 章节更新

WAL 章节新增：

```text
New WAL records are stored as `WAL2 <checksum> <record>` lines;
replay still accepts older plain `SET` / `DEL` / `EXPIREAT` lines.
```

这直接说明格式变化。

然后补充：

```text
WAL replay skips malformed records,
skips checksum mismatches,
detects a final unterminated record as a probable partial write,
and reports applied, skipped, truncated, and checksum-failed counts during startup.
```

它覆盖了三类跳过：

```text
malformed records
checksum mismatches
final unterminated record
```

---

# 19. README Roadmap 调整

第二十二版 Roadmap 里还有：

```text
Add WAL checksums for stronger recovery guarantees.
```

第二十三版已经完成。

所以 Roadmap 改成：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL compaction or startup repair tooling for long-running logs.
```

WAL 后续可以继续做：

```text
compaction
repair
```

---

# 20. a/23 归档目录

第二十三版新增归档：

```text
a/23/图片/
a/23/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-wal-tests.png
05-cli-wal-checksum-smoke.png
06-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 21. 01-cmake-configure.png

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
CMake 能识别 0.23.0
现有构建目标都能正常生成
```

---

# 22. 02-cmake-build.png

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
WAL2 encode/decode 能编译
FNV-1a checksum 能编译
checksum_failed report 能编译
CLI/server 启动输出调整能编译
旧目标没有构建回归
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
WAL checksum 改动通过
前面二十二版积累的能力没有回归
```

---

# 24. 04-wal-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_wal_tests.exe
```

结果：

```text
退出码 0
Result: PASS
```

这张图专门单独复跑 WAL 测试。

原因是第二十三版主要改动就在：

```text
WriteAheadLog
WalReplayReport
tests/wal_tests.cpp
```

---

# 25. 05-cli-wal-checksum-smoke.png

第五张图是真实 CLI smoke。

它真实运行两次：

```text
.\cmake-build-debug\minikv_cli.exe <wal_path>
```

第一次输入：

```text
SET good value
SET bad original
EXIT
```

第一轮会生成两条 WAL2 记录。

中间脚本做两件事：

```text
把 SET bad original 改成 SET bad tampered
追加一条旧格式裸记录 SET legacy old
```

第二次输入：

```text
GET good
GET bad
GET legacy
EXIT
```

---

# 26. CLI smoke 的 replay 结果

第二次启动输出：

```text
2 records replayed, 1 skipped, 0 truncated, 1 checksum failed
```

拆开看：

```text
2 records replayed
 -> WAL2 SET good value
 -> 旧格式 SET legacy old

1 skipped
 -> 被篡改的 WAL2 SET bad tampered

0 truncated
 -> 文件末尾完整

1 checksum failed
 -> 被篡改的 WAL2 checksum 不匹配
```

---

# 27. CLI smoke 的 GET 结果

第二次会话里：

```text
GET good
 -> value

GET bad
 -> (nil)

GET legacy
 -> old
```

这证明：

```text
未篡改 WAL2 能恢复
篡改 WAL2 被跳过
旧格式裸记录仍然兼容
```

---

# 28. CLI smoke 的最终 WAL 文件

最终 WAL 内容：

```text
WAL2 775d3cab2dd9031d SET good value
WAL2 411bcd301442e643 SET bad tampered
SET legacy old
```

注意第二条：

```text
checksum 仍然是 original 的 checksum
payload 已经变成 tampered
```

所以 replay 会识别为 checksum mismatch。

---

# 29. 06-version-docs-check.png

第六张图对应版本和文档一致性检查。

检查项包括：

```text
CMake version is 0.23.0
README current version is Version 23
README documents WAL2 checksum records
README roadmap moved past WAL checksums
WalReplayReport has checksum_failed_records
WriteAheadLog append encodes WAL2 records
WriteAheadLog replay decodes WAL2 records
CLI startup prints checksum failures
Server startup logs checksum_failed
wal_tests cover checksum mismatch
```

结果：

```text
Result: PASS
```

意义是：

```text
代码版本、README 描述、WAL 实现、CLI/server 输出和测试覆盖互相对齐
```

---

# 30. 本次代码增删改总览

第二十三版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.23.0

include/minikv/wal.hpp
 -> WalReplayReport 新增 checksum_failed_records

src/wal.cpp
 -> 新增 WAL2 前缀
 -> 新增 FNV-1a checksum
 -> 新增 format_checksum
 -> 新增 parse_checksum
 -> 新增 encode_wal_record
 -> 新增 decode_wal_record
 -> WriteAheadLog::append 改为写 WAL2 记录
 -> replay_with_report 增加 checksum mismatch 跳过和计数
 -> 保持旧格式裸 WAL 兼容 replay

src/main.cpp
 -> CLI 启动 WAL 时输出 checksum failed 数量

src/server_main.cpp
 -> event=wal_replay 新增 checksum_failed 字段

tests/wal_tests.cpp
 -> 检查新写 WAL 以 WAL2 开头
 -> 旧格式坏 WAL 不计 checksum failed
 -> 新增 WAL2 payload 篡改测试
 -> 验证 checksum mismatch 跳过
 -> 验证旧格式裸记录兼容

README.md
 -> 当前版本说明更新为 Version 23
 -> 能力列表新增 WAL checksum 和 checksum-failed replay report
 -> WAL persistence 章节说明 WAL2 格式和旧格式兼容
 -> Roadmap 移除 WAL checksums
```

第二十三版新增：

```text
a/23/图片/01-cmake-configure.png
a/23/图片/02-cmake-build.png
a/23/图片/03-ctest.png
a/23/图片/04-wal-tests.png
a/23/图片/05-cli-wal-checksum-smoke.png
a/23/图片/06-version-docs-check.png
a/23/解释/说明.md
代码讲解记录/47-wal-checksum-core.md
代码讲解记录/48-version-23-tests-docs.md
```

第二十三版删除：

```text
无源码文件删除
```

---

# 31. 第二十三版之后当前能力总览

到第二十三版为止，mini-kv 已经具备：

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

# 32. 第二十三版和前几版的关系

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
旧格式兼容 replay
```

也就是从：

```text
能 replay
```

推进到：

```text
能报告 replay 结果
```

再推进到：

```text
能发现新格式 WAL payload 损坏
```

---

# 33. 第二十三版之后适合继续做什么

WAL 方向：

```text
WAL compaction
WAL startup repair
WAL snapshot checkpoint
```

客户端体验方向：

```text
交互式行编辑
方向键历史浏览
命令补全
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
2. Add WAL compaction or startup repair tooling for long-running logs.
```

---

# 34. 第二十三版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v23-run 已删除
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/23/图片/
归档说明保留在 a/23/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第四十八次总结

第二十三版配套工作证明了三件事：

```text
wal_tests
 -> WAL2 写入、旧格式兼容、checksum mismatch 跳过、checksum_failed 计数都有自动化测试

真实 CLI smoke
 -> minikv_cli 启动 replay 能同时处理未篡改 WAL2、被篡改 WAL2 和旧格式裸记录

README 和 a/23 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十三版把 WAL 恢复从“跳过明显坏记录和尾部半条记录”推进到“新 WAL2 记录带 checksum，payload 被篡改时可跳过并报告”，并用单元测试、全量 CTest 和真实 CLI smoke 做了复核。
```
