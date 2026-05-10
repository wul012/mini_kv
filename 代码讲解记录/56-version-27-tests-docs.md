# 第五十六次讲解：第二十七版测试、README、归档和整体增删改

第五十五次讲了第二十七版 WAL auto compact 的核心代码。
这次讲第二十七版配套内容：

```text
CMakeLists.txt
README.md
tests/wal_tests.cpp
a/27/解释/说明.md
a/27/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.27.0
--auto-compact-wal 可用于 CLI
--auto-compact-wal 可用于 server
启动期可以自动 compact 长 WAL
运行期 WAL 写命令可以自动 compact 长 WAL
真实 CLI smoke 通过
真实 server + client smoke 通过
关键命令输出已经按 a/27 归档
```

---

# 1. 第二十七版的定位

第二十四版做的是：

```text
COMPACT
```

用户可以手动把长 WAL 重写成短 WAL。

第二十六版做的是：

```text
WALINFO
compact hint
```

系统可以告诉用户什么时候值得 compact。

第二十七版做的是：

```text
--auto-compact-wal
```

用户开启后，系统可以自动 compact。

它的定位是：

```text
WAL 维护自动化的第一步
```

---

# 2. CMakeLists.txt 版本号更新

第二十七版把版本号改成：

```cmake
project(mini_kv VERSION 0.27.0 LANGUAGES CXX)
```

本版没有新增 executable target。
仍然复用已有目标：

```text
minikv_cli
minikv_server
minikv_client
minikv_wal_tests
```

变化在核心库和入口参数：

```text
minikv 库新增 compact_if_recommended
CLI 新增 --auto-compact-wal
server 新增 --auto-compact-wal
wal_tests 新增自动 compact 覆盖
```

---

# 3. wal_tests 覆盖“不需要 compact”

在第二十六版 WALINFO 测试的后半段，已经手动执行过：

```text
COMPACT
```

此时 WAL 状态已经是：

```text
wal_records=1
live_keys=1
compact_recommended=no
```

第二十七版继续增加：

```cpp
minikv::WalAutoCompactReport skipped_compact;
assert(wal.compact_if_recommended(store, &skipped_compact));
assert(!skipped_compact.compacted);
assert(skipped_compact.before.records == 1);
assert(skipped_compact.after.records == 1);
```

这证明：

```text
当 compact_recommended=false
compact_if_recommended 不会乱重写 WAL
```

也就是说，自动 compact 不是每次都 compact。
它只在维护阈值建议 compact 时动作。

---

# 4. wal_tests 构造运行期自动 compact 场景

测试新增一个独立 WAL 文件：

```cpp
const auto auto_compact_path =
    std::filesystem::temp_directory_path() /
    ("minikv-wal-auto-compact-test-" + std::to_string(suffix) + ".wal");
```

然后打开命令层选项：

```cpp
minikv::CommandProcessorOptions options;
options.auto_compact_wal = true;
minikv::CommandProcessor processor{store, &wal, options};
```

这一步很关键。
它不是直接调用 `wal.compact_if_recommended()`。
而是模拟真实运行期：

```text
用户执行写命令
 -> CommandProcessor append WAL
 -> CommandProcessor 修改 Store
 -> CommandProcessor 自动检查 compact
```

---

# 5. 为什么循环写 8 次

测试执行：

```cpp
for (int index = 0; index < 8; ++index) {
    auto result = processor.execute("SET hot value" + std::to_string(index));
    assert(result.response.rfind("OK ", 0) == 0);
}
```

第二十六版的 compact hint 阈值里有：

```text
compact_hint_min_records = 8
compact_hint_record_ratio = 4
```

连续写同一个 key 8 次后：

```text
records=8
live_keys=1
expected_records=1 * 4 = 4
```

满足：

```text
records >= 8
records > expected_records
```

所以第八次写入后会触发自动 compact。

---

# 6. wal_tests 验证最终值没有丢

自动 compact 后，测试先查值：

```cpp
auto result = processor.execute("GET hot");
assert(result.response == "value7");
```

这证明：

```text
自动 compact 不会丢掉最新值
```

