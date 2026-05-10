# 第五十四次讲解：第二十六版测试、README、归档和整体增删改

第五十三次讲了第二十六版 WAL maintenance 核心。

这次讲第二十六版配套内容：

```text
CMakeLists.txt
README.md
tests/wal_tests.cpp
tests/command_tests.cpp
a/26/解释/说明.md
a/26/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.26.0
WALINFO 命令可用
CLI 启动会输出 WAL stats / WAL hint
server 启动会输出 event=wal_stats / event=wal_compact_hint
WAL 长历史会推荐 COMPACT
COMPACT 后推荐会消失
真实 CLI 和真实 server 都验证通过
关键命令输出已经按 a/26 归档
```

---

# 1. 第二十六版的定位

第二十四版做的是：

```text
COMPACT
```

第二十五版做的是：

```text
--repair-wal
```

第二十六版做的是：

```text
WAL maintenance reporting
```

它不直接自动 compact。

它先给出：

```text
WAL 文件状态
是否建议 compact
```

让用户有依据地执行维护。

---

# 2. CMakeLists.txt 版本号更新

第二十六版把版本号改成：

```cmake
project(mini_kv VERSION 0.26.0 LANGUAGES CXX)
```

本版没有新增 executable target。

仍然复用已有：

```text
minikv_cli
minikv_server
minikv_client
minikv_wal_tests
minikv_command_tests
```

变化在：

```text
minikv 库新增 maintenance_report
CommandProcessor 新增 WALINFO
CLI/server 启动日志新增 WAL 维护输出
```

---

# 3. command_tests 覆盖 WALINFO 参数错误

`tests/command_tests.cpp` 新增：

```cpp
result = processor.execute("WALINFO extra");
assert(result.response == "ERR usage: WALINFO");
```

这证明：

```text
WALINFO 不接受参数
```

和 `SIZE`、`COMPACT` 一样，维护查询命令必须是单独一条。

---

# 4. command_tests 覆盖无 WAL 错误

继续：

```cpp
result = processor.execute("WALINFO");
assert(result.response == "ERR WAL not enabled");
```

这里的 `processor` 是纯内存模式：

```cpp
minikv::CommandProcessor processor{store};
```

所以没有 WAL path。

没有 WAL 文件，就不能报告：

```text
wal_bytes
wal_records
```

返回 `ERR WAL not enabled` 是合理的。

---

# 5. command_tests 检查 HELP

新增：

```cpp
result = processor.execute("HELP");
assert(result.response.find("COMPACT") != std::string::npos);
assert(result.response.find("WALINFO") != std::string::npos);
```

这保证帮助文本里同时出现：

```text
COMPACT
WALINFO
```

一个负责维护动作，一个负责维护观察。

---

# 6. wal_tests 新增 info_path

`tests/wal_tests.cpp` 新增：

```cpp
const auto info_path =
    std::filesystem::temp_directory_path() / ("minikv-wal-info-test-" + std::to_string(suffix) + ".wal");
std::filesystem::remove(info_path);
```

这是专门给 WALINFO 和 maintenance_report 用的测试文件。

它和前面的：

```text
normal WAL
corrupt WAL
checksum WAL
compact WAL
repair WAL
```

分开，避免场景互相污染。

---

# 7. wal_tests 构造长历史

测试里：

```cpp
for (int index = 0; index < 10; ++index) {
    auto result = processor.execute("SET hot value" + std::to_string(index));
    assert(result.response.rfind("OK ", 0) == 0);
}
```

这会对同一个 key：

```text
hot
```

写 10 次。

WAL 文件有：

```text
10 条 SET 记录
```

但 Store 当前只有：

```text
1 个 live key
```

这正好触发 compact hint。

---

# 8. WALINFO 第一次断言

执行：

```cpp
auto result = processor.execute("WALINFO");
```

断言：

```cpp
assert(result.response.find("wal_records=10") != std::string::npos);
assert(result.response.find("live_keys=1") != std::string::npos);
assert(result.response.find("compact_recommended=yes") != std::string::npos);
```

这验证命令层输出。

它不写死 `wal_bytes` 数值。

原因是具体字节数和 WAL2 checksum 字符串、换行有关，不需要在单测里绑定死。

---

# 9. maintenance_report 第一次断言

测试还直接调用：

```cpp
auto report = wal.maintenance_report(store);
```

断言：

```cpp
assert(report.records == 10);
assert(report.live_keys == 1);
assert(report.bytes > 0);
assert(report.compact_recommended);
```

这证明：

```text
WAL 模块 API
```

和：

```text
CommandProcessor 的 WALINFO 输出
```

是一致的。

---

# 10. 执行 COMPACT

测试继续：

```cpp
result = processor.execute("COMPACT");
assert(result.response == "OK compacted 1");
```

因为当前只有一个 live key：

```text
hot
```

所以 compacted key 数量是：

```text
1
```

---

# 11. WALINFO 第二次断言

COMPACT 后再次：

```cpp
result = processor.execute("WALINFO");
```

断言：

