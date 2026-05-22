# mini-kv v137: human approval post-echo decision gate 非参与回执
## 本版目标

v137 的目标是给 Node v311 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v310 的 `human approval artifact review post-echo decision gate`，证明自己理解“Node v309 已经完成上游 echo，但进入 runtime shell 讨论前仍缺哪些明确前置审批”，同时继续声明 mini-kv 只提供证据，不参与实现。

它不是什么：不是 runtime shell 实现，不是 runtime shell 调用入口，不绕过人工审批/契约，不接收 credential value，不解析 raw endpoint URL，不实例化 provider/resolver client，不发送 HTTP/TCP，不写 approval ledger/schema/storage，不自动启动上游，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不让 mini-kv 成为 audit/order authority。
## 模块职责

`src/runtime_credential_resolver_human_approval_post_echo_receipts.cpp` 是本版核心实现。它固定 Node v310 的 post-echo decision gate 快照：`profile_version=managed-audit-manual-sandbox-connection-credential-resolver-human-approval-artifact-review-post-echo-decision-gate.v1`、`decision_gate_state=human-approval-artifact-review-post-echo-decision-gate-ready`、decision digest `91c0927b63fee6dd0fb44e27cd82c7f700ceeeb4cef750606777bd4d3436102c`、6 个 documented-missing prerequisite、9 个 explicit no-go condition、17/17 v310 check，以及从 Node v309 继承的 23/23 上游 echo 验证。

`include/minikv/runtime_evidence_receipts.hpp` 只新增两个声明：一个 digest 函数，一个 JSON formatter。这样 v137 是 runtime evidence receipt 家族的新成员，但不会继续扩大 v136 的 human approval artifact receipt 文件。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt` 字段，`diagnostics.notes` 增加 v137 标记，并把 `node_consumption` 首句滚动到 Node v311，同时保留 Node v309、v307、v305 等历史消费语义。

`fixtures/release/credential-resolver-human-approval-artifact-review-post-echo-decision-gate-non-participation-receipt.json` 是独立 fixture。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 同步暴露同一份回执，并加入 Node v311 可消费的 expected string 与新测试目标。
## 关键字段怎么读

`read_only=true` 和 `execution_allowed=false` 说明这只是控制面可读证据，不是 mini-kv 命令执行许可。

`human_approval_artifact_review_post_echo_decision_gate_non_participation_receipt_only=true` 说明 mini-kv 只做“人工审批制品审阅 post-echo 决策门的非参与回执”，不把 Node v310 的决策门解释成 runtime shell implementation/invocation。

`source_node_v310_reference.decision_gate.required_prerequisites` 的 6 项全是 `documented-missing`：signed human approval artifact、credential handle approval、endpoint handle allowlist approval、no-network safety fixture、abort/rollback semantics、Java/mini-kv decision echo。它们是继续讨论 runtime shell 前必须补齐的前置条件。

`explicit_no_go_conditions` 的 9 项继续挡住 runtime shell implementation/invocation、credential value read、raw endpoint URL parse、provider/client instantiation、external request、ledger/schema write、mini-kv write/authority 和 automatic upstream start。

`ready_for_node_v311_human_approval_artifact_review_post_echo_decision_upstream_echo_verification=true` 只表示 Node v311 可以读取这份 mini-kv v137 回执做上游对齐；`ready_for_node_v311_before_upstream_echo=false`、`ready_for_disabled_runtime_shell_implementation=false`、`ready_for_managed_audit_resolver_implementation=false` 则继续挡住实现跳跃。

`credential_value_accepted=false`、`raw_endpoint_url_accepted=false`、`provider_client_instantiation_allowed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`schema_migration_executed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`automatic_upstream_start=false` 共同封住秘密、端点、provider/client、网络、ledger/schema、恢复/压缩/写命令、自动启动和 authority 边界。
## 测试保护什么

`tests/credential_resolver_human_approval_post_echo_decision_gate_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，确保 Node v310 decision digest、6/6/9 计数、17/17 检查、Node v309 source digest、v137 digest 和所有关闭边界一致。

`runtime_smoke_evidence_tests.cpp`、`release_verification_manifest_tests.cpp`、`smokejson_command_receipt_tests.cpp` 继续保护滚动入口。旧测试不要求历史消费语义永远是首句，而是要求 v137/v311 成为最新首句，同时 v136/v309、v135/v307 等历史语义仍然存在。

本版保留 CLI/TCP 真实 smoke：`SMOKEJSON` 能暴露 v137 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`CHECKJSON SETNXEX` 只展示风险和用法而不执行，`GET restore:real-read-token` 返回 `(nil)`，说明没有写入 restore token 或触发恢复、压缩、写命令。

一句话总结：v137 给 Node v311 提供了 post-echo decision gate 的 mini-kv 侧只读证据，同时把 credential、endpoint、network、provider/client、ledger/schema、auto-start、restore/compact/write 命令和 audit/order authority 边界继续锁住。
