# mini-kv v881-v895 candidate document request package closeout

## 本版目标

v881-v895 这一批把 v880 的 compared package evidence intake audit 冻结成 `fixtures/release/shard-readiness-v880.json`，然后新增一个只读的 candidate document request package closeout 链路。它对齐 Node 最新计划 `docs/plans3/v1386-controlled-read-only-shard-preview-candidate-document-request-package-closeout-roadmap.md`，把未来真实 compared package evidence candidate document 需要供应的材料拆成 15 个 request item、15 个 acceptance check、38 个 gate 和 20 个 requested candidate fields。

它不是什么也很重要：它不是真实 candidate document intake，不创建、不导入、不解析、不评价、不接受、不拒绝真实文档；不是 active shard router；不打开 Node/Java；不写 WAL；不触发 LOAD/RESTORE/COMPACT；不捕获 approval grant；不生成 signed approval；不导入 runtime payload。

## 模块拆分

这次没有继续沿用超长文件名，而是新增短命名模块：

- `include/minikv/shard_preview_candidate_request_package.hpp` 与 `src/shard_preview_candidate_request_package.cpp` 负责顶层 JSON、当前阶段、digest marker 和 shard readiness status。
- `*_stages` 保存 v881-v895 的 15 阶段 catalog，并通过 `shard_route_preview_stage_chain` 检查 release/source fixture 连续性。
- `*_items` 保存 15 个 request item，说明未来操作者或上游 producer 应该供应什么 handle 或字段。
- `*_checks` 保存 15 个 acceptance check，说明未来消费方如何判断供应材料是否满足请求。
- `*_gates` 保存 38 个 gate，覆盖 missing/synthetic/unreviewed document、payload import、evaluation、approval、runtime、write、WAL、archive walk、sibling mutation 等禁止路径。
- `*_validation` 聚合 planned/completed counts、source chain complete 和 closeout complete 等机器可断言结果。

这个拆法的目的不是追求形式，而是把“请求项、验收项、禁止项、验证项”分开维护，避免把一个越来越大的 JSON 字符串模块拖成难读的巨型文件。

## 命令契约

新增命令：

```text
SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECANDIDATEDOCUMENTREQUESTPACKAGECLOSEOUTJSON
```

它在 `src/command.cpp` 中注册为 `RuntimeEvidence`，在 `src/command_contracts.cpp` 中归类为 `read`，并在 `src/line_editor.cpp` 中加入补全。命令只接受空参数；一旦带额外 token，就返回 usage 错误，不会把参数解释成路径、payload、文档或执行请求。

`COMMANDS` 的计数从 58 变为 59，同时修复了 v880 旧条目和 `COMMANDS` 条目贴在同一行的可读性瑕疵。

## SHARDJSON 集成

`src/shard_readiness.cpp` 的 `releaseVersion` 更新为 `v895`。顶层 `status` 不再硬编码旧字符串，而是来自 `shard_preview_candidate_request_package::candidate_document_request_package_status()`，也就是当前阶段 catalog 的 stage 加 `-read-only` 后缀。

`evidenceDigest()` 也改成基于当前 `release_version` 生成 digest prefix，这样 v881-v895 逐阶段生成历史 fixture 时，digest 不会继续挂在 `mini-kv-shard-readiness-v880` 这个旧前缀上。

新增的 `shardPreviewCandidateDocumentRequestPackageCloseout` 区块保留这些核心边界字段：

- `candidateDocumentRequestPackageCloseoutOnly=true`
- `realCandidateDocumentPresent=false`
- `syntheticCandidateDocumentAccepted=false`
- `stagedCandidatePayloadImported=false`
- `candidateDocumentEvaluated=false`
- `candidateDocumentAccepted=false`
- `candidateApprovalGrantCaptured=false`
- `candidateSignedApprovalEmitted=false`
- `documentRouterInstalled=false`
- `writeRoutingAllowed=false`
- `touchesWal=false`
- `executionAllowed=false`

控制面读取这些字段时，应理解为“请求包已经准备好给未来真实文档 intake 使用”，而不是“真实文档已经被 mini-kv 接受”。

## Fixture 证据

本批次保留滚动 current，同时新增版本化 fixture：

- `fixtures/release/shard-readiness-v880.json` 冻结 v880。
- `fixtures/release/shard-readiness-v881.json` 到 `fixtures/release/shard-readiness-v894.json` 逐阶段生成。
- `fixtures/release/shard-readiness.json` 更新为 current v895。

生成历史 fixture 时，每个阶段都临时设置对应的 `current_candidate_document_request_package_stage_count` 和 `release_version`，增量构建 `minikv_cli` 后通过 `SHARDJSON` 输出纯 JSON，再恢复最终 v895。这样 v895 的 `sourceFrozenFixturePath=fixtures/release/shard-readiness-v894.json` 是真实存在的版本化文件，不依赖 rolling current。

## 测试与 smoke

新增 6 个聚焦测试：

- `candidate_request_package_stages_tests`
- `candidate_request_package_items_tests`
- `candidate_request_package_checks_tests`
- `candidate_request_package_gates_tests`
- `candidate_request_package_validation_tests`
- `candidate_request_package_tests`

这些测试分别保护阶段连续性、item/check/gate 的 clamp 行为、validation 失败态、最终 JSON 的 counts 和只读边界。旧的 `command_tests`、`shard_readiness_tests`、`line_editor_tests` 也同步覆盖新命令、catalog、HELP、补全和 `SHARDJSON` 区块。

最终验证包括：

- 全量构建通过。
- 154 条 CTest 全部通过。
- CLI smoke 验证 `COMMANDS` 计数 59、新命令 JSON、`SHARDJSON` release/status/section。
- TCP smoke 启动 `minikv_server`，用 `minikv_client` 验证新只读命令和 `SHARDJSON` 关键字段，并在结束后停止服务端。

## 一句话总结

v881-v895 把“未来真实 compared package evidence candidate document 应该供应什么”做成了可版本化、可测试、可归档的只读请求包，同时继续守住 mini-kv 不做文档 intake、不做 router、不写、不执行的边界。
