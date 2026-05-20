# v118 Credential Resolver Production-Readiness Blocked-Decision Non-Participation Receipt

## 目标和边界

本版给 Node v269 增加 mini-kv 的只读回执，用来对应 Node v268 的 credential resolver production-readiness blocked decision gate。它的角色是让控制面确认：Node v267 的 fake-shell archive echo 链已经就绪，但 Node v268 仍把真实 credential resolver 前置实施计划判定为 blocked，所以 mini-kv 只能回显阻断决策证据。

它不是什么：不是新的执行入口，不是 resolver，不是 resolver client，不是 secret provider，不是 endpoint parser，不会读 credential value，不会解析 raw endpoint URL，不会发外部请求，不会连接 managed audit，不会写本地 storage、approval ledger、managed audit state 或 production record，也不会执行 schema migration、`LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。

## 版本位置

Node v268 的核心结论是 `readiness_decision=blocked`：25 个检查中只有 15 个通过，10 个 pre-implementation requirement 仍然缺失，并形成 10 个 production blocker。mini-kv v118 做的是把这个 blocked decision 镜像成稳定的 runtime evidence，供 Node v269 做上游 echo verification。

这个范围是合理的，因为它没有推进真实 resolver 或 secret provider，只把已存在的 Node v268 阻断结论接入 `SMOKEJSON`、release fixture 和 manifest。优化项也一起处理了：v118 没有继续堆进旧的 credential resolver receipt 文件，而是拆到 `src/runtime_credential_resolver_decision_receipts.cpp`，让 v114-v117 的文件继续保持可审。

## 代码模块

`include/minikv/runtime_evidence_receipts.hpp` 仍是公共契约入口，本版新增 `credential_resolver_production_readiness_blocked_decision_non_participation_receipt_digest` 和 `format_credential_resolver_production_readiness_blocked_decision_non_participation_receipt_json`。

`src/runtime_credential_resolver_decision_receipts.cpp` 是新拆出的实现文件。它固定 Node v268 的 profile、route、decision digest、blocked state、Node v267 source digest、检查计数和 blocker 代码，并生成 receipt digest。它只负责 blocked decision 类回执，不改 WAL、snapshot、restore、command parser 或写命令处理。

`src/command_response_formatters.cpp` 把 `credential_resolver_production_readiness_blocked_decision_non_participation_receipt` 加进 `SMOKEJSON` 聚合对象、notes 和 diagnostics。控制面既可以读独立 fixture，也可以从真实运行时 `SMOKEJSON` 读到同一份字段。

## 契约字段

控制面读到 `read_only=true`、`execution_allowed=false`、`blocked_decision_only=true`、`production_readiness_gate_only=true` 和 `read_only_decision_gate=true` 时，应理解为 mini-kv 只是在回显 Node v268 的 blocked gate，不授予任何实施权限。

关键阻断事实是：`source_readiness_decision=blocked`、`check_count=25`、`passed_check_count=15`、`missing_pre_implementation_requirement_count=10`、`production_blocker_count=10`，并且十个 pre-implementation requirement 都是 false，包括 plan document、credential handle boundary、endpoint handle boundary、secret provider stub、operator approval、rollback、redaction、external request simulation、schema migration policy 和 audit ledger write policy。

硬边界字段仍然全部关住：`real_resolver_implementation_allowed=false`、`credential_resolver_implemented=false`、`credential_resolver_invoked=false`、`resolver_client_instantiated=false`、`secret_provider_instantiated=false`、`credential_value_read_allowed=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`storage_write_allowed=false`、`approval_ledger_write_allowed=false`、`schema_migration_execution_allowed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`managed_audit_storage_backend=false` 和 `order_authoritative=false`。

## Fixture 和测试

`fixtures/release/credential-resolver-production-readiness-blocked-decision-non-participation-receipt.json` 是 v118 独立回执。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 内嵌同一份 receipt，保证独立文件、manifest 和真实 `SMOKEJSON` 的 digest 一致。

`tests/credential_resolver_production_readiness_blocked_decision_non_participation_receipt_tests.cpp` 检查独立 fixture、runtime smoke fixture、release manifest、真实 `CommandProcessor` 的 `SMOKEJSON`，以及 `GET restore:real-read-token`。它覆盖 Node v268 blocked decision、Node v267 source 摘要、10 个 blocker、receipt digest，以及 no-start/no-write/no-credential/no-resolver/no-schema/no-restore 边界。

已有 `command_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 也同步检查新 `SMOKEJSON` key、diagnostics hint、manifest test 名称和 fixture 路径，避免运行时聚合与仓库证据漂移。

## 总结

v118 让跨项目链路可以继续推进到 Node v269 的 blocked-decision upstream echo verification，但 mini-kv 仍然只是只读证据提供者，不成为 resolver、credential reader、endpoint parser、transport、managed audit store 或 Java order authority。