```cpp
assert(result.response.find("wal_records=1") != std::string::npos);
assert(result.response.find("live_keys=1") != std::string::npos);
assert(result.response.find("compact_recommended=no") != std::string::npos);
```

这证明：

```text
compact 后长历史已消失
```

维护提示也随之消失。

---

# 12. maintenance_report 第二次断言

最后：

```cpp
report = wal.maintenance_report(store);
assert(report.records == 1);
assert(report.live_keys == 1);
assert(report.bytes > 0);
assert(!report.compact_recommended);
```

这再次确认：

```text
WAL 文件实际状态
```

已经从：

```text
10 records / 1 live key / recommended
```

变成：

```text
1 record / 1 live key / not recommended
```

---

# 13. README 当前版本更新

README 当前版本从：

```text
Version 25 is a runnable in-memory KV service with startup WAL repair:
```

改成：

```text
Version 26 is a runnable in-memory KV service with WAL maintenance reporting:
```

关键词是：

```text
WAL maintenance reporting
```

说明本版重点是维护观测。

---

# 14. README 能力列表新增 maintenance

能力列表新增：

```text
WAL maintenance reporting exposes log bytes, record count, live keys, and compact hints
```

这句话对应 `WalMaintenanceReport` 的四个字段：

```text
bytes
records
live_keys
compact_recommended
```

---

# 15. README 命令列表新增 WALINFO

CLI commands 新增：

```text
WALINFO
```

位置在：

```text
COMPACT
WALINFO
HELP
EXIT
```

这表达：

```text
COMPACT 是动作
WALINFO 是观察
```

---

# 16. README TCP 示例新增 WALINFO

TCP protocol 示例也新增：

```text
WALINFO
```

因为 TCP server 也走同一个：

```text
CommandProcessor
```

所以 inline TCP 和 RESP TCP 都可以执行 WALINFO。

---

# 17. README WAL persistence 章节更新

WAL 章节新增：

```text
Startup also reports WAL bytes, physical record count, live keys,
and whether compaction is recommended;
use `WALINFO` at runtime to inspect the same maintenance signal.
```

这说明：

```text
启动时会自动报告
运行中可以用 WALINFO 再查
```

---

# 18. README 测试说明更新

`wal_tests` 描述新增：

```text
WAL maintenance hints
```

说明 WAL 测试现在覆盖：

```text
WALINFO
maintenance_report
compact_recommended yes/no
COMPACT 后提示消失
```

---

# 19. README Roadmap 调整

第二十五版 Roadmap 里还有：

```text
Add WAL size reporting and automatic compact hints for long-running logs.
```

第二十六版已经完成：

```text
WAL size reporting
compact hints
```

所以 Roadmap 改成：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add optional automatic WAL compaction based on maintenance thresholds.
```

也就是下一步可以从：

```text
提示用户 compact
```

推进到：

```text
满足阈值时自动 compact
```

---

# 20. a/26 归档目录

第二十六版新增归档：

```text
a/26/图片/
a/26/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-wal-tests.png
05-cli-walinfo-smoke.png
06-server-walinfo-smoke.png
07-version-docs-check.png
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
CMake 能识别 0.26.0
当前构建目录可以继续生成
```

---

# 22. 02-cmake-build.png

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
WalMaintenanceReport 能编译
WriteAheadLog::maintenance_report 能编译
WALINFO 能编译
CLI/server WAL stats 日志能编译
新增测试能编译
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
WAL maintenance 改动通过
前面二十五版积累的能力没有回归
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
minikv_wal_tests.exe: PASS
```

它重点证明：

```text
WALINFO 能看到记录数和 live key
长历史会 compact_recommended=yes
COMPACT 后 compact_recommended=no
maintenance_report API 与 WALINFO 命令一致
旧 WAL checksum / compact / repair 覆盖仍然通过
```

---

# 25. 05-cli-walinfo-smoke.png

第五张图是真实 CLI smoke。

流程是：

```text
启动 minikv_cli.exe <wal_path>
连续 10 次 SET hot valueN
执行 WALINFO
执行 COMPACT
再次执行 WALINFO
EXIT
```

关键输出：

```text
WAL stats: bytes=0 records=0 live_keys=0 compact_recommended=no
wal_bytes=380 wal_records=10 live_keys=1 compact_recommended=yes
OK compacted 1
wal_bytes=38 wal_records=1 live_keys=1 compact_recommended=no
```

最终 WAL 文件只剩：

```text
WAL2 ... SET hot value9
```

---

# 26. 06-server-walinfo-smoke.png

第六张图是真实 server smoke。

流程是：

```text
先用 CLI 生成长 WAL
启动 minikv_server.exe <port> 127.0.0.1 <wal_path> --accept-poll-ms 25
检查 server 启动日志
用 minikv_client.exe 连接 server
执行 WALINFO / COMPACT / WALINFO / GET srv / QUIT
停止 server
```

server 启动日志包含：

```text
event=wal_stats ... wal_records=10 live_keys=1 compact_recommended=true
event=wal_compact_hint ...
```

客户端输出包含：

