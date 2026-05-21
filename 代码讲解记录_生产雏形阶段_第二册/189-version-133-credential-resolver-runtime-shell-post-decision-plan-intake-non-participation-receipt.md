# mini-kv v133：runtime shell post-decision plan intake 非参与回执

## 本版目标

v133 的目标是给 Node v302 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v301 的 `post-decision continuation plan intake`，确认自己只承认“继续 blocked planning”这个计划入口，不把它解释成 runtime shell 实现许可。

它不是什么：不是新的执行入口，不绕过审批/契约，不读取 credential value，不解析 raw endpoint URL，不发 HTTP/TCP，不写 ledger/schema/storage，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不让 mini-kv 成为 audit/order authority。

## 模块职责

`src/runtime_credential_resolver_runtime_shell_post_decision_plan_receipts.cpp` 是本版核心实现。它固化 Node v301 的关键快照：`plan_intake_state`、`intake_digest`、`proof_digest`、`selected_continuation_decision=continue-blocked-planning`、4 个 continuation option、25/25 检查，以及 Node v300 upstream echo digest。

`include/minikv/runtime_evidence_receipts.hpp` 只新增两个声明：一个 digest 函数，一个 JSON formatter。这样对外仍然是 runtime evidence receipt 家族的一员，旧 receipt 文件不用继续膨胀。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt` 字段，`diagnostics.notes` 增加 v133 标记，并把 `node_consumption` 首句滚动到 Node v302，同时保留 v132/v131 等历史消费语义。

## 关键字段怎么读

`read_only=true` 和 `execution_allowed=false` 说明这只是控制面可读证据，不是 mini-kv 命令执行许可。

`runtime_shell_post_decision_plan_intake_non_participation_receipt_only=true` 说明 mini-kv 只参与“回执证明”，不参与 runtime shell implementation/invocation。

`selected_continuation_decision=continue-blocked-planning` 来自 Node v301，意思是继续记录 blocked planning，而不是批准 runtime shell。

`ready_for_node_v302_post_decision_plan_intake_upstream_echo_verification=true` 只表示 Node v302 可以读这个回执做上游对齐；`ready_for_node_v302_runtime_shell_implementation=false` 明确挡住实现跳跃。

`credential_value_read_allowed=false`、`raw_endpoint_url_parse_allowed=false`、`provider_client_instantiation_allowed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`schema_migration_executed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false` 共同封住 credential、endpoint、provider/client、network、ledger/schema、restore/compact/write 命令边界。

## Fixture 与 Manifest

`fixtures/release/credential-resolver-runtime-shell-post-decision-plan-intake-non-participation-receipt.json` 是独立 fixture，来自真实 `SMOKEJSON` 输出抽取，receipt digest 为 `fnv1a64:d2739fd08da1653e`。

`fixtures/release/runtime-smoke-evidence.json` 从 v18 滚动到 `mini-kv-runtime-smoke-evidence.v19`，新增 v133 回执字段和 Node v302 消费提示。

`fixtures/release/verification-manifest.json` 新增 `minikv_runtime_shell_post_decision_plan_intake_receipt_tests`、v133 fixture 路径、read-only smoke expected 文案，以及同一份 v133 回执快照。

## 测试保护什么

`tests/credential_resolver_runtime_shell_post_decision_plan_intake_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `SMOKEJSON` 输出，确保 v301/v300 关键 digest、25/25 检查、continuation options、v133 digest 和所有关闭边界一致。

`runtime_smoke_evidence_tests.cpp`、`release_verification_manifest_tests.cpp`、`smokejson_command_receipt_tests.cpp` 继续保护滚动证据入口。旧测试不再要求 v132 永远是 `node_consumption` 首句，而是要求 v133 成为新首句，同时 v132 历史语义仍存在。

`runtime_no_start_no_write_follow_up_tests.cpp` 和 `operator_window_no_start_no_write_receipt_tests.cpp` 只把 runtime smoke evidence 版本号跟到 v19，原本保护的 no-start/no-write 语义不变。

## 真实 Smoke 证明

本版真实 CLI smoke 执行 `SMOKEJSON`、`CHECKJSON SMOKEJSON`、`GET restore:real-read-token`、`QUIT`。结果证明 `SMOKEJSON` 暴露 v133 回执，`CHECKJSON SMOKEJSON` 仍是只读命令，restore token 仍为 `(nil)`，没有写入或 restore 痕迹。

一句话总结：v133 给 Node v302 提供了继续 blocked planning 的 mini-kv 侧证据，同时把 runtime shell、credential、endpoint、network、ledger/schema、restore/compact/write 命令和 authority 边界继续锁住。
