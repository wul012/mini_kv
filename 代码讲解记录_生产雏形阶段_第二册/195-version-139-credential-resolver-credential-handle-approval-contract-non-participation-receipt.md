# mini-kv v139: credential-handle approval contract 非参与回执

v139 的目标是给 Node v318 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v317 的 `credential-handle approval contract intake`，确认自己理解凭证句柄审批的非秘密字段合同，但不把自己变成 credential handle 的存储方、校验方、解析方、审批状态权威方或 credential resolver 实现方。

这版不是什么：不是 runtime shell，不是 credential resolver 实现，不读取 credential value，不解析 raw endpoint URL，不实例化 secret provider 或 resolver client，不发 HTTP/TCP 外部请求，不写 approval ledger、schema 或 storage，不自动启动 Node/Java/mini-kv 上游，也不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`。

`include/minikv/runtime_evidence_receipts.hpp` 只新增 v139 的 digest 与 JSON formatter 声明。入口仍然是 runtime evidence receipt 家族，不改变命令分发表、Store、WAL、snapshot、restore 或 TCP 协议行为。

`src/runtime_credential_resolver_credential_handle_approval_receipts.cpp` 是本版核心实现。它把 Node v317 contract intake、Node v316 signed artifact prerequisite closure review、Node v313 catalog 指向、字段计数、warning/recommendation 和关闭边界集中在一个独立翻译单元里，避免继续膨胀 v138 的 signed artifact contract receipt 文件。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_credential_handle_approval_contract_non_participation_receipt` 字段，`diagnostics.notes` 增加 v139 标记，`node_consumption` 首句滚动到 Node v318，同时保留 Node v315、v311、v309 等历史消费语义。

这份回执最重要的控制面语义有三组。第一组是 Node v317 的合同事实：`contract_digest=298ffb48a00aab4f4630b42fc7b48805185d50a5465938768bd78943e05ae817`、10 个 required fields、8 个 prohibited fields、5 个 rejection reasons、9 个 no-go boundaries、20/20 checks。第二组是 prerequisite 状态：`credential-handle-approval` 从 `still-missing` 进入 `contract-intake-defined`，但 endpoint allowlist、no-network safety、abort/rollback 仍未关闭。第三组是 mini-kv 自身边界：`credential_handle_stored=false`、`credential_handle_validated=false`、`credential_handle_resolved=false`、`credential_handle_authoritative=false`、`credential_approval_status_authoritative=false`。

`fixtures/release/credential-resolver-credential-handle-approval-contract-non-participation-receipt.json` 是 standalone fixture；`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 同步嵌入同一个 receipt。这样 Node v318 可以从独立 fixture、runtime smoke 或 manifest 三个入口读到一致 digest 和一致边界。

`tests/credential_resolver_credential_handle_approval_contract_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，覆盖 Node v317 合同 digest、Node v316 source digest、字段计数、prerequisite transition、v139 digest、以及所有 credential handle / credential value / endpoint / network / write / authority 关闭边界。

滚动测试继续保护入口稳定性：旧测试不再要求 v138 必须是 `node_consumption` 第一句，而是确认历史 v138 语义仍存在；v139 独立测试则断言 Node v318/v139 成为最新首句。这样后续版本可以继续滚动首句，同时不会误伤历史证据链。

本版验证闭环包括 `cmake --build cmake-build-v139 --parallel 2`、定向 CTest、全量 CTest、CLI smoke 和 TCP smoke。CLI/TCP smoke 都证明 `SMOKEJSON` 暴露 v139 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`GET restore:real-read-token` 返回 `(nil)`，没有写入 restore token 或触发恢复、压缩、写命令。

归档位置使用 v122 之后的新根：`d/139/图片/` 保存命令截图，`d/139/解释/说明.md` 解释每张截图对应的命令、结果和意义。截图通过 Playwright MCP 对临时本地证据页生成；临时 HTTP 服务只用于截图，截图完成后停止并清理 `_logs`。

一句话总结：v139 给 Node v318 提供了 credential-handle approval contract 的 mini-kv 侧只读非参与证据，同时把 credential value、raw endpoint URL、provider/client、HTTP/TCP、ledger/schema/storage、auto-start、restore/compact/write 命令和 audit/order authority 边界继续锁住。
