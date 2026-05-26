# mini-kv v143: input-hardening candidate gate 非参与回执

v143 的目标是给 Node v330 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v329 的 `implementation candidate gate / input-hardening decision`，说明自己理解为什么 6/6 prerequisite 关闭之后仍要先做 input export hardening，并明确 `mini-kv-stable-current-receipt` 需要稳定 current receipt 路径。

这版不是什么：不是 runtime shell design draft，不是 credential resolver 实现，不是 provider/client 实例化，不读取 credential value，不解析 raw endpoint URL，不发送 HTTP/TCP，不执行 Java SQL、deployment、rollback，不写 approval ledger/schema/storage，不自动启动 Node/Java/mini-kv，也不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX` 或任何 mini-kv write/admin command。

`include/minikv/runtime_evidence_receipts.hpp` 只新增 v143 digest 和 JSON formatter 声明。入口仍属于 runtime evidence receipt 家族，不改变 Store、WAL、snapshot、restore、TCP 协议或命令执行行为。

`src/runtime_credential_resolver_input_hardening_candidate_gate_receipts.cpp` 是本版核心实现。它独立承载 Node v329 candidate gate decision、Node v328 final prerequisite closure、stable current receipt export、checks、summary 和关闭边界。这样 v142 的 abort/rollback semantics receipt 文件不继续膨胀，符合“不要形成巨型代码文件”的维护规则。

控制面最重要的语义有三组。第一组是 Node v329 事实：`candidate_gate_decision=require-input-export-hardening-before-disabled-runtime-design`、`decision_digest=e92109116e9d106ffdaa5691aa2eaa626fb595f8a9f4020dbf903fbce7874e87`、4 个 input-hardening requirements、7 个 no-go conditions、16/16 checks。第二组是 Node v328 来源事实：6/6 prerequisites 已关闭，`next_step_mode=implementation-candidate-gate-only`，但这并不等于 runtime 可以设计或执行。第三组是 mini-kv 自身边界：`stable_current_receipt_export_required=true`、`stable_current_receipt_export_ready=true`、`ready_for_node_v330_before_java_v151=false`、`runtime_shell_design_draft_allowed=false`、`credential_value_read=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`mini_kv_write_command_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`audit_authoritative=false`、`order_authoritative=false`。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt` 字段，`diagnostics.notes` 增加 v143 标记，`node_consumption` 首句滚动到 Node v330，同时保留 Node v327/v142 和更早历史消费语义。因为 runtime fixture 已接近 512KiB 客户端单行上限，本版还把 fixture 里的重复长诊断压缩成短句，保留机器字段和必要历史句子。

`fixtures/release/credential-resolver-implementation-candidate-gate-input-hardening-non-participation-receipt.json` 是 standalone fixture；`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 嵌入同一份 receipt。这样 Node v330 可以从独立 fixture、runtime smoke 或 manifest 三个入口读到同一个 digest、stable current receipt 路径和关闭边界。

`tests/credential_resolver_implementation_candidate_gate_input_hardening_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，覆盖 Node v329 decision digest、Node v328 closure digest、stable current receipt export、v143 digest、counts，以及 runtime shell/provider/client/credential/raw URL/network/Java write/mini-kv write/admin/LOAD/COMPACT/RESTORE/SETNXEX/authority 的关闭边界。`tests/runtime_smoke_evidence_tests.cpp`、`tests/release_verification_manifest_tests.cpp` 和 v142 回归测试也补了滚动断言。

验证闭环包括 clean build、定向 CTest、全量 CTest、真实 CLI smoke 和真实 TCP smoke。CLI/TCP smoke 均证明 `SMOKEJSON` 暴露 v143 回执，candidate gate decision 可读，`GET restore:real-read-token` 返回 `(nil)`；TCP 服务端验证后已停止。归档截图放在 `d/143/图片/`，说明放在 `d/143/解释/说明.md`。

一句话总结：v143 给 Node v330 提供了 mini-kv 侧 stable current receipt/input-hardening 的只读非参与证据，同时把 runtime shell design、provider/client、credential/raw endpoint、HTTP/TCP、Java write、rollback/deployment、ledger/schema/storage、auto-start、mini-kv write/admin、LOAD/COMPACT/RESTORE/SETNXEX 和 audit/order authority 边界继续锁住。
