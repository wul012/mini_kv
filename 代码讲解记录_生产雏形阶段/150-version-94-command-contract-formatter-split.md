# mini-kv v94 command contract formatter 拆分

本版目标是把 `src/command.cpp` 里继续膨胀的命令契约格式化逻辑拆出来。v92 拆了 managed audit receipt formatter，v93 拆了 runtime evidence receipt formatter；v94 接着拆命令目录、`EXPLAINJSON`、`CHECKJSON` 和运行响应 formatter。这样 `src/command.cpp` 从 1433 行降到 713 个物理行，已经低于“1000 行左右”的目标。

这不是新的命令入口，不是权限系统，不是执行审批，不是 Node/Java 适配器，也不是 restore/load/compact 的自动化能力。`CHECKJSON` 和 `EXPLAINJSON` 仍然只解释命令风险，不执行目标命令；write/admin/WAL/snapshot/restore 路径保持不动，mini-kv 仍然不是 Java order authority。

## 本版处在什么位置

mini-kv 的 `command.cpp` 长期承载了三类东西：命令分发、真实 store/WAL/snapshot 执行、运行时证据格式化。v88-v93 已经开始把运行时证据链逐步拆出去，但命令 catalog、风险解释、执行契约还留在同一个文件里。

v94 的拆分幅度是合理的：只移动 command contract formatter 和 command response formatter，不同时改 parser 行为、store 执行、TCP server、WAL、Snapshot 或 fixture 解析框架。这样既能实质降低大文件负担，又不会把行为风险和结构整理混成一个大版本。

## 文件职责

`include/minikv/command_contracts.hpp` 是新的命令契约 formatter 头文件。它公开 `find_command_catalog_entry()`、`is_known_command()`、`format_commands()`、`format_commands_json()`、`format_explain_json()` 和 `format_check_json()`。这些函数只读命令文本并返回文本或 JSON，不接触 `Store`，也不执行任何目标命令。

`src/command_contracts.cpp` 承接原来 `src/command.cpp` 中的 command catalog、命令解释结构、usage warning、side effect 列表、WAL durability 判断和 CHECKJSON/EXPLAINJSON 格式化。为了保持原有 command digest 完全稳定，它保留了原本的 FNV-1a digest 拼接方式，没有改用 runtime evidence digest helper。

`src/command.cpp` 现在只调用新模块。`COMMANDS` / `COMMANDSJSON` 直接调用 `command_contracts::format_commands*()`；`EXPLAINJSON` 和 `CHECKJSON` 直接调用 `command_contracts::format_explain_json()` / `format_check_json()`；`KEYS`、`INFO`、`STATS`、`HEALTH`、`SMOKEJSON`、`STORAGEJSON` 和 `WALINFO` 调用 `command_response_formatters`。真实写命令、读命令、admin 命令处理函数保持原位。

`CMakeLists.txt` 把 `src/command_contracts.cpp` 和 `src/command_response_formatters.cpp` 加入 `minikv` library，并把项目版本推进到 `0.94.0`。

## 控制面如何理解关键字段

`read_only=true` 和 `execution_allowed=false` 在 `CHECKJSON` 中仍然表示只读契约检查。即使目标是 `SET`、`SETNXEX`、`LOAD` 或 `COMPACT`，CHECKJSON 也只是返回风险说明，不执行写入、不加载 snapshot、不重写 WAL。

`command_digest` 仍然是对规范化解释字段的稳定 `fnv1a64:<hex>` 指纹，不是鉴权签名。v94 拆模块后保留原 digest 拼接逻辑，是为了让外部 fixture 和 Node 侧消费不因为代码组织变化而误判契约变化。

`side_effects` 和 `side_effect_count` 继续描述“如果真实执行该命令会涉及什么风险面”。例如 `SETNXEX` 会说明 `store_write`、`store_ttl_update` 和 `wal_append_when_enabled`；`SMOKEJSON` 会说明 `metadata_read`、`store_read` 和 `wal_metadata_read_when_enabled`。这些字段是解释，不是执行记录。

release fixture 新增的 `command contract formatter split only`、`command response formatter split only`、`no CHECKJSON/EXPLAINJSON field contract expansion from command contract split` 和 `no runtime response field contract expansion from command response split` 明确告诉控制面：本版只整理 formatter 模块，不扩展命令契约字段或运行响应字段，不改变写/admin/WAL/snapshot/restore 行为。

## 测试和真实运行

`tests/command_tests.cpp` 继续覆盖 `COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON`、稳定 CHECKJSON fixture、INFOJSON/SMOKEJSON 只读证据和 no-write 边界。因为 formatter 被拆出后仍由 `CommandProcessor` 调用，这些测试能证明外部命令输出保持兼容。

`tests/release_verification_manifest_tests.cpp` 新增断言确认 release manifest 记录了 `include/minikv/command_contracts.hpp`、`src/command_contracts.cpp`、`include/minikv/command_response_formatters.hpp` 和 `src/command_response_formatters.cpp`，并确认 CMake 已编译新源文件。它还断言本版边界是 command contract formatter split 和 command response formatter split，不扩展 CHECKJSON/EXPLAINJSON 或运行响应字段契约。

`tests/runtime_smoke_evidence_tests.cpp` 新增断言确认 runtime smoke fixture 记录 command contract formatter split 和 command response formatter split 只是代码组织证据，不是新的命令执行路径。

真实 smoke 启动 `minikv_server`，通过 `minikv_client` 执行 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token` 和 `QUIT`。结果证明 v94 的运行时证据仍然只读，`GET restore:real-read-token` 仍为 `(nil)`，没有执行 LOAD、COMPACT、SETNXEX、RESTORE 或 managed audit 写入。

## 一句话总结

v94 把命令契约和运行响应 formatter 从 `src/command.cpp` 中拆出来，让 mini-kv 的命令解释/检查能力更容易维护，同时保持 Node/Java/mini-kv 之间的松耦合和只读证据边界不变。
