# 第五十八次讲解：第二十八版测试、README、归档和整体增删改

第五十七次讲了第二十八版 WAL maintenance options 和 compact stats 的核心代码。
这次讲第二十八版配套内容：

```text
CMakeLists.txt
README.md
tests/wal_tests.cpp
a/28/解释/说明.md
a/28/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.28.0
WAL compact 阈值可以配置
WALINFO 可以输出 compact 阈值
WALINFO 可以输出 compact 统计
manual / auto / repair compact 分开计数
真实 CLI smoke 通过
真实 server + client smoke 通过
关键命令输出已经按 a/28 归档
```

---

# 1. 第二十八版的定位

第二十六版做的是：

```text
WALINFO
compact hint
```

第二十七版做的是：

```text
--auto-compact-wal
```

第二十八版做的是：

```text
可配置 compact thresholds
运行期 compact counters
```

也就是从：

```text
可以自动维护
```

推进到：

```text
维护策略可调，维护效果可看
```

---

# 2. CMakeLists.txt 版本号更新

第二十八版把版本号改成：

```cmake
project(mini_kv VERSION 0.28.0 LANGUAGES CXX)
```

本版没有新增 executable target。
仍然复用已有目标：

```text
minikv_cli
minikv_server
minikv_client
minikv_wal_tests
```

变化主要在：

```text
minikv 库新增 WalMaintenanceOptions / WalCompactionStats
CLI/server 新增 WAL compact 阈值参数
WALINFO 输出更多维护字段
wal_tests 新增自定义阈值和 compact 统计覆盖
```

---

# 3. wal_tests 验证 repair compact 统计

第二十八版在 repair 测试中新增：

```cpp
const auto maintenance = wal.maintenance_report(repaired_store);
assert(maintenance.compaction_stats.manual_compactions == 0);
assert(maintenance.compaction_stats.auto_compactions == 0);
assert(maintenance.compaction_stats.repair_compactions == 1);
assert(maintenance.compaction_stats.compacted_keys == 3);
assert(maintenance.compaction_stats.records_removed > 0);
assert(maintenance.compaction_stats.bytes_saved > 0);
```

这证明：

```text
repair() 做的 WAL 重写会计入 repair_compactions
不会误算成 manual_compactions
也不会误算成 auto_compactions
```

这是 v28 的一个重要修正。
因为 repair 内部也会重写 WAL，但来源和用户手动 `COMPACT` 不一样。

---

# 4. wal_tests 验证默认阈值没有变

在 WALINFO 测试中新增：

```cpp
assert(report.options.compact_min_records == 8);
assert(report.options.compact_record_ratio == 4);
assert(report.options.compact_min_bytes == 64 * 1024);
```

这证明不传新参数时，默认策略仍然是：

```text
min_records=8
record_ratio=4
min_bytes=65536
```

所以第二十八版保持向后兼容。

---

# 5. wal_tests 验证手动 COMPACT 统计

执行：

```cpp
result = processor.execute("COMPACT");
assert(result.response == "OK compacted 1");
```

之后新增断言：

```cpp
assert(report.compaction_stats.manual_compactions == 1);
assert(report.compaction_stats.auto_compactions == 0);
assert(report.compaction_stats.repair_compactions == 0);
assert(report.compaction_stats.compacted_keys == 1);
assert(report.compaction_stats.records_removed == 9);
assert(report.compaction_stats.bytes_saved > 0);
```

场景是同一个 key 写了 10 次，compact 后只剩 1 条。

所以：

```text
records_removed=9
```

这证明手动 compact 的收益统计正确。

---

# 6. wal_tests 验证自动 compact 统计

第二十七版已有自动 compact 测试：

```text
同一个 key 写 8 次
默认阈值触发自动 compact
```

第二十八版新增统计断言：

```cpp
assert(report.compaction_stats.manual_compactions == 0);
assert(report.compaction_stats.auto_compactions == 1);
assert(report.compaction_stats.repair_compactions == 0);
assert(report.compaction_stats.compacted_keys == 1);
assert(report.compaction_stats.records_removed == 7);
assert(report.compaction_stats.bytes_saved > 0);
```

因为写了 8 条，compact 后剩 1 条：

```text
records_removed=7
```

这证明自动 compact 的计数和收益也正确。

---

# 7. wal_tests 验证自定义阈值

新增测试文件：

```cpp
const auto custom_compact_path =
    std::filesystem::temp_directory_path() /
    ("minikv-wal-custom-compact-test-" + std::to_string(suffix) + ".wal");
```

自定义阈值：

```cpp
minikv::WalMaintenanceOptions maintenance_options;
maintenance_options.compact_min_records = 4;
maintenance_options.compact_record_ratio = 2;
maintenance_options.compact_min_bytes = 1024 * 1024;
```

