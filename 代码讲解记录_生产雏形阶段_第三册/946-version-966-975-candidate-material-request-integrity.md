# mini-kv v966-v975 candidate material request integrity closeout

## 本版目标

v966-v975 接在 v956-v965 后面走，但它不是继续扩大材料请求，也不是打开材料接收。Node 最新计划 `docs/plans3/v1446-controlled-read-only-shard-preview-candidate-document-material-request-closeout-roadmap.md` 已经把 stop condition 写清楚：在外部 reviewed real material 真实提供并被审查前，Node 不应继续做新的 material request echo，mini-kv 也不应消费、解析或验证外部材料。

所以本轮 C++ 侧做的是更硬的一层证据冻结：把 v965 的 `SHARDROUTECANDIDATEMATERIALREQUESTJSON` 作为唯一来源，生成 v966-v975 的版本化 integrity fixture，并证明这条链只是只读完整性检查。它明确不是什么：

- 不是新的 candidate document intake 入口。
- 不是第二份 material request echo。
- 不是读取 reviewed real candidate document、raw endpoint、credential value 或 source body。
- 不是安装 active shard router、document router 或 write routing。
- 不是写 store、WAL、snapshot、restore、load、compact 或 sibling 项目。
- 不是 approval、signature、runtime payload 或 execution 的前置绕行。

一句话说，v966-v975 回答的是：“v965 的材料请求证据是否已冻结成可版本化复核的完整性 fixture，并且 Node v1446 的暂停条件是否被 mini-kv 遵守？”答案是：是，并且没有产生执行副作用。

## 模块拆分

主入口是：

```text
SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON
```

对应核心文件：

- `include/minikv/shard_preview_candidate_material_request_integrity.hpp`
- `src/shard_preview_candidate_material_request_integrity.cpp`

核心文件只负责聚合 JSON：读取当前 integrity stage、source v965 material request 状态、15 条 integrity check、validation 汇总，再输出一个完整的 read-only contract。为了避免把一个文件继续堆大，本版把职责拆成三个辅助模块：

- `shard_preview_candidate_material_request_integrity_stages.*`：维护 v966-v975 的 10 阶段链，包含每版 release、source frozen release 和 source fixture path。
- `shard_preview_candidate_material_request_integrity_checks.*`：维护 15 条完整性检查，覆盖 source freeze、Node stop condition、fixture range、command catalog、encoding、archive reference、no material consumption、no router/write/WAL/execution 等边界。
- `shard_preview_candidate_material_request_integrity_validation.*`：把 source stage count、item/check/gate/field count、published/planned check count 和 read-only 边界汇总成 validation section。

这种拆法让 catalog、check、validation 各自短而稳定，命令入口只做聚合，不把测试数据、检查声明和格式化逻辑全塞进一个巨型文件。

## 命令契约

`src/command.cpp` 新增 `SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON`，只接受空参数。多余参数会返回：

```text
ERR usage: SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON
```

`src/command_contracts.cpp` 把它纳入 stable read command：

- `category=read`
- `mutates_store=false`
- `touches_wal=false`
- `stable=true`

`COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 和 line editor completion 都能看到它，但它不会被归入 write/admin/runtime execution。这个细节很重要：后续 Node 或 Java 如果只读消费命令目录，可以看到 integrity 证据已经存在，但不能把它误解成可执行入口。

## SHARDJSON 聚合

`src/shard_readiness.cpp` 的 rolling current 从 v965 推到 v975，并新增：

```text
shardPreviewCandidateMaterialRequestIntegrity
```

顶层状态变成：

```text
route-preview-candidate-material-request-integrity-closeout-summary-read-only
```

关键字段的控制面含义如下：

- `sourceCandidateMaterialRequestReleaseVersion=v965`：本轮唯一来源是已经冻结的 v965 material request，不跳过它，也不再生成新的 request echo。
- `sourceNodeMaterialRequestReleaseVersion=Node v1446`：计划来源仍然是 Node 的 material request closeout，但 mini-kv 只读对齐，不修改 Node。
- `candidateMaterialRequestIntegrityReleaseVersion=v975`：当前完整性链已经走到 v975 closeout summary。
- `publishedStageCount=10` / `plannedStageCount=10`：v966-v975 的 10 个阶段都已发布。
- `plannedIntegrityCheckCount=15` / `completedIntegrityCheckCount=15`：完整性检查目录已经完整发布。
- `nodeStopConditionRespected=true`：遵守 Node v1446 的暂停条件，不消费 reviewed material。
- `additionalMaterialRequestEchoCreated=false` / `externalReviewedMaterialConsumed=false`：没有重复创建请求，也没有读取真实外部材料。
- `documentIntakeOpened=false` / `documentRouterInstalled=false`：没有打开文档接收或路由。
- `writeRoutingAllowed=false` / `touchesWal=false` / `executionAllowed=false`：不写、不碰 WAL、不执行。

这些字段不是装饰性描述，而是给控制面读者看的硬边界：它们说明 mini-kv 当前只是把旧证据冻结、计数、比对和归档，不具备运行时材料处理权限。

## Fixture 链

本轮生成：

```text
fixtures/release/shard-readiness-v966.json
...
fixtures/release/shard-readiness-v975.json
fixtures/release/shard-readiness.json
```

每个 fixture 都来自真实 `minikv_cli.exe` 的 `SHARDJSON` 输出。生成时只临时切换两个编译期常量：`release_version` 和 `current_candidate_material_request_integrity_stage_count`，每个版本重建 `minikv_cli` 后导出 JSON，最后恢复到 v975 并再次构建。

生成后复核了 11 个 fixture 都是 UTF-8 no BOM，并确认 v975 包含 contract、source v965、15 条 check、Node stop condition，以及 no material consumption / no router / no write / no WAL / no execution 的关键 false 字段。

## 测试覆盖

新增测试：

- `candidate_material_request_integrity_stages_tests`
- `candidate_material_request_integrity_checks_tests`
- `candidate_material_request_integrity_validation_tests`
- `candidate_material_request_integrity_tests`
- `candidate_material_request_integrity_fixture_tests`

它们分别保护阶段链、15 条完整性检查、validation 汇总、主命令 JSON 和版本化 fixture。旧测试也同步更新：

- `command_tests`：命令总数从 63 到 64，验证 `COMMANDS`、`COMMANDSJSON`、usage 和 read-only catalog。
- `line_editor_tests`：验证新命令补全，并处理与旧 material request 命令共享前缀后的 common-prefix 行为。
- `shard_readiness_tests`：验证 SHARDJSON 顶层状态滚到 v975，并检查 integrity section。
- 旧 value-supply 相关测试：只更新 rolling current status，不改变历史 section 的语义。

最终验证顺序是：构建、定向 CTest、全量 CTest、CLI smoke、TCP smoke。定向 14/14 通过，全量 181/181 通过；CLI 和 TCP 都证明新命令可读、命令目录可见、v975 status 正确，并且 execution 仍为 false。

## 对后续协作的价值

v966-v975 把 v965 的 material request closeout 做成了更稳的“冻结证据 + 完整性检查”层。后续 Node/Java 如果继续推进，应该先解决真实 reviewed material 的提供和审查，而不是让 mini-kv 抢跑 intake、router、write 或 execution。mini-kv 现在提供的是一条可归档、可回放、可被控制面只读消费的完整性证据链。
