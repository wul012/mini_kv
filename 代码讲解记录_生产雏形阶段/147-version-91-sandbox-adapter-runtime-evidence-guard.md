# 147 - Version 91 Sandbox Adapter Runtime Evidence Guard

## 本版目标

v91 的目标是给 Node v225 managed audit sandbox adapter dry-run package 提供一个只读前置守卫：`INFOJSON` 和 `SMOKEJSON` 现在都会暴露 `managed_audit_sandbox_adapter_non_participation_receipt`，让控制面在做沙箱适配器 dry-run 前先确认 mini-kv 不参与沙箱审计存储、不读凭据、不执行 schema migration、不写 managed audit state。

这不是新的执行入口，不是 sandbox audit storage backend，不绕过 Java/Node 审批链，不执行 LOAD/COMPACT/RESTORE/SETNXEX，不写 sandbox dry-run records，也不把 mini-kv 变成 Java order authority。

## 运行证据 helper

`include/minikv/runtime_evidence.hpp` 和 `src/runtime_evidence.cpp` 把本版新增收据需要的 JSON 字符串、布尔值、字符串数组、对象拼接和 FNV-1a digest 生成集中起来。

这个 helper 的边界很窄：它只服务运行证据格式化，避免继续把新的 JSON/digest 拼接散落进 `command.cpp`。它没有接入命令派发表，没有读取存储，也不会触发 WAL、Snapshot、Restore 或任何写路径。

## Runtime Receipt

`src/command.cpp` 增加了 `RuntimeManagedAuditSandboxAdapterNonParticipationReceipt`，并通过 `format_runtime_managed_audit_sandbox_adapter_non_participation_receipt_json()` 输出到 `INFOJSON` 和 `SMOKEJSON`。

关键字段的控制面含义：

- `consumed_release_version="v90"`、`consumed_artifact_path_hint="c/90/"`、`consumed_receipt_digest="fnv1a64:0dfb07cd2f8de289"`：v91 沙箱收据消费上一版 external adapter non-participation evidence。
- `current_artifact_path_hint="c/91/"` 和 `receipt_digest="fnv1a64:fd925623ea87eac4"`：本版归档和运行输出应对齐到 v91。
- `sandbox_adapter_storage_backend=false`、`participates_in_sandbox_adapter=false`：mini-kv 不是沙箱审计存储后端，也不参与沙箱适配器。
- `credential_value_read_allowed=false`、`production_credential_read_allowed=false`：mini-kv 不读沙箱凭据值，也不读生产凭据。
- `schema_migration_execution_allowed=false`、`sandbox_managed_audit_state_write_allowed=false`、`sandbox_dry_run_records_written=false`：沙箱 schema migration、managed audit state 写入、dry-run 记录写入都必须发生在 mini-kv 之外。
- `write_handler_changed=false`、`admin_handler_changed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`：本版没有修改写/admin/WAL/snapshot/restore 行为。

## Fixtures And Tests

`fixtures/readonly/infojson-empty-inline.json`、`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 更新到 `0.91.0` / `v91`，并加入沙箱适配器不参与收据。

测试覆盖分三层：

- `command_tests` 和 `readonly_fixture_tests` 验证真实 `SMOKEJSON` / `INFOJSON` 输出包含沙箱收据，并保持只读、无执行、非 order-authoritative。
- `runtime_smoke_evidence_tests` 验证 runtime smoke fixture、Node v225 消费字段、pause conditions、diagnostics 和真实 `SMOKEJSON` 输出一致。
- `release_verification_manifest_tests` 验证 release manifest、CMake/CTest 命令、v91 digest、沙箱收据和 runtime 输出一致。

这些断言保护的是控制面读证据时的边界判断：Node 可以读 mini-kv 的证据，但不能据此让 mini-kv 承担沙箱审计存储、凭据读取、schema migration、restore 或 managed audit 写入职责。

## Smoke And Archive

本版真实验证继续使用 CLion 捆绑 CMake/CTest 工具链，构建目录为 `cmake-build-v91`。归档位于 `c/91/图片/` 和 `c/91/解释/说明.md`，记录 CMake configure/build、CTest 31/31 通过、真实 TCP smoke，以及验证结束后停止服务端进程。

一句话总结：v91 给 Node v225 的沙箱适配器 dry-run 增加了可验证的 mini-kv 非参与证据，同时把新增 JSON/digest 生成收束到小 helper，继续保持 mini-kv 只是只读运行证据提供方。