构造 WAL：

```cpp
minikv::WriteAheadLog wal{custom_compact_path, maintenance_options};
```

这证明 `WriteAheadLog` 的 options 构造函数可用。

---

# 8. 为什么自定义阈值写 4 次就触发

测试连续执行：

```cpp
for (int index = 0; index < 4; ++index) {
    processor.execute("SET tuned value" + std::to_string(index));
}
```

当前状态：

```text
records=4
live_keys=1
compact_min_records=4
compact_record_ratio=2
expected_records=1 * 2 = 2
```

满足：

```text
records >= compact_min_records
records > expected_records
```

所以第 4 次写入后会自动 compact。

---

# 9. wal_tests 验证 WALINFO 输出自定义阈值

测试执行：

```cpp
auto result = processor.execute("WALINFO");
```

然后断言：

```cpp
assert(result.response.find("compact_min_records=4") != std::string::npos);
assert(result.response.find("compact_record_ratio=2") != std::string::npos);
assert(result.response.find("compact_min_bytes=1048576") != std::string::npos);
```

这证明命令层输出和 WAL options 一致。

---

# 10. wal_tests 验证 WALINFO 输出统计

同一段测试还断言：

```cpp
assert(result.response.find("auto_compactions=1") != std::string::npos);
assert(result.response.find("records_removed=3") != std::string::npos);
```

写 4 条同 key，compact 后剩 1 条。
所以：

```text
records_removed=3
```

这证明 `WALINFO` 不是只输出阈值，也输出 compact 统计。

---

# 11. README 当前版本更新

README 当前版本从：

```text
Version 27 is a runnable in-memory KV service with optional automatic WAL compaction:
```

改成：

```text
Version 28 is a runnable in-memory KV service with configurable WAL compaction maintenance:
```

关键词是：

```text
configurable WAL compaction maintenance
```

说明本版重点是“可配置维护”。

---

# 12. README 能力列表新增 thresholds 和 counters

能力列表新增：

```text
WAL compaction thresholds are configurable, and `WALINFO` reports compact counters and saved work
```

这句话对应两个代码新增：

```text
WalMaintenanceOptions
WalCompactionStats
```

也对应用户可见输出：

```text
compact_min_records
compact_record_ratio
compact_min_bytes
manual_compactions
auto_compactions
repair_compactions
records_removed
bytes_saved
```

---

# 13. README Run 章节新增 CLI 示例

新增：

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal --auto-compact-wal --wal-compact-min-records 4 --wal-compact-record-ratio 2 --wal-compact-min-bytes 1048576
```

这展示了：

```text
CLI 可以开启自动 compact
CLI 可以调 compact 阈值
```

---

# 14. README Run 章节新增 server 示例

新增：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal --auto-compact-wal --wal-compact-min-records 4 --wal-compact-record-ratio 2 --wal-compact-min-bytes 1048576
```

这展示了：

```text
server 也支持同样的 WAL compact 阈值参数
```

CLI 和 server 保持一致，用户不需要记两套参数名。

---

# 15. README TCP server options 更新

server 参数说明扩展为：

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--auto-compact-wal]
                  [--wal-compact-min-records count] [--wal-compact-record-ratio ratio]
                  [--wal-compact-min-bytes bytes] [--max-request-bytes bytes]
                  [--accept-poll-ms ms]
```

它把 WAL 维护参数和 TCP 参数都列出来。

WAL 维护参数：

```text
--repair-wal
--auto-compact-wal
--wal-compact-min-records
--wal-compact-record-ratio
--wal-compact-min-bytes
```

TCP 参数：

```text
--max-request-bytes
--accept-poll-ms
```

---

# 16. README WAL persistence 章节更新

WAL 章节新增说明：

```text
Startup also reports WAL bytes, physical record count, live keys, compact thresholds,
compaction counters, and whether compaction is recommended;
use `WALINFO` at runtime to inspect the same maintenance signal.
```

还新增：

```text
Use `--wal-compact-min-records`, `--wal-compact-record-ratio`,
and `--wal-compact-min-bytes` with a WAL path to tune those thresholds.
```

这明确说明：

```text
阈值参数必须搭配 WAL path
```

---

# 17. README WALINFO 字段列表

README 新增：

```text
wal_bytes wal_records live_keys compact_recommended
compact_min_records compact_record_ratio compact_min_bytes
manual_compactions auto_compactions repair_compactions
compacted_keys records_removed bytes_saved
```

这相当于给 `WALINFO` 输出做了字段索引。

用户看到一长串 `key=value` 时，可以回到 README 查每类字段。

---

# 18. README 测试说明更新

`wal_tests` 描述新增：

```text
configurable compact thresholds
compact counters
```

说明 WAL 测试现在覆盖：

```text
默认阈值
自定义阈值
手动 compact 统计
自动 compact 统计
repair compact 统计
compact 收益统计
```

---

# 19. README Roadmap 更新

第二十七版 Roadmap 里有：

```text
Add configurable WAL compaction thresholds and runtime compaction counters.
```

第二十八版已经完成。
所以 Roadmap 更新为：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add runtime server health and WAL maintenance metrics commands.
```

