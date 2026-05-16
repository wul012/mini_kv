# 143. v87 managed audit adapter non-authoritative storage receipt

## 本版目标

v87 按 `D:\nodeproj\orderops-node\docs\plans\v215-post-dry-run-adapter-roadmap.md` 推进 mini-kv 侧任务：补一个 `managed_audit_adapter_non_authoritative_storage_receipt`，让后续 Node v217 在做 managed audit adapter production-hardening readiness gate 前，能只读确认 mini-kv 不是 managed audit store，也不是 Java 订单权威状态来源。

它不是什么同样重要：它不是新的执行入口，不会执行 `LOAD`、`COMPACT`、`SETNXEX`、`RESTORE`，不会写 managed audit 状态，不开放 admin 命令，不允许 storage write，也不会绕过 Java 审批、ledger 或 Node 控制面契约。它只是 C++ 侧给控制面读取的一份非权威存储边界回执。

## 1. 运行时入口

`CMakeLists.txt` 把工程版本推进到 `0.87.0`。`src/command.cpp` 保留 v86 的 `managed_audit_adapter_restore_boundary_receipt`，再新增：

```text
RuntimeManagedAuditAdapterNonAuthoritativeStorageReceipt
 -> consumer = Node v217 managed audit adapter production-hardening readiness gate
 -> consumed_by = Node v215 managed audit dry-run adapter candidate
 -> consumed_release_version = v86
 -> consumed_artifact_path_hint = c/86/
 -> consumed_receipt_digest = fnv1a64:f39d8e3ef98654ea
 -> current_artifact_path_hint = c/87/
 -> managed_audit_store = false
 -> storage_write_allowed = false
 -> admin_commands_allowed = false
 -> restore_execution_allowed = false
 -> load_restore_compact_executed = false
 -> managed_audit_write_executed = false
 -> order_authoritative = false
```

这里的 `v86/c/86/f39d8e3ef98654ea` 是上游输入：Node v215 已经可以消费 v86 的 restore/write 边界回执。`v87/c/87` 是本版输出：Node v217 再进一步确认 mini-kv 不是 managed audit 的存储后端，也不承担订单权威。

## 2. receipt digest 契约

本版新增 `managed_audit_adapter_non_authoritative_storage_receipt_digest()`，把版本、被消费的 v86 receipt、当前归档位置，以及所有关键边界布尔值一起纳入 digest 输入。

最终输出：

```text
receipt_digest = fnv1a64:111f0daf1283eab6
```

控制面读到这个 digest 时，只能理解为“v87 证明 mini-kv 在 managed audit adapter readiness gate 前仍是非权威、只读证据提供者”。它不是签名，不代表真实 adapter 已经接通，不代表 managed audit 写入发生，也不代表恢复命令被执行。

## 3. INFOJSON / SMOKEJSON 给 Node 怎么读

`INFOJSON` 追加 `managed_audit_adapter_non_authoritative_storage_receipt`，让 Node 在不写 mini-kv 的情况下核对 storage 边界。

`SMOKEJSON` 同样追加该节点，并把 `diagnostics.node_consumption` 推进到 Node v217：

```text
Node v217 may verify the mini-kv managed audit adapter non-authoritative storage receipt ...
before managed audit adapter production-hardening readiness gate work
```

关键字段的含义很直接：

```text
managed_audit_store=false        -> mini-kv 不是审计存储
storage_write_allowed=false      -> 本回执不允许写存储
admin_commands_allowed=false     -> 本回执不开放 admin 命令
restore_execution_allowed=false  -> 本回执不允许恢复执行
managed_audit_write_executed=false -> 没有写入 managed audit 状态
order_authoritative=false        -> mini-kv 不做 Java 订单权威
```

## 4. fixture 和 release manifest

`fixtures/readonly/infojson-empty-inline.json` 精确同步空库 `INFOJSON`，新增 v87 receipt 节点。

`fixtures/release/runtime-smoke-evidence.json` 面向 Node v217，要求控制面核对 v86 restore boundary receipt 已被消费，同时检查 v87 新字段：

```text
managed_audit_adapter_non_authoritative_storage_receipt.consumed_release_version equals v86
managed_audit_adapter_non_authoritative_storage_receipt.consumed_artifact_path_hint equals c/86/
managed_audit_adapter_non_authoritative_storage_receipt.consumed_receipt_digest equals fnv1a64:f39d8e3ef98654ea
managed_audit_adapter_non_authoritative_storage_receipt.current_artifact_path_hint equals c/87/
managed_audit_adapter_non_authoritative_storage_receipt.receipt_digest equals fnv1a64:111f0daf1283eab6
managed_audit_adapter_non_authoritative_storage_receipt.managed_audit_store is false
managed_audit_adapter_non_authoritative_storage_receipt.storage_write_allowed is false
managed_audit_adapter_non_authoritative_storage_receipt.admin_commands_allowed is false
```

`fixtures/release/verification-manifest.json` 同步记录 CMake、CTest、真实 TCP smoke 的预期字段，并新增边界 `managed audit adapter non-authoritative storage receipt only`、`not managed audit store`、`no storage write`、`no admin commands`。

## 5. 测试保护了什么

`tests/command_tests.cpp` 保护真实 `SMOKEJSON` 必须输出 v87 receipt，并验证 storage/admin/restore/write/order-authoritative 边界全部保持 false。

`tests/readonly_fixture_tests.cpp` 用真实 `INFOJSON` 精确对比 fixture，防止控制面样例漂移。

`tests/runtime_smoke_evidence_tests.cpp` 同时检查 release fixture 和真实 `SMOKEJSON`，保护 Node v217 所需字段、warning、pause condition 和 no-write 边界。

`tests/release_verification_manifest_tests.cpp` 保护 release manifest 中的命令、真实 smoke 预期、边界列表、运行时输出字段和 v87 新回执 digest。

本轮本地验证结果：CMake 配置成功，构建成功，CTest `31/31` 通过；真实 TCP smoke 使用 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`，确认 `receipt_digest=fnv1a64:111f0daf1283eab6`，并且 `GET restore:real-read-token` 返回 `(nil)`。

## 6. 归档和成熟度变化

运行截图和解释写入：

```text
c/87/图片/
c/87/解释/说明.md
```

v87 没有增加新的写能力，而是把跨项目证据链从“mini-kv 不执行 restore/write”继续推进到“mini-kv 不是 managed audit store”。这让后续 Node v217 可以把 mini-kv 当成只读证据来源，而不是误认为它能承载审计写入或订单权威。

## 一句话总结

v87 把 mini-kv 的跨项目边界从 v86 的 restore/write 回执推进到 managed audit adapter 前的非权威存储回执，为 Node v217 的真实 adapter readiness gate 提供可验证但不越界的 C++ 侧证据。
