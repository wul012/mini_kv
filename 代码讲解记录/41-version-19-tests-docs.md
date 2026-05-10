# 第四十一次讲解：第十九版测试、README、归档和整体增删改

第四十次讲了第十九版 WAL replay report 和恢复加固核心。

这次讲第十九版配套内容：

```text
CMakeLists.txt
README.md
tests/wal_tests.cpp
tests/snapshot_tests.cpp
a/19/解释/说明.md
a/19/图片/
```

它们负责说明和证明：

```text
当前项目版本进入 0.19.0
WAL replay 能报告 applied / skipped / truncated
CLI 和 server 启动时能输出恢复报告
坏 WAL 不会污染 Store
坏 snapshot 不会替换现有 Store
关键命令输出已经按 a/19 归档
```

---

# 1. 第十九版的定位

第十六到第十八版主要在补 TCP / RESP：

```text
单连接成功 pipeline
单连接兼容边界
多连接并发
```

第十九版转向持久化可靠性：

```text
WAL replay report
坏 WAL 记录跳过
尾部半条 WAL 记录识别
坏 snapshot 不替换 Store
```

也就是从：

```text
网络协议更稳
```

转到：

```text
重启恢复更可观察、更保守
```

---

# 2. CMakeLists.txt 版本号更新

第十九版把版本号改成：

```cmake
project(mini_kv VERSION 0.19.0 LANGUAGES CXX)
```

本版没有新增新的测试 target。

原因是持久化相关测试已经存在：

```text
minikv_wal_tests
minikv_snapshot_tests
```

第十九版是在这两个测试里加场景。

所以构建目标数量不变。

---

# 3. README 当前版本更新

README 当前版本从：

```text
Version 18 is a runnable in-memory KV service with concurrent RESP-over-TCP client coverage:
```

改成：

```text
Version 19 is a runnable in-memory KV service with WAL and snapshot recovery hardening:
```

关键词是：

```text
WAL and snapshot recovery hardening
```

这说明第十九版的重点是持久化恢复，而不是协议或客户端功能。

---

# 4. README 能力列表新增项

第十九版新增：

```text
WAL replay reports applied, skipped, and truncated records
Snapshot load rejects corrupt files without replacing the current store
```

第一条对应：

```text
WalReplayReport
replay_with_report
CLI / server 启动输出
```

第二条对应：

```text
snapshot_tests 新增坏文件保护断言
```

---

# 5. README WAL 说明更新

WAL persistence 章节新增说明：

```text
WAL replay skips malformed records,
detects a final unterminated record as a probable partial write,
and reports applied, skipped, and truncated counts during startup.
```

拆开看：

```text
malformed records
 -> 坏记录跳过

final unterminated record
 -> 末尾无换行的记录视为疑似部分写入

reports counts
 -> 启动时输出 applied / skipped / truncated
```

这和第十九版代码行为一致。

---

# 6. README Snapshot 说明更新

Snapshots 章节新增：

```text
Corrupt snapshot files are rejected before replacing the current in-memory store.
```

它表达的是一个重要安全边界：

```text
只有 snapshot 文件完整解析成功
才会替换当前 Store
```

坏文件不会导致：

```text
当前内存数据被清空
当前内存数据被部分替换
loaded 输出参数被误改
```

---

# 7. README Roadmap 调整

第十八版 roadmap 里还有：

```text
Harden WAL and snapshot recovery for partial or corrupt files.
```

第十九版已经完成第一轮加固。

所以 Roadmap 调整为：

```text
1. Add persistent client history or interactive line editing.
2. Add CI coverage for build and CTest.
```

含义是：

```text
持久化恢复已有基础保护
下一步可以回到客户端体验或工程自动化
```

---

# 8. wal_tests 扩展

第十九版 `tests/wal_tests.cpp` 做了两类验证：

```text
正常 WAL 路径
坏 WAL / 尾部半条记录路径
```

正常路径继续验证：

```text
SET
DEL
EXPIRE
过期数据不会恢复
```

坏路径新增验证：

```text
GARBAGE record
EXPIREAT kept not-a-number
末尾无换行 SET partial value
```

---

# 9. wal_tests 的坏 WAL 预期

坏 WAL fixture 是：

```text
SET kept value
GARBAGE record
EXPIREAT kept not-a-number
SET partial value
```

最后一行没有换行。

预期：

```text
SET kept value
 -> applied

GARBAGE record
 -> skipped

EXPIREAT kept not-a-number
 -> skipped

SET partial value
 -> truncated + skipped
```

所以 report 应该是：

```text
applied_records = 1
skipped_records = 3
truncated_records = 1
```

---

# 10. snapshot_tests 扩展

第十九版 `tests/snapshot_tests.cpp` 保留正常 save / load。

然后新增坏 snapshot：

```text
MINIKV_SNAPSHOT 1
ENTRY - "broken"
```

这条 ENTRY 少了 value。

测试先准备一个已有 Store：

```text
existing -> survives
```

然后尝试 load 坏文件。

预期：

```text
load 返回 false
loaded 仍然是原来的 123
existing 还在
Store size 仍然是 1
```

---

# 11. a/19 归档目录

第十九版新增归档：

```text
a/19/图片/
a/19/解释/说明.md
```

图片目录包含：

```text
01-cmake-configure.png
02-cmake-build.png
03-ctest.png
04-persistence-tests.png
05-wal-replay-report-smoke.png
```

解释目录包含：

```text
说明.md
```

---

# 12. 01-cmake-configure.png

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
CMake 能识别 0.19.0
现有构建目标都能正常生成
```

---

# 13. 02-cmake-build.png

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
WalReplayReport 能编译
replay_with_report 能编译
CLI/server 启动日志调整能编译
WAL/Snapshot 测试扩展能编译
旧目标没有编译回归
```

