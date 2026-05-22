# mini-kv v135: approval prerequisite artifact intake 非参与回执

## 本版目标

v135 的目标是给 Node v307 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v306 的 `approval prerequisite artifact intake plan`，证明自己理解“后续 runtime shell 前置审批制品应该长什么样”，但不把这件事解释成可以实现或调用 runtime shell。

它不是什么：不是新的执行入口，不绕过审批/契约，不接收 credential value，不解析 raw endpoint URL，不实例化 provider/resolver client，不发送 HTTP/TCP，不写 approval ledger/schema/storage，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不让 mini-kv 成为 audit/order authority。

## 模块职责

`src/runtime_credential_resolver_approval_prerequisite_artifact_receipts.cpp` 是本版核心实现。它固定 Node v306 的审批前置制品接收计划快照：`profile_version=managed-audit-manual-sandbox-connection-credential-resolver-approval-prerequisite-artifact-intake-plan.v1`、`plan_state=approval-prerequisite-artifact-intake-plan-ready`、artifact digest `72f3e90606e40a978611fa4b8596c76c3ebc468124c4ead7bb9c4833130ee9c2`、12 个 required field、8 个 prohibited field、9 个 rejection reason、12 个 no-go boundary，以及从 Node v305 继承的 6 个 documented-missing prerequisite 和 8 个 no-go condition。

`include/minikv/runtime_evidence_receipts.hpp` 只新增两个声明：一个 digest 函数，一个 JSON formatter。这样 v135 是 runtime evidence receipt 家族的新成员，但不会继续扩大 v134 的 stop/prerequisite receipt 文件。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt` 字段，`diagnostics.notes` 增加 v135 标记，并把 `node_consumption` 首句滚动到 Node v307，同时保留 v305/v304 等历史消费语义。

`fixtures/release/credential-resolver-approval-prerequisite-artifact-intake-non-participation-receipt.json` 是独立 fixture。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 同步暴露同一份回执，并加入 Node v307 可消费的 expected string 与新测试目标。

## 关键字段怎么读

`read_only=true` 和 `execution_allowed=false` 说明这只是控制面可读证据，不是 mini-kv 命令执行许可。

`approval_prerequisite_artifact_intake_non_participation_receipt_only=true` 说明 mini-kv 只做“审批前置制品接收计划的非参与回执”，不参与 runtime shell implementation/invocation。

`required_field_count=12` 对应 Node v306 要求的非秘密制品字段，包括 `artifact_id`、`source_node_verification`、`operator_approval_reference`、credential/endpoint handle review、no-network safety test、manual abort/rollback semantics、Java/mini-kv echo version、operator identity 和 audit correlation。

`prohibited_field_count=8` 明确禁止 `credential_value`、`raw_endpoint_url`、secret provider config、resolver client config、external request payload、approval ledger mutation、schema migration SQL 和 mini-kv write command。

`ready_for_node_v307_approval_prerequisite_artifact_upstream_echo_verification=true` 只表示 Node v307 可以读取这份 mini-kv v135 回执做上游对齐；`ready_for_node_v307_before_upstream_echo=false`、`ready_for_disabled_runtime_shell_implementation=false`、`ready_for_managed_audit_resolver_implementation=false` 则继续挡住实现跳跃。

`credential_value_accepted=false`、`raw_endpoint_url_accepted=false`、`provider_or_client_config_accepted=false`、`external_request_payload_accepted=false`、`approval_ledger_mutation_accepted=false`、`schema_migration_sql_accepted=false`、`mini_kv_write_command_accepted=false` 共同封住秘密、端点、provider/client、网络、ledger/schema、写命令与 authority 边界。

## 测试保护什么

`tests/credential_resolver_approval_prerequisite_artifact_intake_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，确保 Node v306 artifact digest、12/8/9/12 计数、v135 digest、Node v305 继承证据和所有关闭边界一致。

`runtime_smoke_evidence_tests.cpp`、`release_verification_manifest_tests.cpp`、`smokejson_command_receipt_tests.cpp` 继续保护滚动入口。旧测试不再要求 v305 永远是 `node_consumption` 首句，而是要求 v307 成为最新首句，同时 v305 历史语义仍然存在。

本版还补了 CLI/TCP 真实 smoke：`SMOKEJSON` 能暴露 v135 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`CHECKJSON LOAD/COMPACT/SETNXEX` 只展示风险而不执行，`GET restore:real-read-token` 返回 `(nil)`，说明没有写入 restore token 或触发恢复/压缩/写命令。

一句话总结：v135 给 Node v307 提供了审批前置制品接收计划的 mini-kv 侧证据，同时把 credential、endpoint、network、provider/client、ledger/schema、restore/compact/write 命令和 audit/order authority 边界继续锁住。
