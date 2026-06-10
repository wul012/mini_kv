# mini-kv v1261-v1295 production live capture release evidence review non-participation

## 1. 本版目标和边界

v1261-v1295 的目标，是把 Node v1687-v1721 的 `production live probe read-only window capture release evidence review` 模块拆分计划，落成 mini-kv 侧的只读非参与证据链。

它不是新的执行入口，不是 release evidence review 的实现，不读取 Node 的 release review endpoint，不导入 payload / fixture，不渲染 Node Markdown，不运行 Node loader，也不把 mini-kv 变成 release review router、archive verifier、storage backend 或 Java evidence authority。

这一批的角色很窄：冻结 v1260 的 archive verification non-participation 结果，然后用 v1261-v1295 的版本化 fixture 说明后续 Node release review 拆分只被 mini-kv 当作 sibling read-only plan context。所有关键字段继续保持 `readOnly=true`、`executionAllowed=false`、`writeRoutingAllowed=false`、`touchesWal=false`。

## 2. 核心模块拆分

本版没有继续膨胀已有 v1260 模块，而是新增独立模块：

- `include/minikv/shard_preview_production_live_capture_release_evidence_review_non_participation.hpp`
- `src/shard_preview_production_live_capture_release_evidence_review_non_participation.cpp`
- `src/shard_preview_production_live_capture_release_evidence_review_non_participation_stages.cpp`
- `src/shard_preview_production_live_capture_release_evidence_review_non_participation_checks.cpp`
- `src/shard_preview_production_live_capture_release_evidence_review_non_participation_validation.cpp`

`core` 文件只负责组装 JSON、引用 source integrity、拼接 stage chain、checks 和 validation。它的 source integrity 指向 v1260：

- source command: `SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON`
- source fixture: `fixtures/release/shard-readiness-v1260.json`
- source status: `route-preview-production-live-capture-archive-verification-non-participation-closeout-summary-read-only`

`stages` 文件声明 v1261-v1295 的 35 段发布链。前 30 段映射 Node v1687-v1716 的 release review 类型、builder、field guide、endpoint、digest、check、policy 和 renderer 拆分点；后 5 段收口测试、HTTP smoke、segmented full Vitest、cleanup 和 closeout。

`checks` 文件把每段计划对应到一个保护断言。每个 check 都重复写出关键 false 边界：不执行 release review loader / renderer，不读取 release review endpoint，不导入 release review payload / fixture，不生成 mini-kv digest，不安装 router，不写、不碰 WAL、不执行。

`validation` 文件只做三类判断：

- v1260 source integrity 是否完整：`sourceIntegrityPublishedStageCount == 35`
- release review 是否保持关闭：Node 不要求 fresh mini-kv evidence，mini-kv 不启动 release review，也不读 release review endpoint
- 当前链是否达到 35/35：只有 v1295 closeout 才 `countsAligned=true`

## 3. 命令和 SHARDJSON 接线

新增命令：

```text
SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON
```

`src/command.cpp` 将它接入 runtime evidence 分支，只允许无参数读取；`src/command_contracts.cpp` 将它声明为：

- category: `read`
- mutates_store: false
- touches_wal: false
- stable: true

`src/line_editor.cpp` 增加补全候选，避免命令可执行但交互不可发现。

`src/shard_readiness.cpp` 将当前 `releaseVersion` 推到 `v1295`，顶层 `status` 改为 release evidence review non-participation closeout，并在 `SHARDJSON` 中新增：

```json
"shardPreviewProductionLiveCaptureReleaseEvidenceReviewNonParticipation": { ... }
```

这块证据和 v1260 的 archive verification 证据并存：v1260 是 source，v1295 是 current。这样 Node 后续如果只读消费 current fixture，可以看到完整 lineage，而不是只看到最后一句 status。

## 4. 版本化 fixture

新增并更新：

- `fixtures/release/shard-readiness-v1261.json` 到 `fixtures/release/shard-readiness-v1295.json`
- `fixtures/release/shard-readiness.json`

历史 fixture 按阶段截断：v1261 只有 1 段 stage/check，v1295 才有 35 段完整链，并且只有 v1295 的 validation passed/counts aligned 为 true。这样后续排查时能区分“当时发布到了哪一段”，而不是所有历史版本都长得像最终版。

## 5. 测试覆盖

新增 5 个测试：

- stages test：验证 v1261-v1295 连续、source frozen version/path 连续
- checks test：验证 35 个 check、越界保护、关键 false 边界字段
- validation test：验证 source incomplete、release review opened、count mismatch 都会失败
- formatter test：验证核心 JSON、source integrity、Node v1721 closeout、no endpoint/no payload/no execution
- fixture test：验证 v1261-v1295 历史 fixture 和 current fixture 都带新 evidence section

同时更新现有 `command_tests`、`shard_readiness_tests`、`line_editor_tests` 和 operator value supply 相关测试，确保新 current status 不破坏旧消费者。

## 6. 真实 smoke 的含义

本版 CLI / TCP smoke 使用：

```text
CHECKJSON SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON
```

CLI smoke 证明无 WAL 模式下该命令是 read-only、write_command=false、execution_allowed=false。

TCP smoke 更关键：服务端带临时 WAL 启动，响应里 `wal_enabled=true`，但该命令仍是 `touches_wal=false`、`append_when_enabled=false`。这说明“系统开了 WAL”不等于“证据命令会写 WAL”，边界是命令级别成立的。

## 7. 一句话总结

v1261-v1295 把 release evidence review 拆分计划变成 mini-kv 的版本化只读非参与证据：Node 可以消费证据，但 mini-kv 不读取、不导入、不路由、不写入、不触 WAL，也不获得任何 release review 执行权。
