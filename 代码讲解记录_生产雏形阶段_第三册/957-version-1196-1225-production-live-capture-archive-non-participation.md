# 第 1196-1225 版代码讲解：production live capture archive non-participation

本轮目标是把 Node v1632-v1656 的 production live probe read-only window capture archive 拆分计划，转成 mini-kv 侧可版本化、可测试、可归档的只读证据链。

它不是什么：不是 archive loader，不是 active shard router，不是 endpoint reader，不会读取 raw endpoint、credential、secret、archive payload，不会启动 Node、Java 或 mini-kv 服务，不会写入 store，不会触碰 WAL，也不会打开 LOAD/RESTORE/COMPACT 或任何执行权限。

它的角色是延续 v1195 已经冻结的 live capture non-participation 证据，把新的 capture archive module-split 计划记录为 C++ 侧的稳定 evidence，方便后续控制面或人工审查时确认：Node 可以继续拆 archive 代码，但 mini-kv 只是旁路证明，不参与运行。

## 入口命令

本轮新增的外部入口是：

```text
SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVENONPARTICIPATIONJSON
```

它接入了四个命令面：

```text
src/command.cpp
src/command_contracts.cpp
src/line_editor.cpp
tests/command_tests.cpp
```

`COMMANDS` 和 `COMMANDSJSON` 中它被声明为：

```text
category=read
mutates_store=no
touches_wal=no
stable=yes
```

这说明它是读取 evidence 的命令，不是管理命令，也不是写路径。`HELP` 和 line editor completion 同步暴露该命令，是为了让人工和脚本都能找到它；但暴露命令本身不意味着任何 archive 捕获流程被打开。

## 模块拆分

实现没有继续堆进旧的 v1195 文件，而是拆成四个模块：

```text
include/minikv/shard_preview_production_live_capture_archive_non_participation.hpp
include/minikv/shard_preview_production_live_capture_archive_non_participation_stages.hpp
include/minikv/shard_preview_production_live_capture_archive_non_participation_checks.hpp
include/minikv/shard_preview_production_live_capture_archive_non_participation_validation.hpp

src/shard_preview_production_live_capture_archive_non_participation.cpp
src/shard_preview_production_live_capture_archive_non_participation_stages.cpp
src/shard_preview_production_live_capture_archive_non_participation_checks.cpp
src/shard_preview_production_live_capture_archive_non_participation_validation.cpp
```

`core` 文件负责组合最终 JSON：命令名、上游计划、上游 v1195 digest、当前 stage、stage chain、checks、validation、diagnostics，以及所有边界字段。

`stages` 文件维护 30 个版本阶段。v1196-v1220 对齐 Node v1632-v1656，v1221-v1225 补 mini-kv 自身命令目录、fixture export、CI build stability、CLI smoke 和 closeout summary。阶段表使用 `StageRecord`，每条都包含当前版本、冻结来源版本和冻结 fixture 路径。

`checks` 文件维护 30 条检查项。每条检查都输出同一组 false 边界字段，例如：

```text
nodeCaptureArchiveExecuted=false
nodeArchiveLoaderExecuted=false
archiveEndpointRead=false
archivePayloadImported=false
archiveRouterInstalled=false
writeRoutingAllowed=false
touchesWal=false
executionAllowed=false
```

`validation` 文件把上游证据完整性、archive 只读闭合、数量对齐三件事合成一个判断。最终 v1225 要满足：

```text
sourceIntegrityPublishedStageCount=25
sourceIntegrityComplete=true
plannedStageCount=30
publishedStageCount=30
plannedProductionLiveCaptureArchiveNonParticipationCheckCount=30
completedProductionLiveCaptureArchiveNonParticipationCheckCount=30
productionLiveCaptureArchiveNonParticipationValidationPassed=true
```

## 上游证据

本轮不是重新证明 live capture non-participation，而是冻结 v1195 作为 source integrity：

```text
sourceIntegrityCommand=SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON
sourceIntegrityContract=shard-route-preview-production-live-capture-non-participation.v1
sourceIntegrityReleaseVersion=v1195
sourceIntegrityFixturePath=fixtures/release/shard-readiness-v1195.json
sourceIntegrityPublishedStageCount=25
```

