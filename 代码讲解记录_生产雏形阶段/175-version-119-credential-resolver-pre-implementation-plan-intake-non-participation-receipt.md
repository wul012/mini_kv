# v119 Credential Resolver Pre-Implementation Plan-Intake Non-Participation Receipt

## 目标和边界

本版给 Node v272 增加 mini-kv 的只读回执，用来对应 Node v270 的 credential resolver pre-implementation plan intake。它的角色是让控制面确认：Node v270 已经把 Node v269 blocked-decision echo 链接入计划摄取，并定义了十个后续真实 resolver 实施前必须审查的边界。

它不是什么：不是新的执行入口，不是 resolver，不是 resolver client，不是 secret provider，不会读取 credential value，不会解析 raw endpoint URL，不会发外部请求，不会连接 managed audit，不会写 storage、approval ledger、managed audit state 或 production record，也不会执行 schema migration、`LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。

## 版本位置

Node v270 的关键变化不是实施真实 credential resolver，而是把十个前置边界从 v268 的缺失 blocker 推进为 plan-intake-only 的定义状态。mini-kv v119 做的是把这份 plan intake 镜像成稳定 runtime evidence，供 Node v272 在 Java v112 和 mini-kv v119 都存在后继续做 upstream echo verification。

这个范围是合理的，因为它只记录计划边界和不参与证明，没有打开任何运行时能力。优化项也一起处理了：本版没有继续膨胀旧的 credential resolver receipt 文件，而是新增 `src/runtime_credential_resolver_plan_receipts.cpp`，让 plan-intake 类回执独立维护。

## 代码模块

`include/minikv/runtime_evidence_receipts.hpp` 仍是公共契约入口，本版新增 `credential_resolver_pre_implementation_plan_intake_non_participation_receipt_digest` 和 `format_credential_resolver_pre_implementation_plan_intake_non_participation_receipt_json`。

`src/runtime_credential_resolver_plan_receipts.cpp` 是新拆出的实现文件。它固定 Node v270 profile、route、state、plan digest、intake digest、十个 boundary code、十个 requirement code、Node v269 source digest，以及 check/summary 计数，并生成 receipt digest。它只负责 plan-intake 类回执，不改 WAL、snapshot、restore、command parser 或写命令处理。

`src/command_response_formatters.cpp` 把 `credential_resolver_pre_implementation_plan_intake_non_participation_receipt` 加进 `SMOKEJSON` 聚合对象、notes 和 diagnostics。控制面既可以读独立 fixture，也可以从真实运行时 `SMOKEJSON` 读到同一份字段。

## 契约字段

控制面读到 `read_only=true`、`execution_allowed=false`、`plan_intake_only=true`、`read_only_plan_intake=true` 和 `credential_resolver_pre_implementation_plan_intake_non_participation_receipt_only=true` 时，应理解为 mini-kv 只是在回显 Node v270 的计划摄取结果，不授予任何实施权限。

关键计划事实是：`plan_mode=plan-intake-only`、`plan_digest=3aef2d1d10fa5a0063f9be43af49122de0b38999b0d065a8addb5c3ed1f95ad8`、`intake_digest=43c40f8aee830a93fa36845366c948abca1353555756154b8dcbb8053a17e603`、`check_count=26`、`passed_check_count=26`、`boundary_count=10`、`defined_boundary_count=10`、`missing_boundary_count=0`、`production_blocker_count=0`，并且 `next_required_echo_versions` 是 `Java v112` 和 `mini-kv v119`。

十个已定义边界包括 plan document、credential handle、endpoint handle、disabled secret provider stub、operator approval、rollback、redaction、external request simulation、schema migration policy 和 audit ledger write policy。硬边界字段仍然全部关住：`real_resolver_implementation_allowed=false`、`credential_resolver_implemented=false`、`credential_resolver_invoked=false`、`resolver_client_instantiated=false`、`secret_provider_instantiated=false`、`credential_value_read_allowed=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`storage_write_allowed=false`、`approval_ledger_write_allowed=false`、`schema_migration_executed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`managed_audit_storage_backend=false` 和 `order_authoritative=false`。

## Fixture 和测试

`fixtures/release/credential-resolver-pre-implementation-plan-intake-non-participation-receipt.json` 是 v119 独立回执。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 内嵌同一份 receipt，保证独立文件、manifest 和真实 `SMOKEJSON` 的 digest 一致。

`tests/credential_resolver_pre_implementation_plan_intake_non_participation_receipt_tests.cpp` 检查独立 fixture、runtime smoke fixture、release manifest、真实 `CommandProcessor` 的 `SMOKEJSON`，以及 `GET restore:real-read-token`。它覆盖 Node v270 plan intake、Node v269 source echo、十个边界、receipt digest，以及 no-start/no-write/no-credential/no-resolver/no-schema/no-restore 边界。

已有 `command_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 也同步检查新 `SMOKEJSON` key、diagnostics hint、manifest test 名称和 fixture 路径，避免运行时聚合与仓库证据漂移。

## 总结

v119 让跨项目链路可以从 blocked decision 进入 plan-intake upstream echo verification，但 mini-kv 仍然只是只读证据提供者，不成为 resolver、credential reader、endpoint parser、transport、managed audit store 或 Java order authority。
