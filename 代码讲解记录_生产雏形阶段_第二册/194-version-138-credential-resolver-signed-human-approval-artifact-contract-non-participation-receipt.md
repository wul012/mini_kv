# mini-kv v138: signed human approval artifact contract 非参与回执

v138 的目标是给 Node v315 准备 mini-kv 侧的只读上游证据：mini-kv 回显 Node v314 的 `signed human approval artifact contract intake`，确认自己理解签名人工审批制品的非秘密字段合同，但不把自己变成签名制品的存储方、校验方、签名方、验签方或权威方。

这版不是什么：不是 runtime shell，不是 credential resolver 实现，不读取 credential value，不解析 raw endpoint URL，不实例化 provider/client，不发 HTTP/TCP 外部请求，不写 ledger/schema/storage，不自动启动 Node/Java/mini-kv 上游，也不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`。

`include/minikv/runtime_evidence_receipts.hpp` 只新增两个声明：一个 digest 函数和一个 JSON formatter。这样 v138 继续走 runtime evidence receipt 家族的轻量入口，不改变已有命令处理、存储、WAL、snapshot 或 restore 合同。

`src/runtime_credential_resolver_signed_human_approval_artifact_receipts.cpp` 是本版核心实现。它把 Node v314 合同 intake、Node v312 governance stop/prerequisite closure decision、Node v313 catalog 指向、字段计数和边界声明集中在一个独立翻译单元里，避免继续膨胀 v137 的 post-echo decision receipt 文件。

`src/command_smoke_formatters.cpp` 把新回执挂入 `SMOKEJSON`：新增 `credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt` 字段，`diagnostics.notes` 增加 v138 标记，`node_consumption` 首句滚动到 Node v315，同时保留 Node v311、v309、v307 等历史消费语义。

这份回执最重要的控制面语义有三组。第一组是 Node v314 的合同事实：`contract_digest=72498e59c086eadd4d44e80789120de195af1a0b70dd49346b837e2bc8ed4666`、11 个 required fields、8 个 prohibited fields、5 个 rejection reasons、8 个 no-go boundaries、18/18 checks。第二组是 prerequisite 状态：`signed-human-approval-artifact` 从 `still-missing` 进入 `contract-intake-defined`，但其他 credential handle、endpoint allowlist、no-network safety、abort/rollback 仍未关闭。第三组是 mini-kv 自身边界：`signed_human_approval_artifact_stored=false`、`signed_human_approval_artifact_validated=false`、`signed_human_approval_artifact_authoritative=false`、`signed_artifact_signature_verified=false`。

`fixtures/release/credential-resolver-signed-human-approval-artifact-contract-non-participation-receipt.json` 是 standalone fixture；`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 同步嵌入同一个 receipt。这样 Node v315 可以从独立 fixture、runtime smoke 或 manifest 三个入口读到一致 digest 和一致边界。

`tests/credential_resolver_signed_human_approval_artifact_contract_non_participation_receipt_tests.cpp` 是本版独立测试。它同时验证 standalone fixture、runtime smoke fixture、release manifest 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出，覆盖 Node v314 合同 digest、Node v312 source digest、字段计数、prerequisite transition、v138 digest、以及所有 signed artifact / credential / endpoint / network / write / authority 关闭边界。

滚动测试继续保护入口稳定性：`smokejson_command_receipt_tests.cpp`、`runtime_smoke_evidence_tests.cpp`、`release_verification_manifest_tests.cpp` 要求 Node v315/v138 成为最新 `node_consumption` 语义，同时保留历史版本可被后续 Node 复查的语义链。

本版验证闭环包括 `cmake --build cmake-build-v138 --parallel 2`、定向 CTest 4/4、全量 CTest 65/65、CLI smoke 和 TCP smoke。CLI/TCP smoke 都证明 `SMOKEJSON` 暴露 v138 回执，`CHECKJSON SMOKEJSON` 仍是只读解释，`GET restore:real-read-token` 返回 `(nil)`，没有写入 restore token 或触发恢复、压缩、写命令。

归档位置使用 v122 之后的新根：`d/138/图片/` 保存命令截图，`d/138/解释/说明.md` 解释每张截图对应的命令、结果和意义。截图通过 Playwright MCP 对临时本地证据页生成；临时 HTTP 服务只用于截图，截图完成后停止并清理 `_logs`。

一句话总结：v138 给 Node v315 提供了 signed human approval artifact contract 的 mini-kv 侧只读非参与证据，同时把签名制品权威、credential、endpoint、network、provider/client、ledger/schema、auto-start、restore/compact/write 命令和 audit/order authority 边界继续锁住。
