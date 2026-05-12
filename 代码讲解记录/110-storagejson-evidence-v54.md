# 第一百一十次讲解：第五十四版 STORAGEJSON 只读存储证据

第五十四版按照最新跨项目计划推进：

```text
D:\nodeproj\orderops-node\docs\plans\v95-production-gap-roadmap.md
```

计划要求 mini-kv v54 新增只读 storage evidence 输出，覆盖 version、WAL/snapshot 状态、key count 和 read-only diagnostics，同时保持 `CHECKJSON GET` 语义不变，也不让 mini-kv 承担订单权威数据。

本版只做 C++ / mini-kv，不修改 Node 或 Java。

---

# 1. 新增命令

新增命令：

```text
STORAGEJSON
```

它放进 `command_catalog`：

```cpp
{"STORAGEJSON", "read", false, true, true, "Read storage evidence as JSON without mutating data"}
```

含义是：

```text
category=read
mutates_store=false
touches_wal=true
stable=true
```

这里 `touches_wal=true` 不是写 WAL，而是表示它会在 WAL 开启时读取 WAL maintenance metadata。这个语义和 `WALINFO`、`STATSJSON` 保持一致。

---

# 2. STORAGEJSON 返回结构

`STORAGEJSON` 的核心返回字段是：

```text
schema_version
read_only
execution_allowed
version
store.live_keys
store.order_authoritative
wal
snapshot
side_effects
side_effect_count
diagnostics
```

关键边界写死为：

```text
read_only=true
execution_allowed=false
store.order_authoritative=false
diagnostics.order_authoritative=false
diagnostics.write_commands_executed=false
```

这样 Node 后续可以消费 mini-kv 的只读生产证据，但不会把 mini-kv 误判成订单权威库。

---

# 3. WAL 和 snapshot 怎么表达

没有 WAL 时：

```json
"wal":{"enabled":false,"status":"disabled"}
```

开启 WAL 时会补充：

```text
bytes
records
live_keys
compact_recommended
options
compactions
```

这些来自现有 `WalMaintenanceReport`，没有新增 WAL 写入行为。

snapshot 部分表达当前能力，而不是执行快照：

```json
"snapshot":{"supported":true,"mode":"manual","atomic_save":true,"load_replaces_store":true}
```

这说明 mini-kv 支持手动 `SAVE` / `LOAD`，并且 `SAVE` 是原子替换；但 `STORAGEJSON` 本身不会保存或加载文件。

---

# 4. EXPLAINJSON / CHECKJSON 语义

`EXPLAINJSON STORAGEJSON` 新增 side effects：

```text
metadata_read
store_read
wal_metadata_read_when_enabled
```

这三个都是读侧信号：

- `metadata_read` 表示读取命令/运行时证据
- `store_read` 表示读取 live key count
- `wal_metadata_read_when_enabled` 表示 WAL 开启时读取 maintenance metadata

没有新增 `store_write`、`wal_append_when_enabled` 或 `snapshot_file_write`。

---

# 5. 测试覆盖

`tests/command_tests.cpp` 补充了：

```text
COMMANDS / COMMANDSJSON 包含 STORAGEJSON
EXPLAINJSON STORAGEJSON side_effects 正确
STORAGEJSON extra 返回 usage
无 WAL STORAGEJSON 返回 disabled
有 WAL STORAGEJSON 返回 enabled 且不创建 WAL 文件
HELP 包含 STORAGEJSON
```

`tests/line_editor_tests.cpp` 补充了：

```text
STOR -> STORAGEJSON
```

真实 smoke 流程是：

```text
启动 minikv_server
minikv_client 发送 STORAGEJSON
继续发送 GET orderops:1
确认仍然返回 (nil)
发送 QUIT 并停止 server
```

这证明 `STORAGEJSON` 是证据查询，不会写入 `orderops:1`。

---

# 6. README 更新

README 当前版本提升到：

```text
Version 54
```

并补充：

```text
STORAGEJSON
read-only storage evidence
not_order_authoritative
```

Roadmap 也改成让 Node v96 消费 `STORAGEJSON` 作为 mini-kv 只读生产证据。

---

# 7. 一句话回看

第五十四版把 mini-kv 从“命令风险解释和 CHECKJSON fixture”推进到“可被控制面消费的只读存储证据”，但仍然保持松耦合、只读、不接管订单数据。
