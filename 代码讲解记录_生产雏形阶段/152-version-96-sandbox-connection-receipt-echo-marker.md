# mini-kv v96 sandbox connection receipt echo marker

本版目标是给 Node v229 的 manual sandbox connection packet verification 提供一份只读回声证据。它不是新的连接入口，不执行 sandbox connection，不读取 `ORDEROPS_MANAGED_AUDIT_SANDBOX_CREDENTIAL_HANDLE` 的值，不运行 schema rehearsal，不写 managed audit state，也不让 mini-kv 变成 sandbox audit storage backend 或 Java order authority。

为什么做：Node v228 已经生成 manual sandbox connection operator packet，里面有 owner approval artifact id、credential handle、schema rehearsal id、rollback path、timeout budget 和 manual abort marker。mini-kv v96 只把这些字段名以 receipt echo marker 的形式暴露给控制面核对，让 Node v229 在真正校验连接包前确认 mini-kv 仍是 read-only runtime evidence provider。

## 实现位置

`include/minikv/managed_audit_receipts.hpp` 新增两个声明：`sandbox_connection_receipt_echo_marker_digest()` 和 `format_sandbox_connection_receipt_echo_marker_json()`。这保持了 v92 以来的边界：managed audit receipt 的 digest 和 JSON 格式化都在 receipt 模块里，不回流到 `src/command.cpp`。

`src/managed_audit_receipts.cpp` 新增 `RuntimeManagedAuditSandboxConnectionReceiptEchoMarker`。它消费 v95 的 sandbox adapter non-participation receipt，固定 `consumed_release_version="v95"`、`consumed_artifact_path_hint="c/95/"`、`consumed_receipt_digest="fnv1a64:ceaed265f7f9560c"`，并发布本版 marker digest `fnv1a64:b9fc556875ea625b`。这里保留历史 v95 digest 是关键点：v96 不是重写旧收据，而是在旧收据之上声明“连接包字段只读回声”。

`src/command_response_formatters.cpp` 把 `managed_audit_sandbox_connection_receipt_echo_marker` 同时加入 `INFOJSON` 和 `SMOKEJSON`。`SMOKEJSON` 的 diagnostics 也推进到 Node v229，说明控制面可以先验证 v96 marker，再继续验证 v95 sandbox adapter non-participation receipt、v90 external adapter non-participation receipt、v89/v88/v87/v86 历史链，以及 live-read、binary provenance、retention、taxonomy、operator-window、CI 和 artifact-retention 证据。

## 字段含义

`source_operator_packet_profile` 固定为 `managed-audit-manual-sandbox-connection-operator-packet.v1`，`packet_mode` 固定为 `manual-sandbox-connection-operator-packet-only`。这些字段让 Node v229 确认自己正在读取 v228 operator packet 的字段名，而不是从 mini-kv 获取凭据值或连接状态。

`owner_approval_artifact_id_field`、`credential_handle_name_field`、`schema_rehearsal_id_field`、`rollback_path_id_field`、`manual_abort_marker` 都是字段句柄回声。mini-kv 只暴露名字，不读取 secret，不解析 approval artifact，不执行 rollback，也不启动 schema rehearsal。

边界字段全部保持关闭：`execution_allowed=false`、`connection_execution_allowed=false`、`credential_value_read_allowed=false`、`schema_migration_execution_allowed=false`、`managed_audit_write_allowed=false`、`participates_in_sandbox_connection=false`、`node_auto_start_allowed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`order_authoritative=false`。控制面读到这些字段后，应把 mini-kv 理解成证据提供者，而不是执行者。

## Fixture 和测试

`fixtures/readonly/infojson-empty-inline.json` 更新为真实 `INFOJSON` 输出形状，包含新 marker。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 更新到 v96 / Node v229，新增 marker 的期望、boundary、diagnostics，并保留旧 v95 sandbox adapter receipt 链。

`tests/command_tests.cpp` 和 `tests/readonly_fixture_tests.cpp` 保护真实 `SMOKEJSON` / `INFOJSON` 输出里的新字段。`tests/runtime_smoke_evidence_tests.cpp` 和 `tests/release_verification_manifest_tests.cpp` 保护 release fixture、runtime 输出、Node v229 diagnostics、v95 consumed digest、v96 marker digest，以及“不连接、不读凭据、不迁移、不写状态、不自动启动、不执行恢复”的边界。

本版验证包括 CMake configure、CMake build、31 个 CTest 全部通过，以及真实 TCP smoke：`SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`。真实 smoke 证明新 marker 出现在运行输出中，同时 `GET restore:real-read-token` 仍为 `(nil)`，没有执行 `SETNXEX`、`LOAD`、`COMPACT` 或 restore。

一句话总结：v96 给 Node v229 增加了可验证的 sandbox connection operator packet 字段回声，同时继续把 mini-kv 固定在只读证据提供方的位置，为 Node/Java/mini-kv 的松耦合融合保留清楚边界。