也就是下一步可以继续做：

```text
HEALTH
STATS
METRICS
```

这类运行期观测命令。

---

# 20. a/28 归档目录

第二十八版新增归档：

```text
a/28/图片/
a/28/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-wal-tests.png
05-cli-custom-thresholds-smoke.png
06-server-custom-thresholds-smoke.png
07-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 21. 01-cmake-configure.png

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
CMake 能识别 0.28.0
当前构建目录可以继续生成
```

---

# 22. 02-cmake-build.png

对应命令：

```text
cmake --build cmake-build-debug --clean-first
```

结果：

```text
先清理旧构建产物
再完成 38 个 Ninja 构建步骤
```

意义：

```text
从干净状态确认 v28 所有代码能编译链接
```

重点覆盖：

```text
WalMaintenanceOptions
WalCompactionStats
扩展后的 WALINFO
CLI/server 新阈值参数
wal_tests 新增覆盖
```

---

# 23. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

意义：

```text
v28 的阈值配置和 compact 统计没有破坏已有功能
```

---

# 24. 04-wal-tests.png

对应命令：

```text
.\cmake-build-debug\minikv_wal_tests.exe
```

结果：

```text
minikv_wal_tests.exe: PASS
```

意义：

```text
WAL 自定义阈值、manual/auto/repair compact counters、records_removed 和 bytes_saved 都通过专项验证
```

---

# 25. 05-cli-custom-thresholds-smoke.png

这是 CLI 真实运行 smoke。

命令使用：

```text
--auto-compact-wal
--wal-compact-min-records 4
--wal-compact-record-ratio 2
--wal-compact-min-bytes 1048576
```

输入流程：

```text
SET tuned value0
SET tuned value1
SET tuned value2
SET tuned value3
WALINFO
GET tuned
COMPACT
WALINFO
EXIT
```

关键输出：

```text
compact_min_records=4
compact_record_ratio=2
compact_min_bytes=1048576
auto_compactions=1
records_removed=3
bytes_saved=120
value3
manual_compactions=1
```

这证明：

```text
CLI 自定义阈值生效
运行期自动 compact 生效
WALINFO 能看到统计
手动 COMPACT 后 manual_compactions 会增加
```

---

# 26. 06-server-custom-thresholds-smoke.png

这是 server + client 真实运行 smoke。

server 启动时使用：

```text
--auto-compact-wal
--wal-compact-min-records 4
--wal-compact-record-ratio 2
--wal-compact-min-bytes 1048576
--accept-poll-ms 25
```

client 输入：

```text
SET srv value0
SET srv value1
SET srv value2
SET srv value3
WALINFO
GET srv
QUIT
```

server 关键日志：

```text
event=wal_stats ...
compact_min_records=4
compact_record_ratio=2
compact_min_bytes=1048576
auto_compact_wal=true
```

client 关键输出：

```text
auto_compactions=1
records_removed=3
bytes_saved=114
value3
```

最终 WAL 文件只剩：

```text
SET srv value3
```

这证明 TCP 请求路径也使用同一套 WAL maintenance options 和 compact stats。

---

# 27. 07-version-docs-check.png

对应检查：

```text
git diff --check
CMakeLists.txt 版本号为 0.28.0
README Version 28
README --wal-compact-min-records
README compact counters
wal.hpp WalMaintenanceOptions
wal.hpp WalCompactionStats
command.cpp compact_min_records
main.cpp --wal-compact-min-records
server_main.cpp --wal-compact-min-records
wal_tests custom_compact_path
```

结果：

```text
全部 PASS
```

意义：

```text
代码、文档、入口参数、测试和归档一致
```

---

# 28. 本次代码增删改总览

第二十八版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.28.0

README.md
 -> 当前版本说明更新为 Version 28
 -> 能力列表新增 configurable WAL compaction maintenance
 -> Run 章节新增 CLI / server 自定义 compact 阈值示例
 -> TCP server options 新增三个 WAL compact 阈值参数
 -> WAL persistence 章节说明 thresholds、counters 和 WALINFO 字段
 -> wal_tests 说明新增 configurable compact thresholds 和 compact counters
 -> Roadmap 推进到 runtime server health 和 WAL maintenance metrics commands