因为最后一次写入是：

```text
SET hot value7
```

compact 后短 WAL 应该只保留当前 live state。

---

# 7. wal_tests 验证 WALINFO 状态

接着执行：

```cpp
result = processor.execute("WALINFO");
assert(result.response.find("wal_records=1") != std::string::npos);
assert(result.response.find("live_keys=1") != std::string::npos);
assert(result.response.find("compact_recommended=no") != std::string::npos);
```

这证明：

```text
运行期自动 compact 后
WAL 记录数已经从 8 条缩到 1 条
并且不再建议 compact
```

最后直接调用 API 再确认一次：

```cpp
const auto report = wal.maintenance_report(store);
assert(report.records == 1);
assert(report.live_keys == 1);
assert(report.bytes > 0);
assert(!report.compact_recommended);
```

命令层和 WAL API 的结果一致。

---

# 8. README 当前版本更新

README 当前版本改成：

```text
Version 27 is a runnable in-memory KV service with optional automatic WAL compaction:
```

关键词是：

```text
optional automatic WAL compaction
```

说明本版重点不是强制自动维护，而是可选自动维护。

---

# 9. README 能力列表新增自动 compact

能力列表新增：

```text
Optional automatic WAL compaction can rewrite long-running logs at startup and after WAL-backed writes
```

这句话对应两类触发点：

```text
startup
 -> CLI / server replay WAL 后自动 compact

after WAL-backed writes
 -> SET / DEL / EXPIRE 成功写入 WAL 后自动 compact
```

---

# 10. README Run 章节新增 CLI 用法

新增：

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal --auto-compact-wal
```

表示 CLI 可以在 WAL 模式下开启自动 compact。

注意：

```text
--auto-compact-wal 必须搭配 WAL path
```

这和代码里的参数校验一致。

---

# 11. README Run 章节新增 server 用法

新增：

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal --auto-compact-wal
```

表示 TCP server 也能开启自动 compact。

server 启动后会在日志里显示：

```text
auto_compact_wal=true
```

如果启动期真的 compact，还会输出：

```text
event=wal_auto_compact
```

---

# 12. README TCP server options 更新

server 参数说明从：

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

更新为：

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--auto-compact-wal]
                  [--max-request-bytes bytes] [--accept-poll-ms ms]
```

这让用户能在同一个位置看到：

```text
--repair-wal
--auto-compact-wal
--max-request-bytes
--accept-poll-ms
```

分别对应 WAL 修复、WAL 自动维护、请求大小限制和 accept 轮询间隔。

---

# 13. README WAL persistence 章节更新

WAL 章节新增说明：

```text
Start with `--auto-compact-wal` to compact automatically when the same maintenance thresholds recommend it,
both after startup replay and after WAL-backed write commands.
```

这里强调两点：

```text
使用同一套 maintenance thresholds
同时覆盖 startup replay 后和 WAL-backed write 后
```

也就是说，自动 compact 没有另起一套判断标准。
它沿用第二十六版的 `compact_recommended`。

---

# 14. README 测试说明更新

`wal_tests` 描述新增：

```text
automatic WAL compaction
```

这说明 WAL 测试现在覆盖：

```text
WAL2 checksum
旧格式 replay
checksum mismatch
truncated tail
manual COMPACT
repair
maintenance hint
automatic compact
```

---

# 15. README Roadmap 调整

第二十六版 Roadmap 里还有：

```text
Add optional automatic WAL compaction based on maintenance thresholds.
```

第二十七版已经完成。
所以 Roadmap 改成：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add configurable WAL compaction thresholds and runtime compaction counters.
```

也就是下一步可以从：

```text
固定阈值
```

推进到：

```text
可配置阈值
运行期 compact 次数和节省字节统计
```

---

# 16. a/27 归档目录

第二十七版新增归档：

```text
a/27/图片/
a/27/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-wal-tests.png
05-cli-auto-compact-smoke.png
06-server-auto-compact-smoke.png
07-version-docs-check.png
```

解释目录包含：

```text
说明.md
```

---

