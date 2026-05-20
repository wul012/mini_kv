# 176. version 120 credential resolver disabled implementation candidate non-participation receipt

## 本版目标

v120 只做 mini-kv 对 Node v273 `credential resolver disabled implementation candidate review` 的只读非参与回执。它的角色是让后续 Node v274 能确认：mini-kv 理解 Node v273 的 disabled interface / fake wiring review 边界，但 mini-kv 不实现 resolver、不实例化 resolver client 或 secret provider、不读取 credential value、不解析 raw endpoint URL、不发送 external request、不写 managed audit / approval ledger / storage，也不执行 `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`。

它不是新的执行入口，不是 resolver stub，不是 secret provider runtime，不绕过审批契约，不承载 audit/order 权威状态，也不改变 WAL、snapshot、restore、write command 的核心语义。

## 入口和文件职责

`src/runtime_credential_resolver_disabled_candidate_receipts.cpp` 是本版新增的实现文件。它保存 Node v273 的稳定契约字段：`candidateDigest=12862fa65cad3a8db92f88fba86b74c25e584f35614c7a47367d441358b8b7d7`、Node v272 source verification digest、21/21 checks、10 个 candidate decision、4 个 disabled-ready boundary、6 个 approval-required boundary、6 个 request fields、7 个 response fields、6 个 failure classes。

`include/minikv/runtime_evidence_receipts.hpp` 只新增两个公开函数：digest 函数和 JSON formatter。这样 `SMOKEJSON` 能挂载新 receipt，但旧的 credential resolver plan-intake 文件不用继续膨胀。

`src/command_response_formatters.cpp` 把新字段接入 `SMOKEJSON.credential_resolver_disabled_implementation_candidate_non_participation_receipt`，并在 diagnostics 的 `node_consumption` 中把 Node v274 放到最前面。这里仍只是读侧 JSON 聚合，不执行任何命令。

## 关键字段怎么读

`read_only=true` 和 `execution_allowed=false` 表示这是运行时可读证据，不是执行授权。`disabled_implementation_candidate_review_only=true` 和 `read_only_candidate_review=true` 表示 mini-kv 只承认 Node v273 是 review-only。

`ready_for_disabled_resolver_interface_candidate=true` 只说明 Node v273 的 disabled interface candidate 可被后续验证理解，不代表 mini-kv 可以实现 resolver。`real_resolver_implementation_allowed=false`、`resolver_client_instantiated=false`、`secret_provider_instantiated=false`、`credential_value_read_allowed=false`、`raw_endpoint_url_parse_allowed=false`、`external_request_allowed=false`、`approval_ledger_write_allowed=false`、`schema_migration_allowed=false` 和 `load_restore_compact_executed=false` 才是控制面必须优先看的边界。

`candidate_ready_boundary_codes` 只有 `PLAN_DOCUMENT`、`DISABLED_SECRET_PROVIDER_STUB`、`REDACTION_POLICY`、`EXTERNAL_REQUEST_SIMULATION` 四项，且它们仍只是 disabled/fake review 材料。`approval_required_boundary_codes` 保留 credential handle、endpoint handle、operator approval、rollback、schema migration、audit ledger write 六项，说明这些不能在 v120 变成 mini-kv 行为。

## Fixture 和 Manifest

`fixtures/release/credential-resolver-disabled-implementation-candidate-non-participation-receipt.json` 是独立 receipt fixture，保留 source review、source Node v272 reference、candidate decisions、interface shape、fake wiring review、checks、summary、warnings/recommendations 和 pause diagnostics。

`fixtures/release/runtime-smoke-evidence.json` 与 `fixtures/release/verification-manifest.json` 同步暴露同一个 receipt。manifest 增加 `minikv_credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests`，让后续 release verification 能检查独立 fixture、runtime fixture 和 manifest 三处一致。

## 测试保护

`tests/credential_resolver_disabled_implementation_candidate_non_participation_receipt_tests.cpp` 覆盖四层：

- 独立 receipt fixture 必须含 v273 review state、candidate digest、Node v272 digest、counts、candidate ready / approval-required boundary、interface shape、fake wiring review 和所有 no-side-effect flags。
- runtime smoke fixture 必须挂载同一 receipt，并保留 Node v274 consumption hint。
- verification manifest 必须登记新测试、新 fixture 和相同 receipt digest。
- 真实 `CommandProcessor` 执行 `SMOKEJSON` 时必须返回同一 receipt；随后 `GET restore:real-read-token` 仍返回 `(nil)`，证明本版没有开启 restore/load 路径。

## 一句话总结

v120 把 Node v273 的 disabled implementation candidate review 变成 mini-kv 可被 Node v274 只读验证的非参与证据，同时继续守住 mini-kv 不做 resolver、不读密钥、不写审计状态、不触发存储执行的边界。
