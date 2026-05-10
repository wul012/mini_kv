# 第五十二次讲解：第二十五版测试、README、归档和整体增删改

第五十一次讲了第二十五版 WAL repair 核心。

这次讲第二十五版配套内容：

```text
CMakeLists.txt
README.md
tests/wal_tests.cpp
a/25/解释/说明.md
a/25/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.25.0
CLI 支持 --repair-wal
server 支持 --repair-wal
repair 能处理 checksum mismatch、坏行和尾部半条记录
repair 后 WAL 被重写为干净 WAL2
真实 CLI 和真实 server 都能完成启动修复
关键命令输出已经按 a/25 归档
```

---

# 1. 第二十五版的定位

第二十四版做的是：

```text
运行期 COMPACT
用当前 live state 重写短 WAL
```

第二十五版做的是：

```text
启动期 repair
```

它的入口不是用户命令，而是启动参数：

```text
--repair-wal
```

它适合这种场景：

```text
WAL 文件里混有坏记录
普通启动每次都会报告 skipped / truncated / checksum failed
用户希望把可恢复状态保留下来，并把 WAL 文件清理干净
```

---

# 2. CMakeLists.txt 版本号更新

第二十五版把版本号改成：

```cmake
project(mini_kv VERSION 0.25.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然使用已有：

```text
minikv_cli
minikv_server
minikv_client
minikv_wal_tests
```

变化集中在：

```text
minikv 库新增 repair API
CLI / server 启动参数新增 --repair-wal
wal_tests 新增 repair 覆盖
```

---

# 3. wal_tests 新增 repair_path

`tests/wal_tests.cpp` 里新增：

```cpp
const auto repair_path =
    std::filesystem::temp_directory_path() / ("minikv-wal-repair-test-" + std::to_string(suffix) + ".wal");
std::filesystem::remove(repair_path);
```

它独立于前面几个 WAL 测试文件：

```text
path
 -> 正常 WAL2 append / replay

corrupt_path
 -> 旧格式坏记录和尾部半条记录

checksum_path
 -> WAL2 checksum mismatch

compact_path
 -> WAL compact

repair_path
 -> WAL repair
```

测试边界继续保持清楚。

---

# 4. repair 测试准备未来过期时间

测试里计算：

```cpp
const auto future_expiration =
    std::chrono::duration_cast<std::chrono::milliseconds>((minikv::Store::Clock::now() + 60s).time_since_epoch())
        .count();
```

这是为了手动写一条：

```text
EXPIREAT ttl <future_expiration>
```

repair 后要证明：

```text
TTL 可以恢复
```

所以过期时间必须在未来。

---

# 5. repair 测试先写正常 WAL2

测试先通过 `WriteAheadLog::append()` 写入：

```cpp
assert(wal.append("SET good value"));
assert(wal.append("SET bad original"));
assert(wal.append("SET gone old"));
assert(wal.append("DEL gone"));
assert(wal.append("SET ttl keep"));
assert(wal.append("EXPIREAT ttl " + std::to_string(future_expiration)));
```

这些都会写成 WAL2：

```text
WAL2 <checksum> SET good value
WAL2 <checksum> SET bad original
...
```

这里构造了几种状态：

```text
good
 -> 正常可恢复

bad
 -> 后面会被篡改 checksum

gone
 -> 已经删除，不应 repair 后恢复

ttl
 -> 带过期时间，应 repair 后保留 TTL
