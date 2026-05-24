# mini-kv v140: endpoint-handle allowlist approval contract 非参与回执

v140 的目标是给 Node v321 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v320 的 `endpoint-handle allowlist approval contract intake`，确认自己理解 endpoint handle allowlist approval 的非 raw URL 合同，但不把自己变成 endpoint 存储方、endpoint authority 校验方、allowlist authority、连接发起方或审批状态权威方。

这版不是什么：不是 runtime shell，不是 credential resolver 或 endpoint resolver 实现，不读取 credential value，不解析、渲染或保存 raw endpoint URL，不实例化 secret provider 或 resolver client，不发 HTTP/TCP 外部请求，不写 approval ledger、schema 或 storage，不自动启动 Node/Java/mini-kv 上游，也不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`。

`include/minikv/runtime_evidence_receipts.hpp` 只新增 v140 的 digest 与 JSON formatter 声明。入口仍属于 runtime evidence receipt 家族，不改变命令分发表、Store、WAL、snapshot、restore 或 TCP 协议行为。

`src/runtime_credential_resolver_endpoint_handle_allowlist_approval_receipts.cpp` 是本版核心实现。它把 Node v320 contract intake、Node v319 credential-handle approval prerequisite closure review、Node v313 catalog 指向、字段计数、warning/recommendation 和关闭边界集中在一个独立翻译单元里，避免继续膨胀 v139 的 credential-handle approval receipt 文件。当前文件约 457 行，测试约 279 行，仍处在可维护范围内。

这份回执最重要的控制面语义有三组。第一组是 Node v320 合同事实：`contract_digest=4657f89caa6866bad87db284dc98efd8d09a6538d8d735535e6a7e6d4d6c33e5`、10 个 required fields、8 个 prohibited fields、5 个 rejection reasons、9 个 no-go boundaries、20/20 checks。第二组是 prerequisite 状态：`endpoint-handle-allowlist-approval` 从 `still-missing` 进入 `contract-intake-defined`，但 no-network safety 和 abort/rollback 仍未关闭。第三组是 mini-kv 自身边界：`raw_endpoint_url_stored=false`、`endpoint_handle_validated=false`、`endpoint_allowlist_authority=false`、`endpoint_allowlist_approval_status_authoritative=false`、`credential_value_read=false`、`external_request_sent=false`。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt` 字段，`diagnostics.notes` 增加 v140 标记，`node_consumption` 首句滚动到 Node v321，同时保留 Node v318/v139 和更早历史消费语义。旧 v139 测试也改成检查历史句子仍存在，而不是要求它永远是第一句。

`fixtures/release/credential-resolver-endpoint-handle-allowlist-approval-contract-non-participation-receipt.json` 是 standalone fixture；`fixtures/release/runtime-smoke-evidence.json` 与 `fixtures/release/verification-manifest.json` 嵌入同一份 receipt。这样 Node v321 可以从独立 fixture、runtime smoke 或 manifest 三个入口读到同一 digest 与同一关闭边界。

`tests/credential_resolver_endpoint_handle_allowlist_approval_contract_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，覆盖 Node v320 contract digest、Node v319 source digest、字段计数、prerequisite transition、v140 digest，以及 endpoint/raw URL/credential/network/write/authority 的关闭边界。`tests/smokejson_command_receipt_tests.cpp` 继续保护聚合入口，确保新 receipt 和新的 Node v321 消费句子已进入 `SMOKEJSON`。

验证闭环包括 clean build、定向 CTest、全量 CTest 和真实 CLI smoke。CLI smoke 证明 `SMOKEJSON` 暴露 v140 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`GET restore:real-read-token` 返回 `(nil)`，没有写入 restore token 或触发恢复、压缩、写命令。归档截图通过 Playwright MCP 对临时本地证据页生成，放在 `d/140/图片/`；说明在 `d/140/解释/说明.md`。

一句话总结：v140 给 Node v321 提供了 endpoint-handle allowlist approval contract 的 mini-kv 侧只读非参与证据，同时把 raw endpoint URL、endpoint authority、allowlist authority、credential value、provider/client、HTTP/TCP、ledger/schema/storage、auto-start、restore/compact/write 命令和 audit/order authority 边界继续锁住。