```text
wal_records=10 ... compact_recommended=yes
OK compacted 1
wal_records=1 ... compact_recommended=no
value9
```

这证明 server 启动日志和 TCP 命令路径都可用。

---

# 27. 07-version-docs-check.png

第七张图对应版本和文档一致性检查。

检查项包括：

```text
git diff --check: PASS
CMakeLists.txt 版本号为 0.26.0
README 当前版本说明为 Version 26
README 命令列表包含 WALINFO
README WAL persistence 章节说明启动 stats 和 WALINFO
include/minikv/wal.hpp 暴露 WalMaintenanceReport 和 maintenance_report
src/command.cpp 包含 WALINFO
src/main.cpp 包含 WAL stats / WAL hint
src/server_main.cpp 包含 event=wal_stats / event=wal_compact_hint
tests/wal_tests.cpp / tests/command_tests.cpp 包含 WALINFO 覆盖
```

意义是：

```text
代码版本、用户文档、API 入口、命令入口、启动日志和测试覆盖同步
```

---

# 28. 本次代码增删改总览

第二十六版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.26.0

README.md
 -> 当前版本说明更新为 Version 26
 -> 能力列表新增 WAL maintenance reporting
 -> CLI / TCP 命令列表新增 WALINFO
 -> WAL persistence 章节说明启动 stats 和 WALINFO
 -> 测试说明新增 WAL maintenance hints
 -> Roadmap 移除 WAL size reporting，推进到 automatic compaction

include/minikv/wal.hpp
 -> 新增 WalMaintenanceReport
 -> 新增 WriteAheadLog::maintenance_report 声明

src/wal.cpp
 -> 新增 compact hint 阈值
 -> 新增 file_size_or_zero
 -> 新增 count_wal_records
 -> 新增 should_recommend_compaction
 -> 新增 WriteAheadLog::maintenance_report 实现

src/command.cpp
 -> 新增 format_walinfo
 -> 新增 WALINFO 命令
 -> HELP 新增 WALINFO

src/main.cpp
 -> 新增 CLI WAL stats 输出
 -> compact_recommended=yes 时输出 WAL hint

src/server_main.cpp
 -> 新增 event=wal_stats
 -> compact_recommended=true 时输出 event=wal_compact_hint

tests/command_tests.cpp
 -> 覆盖 WALINFO 参数错误
 -> 覆盖无 WAL 模式错误
 -> 覆盖 HELP 包含 WALINFO

tests/wal_tests.cpp
 -> 覆盖长 WAL 历史触发 compact hint
 -> 覆盖 COMPACT 后 hint 消失
 -> 覆盖 maintenance_report API 和 WALINFO 命令一致
```

第二十六版新增：

```text
a/26/图片/01-cmake-configure.png
a/26/图片/02-cmake-build.png
a/26/图片/03-ctest.png
a/26/图片/04-wal-tests.png
a/26/图片/05-cli-walinfo-smoke.png
a/26/图片/06-server-walinfo-smoke.png
a/26/图片/07-version-docs-check.png
a/26/解释/说明.md
代码讲解记录/53-wal-maintenance-core.md
代码讲解记录/54-version-26-tests-docs.md
```

第二十六版删除：

```text
无源码文件删除
```

---

# 29. 第二十六版之后当前能力总览

到第二十六版为止，mini-kv 已经具备：

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
WALINFO 维护查询
启动期 WAL stats / compact hint
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

# 30. 第二十六版和前几版的关系

第二十三版：

```text
WAL2 checksum
```

第二十四版：

```text
COMPACT
```

第二十五版：

```text
--repair-wal
```

第二十六版：

```text
WALINFO
event=wal_stats
event=wal_compact_hint
```

也就是从：

```text
WAL 更可靠
```

推进到：

```text
WAL 可维护
```

再推进到：

```text
WAL 维护状态可观测
```

---

# 31. 第二十六版之后适合继续做什么

WAL 方向：

```text
可配置 compact hint 阈值
自动 compact
COMPACT 前后输出节省字节数
server 定期输出 WAL stats
WALINFO 增加 skipped / checksum_failed 历史摘要
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
2. Add optional automatic WAL compaction based on maintenance thresholds.
```

---

# 32. 第二十六版归档的清理结论

本轮运行后清理结果是：

```text
临时目录 tmp-v26-run 已删除
server smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli 残留进程
关键截图保留在 a/26/图片/
归档说明保留在 a/26/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第五十四次总结

第二十六版配套工作证明了三件事：

```text
wal_tests / command_tests
 -> WALINFO、maintenance_report、compact hint 和 COMPACT 后 hint 消失都有自动化覆盖

真实 CLI / server smoke
 -> CLI 和 server 都能报告 WAL stats，并能通过 WALINFO / COMPACT 验证提示变化

README 和 a/26 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十六版把 WAL 维护从“用户知道可以 COMPACT”推进到“系统会告诉用户当前 WAL 是否值得 COMPACT”，并用单元测试、全量 CTest、真实 CLI smoke、真实 server smoke 和 a/26 归档证明新能力可用。
```