---

# 14. 为什么还是 38 个构建步骤

第十九版没有新增 executable target。

只是修改：

```text
include/minikv/wal.hpp
src/wal.cpp
src/main.cpp
src/server_main.cpp
tests/wal_tests.cpp
tests/snapshot_tests.cpp
README.md
```

所以构建步骤数量仍然是：

```text
38
```

---

# 15. 03-ctest.png

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
持久化恢复加固通过
前面十八版积累的能力没有回归
```

---

# 16. 04-persistence-tests.png

第四张图对应命令：

```text
.\cmake-build-debug\minikv_wal_tests.exe
.\cmake-build-debug\minikv_snapshot_tests.exe
```

结果：

```text
两个测试都 PASS
退出码都是 0
```

这张图专门单独复跑持久化相关测试。

原因是第十九版主要改动就在：

```text
WAL
Snapshot 测试
```

---

# 17. 05-wal-replay-report-smoke.png

第五张图对应真实进程 smoke。

先写 WAL fixture：

```text
SET kept value
BROKEN record
SET partial value
```

最后一行没有换行。

然后启动：

```text
minikv_server.exe 6420 127.0.0.1 tmp-v19-run\corrupt-replay.wal --accept-poll-ms 50
```

服务端启动日志里出现：

```text
event=wal_replay ... records=1 skipped=2 truncated=1
```

---

# 18. smoke 的客户端验证

Python raw socket 发送 inline 命令：

```text
GET kept
GET partial
QUIT
```

预期响应：

```text
value
(nil)
BYE
```

截图里结果是：

```text
MATCH replayed_state True
MATCH wal_replay_report True
```

这证明：

```text
有效完整记录被恢复
尾部半条记录没有恢复
恢复报告和预期一致
```

---

# 19. 本次代码增删改总览

第十九版修改：

```text
CMakeLists.txt
 -> 版本号更新到 0.19.0

include/minikv/wal.hpp
 -> 新增 WalReplayReport
 -> 新增 replay_with_report
 -> replay 旧接口保留

src/wal.cpp
 -> 新增 is_blank
 -> 新增 has_final_newline
 -> replay 委托 replay_with_report
 -> replay_with_report 统计 applied / skipped / truncated

src/main.cpp
 -> CLI 启动 WAL 时输出 replayed / skipped / truncated

src/server_main.cpp
 -> event=wal_replay 输出 records / skipped / truncated

tests/wal_tests.cpp
 -> 新增坏 WAL 和尾部半条记录验证

tests/snapshot_tests.cpp
 -> 新增坏 snapshot 不替换 Store 验证

README.md
 -> 当前版本说明更新为 Version 19
 -> 能力列表新增 WAL replay report
 -> 能力列表新增 corrupt snapshot 保护
 -> WAL / Snapshot 章节补充恢复加固说明
 -> Roadmap 调整为客户端体验和 CI
```

第十九版新增：

```text
a/19/图片/01-cmake-configure.png
a/19/图片/02-cmake-build.png
a/19/图片/03-ctest.png
a/19/图片/04-persistence-tests.png
a/19/图片/05-wal-replay-report-smoke.png
a/19/解释/说明.md
代码讲解记录/40-wal-replay-report.md
代码讲解记录/41-version-19-tests-docs.md
```

第十九版删除：

```text
无源码文件删除
```

---

# 20. 第十九版之后当前能力总览

到第十九版为止，mini-kv 已经具备：

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
外部客户端式 RESP-over-TCP pipeline 验证
RESP-over-TCP null bulk / integer / error / protocol error 验证
并发 RESP-over-TCP 客户端验证
CTest 自动化覆盖
运行归档
代码讲解记录
```

---

# 21. 第十九版和前几版的关系

第四版：

```text
加入 WAL
```

第五版：

```text
加入 Snapshot
```

第十九版：

```text
回头加固 WAL / Snapshot 的坏文件恢复边界
```

也就是从：

```text
能持久化
```

推进到：

```text
坏文件时更保守、更可观测
```

---

# 22. 第十九版之后适合继续做什么

第十九版已经做了第一轮恢复加固。

后续可以继续做：

```text
CI
 -> 每次提交自动跑 CMake build 和 CTest

WAL checksum
 -> 每条记录带校验，恢复时更准确识别损坏

Snapshot 原子写
 -> 写临时文件成功后 rename，避免半个 snapshot 文件

WAL compact
 -> 用 snapshot 压缩历史 WAL

客户端体验
 -> 持久 history 或交互式行编辑
```

---

# 23. 第十九版归档的清理结论

本轮运行后清理结果是：

```text
临时日志目录 tmp-v19-run 已删除
真实 smoke 启动的 minikv_server 已停止
检查后没有 minikv_server / minikv_client 残留进程
关键截图保留在 a/19/图片/
归档说明保留在 a/19/解释/说明.md
```

`git diff --check` 的结果只有换行提示：

```text
LF will be replaced by CRLF
```

这不是代码格式错误。

---

# 第四十一次总结

第十九版配套工作证明了三件事：

```text
wal_tests
 -> WAL 正常恢复、坏记录跳过、尾部半条记录跳过都有自动化测试

snapshot_tests
 -> 坏 snapshot 不替换现有 Store 有自动化测试

真实 WAL smoke
 -> minikv_server.exe 启动时能报告 records / skipped / truncated，并且只恢复完整有效 WAL 记录
```

一句话总结：

```text
第十九版把持久化恢复从“能 replay”推进到“坏记录可跳过、尾部半条可识别、恢复结果可报告、坏 snapshot 不污染当前 Store”，并用 CTest 与真实 server smoke 做了复核。
```