# 17. 01-cmake-configure.png

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
CMake 能识别 0.27.0
当前构建目录可以继续生成
```

---

# 18. 02-cmake-build.png

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
从干净状态确认 v27 所有代码能编译链接
```

重点覆盖：

```text
WalAutoCompactReport
compact_if_recommended
CommandProcessorOptions
CLI --auto-compact-wal
server --auto-compact-wal
TcpServer runtime auto compact 传参
wal_tests 新增覆盖
```

---

# 19. 03-ctest.png

对应命令：

```text
ctest --test-dir cmake-build-debug --output-on-failure
```

结果：

```text
11 个测试全部通过
```

测试列表仍然是：

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
自动 compact 没有破坏前面二十六版积累的功能
```

---

# 20. 04-wal-tests.png

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
WAL 自动 compact 的直接单测通过
```

重点证明：

```text
不建议 compact 时不会误 compact
达到阈值后自动 compact
自动 compact 后 WALINFO 显示 wal_records=1
自动 compact 后最新值仍然存在
```

---

# 21. 05-cli-auto-compact-smoke.png

这是 CLI 真实运行 smoke。

流程一：

```text
不带 --auto-compact-wal
连续 10 次 SET hot valueN
制造长 WAL
```

流程二：

```text
带 --auto-compact-wal 启动 CLI
replay 10 条 WAL
启动期自动 compact
WALINFO
GET hot
```

关键输出：

```text
WAL auto compact: compacted=1 records=10->1 bytes=380->38
wal_records=1 live_keys=1 compact_recommended=no
value9
```

流程三：

```text
空 WAL 带 --auto-compact-wal 启动
连续 8 次 SET live valueN
运行期自动 compact
WALINFO
GET live
```

关键输出：

```text
wal_records=1 live_keys=1 compact_recommended=no
value7
```

最终两个 WAL 文件都只剩一条 WAL2 记录。

---

# 22. 06-server-auto-compact-smoke.png

这是 server + client 真实运行 smoke。

流程：

```text
先用 CLI 构造 10 条 srv key 的长 WAL
启动 minikv_server.exe ... --auto-compact-wal
用 minikv_client.exe 连接
执行 WALINFO
连续 7 次 SET srv liveN
再次 WALINFO
GET srv
QUIT
停止 server
```

server 关键日志：

```text
event=wal_auto_compact ... before_wal_records=10 after_wal_records=1
event=wal_stats ... wal_records=1 live_keys=1 compact_recommended=false
event=server_start ... auto_compact_wal=true
```

client 关键输出：

```text
wal_records=1 live_keys=1 compact_recommended=no
live6
BYE
```

最终 server WAL 文件只剩：

```text
SET srv live6
```

这证明：

```text
server 启动期自动 compact 可用
TCP 写命令路径也能保持短 WAL
```

---

# 23. 07-version-docs-check.png

对应检查：

