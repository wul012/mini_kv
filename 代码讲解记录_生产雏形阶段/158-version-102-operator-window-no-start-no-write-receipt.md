# Version 102 Operator Window No-Start / No-Write Receipt

## 本版目标

v102 承接 Node 最新计划：Node v238 已经把 manual sandbox connection 的 operator window checklist 做完，mini-kv 这一版只补一份 C++ 侧可读回执，让 Node v239 能确认 mini-kv 没有因为这个 checklist 获得任何执行权限。

它不是什么：

- 不是新的 sandbox connection 执行入口。
- 不是 managed audit storage backend。
- 不是 Node、Java 或 mini-kv 自动启动 mini-kv 的许可。
- 不是 credential value 读取、schema rehearsal、schema migration 或 managed audit write。
- 不是 LOAD、COMPACT、RESTORE 或 SETNXEX 执行。
- 不是 Java order authority，也不进入 Java 订单权威链路。

## 运行时入口

运行时入口仍然是 `SMOKEJSON`。本版在 `src/command_response_formatters.cpp` 中新增：

```text
format_operator_window_no_start_no_write_receipt_json(...)
```

然后把结果挂到：

```text
SMOKEJSON.operator_window_no_start_no_write_receipt
```

这个对象的角色很单一：回显 Node v238 checklist 的结构性事实，并把 mini-kv 当前版本、归档路径、live-read echo、当前 digest 集合和硬边界字段放在同一个只读 JSON 对象里。

关键字段含义：

- `source_checklist="Node v238 manual sandbox connection operator window checklist"`：说明来源只是 Node checklist。
- `consumer_hint="Node v239 manual sandbox connection operator window evidence verification"`：说明消费方是下一轮 Node 验证，不是执行器。
- `approval_item_count=3`、`checklist_step_count=8`、`pause_condition_count=8`、`forbidden_operation_count=6`：把 checklist 的规模固定下来，避免下游只看一段自由文本。
- `ready_for_java_v93_echo_receipt=true`：表示可以和 Java v93 echo receipt 一起被 Node 读取。
- `ready_for_managed_audit_sandbox_adapter_connection=false`：明确仍未进入真实 sandbox adapter connection。
- `read_only=true`、`execution_allowed=false`：控制面只能读，不能把回执解释成执行许可。
- `node_auto_start_allowed=false`、`java_auto_start_allowed=false`、`mini_kv_auto_start_allowed=false`：三方都不能据此自动启动 mini-kv。
- `storage_write_allowed=false`、`managed_audit_write_executed=false`、`sandbox_managed_audit_state_write_allowed=false`：不写本地存储，也不写 managed audit state。
- `credential_value_read_allowed=false`：只允许字段句柄或标记名，不允许凭证值。
- `schema_rehearsal_execution_allowed=false`、`schema_migration_execution_allowed=false`：不执行 schema rehearsal/migration。
- `restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`：不执行 restore/load/compact/token claim。
- `order_authoritative=false`：mini-kv 仍不是 Java 订单权威。

## Fixture 与 Manifest

新增文件：

```text
fixtures/release/operator-window-no-start-no-write-receipt.json
```

它把 v102 的 operator-window 回执固化成文件级契约，方便 Node v239 在不启动额外服务、不写 managed audit、不读取凭证值的前提下做证据核对。

同步滚动的现有证据：

- `fixtures/release/runtime-smoke-evidence.json` 升到 v102，并纳入 `operator_window_no_start_no_write_receipt`。
- `fixtures/release/verification-manifest.json` 增加新 fixture 和 `minikv_operator_window_no_start_no_write_receipt_tests`。
- `fixtures/release/current-runtime-fixture-rolling-guard.json` 继续固定 historical consumed digest anchors，只滚动当前版本、归档路径、session echo 和当前 digest。
- `fixtures/release/runtime-no-start-no-write-follow-up.json` 继续保留 v102 no-start/no-write follow-up，作为新 operator-window 回执的前置证据之一。

这些文件共同表达一件事：Node 可以验证 mini-kv 的证据链完整性，但不能获得连接、写入、凭证、schema、restore 或 order-authority 权限。

## 测试保护什么

新增测试：

```text
tests/operator_window_no_start_no_write_receipt_tests.cpp
```

它保护四类行为：

- 新 fixture 必须存在，并包含 Node v238 checklist 的 producer、state、3/8/8/6 计数和 Node v239 consumer hint。
- v102 当前版本、`c/102/` 归档路径、`mini-kv-live-read-v102` echo 以及当前 digest 集合必须一致。
- 所有 no-start/no-write/no-execution/no-credential/no-schema/no-restore/order-authority 字段必须保持硬 `false` 边界。
- 真实 `SMOKEJSON` 输出必须包含 `operator_window_no_start_no_write_receipt`，并且 `GET restore:real-read-token` 仍然返回 `(nil)`。

同时保留并更新 runtime smoke、release manifest、current rolling guard、readonly fixture、managed audit receipt、restore boundary 和 digest package 相关测试，避免新回执破坏历史 v84-v96 consumed digest anchors。

## 真实验证与归档

本版使用 CLion bundled CMake/CTest 工具链完成验证：

- CMake configure 成功。
- CMake build 成功。
- CTest `35/35` passed。
- 真实 TCP smoke 在 `127.0.0.1:6502` 运行，读取 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH` 和 `GET restore:real-read-token`，结果仍为 `(nil)`。

归档截图放在 `c/102/图片/`，说明放在 `c/102/解释/说明.md`。验证结束后停止了本版启动的 server，不留下后台进程。

一句话总结：v102 让 Node v239 可以把 Node v238 checklist、Java v93 echo 和 mini-kv 当前只读证据合在一起验证，但 mini-kv 仍只提供证据，不自启、不连接、不读凭证、不写 audit、不执行 restore/load/compact/SETNXEX，也不进入 Java order authority。
