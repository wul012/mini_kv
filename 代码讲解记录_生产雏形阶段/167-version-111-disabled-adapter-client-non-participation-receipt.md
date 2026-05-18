# mini-kv v111 disabled adapter client non-participation receipt

本版目标是给 Node v254 准备 mini-kv 侧的只读非参与证据。Node v252 已经定义 disabled adapter client precheck，Node v253 已经定义 test-only fake transport shell；mini-kv v111 只回显这些上游证据的字段形状和边界，并证明自己没有被带入真实连接链路。

它不是什么：不是新的 client，不是 managed audit storage backend，不读取 credential value，不发 external request，不写 storage，不执行 LOAD / COMPACT / RESTORE / SETNXEX，也不绕过 Node / Java 的审批和契约边界。

## 入口和数据结构

`include/minikv/runtime_evidence_receipts.hpp` 新增两个公开函数：

```cpp
disabled_adapter_client_non_participation_receipt_digest(read_commands)
format_disabled_adapter_client_non_participation_receipt_json(read_commands)
```

实现放在 `src/runtime_evidence_receipts.cpp`。常量区记录 Node v252/v253 的 profile、state、env handle count、failure class count、request/response shape count、failure mapping count 和 guard condition count。digest 也把这些 count 和关键边界布尔值纳入输入，避免后续只改展示文本却不改证据指纹。

`src/command_response_formatters.cpp` 继续作为 SMOKEJSON 组合入口。本版只是把 `disabled_adapter_client_non_participation_receipt` 接到 SMOKEJSON 中，并在 notes/diagnostics 中加入 Node v254 消费提示；没有触碰 command dispatch、write/admin handler、WAL、snapshot 或 restore 流程。

## 控制面如何理解

这份 receipt 的核心字段分两组。

第一组是 Node v252 precheck 回显：`source_required_env_handle_count=5`、`source_failure_class_count=6`、`source_dry_run_response_field_count=10`、`source_client_may_be_instantiated=false`、`source_external_request_may_be_sent=false`、`source_credential_value_may_be_loaded=false`。控制面可以用这些字段确认 disabled client 仍然是 handle-only、not-implemented、默认 disabled。

第二组是 Node v253 fake transport shell 回显：`source_fake_transport_only=true`、`source_real_client_implemented=false`、`source_real_transport_allowed=false`、`source_request_shape_field_count=8`、`source_response_shape_field_count=9`、`source_failure_mapping_count=6`、`source_guard_condition_count=7`。这说明本版只承认测试内 fake transport shape，不承认真实网络 transport。

mini-kv 自身边界继续是硬 false：`mini_kv_auto_start_allowed=false`、`storage_write_allowed=false`、`credential_value_read_allowed=false`、`external_request_sent=false`、`managed_audit_storage_backend=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`order_authoritative=false`。

## Fixture 和测试

新增 `fixtures/release/disabled-adapter-client-non-participation-receipt.json`，并把同一 receipt 嵌入 `fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json`。独立 fixture 是给后续人工和上游验证看的，runtime/manifest 嵌入则保证现有 release evidence 链能被 Node v254 直接读取。

新增 `tests/disabled_adapter_client_non_participation_receipt_tests.cpp`，覆盖独立 fixture、runtime smoke fixture、manifest fixture 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出。测试断言 env/failure/shape count、fake transport-only、无真实 client/transport、无 external request、无 credential read、无 storage write、无 restore/load/compact/SETNXEX 和非 order authoritative。

本版还回跑了 `runtime_smoke_evidence_tests`、`release_verification_manifest_tests` 和 v108 precheck receipt 测试，确认新增 Node v254 证据没有破坏历史 Node v246 前缀和 v102 runtime evidence 结构。

## 归档和成熟度

归档放在 `c/111/`，包含 CMake configure、build、CTest、真实 server/client smoke，以及行数统计。真实 smoke 仍然只读执行 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`，并在验证后停止本轮启动的 server。

一句话总结：v111 把 Node v252/v253 的 disabled adapter client 边界接成 mini-kv 的只读证据链，让后续 Node v254 可以验证三方一致性，同时 mini-kv 仍然停在 runtime evidence provider 的位置。