```text
git diff --check
CMakeLists.txt 版本号
README Version 27
README --auto-compact-wal
wal.hpp WalAutoCompactReport
wal.hpp compact_if_recommended
command.hpp auto_compact_wal
main.cpp --auto-compact-wal
server_main.cpp event=wal_auto_compact
tcp_server.cpp auto_compact_wal
wal_tests auto_compact_wal
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

# 24. 本次代码增删改总览

第二十七版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.27.0

README.md
 -> 当前版本说明更新为 Version 27
 -> 能力列表新增 optional automatic WAL compaction
 -> Run 章节新增 CLI / server --auto-compact-wal 示例
 -> TCP server options 新增 --auto-compact-wal
 -> WAL persistence 章节说明启动期和写命令后自动 compact
 -> wal_tests 说明新增 automatic WAL compaction
 -> Roadmap 从 automatic compaction 推进到 configurable thresholds 和 runtime counters

include/minikv/wal.hpp
 -> 新增 WalAutoCompactReport
 -> 新增 WriteAheadLog::compact_if_recommended 声明

src/wal.cpp
 -> 新增 make_maintenance_report
 -> 新增 compact_wal_file
 -> compact 复用 compact_wal_file
 -> maintenance_report 复用 make_maintenance_report
 -> 新增 compact_if_recommended 实现

include/minikv/command.hpp
 -> 新增 CommandProcessorOptions
 -> CommandProcessor 构造函数接收 options
 -> 新增 auto_compact_wal_if_needed 声明

src/command.cpp
 -> 保存 CommandProcessorOptions
 -> 新增 auto_compact_wal_if_needed
 -> WAL 模式 SET / DEL / EXPIRE 成功后触发自动 compact 检查

include/minikv/tcp_server.hpp
 -> TcpServer::Options 新增 auto_compact_wal

src/tcp_server.cpp
 -> serve_client 接收 auto_compact_wal
 -> 每个连接构造 CommandProcessorOptions
 -> TcpServer::run 创建连接线程时传递 options_.auto_compact_wal

src/main.cpp
 -> CLI 解析 --auto-compact-wal
 -> 要求 auto compact 必须搭配 WAL path
 -> 启动 replay/repair 后执行 compact_if_recommended
 -> 交互命令层传入 auto_compact_wal option

src/server_main.cpp
 -> server 解析 --auto-compact-wal
 -> 要求 auto compact 必须搭配 WAL path
 -> 启动 replay/repair 后执行 compact_if_recommended
 -> 输出 event=wal_auto_compact 或 event=wal_auto_compact_skipped
 -> event=server_start 输出 auto_compact_wal=true/false

tests/wal_tests.cpp
 -> 覆盖不需要 compact 时 compact_if_recommended 不动作
 -> 覆盖 CommandProcessorOptions::auto_compact_wal 运行期自动 compact
```

第二十七版新增：

```text
a/27/图片/01-cmake-configure.png
a/27/图片/02-cmake-build.png
a/27/图片/03-ctest.png
a/27/图片/04-wal-tests.png
a/27/图片/05-cli-auto-compact-smoke.png
a/27/图片/06-server-auto-compact-smoke.png
a/27/图片/07-version-docs-check.png
a/27/解释/说明.md
代码讲解记录/55-wal-auto-compact-core.md
代码讲解记录/56-version-27-tests-docs.md
```

第二十七版删除：

```text
无源码文件删除
```

---

# 25. 第二十七版之后当前能力总览

到第二十七版为止，mini-kv 在 WAL 方向已经具备：

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
```

---

# 26. 第二十七版之后适合继续做什么

WAL 自动维护方向：

```text
把 compact 阈值做成 CLI/server 参数
记录 auto compact 次数
记录 compact 节省的字节数
server 运行期 auto compact 输出结构化日志或指标
WALINFO 展示最近一次 compact 信息
```

客户端体验方向：

```text
交互式行编辑
方向键历史浏览
命令补全
```

工程质量方向：

```text
格式检查
静态分析
Sanitizer CI
benchmark 输出更多指标
```

README 当前 Roadmap 已经推进到：

```text
1. Add interactive line editing for the bundled TCP client.
2. Add configurable WAL compaction thresholds and runtime compaction counters.
```

---

# 27. 第二十七版归档的清理结论

本轮运行后清理结果是：

```text
临时目录 tmp-v27-run 已删除
server smoke 启动的 minikv_server 已停止
没有 minikv_server / minikv_client / minikv_cli / minikv_wal_tests 残留进程
关键截图保留在 a/27/图片/
归档说明保留在 a/27/解释/说明.md
```

`git diff --check` 结果为：

```text
PASS
```

---

# 第五十六次总结

第二十七版配套工作证明了三件事：

```text
wal_tests
 -> compact_if_recommended 和运行期自动 compact 有自动化覆盖

真实 CLI / server smoke
 -> CLI 和 server 都能在 --auto-compact-wal 下自动把长 WAL 压成短 WAL

README 和 a/27 归档
 -> 用户说明、版本号、测试截图和验证解释已经同步
```

一句话总结：

```text
第二十七版把 WAL 维护从“能提示用户何时 COMPACT”，推进到“用户授权后系统能自动 COMPACT”，并用单元测试、全量 CTest、真实 CLI smoke、真实 server smoke 和 a/27 归档证明新能力可用。
```

