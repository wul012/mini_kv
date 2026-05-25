# mini-kv v142: abort/rollback semantics contract 非参与回执

v142 的目标是给 Node v327 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v326 的 `abort/rollback semantics contract intake`，说明自己理解最后一个前置条件的字段、拒绝码、禁止动作、来源链和后续消费顺序，但不把自己变成 abort 执行器、rollback 执行器、runbook 权威、cleanup evidence 权威或恢复 checkpoint 权威。

这版不是什么：不是 runtime shell，不是 credential resolver 实现，不是 rollback 执行入口，不执行 deployment action，不执行 Java SQL，不发送 HTTP/TCP，不打开 socket，不读取 credential value，不解析 raw endpoint URL，不实例化 secret provider 或 resolver client，不写 approval ledger/schema/storage，不自动启动 Node/Java/mini-kv，也不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`。

`include/minikv/runtime_evidence_receipts.hpp` 只新增 v142 的 digest 和 JSON formatter 声明。入口仍然属于 runtime evidence receipt 家族，不改变 Store、WAL、snapshot、restore、TCP 协议或命令执行行为。

`src/runtime_credential_resolver_abort_rollback_semantics_receipts.cpp` 是本版核心实现。它独立承载 Node v326 contract intake、Node v325 no-network safety fixture prerequisite closure review、Node v313 catalog 指向、字段计数、warning/recommendation 和关闭边界。这样 v141 的 no-network safety fixture receipt 文件不用继续膨胀，也符合“不要形成巨型代码文件”的维护规则。

控制面最重要的语义有三组。第一组是 Node v326 合同事实：`contract_digest=fe05bcfd65aabf56ef170bf458837053a11edf0ae44ad203a88d4ecd284299f9`、10 个 required fields、14 个 prohibited fields、6 个 rejection reasons、11 个 no-go boundaries、21/21 checks。第二组是 prerequisite 状态：`abort-rollback-semantics` 从 `still-missing` 进入 `contract-intake-defined`，但需要 Java v150 和 mini-kv v142 upstream echo 后，Node v327 才能继续验证。第三组是 mini-kv 自身边界：`read_only=true`、`execution_allowed=false`、`abort_rollback_semantics_executed=false`、`rollback_execution_allowed=false`、`deployment_action_allowed=false`、`java_sql_execution_allowed=false`、`mini_kv_write_command_allowed=false`、`runtime_shell_command_rendered=false`、`audit_authoritative=false`、`order_authoritative=false`。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_abort_rollback_semantics_contract_non_participation_receipt` 字段，`diagnostics.notes` 增加 v142 标记，`node_consumption` 首句滚动到 Node v327，同时保留 Node v324/v141 和更早历史消费语义。v141 的测试继续检查历史句子仍存在，而不是要求它永远位于第一句。

`fixtures/release/credential-resolver-abort-rollback-semantics-contract-non-participation-receipt.json` 是 standalone fixture；`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 嵌入同一份 receipt。这样 Node v327 可以从独立 fixture、runtime smoke 或 manifest 三个入口读到同一个 digest 和同一组关闭边界。

`tests/credential_resolver_abort_rollback_semantics_contract_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，覆盖 Node v326 contract digest、Node v325 review digest、字段计数、prerequisite transition、v142 digest，以及 rollback/deployment/Java SQL/mini-kv write/runtime shell/network/credential/raw URL/provider/client/write/authority 的关闭边界。`tests/smokejson_command_receipt_tests.cpp`、`tests/runtime_smoke_evidence_tests.cpp`、`tests/release_verification_manifest_tests.cpp` 继续保护聚合入口和归档 fixture。

验证闭环包括 clean configure/build、定向 CTest、全量 CTest、真实 CLI smoke 和真实 TCP smoke。CLI smoke 证明 `SMOKEJSON` 暴露 v142 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`GET restore:real-read-token` 返回 `(nil)`；TCP smoke 证明同一回执可以经 server/client 真实链路读取，随后服务端已停止。归档截图放在 `d/142/图片/`，说明放在 `d/142/解释/说明.md`。

一句话总结：v142 给 Node v327 提供了 abort/rollback semantics contract 的 mini-kv 侧只读非参与证据，同时把 rollback execution、deployment、Java SQL、mini-kv write、runtime shell、network、credential/raw endpoint、provider/client、ledger/schema/storage、auto-start、restore/compact/write 命令和 audit/order/abort-rollback authority 边界继续锁住。