include/minikv/wal.hpp
 -> 新增 WalMaintenanceOptions
 -> 新增 WalCompactionStats
 -> WalMaintenanceReport 新增 options 和 compaction_stats
 -> WriteAheadLog 构造函数支持传入 WalMaintenanceOptions

src/wal.cpp
 -> 新增 CompactionKind
 -> 新增 normalize_options
 -> should_recommend_compaction 改用 WalMaintenanceOptions
 -> make_maintenance_report 返回 options 和 compaction stats
 -> 新增 saved_bytes / removed_records
 -> 新增 record_compaction
 -> manual / automatic / repair compact 分开统计

src/command.cpp
 -> WALINFO 输出 compact_min_records / compact_record_ratio / compact_min_bytes
 -> WALINFO 输出 manual_compactions / auto_compactions / repair_compactions
 -> WALINFO 输出 compacted_keys / records_removed / bytes_saved

src/main.cpp
 -> CLI 新增 --wal-compact-min-records
 -> CLI 新增 --wal-compact-record-ratio
 -> CLI 新增 --wal-compact-min-bytes
 -> CLI 校验自定义阈值必须搭配 WAL path
 -> CLI 启动 WAL stats 输出阈值和统计

src/server_main.cpp
 -> server 新增 --wal-compact-min-records
 -> server 新增 --wal-compact-record-ratio
 -> server 新增 --wal-compact-min-bytes
 -> server 校验自定义阈值必须搭配 WAL path
 -> event=wal_stats 输出阈值和统计

tests/wal_tests.cpp
 -> 覆盖默认阈值
 -> 覆盖 repair_compactions
 -> 覆盖 manual_compactions
 -> 覆盖 auto_compactions
 -> 覆盖 custom compact thresholds
 -> 覆盖 records_removed 和 bytes_saved
```

第二十八版新增：

```text
a/28/图片/01-cmake-configure.png
a/28/图片/02-cmake-build.png
a/28/图片/03-ctest.png
a/28/图片/04-wal-tests.png
a/28/图片/05-cli-custom-thresholds-smoke.png
a/28/图片/06-server-custom-thresholds-smoke.png
a/28/图片/07-version-docs-check.png
a/28/解释/说明.md
代码讲解记录/57-wal-maintenance-options-stats.md
代码讲解记录/58-version-28-tests-docs.md
```

第二十八版删除：

```text
无源码文件删除
```

---

# 29. 第二十八版之后当前能力总览

到第二十八版为止，mini-kv 在 WAL 方向已经具备：

```text
WAL append
WAL replay
WAL2 checksum
旧 WAL 兼容 replay
checksum mismatch 跳过
truncated tail 跳过
WalReplayReport
manual COMPACT
startup --repair-wal
WalMaintenanceReport
WALINFO
startup WAL stats
compact hint
optional --auto-compact-wal
startup auto compact
runtime auto compact after WAL-backed writes
WalMaintenanceOptions
configurable compact thresholds
WalCompactionStats
manual / auto / repair compact counters
records_removed
bytes_saved
```

这条线的演进是：

```text
可持久化
 -> 可恢复
 -> 可校验
 -> 可压缩
 -> 可修复
 -> 可观测
 -> 可自动维护
 -> 可配置、可计量
```

---

# 30. 第二十八版之后适合继续做什么

运行期观测方向：

```text
STATS 命令
HEALTH 命令
server uptime
connection stats 查询
WAL maintenance metrics 查询
```

WAL 维护方向：

```text
WALINFO 展示最近一次 compact 的 before / after
server 运行期 auto compact 输出结构化事件
compact 失败计数
```

客户端体验方向：

```text
交互式行编辑
方向键历史浏览
命令补全
```

---

# 31. 第二十八版归档的清理结论

本轮运行后清理结果是：

```text
临时目录 tmp-v28-run 已删除
server smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli / minikv_wal_tests 残留进程
关键截图保留在 a/28/图片/
归档说明保留在 a/28/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第五十八次总结

第二十八版配套工作证明了三件事：

```text
wal_tests
 -> 默认阈值、自定义阈值、manual/auto/repair compact counters 和 compact 收益统计都有自动化覆盖

真实 CLI / server smoke
 -> CLI 和 server 都能使用自定义阈值，并在 WALINFO 中看到 compact counters

README 和 a/28 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十八版把 WAL maintenance 从“自动 compact 可用”，推进到“compact 策略可配置、compact 效果可观察”，并用单元测试、全量 CTest、真实 CLI smoke、真实 server smoke 和 a/28 归档证明新能力可用。
```