```

---

# 6. 手动篡改 bad 记录

测试读出文件内容：

```cpp
std::ifstream input{repair_path, std::ios::binary};
std::string contents{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
```

然后替换：

```cpp
contents.replace(position, std::string{"SET bad original"}.size(), "SET bad tampered");
```

这样文件里变成：

```text
WAL2 <checksum-of-original> SET bad tampered
```

checksum 和 payload 不匹配。

repair 时这条记录应该被跳过。

---

# 7. 追加旧格式裸记录

测试追加：

```cpp
contents += "SET legacy old\n";
```

这是旧格式 WAL。

它没有 WAL2 前缀，也没有 checksum。

repair 应该兼容它：

```text
replay 时恢复 legacy=old
compact 后重写为 WAL2 SET legacy old
```

这证明 repair 也能顺便迁移旧格式可恢复记录。

---

# 8. 追加坏行和尾部半条记录

测试继续追加：

```cpp
contents += "GARBAGE record\n";
contents += "SET partial value";
```

这里有两个坏点：

```text
GARBAGE record
 -> 语法不是 SET / DEL / EXPIREAT
 -> skipped_records +1

SET partial value
 -> 文件末尾没有换行
 -> truncated_records +1
 -> skipped_records +1
```

repair 后它们都不应该出现在新 WAL 里。

---

# 9. 调用 repair

核心测试：

```cpp
minikv::Store repaired_store;
minikv::WriteAheadLog wal{repair_path};
minikv::WalRepairReport repair;
assert(wal.repair(repaired_store, &repair));
```

它做两件事：

```text
把可恢复记录 replay 到 repaired_store
把 repaired_store compact 回 repair_path
```

所以这一步之后：

```text
Store 已恢复
WAL 文件也已被重写
```

---

# 10. repair report 断言

测试断言：

```cpp
assert(repair.replay.applied_records == 6);
assert(repair.replay.skipped_records == 3);
assert(repair.replay.truncated_records == 1);
assert(repair.replay.checksum_failed_records == 1);
assert(repair.compacted_keys == 3);
```

applied_records 为什么是 6？

```text
SET good value
SET gone old
DEL gone
SET ttl keep
EXPIREAT ttl ...
SET legacy old
```

bad checksum、GARBAGE、partial 都没有应用。

skipped_records 为什么是 3？

```text
bad checksum
GARBAGE record
尾部 partial
```

compacted_keys 为什么是 3？

repair 后 live key 是：

```text
good
legacy
ttl
```

---

# 11. repaired_store 状态断言

测试继续检查：

```cpp
assert(repaired_store.get("good") == std::optional<std::string>{"value"});
assert(repaired_store.get("legacy") == std::optional<std::string>{"old"});
assert(repaired_store.get("ttl") == std::optional<std::string>{"keep"});
assert(repaired_store.ttl("ttl").has_value());
assert(!repaired_store.get("bad").has_value());
assert(!repaired_store.get("gone").has_value());
assert(!repaired_store.get("partial").has_value());
```

这证明：

```text
正常记录恢复
旧格式记录恢复
TTL 恢复
checksum mismatch 不恢复
已删除 key 不恢复
尾部半条记录不恢复
```

---

# 12. repair 后检查 WAL 文件行数

repair 后再次读取 `repair_path`：

```cpp
assert(lines.size() == 4);
```

为什么是 4 行？

```text
good
 -> SET good value

legacy
 -> SET legacy old

ttl
 -> SET ttl keep
 -> EXPIREAT ttl ...
```

三个 live key，但其中一个有 TTL，所以 WAL 行数是 4。

---

# 13. repair 后每行都必须是 WAL2

测试循环：

```cpp
for (const auto& record : lines) {
    assert(record.rfind("WAL2 ", 0) == 0);
    contents += record;
    contents += '\n';
}
```

这确认 repair 后的文件不再混有旧格式裸记录。

即使原文件里有：

```text
SET legacy old
```

repair 后也会变成：

```text
WAL2 <checksum> SET legacy old
```

---

# 14. repair 后 WAL 内容检查

测试检查保留内容：

```cpp
assert(contents.find("SET good value") != std::string::npos);
assert(contents.find("SET legacy old") != std::string::npos);
assert(contents.find("SET ttl keep") != std::string::npos);
assert(contents.find("EXPIREAT ttl ") != std::string::npos);
```

这些都是可恢复 live state。

同时检查删除内容：

```cpp
assert(contents.find("SET bad") == std::string::npos);
assert(contents.find("gone") == std::string::npos);
assert(contents.find("GARBAGE") == std::string::npos);
assert(contents.find("partial") == std::string::npos);
```

这证明 repair 后 WAL 已经清理掉坏历史和无效历史。

---

# 15. repair 后再次 replay

最后再模拟重启：

```cpp
minikv::Store restored;
minikv::WriteAheadLog wal{repair_path};
const auto report = wal.replay_with_report(restored);
```

断言：

```cpp
assert(report.applied_records == 4);
assert(report.skipped_records == 0);
assert(report.truncated_records == 0);
assert(report.checksum_failed_records == 0);
```

这证明 repair 后的 WAL 是干净的。

后续启动不会再报告：

```text
skipped
truncated
checksum failed
```

---

# 16. README 当前版本更新

README 当前版本从：

```text
Version 24 is a runnable in-memory KV service with manual WAL compaction:
```

改成：

```text
Version 25 is a runnable in-memory KV service with startup WAL repair:
```

关键词是：

```text
startup WAL repair
```

说明本版重点在启动阶段处理 WAL 损坏。

---

# 17. README 能力列表新增 repair

能力列表新增：

```text
Startup WAL repair can rebuild damaged logs with `--repair-wal`
```

这句话说明：

```text
repair 是启动期能力
入口是 --repair-wal
目标是 damaged logs
```

---

# 18. README Run 章节新增 CLI repair

新增：

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal --repair-wal
```

这告诉用户：

```text
CLI 模式也能先修复 WAL
修复后再进入交互式命令行
```

---

# 19. README Run 章节新增 server repair

新增：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal --repair-wal
```

这告诉用户：

```text
server 监听前会先 repair WAL
修复后的 Store 再对外提供 TCP 服务
```

---

# 20. README server options 更新

server 参数说明从：

```text
minikv_server.exe [port] [host] [wal_path] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

变成：

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

这和 `src/server_main.cpp` 的 usage 保持一致。

---

# 21. README WAL persistence 章节更新

WAL 章节新增：

```text
Start `minikv_cli` or `minikv_server` with `--repair-wal`
to replay the recoverable records and immediately rewrite the WAL
to a clean compacted WAL2 file before serving commands.
```

这里有几个关键词：

```text
recoverable records
 -> 可恢复记录保留

clean compacted WAL2 file
 -> 输出是干净短 WAL2

before serving commands
 -> 在 CLI/server 真正工作前完成
```

---

# 22. README 测试说明更新

测试说明里 `wal_tests` 增加：

```text
WAL repair rewriting
```

完整含义是：

```text
wal_tests 现在覆盖：
WAL2 checksum
旧格式兼容
checksum mismatch 跳过
尾部半条记录检测
compacted WAL replay
WAL repair rewriting
```

---

# 23. README Roadmap 调整

第二十四版 Roadmap 里还有：

```text
Add WAL startup repair tooling for damaged logs.
```

第二十五版已经完成。

所以 Roadmap 改成：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add WAL size reporting and automatic compact hints for long-running logs.
```

WAL 后续方向从：

```text
修复 damaged logs
```

推进到：

```text
观测 WAL 大小并提示 compact
```

---

# 24. a/25 归档目录

第二十五版新增归档：

```text
a/25/图片/
a/25/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-wal-tests.png
05-cli-wal-repair-smoke.png
06-server-wal-repair-smoke.png
07-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 25. 01-cmake-configure.png

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
CMake 能识别 0.25.0
当前构建目录可以继续生成
```

---

# 26. 02-cmake-build.png

第二张图对应命令：

```text
cmake --build cmake-build-debug
```

结果：

```text
22 个 Ninja 构建步骤全部完成
```

意义：

```text
WalRepairReport 能编译
WriteAheadLog::repair 能编译
CLI/server --repair-wal 能编译
新增 wal_tests 能编译
旧 CLI / server / client / benchmark / tests 目标没有构建回归
```

---

# 27. 03-ctest.png

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
WAL repair 改动通过
前面二十四版积累的能力没有回归
```

---

# 28. 04-wal-tests.png

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
repair 可以跳过 checksum mismatch
repair 可以跳过坏行
repair 可以跳过尾部半条记录
repair 可以保留旧格式裸记录
repair 可以保留 TTL
repair 后 WAL 再次 replay 是干净的
```

---

# 29. 05-cli-wal-repair-smoke.png

第五张图是真实 CLI repair smoke。

它真实运行两次：

```text
.\cmake-build-debug\minikv_cli.exe tmp-v25-run\cli-repair-smoke.wal
```

第一轮输入：

```text
SET good value
SET bad original
EXIT
```

中间手工损坏 WAL：

```text
SET bad original
 -> SET bad tampered

追加 SET legacy old
追加 GARBAGE record
追加无换行 SET partial value
```

第二轮使用：

```text
--repair-wal
```

---

# 30. CLI repair smoke 的结果

第二次启动输出：

```text
2 records replayed, 3 skipped, 1 truncated, 1 checksum failed; repaired to 2 keys
```

命令结果：

```text
GET good
 -> value

GET bad
 -> (nil)

GET legacy
 -> old
```

最终 WAL 文件只剩：

```text
WAL2 ... SET good value
WAL2 ... SET legacy old
```

这证明 CLI 启动入口 repair 生效。

---

# 31. 06-server-wal-repair-smoke.png

第六张图是真实 server repair smoke。

流程是：

```text
先用 CLI 生成 WAL
手工制造 checksum mismatch / 旧格式裸记录 / 坏行 / 尾部半条记录
启动 minikv_server.exe <port> 127.0.0.1 <wal> --repair-wal --accept-poll-ms 25
等待 event=tcp_listen
用 minikv_client.exe 连接 server
执行 GET srv / GET bad / GET legacy / QUIT
停止 server
```

server 启动日志包含：

```text
event=wal_repair ... records=2 skipped=3 truncated=1 checksum_failed=1 compacted_keys=2
```

---

# 32. server repair smoke 的查询结果

客户端查询结果：

```text
GET srv
 -> value

GET bad
 -> (nil)

GET legacy
 -> srvold
```

repair 后 server WAL 文件只剩：

```text
WAL2 ... SET legacy srvold
WAL2 ... SET srv value
```

这证明 server 监听前完成 repair，TCP 客户端读到的是修复后的 Store。

---

# 33. 07-version-docs-check.png

第七张图对应版本和文档一致性检查。

检查项包括：

```text
git diff --check: PASS
CMakeLists.txt 版本号为 0.25.0
README 当前版本说明为 Version 25
README CLI / server 示例包含 --repair-wal
README WAL persistence 章节说明 repair
include/minikv/wal.hpp 暴露 WalRepairReport 和 WriteAheadLog::repair
src/main.cpp 包含 CLI --repair-wal
src/server_main.cpp 包含 server --repair-wal 和 event=wal_repair
tests/wal_tests.cpp 包含 repair 覆盖
```

意义是：

```text
代码版本、用户文档、API 入口、启动入口、日志和测试覆盖同步
```

---

# 34. 本次代码增删改总览

第二十五版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.25.0

README.md
 -> 当前版本说明更新为 Version 25
 -> 能力列表新增 startup WAL repair
 -> Run 章节新增 CLI / server --repair-wal 示例
 -> server options 新增 --repair-wal
 -> WAL persistence 章节说明启动 repair
 -> 测试说明新增 WAL repair rewriting
 -> Roadmap 移除 WAL startup repair

include/minikv/wal.hpp
 -> 新增 WalRepairReport
 -> 新增 WriteAheadLog::repair 声明

src/wal.cpp
 -> 新增 WriteAheadLog::repair 实现

src/main.cpp
 -> 新增 CliOptions
 -> 新增 parse_options
 -> usage 新增 --repair-wal
 -> 新增 print_wal_replay / print_wal_repair
 -> 启动 WAL 时支持 repair 分支

src/server_main.cpp
 -> usage 新增 --repair-wal
 -> 新增 log_wal_replay / log_wal_repair
 -> 参数解析支持 --repair-wal
 -> 启动 WAL 时支持 repair 分支

tests/wal_tests.cpp
 -> 新增 repair_path 场景
 -> 构造 checksum mismatch、坏行、尾部半条记录和旧格式裸记录
 -> 验证 repair report
 -> 验证 repair 后 Store
 -> 验证 repair 后 WAL 文件
 -> 验证 repair 后再次 replay 干净
```

第二十五版新增：

```text
a/25/图片/01-cmake-configure.png
a/25/图片/02-cmake-build.png
a/25/图片/03-ctest.png
a/25/图片/04-wal-tests.png
a/25/图片/05-cli-wal-repair-smoke.png
a/25/图片/06-server-wal-repair-smoke.png
a/25/图片/07-version-docs-check.png
a/25/解释/说明.md
代码讲解记录/51-wal-repair-core.md
代码讲解记录/52-version-25-tests-docs.md
```

第二十五版删除：

```text
无源码文件删除
```

---

# 35. 第二十五版之后当前能力总览

到第二十五版为止，mini-kv 已经具备：

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
启动期 WAL repair
repair 后 WAL 重写为干净 WAL2
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

# 36. 第二十五版和前几版的关系

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
```

第二十五版：

```text
WAL repair
启动时 replay 可恢复记录
再 compact 成干净 WAL2
```

也就是从：

```text
能发现问题
```

推进到：

```text
能清理问题
```

---

# 37. 第二十五版之后适合继续做什么

WAL 方向：

```text
WAL 文件大小统计
启动时输出 wal_bytes
运行期 COMPACT 前后输出 saved bytes
WAL 过大时提示用户执行 COMPACT
自动 compact 阈值
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
2. Add WAL size reporting and automatic compact hints for long-running logs.
```

---

# 38. 第二十五版归档的清理结论

本轮运行后清理结果是：

```text
临时目录 tmp-v25-run 已删除
server smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/25/图片/
归档说明保留在 a/25/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第五十二次总结

第二十五版配套工作证明了三件事：

```text
wal_tests
 -> repair 的 replay 统计、Store 状态、WAL 文件重写和二次 replay 干净性都有自动化测试

真实 CLI / server smoke
 -> --repair-wal 在两个启动入口都能修复 damaged WAL，并且修复后的数据可查询

README 和 a/25 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十五版把 WAL 恢复从“能跳过并报告损坏”推进到“启动时可主动 repair 并重写干净 WAL2”，并用单元测试、全量 CTest、真实 CLI smoke、真实 server smoke 和 a/25 归档证明新能力可用。
```
