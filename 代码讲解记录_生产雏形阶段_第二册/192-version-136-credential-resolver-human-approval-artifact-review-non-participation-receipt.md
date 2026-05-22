# mini-kv v136: human approval artifact review 非参与回执
## 本版目标

v136 的目标是给 Node v309 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v308 的 `human approval artifact review packet`，证明自己理解“人工提交的审批前置制品审阅包应该具备哪些字段、缺哪些字段如何拒绝、哪些字段绝不能出现”，但不把这件事解释成可以实现或调用 runtime shell。

它不是什么：不是新的执行入口，不绕过人工审批/契约，不接收 credential value，不解析 raw endpoint URL，不实例化 provider/resolver client，不发送 HTTP/TCP，不写 approval ledger/schema/storage，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不让 mini-kv 成为 audit/order authority。
## 模块职责

`src/runtime_credential_resolver_human_approval_artifact_receipts.cpp` 是本版核心实现。它固定 Node v308 的人工审批制品审阅包快照：`profile_version=managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-packet.v1`、`review_packet_state=human-approval-artifact-review-packet-ready`、packet digest `b0dda954c509337c96a645c177be521d0a200d8f8e6d52081ed8c0df9a43ccf3`、9 个 required field、9 个 prohibited field、13 个 rejection reason、9 个 missing-field check、12 个 no-go boundary，以及从 Node v307 继承的三方 artifact contract 对齐证据。

`include/minikv/runtime_evidence_receipts.hpp` 只新增两个声明：一个 digest 函数，一个 JSON formatter。这样 v136 是 runtime evidence receipt 家族的新成员，但不会继续扩大 v135 的 approval prerequisite artifact receipt 文件。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_human_approval_artifact_review_non_participation_receipt` 字段，`diagnostics.notes` 增加 v136 标记，并把 `node_consumption` 首句滚动到 Node v309，同时保留 Node v307、v305 等历史消费语义。

`fixtures/release/credential-resolver-human-approval-artifact-review-non-participation-receipt.json` 是独立 fixture。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 同步暴露同一份回执，并加入 Node v309 可消费的 expected string 与新测试目标。
## 关键字段怎么读

`read_only=true` 和 `execution_allowed=false` 说明这只是控制面可读证据，不是 mini-kv 命令执行许可。

`human_approval_artifact_review_non_participation_receipt_only=true` 说明 mini-kv 只做“人工审批制品审阅包的非参与回执”，不参与 runtime shell implementation/invocation。

`required_field_count=9` 对应 Node v308 要求的审阅包字段，包括 `artifact_id`、`operator_approval_reference`、`credential_handle_review_status`、`endpoint_handle_allowlist_review_status`、`no_network_safety_test_reference`、`manual_abort_semantics_reference`、`rollback_semantics_reference`、`created_by_operator_identity` 和 `audit_correlation_id`。

`prohibited_field_count=9` 明确禁止 `credential_value`、`raw_endpoint_url`、secret provider config、resolver client config、external request payload、approval ledger mutation、schema migration SQL、mini-kv write command 和 runtime shell invocation request。

`ready_for_node_v309_human_approval_artifact_review_upstream_echo_verification=true` 只表示 Node v309 可以读取这份 mini-kv v136 回执做上游对齐；`ready_for_node_v309_before_upstream_echo=false`、`ready_for_disabled_runtime_shell_implementation=false`、`ready_for_managed_audit_resolver_implementation=false` 则继续挡住实现跳跃。

`credential_value_accepted=false`、`raw_endpoint_url_accepted=false`、`provider_client_instantiation_allowed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`schema_migration_executed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false` 共同封住秘密、端点、provider/client、网络、ledger/schema、恢复/压缩/写命令和 authority 边界。
## 测试保护什么

`tests/credential_resolver_human_approval_artifact_review_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，确保 Node v308 packet digest、9/9/13/9/12 计数、v136 digest、Node v307 继承证据和所有关闭边界一致。

`runtime_smoke_evidence_tests.cpp`、`release_verification_manifest_tests.cpp`、`smokejson_command_receipt_tests.cpp` 继续保护滚动入口。旧测试不再要求 v307 永远是 `node_consumption` 首句，而是要求 v309 成为最新首句，同时 v307/v135 历史语义仍然存在。

本版还补了 CLI/TCP 真实 smoke：`SMOKEJSON` 能暴露 v136 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`CHECKJSON LOAD/COMPACT/SETNXEX` 只展示风险而不执行，`GET restore:real-read-token` 返回 `(nil)`，说明没有写入 restore token 或触发恢复、压缩、写命令。

一句话总结：v136 给 Node v309 提供了人工审批制品审阅包的 mini-kv 侧只读证据，同时把 credential、endpoint、network、provider/client、ledger/schema、restore/compact/write 命令和 audit/order authority 边界继续锁住。