这几个字段的意思是：archive non-participation 建立在 v1195 已完成的 live capture non-participation 之上，而不是绕开它、覆盖它，或要求 Node/Java/mini-kv 再跑一次 live capture 证据。

## 控制面如何理解关键字段

`readOnly=true` 表示这个 JSON 是证据读取结果。它只告诉控制面“当前 mini-kv 对 archive capture 的态度”，不授予任何动作。

`executionAllowed=false` 表示即使控制面读到了完整 evidence，也不能把它解释成执行票据。archive capture、loader、verification consumer、release review consumer 都仍由各自项目按自己的边界处理。

`sourceNodeRequiresFreshMiniKvEvidence=false` 和 `sourceNodeRequiresFreshJavaEvidence=false` 表示 Node v1632-v1656 的 archive module split 不要求 mini-kv 或 Java 同步产出新运行时结果；mini-kv 本轮产出的是旁路证据，不是 Node 的前置条件。

`archiveEndpointRead=false`、`rawEndpointParsed=false`、`credentialValueRead=false`、`secretValueRead=false` 共同证明：endpoint 相关内容只作为计划或常量上下文出现，mini-kv 不读取真实 endpoint，也不解析或持久化 raw endpoint。

`archivePayloadImported=false`、`archiveFixtureImported=false`、`archiveStateAccepted=false` 证明 archive 内容没有进入 mini-kv 的运行态，更没有被当成状态机输入。

`activeRouterInstalled=false`、`archiveRouterInstalled=false`、`writeRoutingAllowed=false`、`touchesWal=false` 则把路由、写入、WAL 三条最危险路径全部锁死。

## SHARDJSON 嵌入

`src/shard_readiness.cpp` 把 rolling release 推进到：

```text
releaseVersion=v1225
status=route-preview-production-live-capture-archive-non-participation-closeout-summary-read-only
```

同时保留旧 section：

```text
shardPreviewProductionLiveCaptureNonParticipation
```

并新增：

```text
shardPreviewProductionLiveCaptureArchiveNonParticipation
```

这很重要：旧 v1195 是来源证据，新 v1225 是当前证据。二者同时存在，控制面可以看见来源链，不会误以为 archive 证据凭空出现。

## Fixture 链

本轮新增 30 个版本化 fixture：

```text
fixtures/release/shard-readiness-v1196.json
...
fixtures/release/shard-readiness-v1225.json
fixtures/release/shard-readiness.json
```

生成方式是逐版调整 `current_stage_count` 和 rolling `release_version`，重建 CLI 后用 `SHARDJSON` 输出冻结。最终源码恢复到 v1225，rolling fixture 与 `shard_readiness::format_fixture()` 保持字节一致。

## 测试覆盖

新增测试分五层：

```text
tests/production_live_capture_archive_non_participation_stages_tests.cpp
tests/production_live_capture_archive_non_participation_checks_tests.cpp
tests/production_live_capture_archive_non_participation_validation_tests.cpp
tests/production_live_capture_archive_non_participation_tests.cpp
tests/production_live_capture_archive_non_participation_fixture_tests.cpp
```

stage 测试保护 v1196-v1225 的版本连续性、source frozen 版本连续性和最终 stage 名称。

checks 测试保护 30 条检查项和核心 false 边界字段，尤其是 archive loader、archive endpoint、archive payload、router、write、WAL、execution。

validation 测试分别证明通过场景、source 不完整、archive 被打开、count 不对齐都会影响 validation 结果。

command/readiness/line-editor 测试证明该证据真正接入外部命令、命令目录、帮助、补全和 SHARDJSON，而不是孤立代码。

fixture 测试证明 v1196-v1225 每个版本文件都包含 archive non-participation section，rolling current 推到 v1225，同时旧 production live capture non-participation 仍停在 v1195 作为来源。

## 一句话总结

v1196-v1225 把 Node capture archive 拆分计划沉淀成 mini-kv 侧 30 版只读证据链：源自 v1195、版本化到 v1225、拆分到小模块、测试到命令和 fixture，同时继续证明 no router、no write、no WAL、no execution。
