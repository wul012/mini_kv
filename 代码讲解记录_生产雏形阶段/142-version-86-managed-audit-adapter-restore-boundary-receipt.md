# 142. v86 managed audit adapter restore boundary receipt

## 本版目标

v86 按 `D:\nodeproj\orderops-node\docs\plans\v213-post-restore-drill-plan-roadmap.md` 推进 mini-kv 侧任务：补一个 `managed_audit_adapter_restore_boundary_receipt`，让后续 Node v215 在做 managed audit dry-run adapter candidate 前，能只读确认 mini-kv 仍然不会执行 restore/write 相关动作。

它不是什么同样重要：它不是新的恢复入口，不是 adapter 执行凭证，不会执行 `LOAD`、`COMPACT`、`SETNXEX`、`RESTORE`，不会写 managed audit，也不会把 mini-kv 接入 Java 订单权威链路。它只是 C++ 侧给控制面的边界回执。

## 1. 运行时入口

`CMakeLists.txt` 把工程版本推进到 `0.86.0`。`src/command.cpp` 保留 v85 的 `retention_provenance_replay_marker`，再新增：

```text
RuntimeManagedAuditAdapterRestoreBoundaryReceipt
 -> consumer = Node v215 managed audit dry-run adapter candidate
 -> consumed_by = Node v213 managed audit packet restore drill plan
 -> consumed_release_version = v85
 -> consumed_artifact_path_hint = c/85/
 -> consumed_marker_digest = fnv1a64:1ea4570c967cfdb1
 -> current_artifact_path_hint = c/86/
 -> adapter_write_allowed = false
 -> restore_execution_allowed = false
 -> load_restore_compact_executed = false
 -> managed_audit_write_executed = false
 -> order_authoritative = false
```

这里的 `v85/c/85` 是上游输入：Node v213 已消费的 v85 replay marker。`v86/c/86` 是当前版本归档位置。两者分开，是为了让 Node v215 能确认“上游证据已存在，但当前 mini-kv 仍然不执行恢复或写审计”。

## 2. receipt digest 契约

本版新增 `managed_audit_adapter_restore_boundary_receipt_digest()`，输入包括版本、消费来源、消费 marker digest、当前归档位置，以及所有关键边界布尔值。

最终输出：

```text
receipt_digest = fnv1a64:f39d8e3ef98654ea
```

控制面读到这个 digest 时，只能理解为“v86 对 mini-kv adapter 恢复/写入边界的只读指纹”。它不是签名，不代表 adapter 已经执行，不代表 restore 已发生。

## 3. INFOJSON / SMOKEJSON 给 Node 怎么读

`INFOJSON` 在 `retention_provenance_replay_marker` 后追加 `managed_audit_adapter_restore_boundary_receipt`。这让 Node 在不写 mini-kv 的情况下核对恢复边界。

`SMOKEJSON` 同样追加该节点，并把 `diagnostics.node_consumption` 推进到 Node v215：

```text
Node v215 may verify the mini-kv managed audit adapter restore boundary receipt ... before managed audit dry-run adapter candidate work
```

`diagnostics.notes` 新增 `managed_audit_adapter_restore_boundary_receipt`。关键字段 `adapter_write_allowed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`managed_audit_write_executed=false`、`order_authoritative=false` 一起构成 v86 的边界。

## 4. fixture 和 release manifest

`fixtures/readonly/infojson-empty-inline.json` 精确同步空库 `INFOJSON`，新增 receipt 节点。

`fixtures/release/runtime-smoke-evidence.json` 面向 Node v215，新增验收点：

```text
managed_audit_adapter_restore_boundary_receipt.consumed_release_version equals v85
managed_audit_adapter_restore_boundary_receipt.consumed_artifact_path_hint equals c/85/
managed_audit_adapter_restore_boundary_receipt.consumed_marker_digest equals fnv1a64:1ea4570c967cfdb1
managed_audit_adapter_restore_boundary_receipt.current_artifact_path_hint equals c/86/
managed_audit_adapter_restore_boundary_receipt.receipt_digest equals fnv1a64:f39d8e3ef98654ea
managed_audit_adapter_restore_boundary_receipt.adapter_write_allowed is false
managed_audit_adapter_restore_boundary_receipt.restore_execution_allowed is false
managed_audit_adapter_restore_boundary_receipt.load_restore_compact_executed is false
```

`fixtures/release/verification-manifest.json` 同步记录 CMake、CTest、真实 TCP smoke 的预期字段，并新增边界 `managed audit adapter restore boundary receipt only`。

## 5. 测试保护了什么

`tests/command_tests.cpp` 保护真实 `SMOKEJSON` 必须输出 receipt 节点，并验证 adapter write、restore execution、LOAD/COMPACT/SETNXEX/RESTORE 执行和 order-authoritative 都保持 false。

`tests/readonly_fixture_tests.cpp` 用真实 `INFOJSON` 精确对比 fixture，防止控制面样例漂移。

`tests/runtime_smoke_evidence_tests.cpp` 同时检查 release fixture 和真实 `SMOKEJSON`，保护 Node v215 所需字段、warning、pause condition 和 no-write 边界。

`tests/release_verification_manifest_tests.cpp` 保护 release manifest 中的命令、真实 smoke 预期、边界列表和运行时输出字段。

本轮本地验证结果：CMake 构建成功，CTest `31/31` 通过；真实 TCP smoke 使用 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`，确认 `receipt_digest=fnv1a64:f39d8e3ef98654ea`，且 `GET restore:real-read-token` 返回 `(nil)`。

## 一句话总结

v86 把 mini-kv 的跨项目证据从“v85 已能证明上游 replay marker 被消费”推进到“能给 Node v215 的 dry-run adapter 明确提供不恢复、不写审计、不承载订单权威的 C++ 侧边界回执”。