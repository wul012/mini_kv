# mini-kv v134：runtime shell chain stop/prerequisite 非参与回执

## 本版目标

v134 的目标是给 Node v305 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v304 的 `runtime shell chain stop-or-prerequisite decision record`，确认自己只承认“必须先补齐显式审批前置条件”这个决策，不把它解释成 runtime shell 实现许可。

它不是什么：不是新的执行入口，不绕过审批/契约，不读取 credential value，不解析 raw endpoint URL，不实例化 provider/resolver client，不发 HTTP/TCP，不写 ledger/schema/storage，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不让 mini-kv 成为 audit/order authority。

## 模块职责

`src/runtime_credential_resolver_runtime_shell_stop_prerequisite_receipts.cpp` 是本版核心实现。它固化 Node v304 的关键快照：`decision_digest=9212d0b804fdc1eda9098ac70d2441681730a98ff736776859811df9e288a654`、`runtime_shell_chain_decision=require-explicit-approval-prerequisites-before-runtime-shell`、`selected_path=continue-only-as-blocked-prerequisite-review`、6 个缺失前置、8 个 no-go condition、17/17 检查，以及 Node v303 upstream echo digest。

`include/minikv/runtime_evidence_receipts.hpp` 只新增两个声明：一个 digest 函数，一个 JSON formatter。这样 v134 是 runtime evidence receipt 家族的新成员，但不会继续扩大 v132/v133 已经稳定的实现文件。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt` 字段，`diagnostics.notes` 增加 v134 标记，并把 `node_consumption` 首句滚动到 Node v305，同时保留 v133/v132/v131 等历史消费语义。

## 关键字段怎么读

`read_only=true` 和 `execution_allowed=false` 说明这只是控制面可读证据，不是 mini-kv 命令执行许可。

`runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_only=true` 说明 mini-kv 只参与“停止/前置条件决策回执证明”，不参与 runtime shell implementation/invocation。

`runtime_shell_chain_decision=require-explicit-approval-prerequisites-before-runtime-shell` 来自 Node v304，意思是运行壳链路必须先补齐显式审批前置条件，而不是现在开始实现。

`ready_for_node_v305_stop_prerequisite_upstream_echo_verification=true` 只表示 Node v305 可以读这个回执做上游对齐；`ready_for_node_v305_runtime_shell_implementation=false` 明确挡住实现跳跃。

`prerequisite_count=6` 和 `missing_runtime_prerequisite_count=6` 说明 operator approval artifact、credential handle readiness、raw endpoint allowlist review、no-network tests、manual abort semantics、Java/mini-kv prerequisite echo 仍然只是 documented-missing 前置项。

`credential_value_read_allowed=false`、`raw_endpoint_url_parse_allowed=false`、`provider_client_instantiation_allowed=false`、`external_request_sent=false`、`approval_ledger_written=false`、`schema_migration_executed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false` 共同封住 credential、endpoint、provider/client、network、ledger/schema、restore/compact/write 命令边界。

## Fixture 与 Manifest

`fixtures/release/credential-resolver-runtime-shell-chain-stop-or-prerequisite-non-participation-receipt.json` 是独立 fixture，来自真实 `SMOKEJSON` 输出抽取，receipt digest 为 `fnv1a64:7178c69528e0c208`。

`fixtures/release/runtime-smoke-evidence.json` 从 v19 滚动到 `mini-kv-runtime-smoke-evidence.v20`，新增 v134 回执字段和 Node v305 消费提示。

`fixtures/release/verification-manifest.json` 新增 `minikv_runtime_shell_chain_stop_prerequisite_receipt_tests`、v134 fixture 路径、read-only smoke expected 文案，以及同一份 v134 回执快照。

## 测试保护什么

`tests/credential_resolver_runtime_shell_chain_stop_or_prerequisite_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `SMOKEJSON` 输出，确保 v304/v303 关键 digest、17/17 检查、6 个缺失前置、8 个 no-go condition、v134 digest 和所有关闭边界一致。

`runtime_smoke_evidence_tests.cpp`、`release_verification_manifest_tests.cpp`、`smokejson_command_receipt_tests.cpp` 继续保护滚动证据入口。旧测试不再要求 v133 永远是 `node_consumption` 首句，而是要求 v134 成为新首句，同时 v133/v132 历史语义仍存在。

`runtime_no_start_no_write_follow_up_tests.cpp` 和 `operator_window_no_start_no_write_receipt_tests.cpp` 只把 runtime smoke evidence 版本号跟到 v20，原本保护的 no-start/no-write 语义不变。

## 真实 Smoke 证明

本版真实 CLI smoke 执行 `SMOKEJSON`、`CHECKJSON SMOKEJSON`、`GET restore:real-read-token`、`QUIT`。结果证明 `SMOKEJSON` 暴露 v134 回执，`CHECKJSON SMOKEJSON` 仍是只读命令，restore token 仍为 `(nil)`，没有写入或 restore 痕迹。

一句话总结：v134 给 Node v305 提供了 runtime shell chain 停止/前置条件决策的 mini-kv 侧证据，同时把 runtime shell、credential、endpoint、network、ledger/schema、restore/compact/write 命令和 authority 边界继续锁住。
