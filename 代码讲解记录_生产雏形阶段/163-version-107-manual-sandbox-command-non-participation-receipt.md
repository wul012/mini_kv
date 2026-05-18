# mini-kv v107 manual sandbox command non-participation receipt

本版目标是给 Node v244 一个只读回执：Node v241 已经生成 disabled dry-run command package，mini-kv 需要证明自己不会因为这个包被自动启动、写 storage、读 credential value、执行 restore/load/compact/SETNXEX，或变成 managed audit storage backend。

这不是新的执行入口，不是连接授权，不是 credential 校验器，不是 restore 演练，也不是把 mini-kv 接入 managed audit。它只是一个 evidence receipt，供 Node 后续 upstream echo verification 只读核对。

## 入口

运行时入口仍然是 `SMOKEJSON`。v107 在 `src/command_response_formatters.cpp` 的 `format_smoke_json()` 中新增字段：

- `manual_sandbox_dry_run_command_non_participation_receipt`
- `diagnostics.node_consumption` 更新为 Node v244 可消费，同时保留 Node v239 老 operator-window 证据链语义
- `diagnostics.notes` 新增 receipt 名称，方便下游扫描

静态 fixture 入口是：

- `fixtures/release/manual-sandbox-dry-run-command-non-participation-receipt.json`
- `fixtures/release/runtime-smoke-evidence.json`
- `fixtures/release/verification-manifest.json`

## 回执职责

回执只回显 Node v241 command package 的形状，而不是读取或认证 Node 的真实 package digest：

- `source_package_command_count=6`
- `source_package_disabled_by_default=true`
- `source_package_dry_run_only=true`
- `source_package_carries_credential_value=false`
- `source_package_actual_connection_attempted=false`
- `source_package_managed_audit_state_write_requested=false`
- `source_package_mini_kv_write_permission_requested=false`

同时 mini-kv 自己的边界字段全部保持关闭：

- `mini_kv_auto_start_allowed=false`
- `storage_write_allowed=false`
- `credential_value_read_allowed=false`
- `restore_execution_allowed=false`
- `load_restore_compact_executed=false`
- `setnxex_execution_allowed=false`
- `managed_audit_store=false`
- `order_authoritative=false`

## 代码位置

`include/minikv/runtime_evidence_receipts.hpp` 暴露两个函数：

- `manual_sandbox_dry_run_command_non_participation_receipt_digest(...)`
- `format_manual_sandbox_dry_run_command_non_participation_receipt_json(...)`

`src/runtime_evidence_receipts.cpp` 负责生成 JSON 和 FNV digest。digest 的输入包含 Node v241 command package shape、v107 artifact hint、read command digest 和所有 no-start/no-write/no-restore 边界字段。

`src/command_response_formatters.cpp` 只负责把该 receipt 组合进 `SMOKEJSON`，不改变任何命令执行路径。

## 测试

`tests/manual_sandbox_dry_run_command_non_participation_receipt_tests.cpp` 同时检查三层：

1. 静态 receipt fixture 存在，字段和 runtime digest 一致。
2. `runtime-smoke-evidence.json` 与 `verification-manifest.json` 都引用该 receipt。
3. 真实 `CommandProcessor` 执行 `SMOKEJSON` 能看到同一组字段，并且 `GET restore:real-read-token` 仍是 `(nil)`。

已有的 `command_tests`、`runtime_smoke_evidence_tests`、`release_verification_manifest_tests` 也同步更新，确保新增 v107 消费语义不会破坏 v239 历史证据链。

## 归档

本版运行截图和说明写入：

- `c/107/图片/`
- `c/107/解释/说明.md`

## 一句话总结

v107 把 Node v241 disabled dry-run command package 对 mini-kv 的 non-participation/no-start/no-write 边界变成可读、可测、可归档的证据，给 Node v244 做三方只读对齐，同时保持 mini-kv 存储和恢复执行面不变。
