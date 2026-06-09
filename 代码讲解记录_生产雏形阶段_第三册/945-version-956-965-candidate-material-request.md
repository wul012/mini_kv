# mini-kv v956-v965 candidate material request closeout

## 本版目标

v956-v965 继续沿着 Node v1446 的最新计划推进：Node 已经把 v1421 intake packet 扩展成 reviewed real candidate document material request package，包含 25 个 material request items、25 个 acceptance checks、20 个请求字段和 40 个 gate。mini-kv 这一轮做的不是“接收材料”，而是把这个请求层冻结成可被 Node/Java 后续读取的 C++ 侧只读证据。

它明确不是什么：

- 不是新的 document intake 入口；
- 不是读取 reviewed real candidate document；
- 不是解析 raw source URI、digest、canonical body 或 field table；
- 不是安装 document router；
- 不是写 store、WAL、snapshot、restore 或 sibling 项目；
- 不是 approval/signature/runtime execution 的前置绕行。

一句话说，v956-v965 只回答“mini-kv 是否知道未来要向外部 reviewer 请求哪些材料、哪些检查必须满足，并且当前仍没有消费材料”。答案是知道，但不执行。

## 模块拆分

入口是 `SHARDROUTECANDIDATEMATERIALREQUESTJSON`，实现位于：

- `include/minikv/shard_preview_candidate_material_request.hpp`
- `src/shard_preview_candidate_material_request.cpp`

核心文件只负责聚合 JSON：读取当前 stage、source v955 intake packet 状态、item/check catalog、validation 结果，并输出一个完整的 read-only contract。

为了避免继续堆大文件，本版把目录拆成四个辅助模块：

- `shard_preview_candidate_material_request_stages.*`：维护 v956-v965 的 10 阶段链，以及每一版冻结的 source fixture。
- `shard_preview_candidate_material_request_items.*`：维护 25 个 request item，例如 reviewer identity、source URI handle、document digest、canonical body、field table、comparison binding、signature attestation、redaction log、archive index、cleanup proof。
- `shard_preview_candidate_material_request_checks.*`：维护 25 个 acceptance check。它们都是“需要外部材料出现后才能满足”的检查，不由 mini-kv runtime 满足。
- `shard_preview_candidate_material_request_validation.*`：验证 v955 source intake packet 已冻结、25/25 item 与 25/25 check 对齐、10/10 stage 完成，并继续输出 no material/no write/no execution 字段。

这种拆法让新增证据链保持短文件边界：catalog 归 catalog，validation 归 validation，命令入口只做聚合。

## 命令契约

命令接入点在 `src/command.cpp`：

```text
SHARDROUTECANDIDATEMATERIALREQUESTJSON
```

它被注册为 runtime evidence read command，只接受空参数；多余参数返回：

```text
ERR usage: SHARDROUTECANDIDATEMATERIALREQUESTJSON
```

命令目录在 `src/command_contracts.cpp` 中声明为：

- `category=read`
- `mutates_store=false`
- `touches_wal=false`
- `stable=true`

因此 `COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、line editor completion 都能发现它，但不会把它归为 write/admin/runtime execution。

## SHARDJSON 聚合

`src/shard_readiness.cpp` 的 rolling current 从 v955 滚到 v965。新增 section：

```text
shardPreviewCandidateMaterialRequest
```

顶层状态变为：

```text
route-preview-candidate-material-request-closeout-summary-read-only
```

关键字段的含义如下：

- `sourceCandidateIntakePacketReleaseVersion=v955`：本轮只消费上一轮 mini-kv 已冻结的 intake packet，不跳过 v955。
- `sourceNodeMaterialRequestReleaseVersion=Node v1446`：说明计划来源是 Node v1422-v1446 material request closeout。
- `plannedMaterialRequestItemCount=25` / `completedMaterialRequestItemCount=25`：mini-kv 已列出请求清单，但没有读取清单对应的真实材料。
- `plannedAcceptanceCheckCount=25` / `completedAcceptanceCheckCount=25`：mini-kv 已列出接受前检查，但这些检查不是 runtime 满足的 acceptance。
- `externalReviewedMaterialRequested=true`：请求已经形成。
- `reviewedMaterialPresent=false`、`reviewedRealCandidateDocumentRead=false`、`materialRequestSatisfied=false`：材料仍没有出现，也没有被 mini-kv 读取或满足。
- `documentRouterInstalled=false`、`writeRoutingAllowed=false`、`touchesWal=false`、`executionAllowed=false`：运行时边界仍是硬关闭。

## Fixture 链

本版生成：

```text
fixtures/release/shard-readiness-v956.json
...
fixtures/release/shard-readiness-v965.json
fixtures/release/shard-readiness.json
```

每一版都是真实 CLI 输出的 `SHARDJSON`，不是手写 JSON。生成时临时切换 `release_version` 和 `current_candidate_material_request_stage_count`，重建后输出 fixture，最后恢复到 v965 并再次构建。

生成后额外修正为 UTF-8 no BOM。这个细节很重要：`shard_readiness_tests` 会比较 current fixture 与格式化输出完全一致，BOM 会导致首字节不一致。

## 测试覆盖

新增测试：

- `candidate_material_request_stages_tests`
- `candidate_material_request_items_tests`
- `candidate_material_request_checks_tests`
- `candidate_material_request_validation_tests`
- `candidate_material_request_tests`
- `candidate_material_request_fixture_tests`

它们分别守住阶段链、25 个 item、25 个 check、validation 汇总、主 JSON 和版本 fixture。

同步更新的旧测试覆盖：

- `command_tests`：命令数从 62 增至 63；新命令出现在 `COMMANDS`、`COMMANDSJSON`，并验证 usage 和 read-only 输出。
- `line_editor_tests`：新增命令补全。
- `shard_readiness_tests`：顶层 rolling current 滚到 v965，并验证 material request section。
- 旧 value-supply 相关测试：只更新 current `SHARDJSON` status，历史 section 行为不变。

这些测试保护的是边界，不只是字段存在：新命令不能写 store、不能碰 WAL、不能打开 document intake、不能安装 router、不能让 runtime execution 变成 true。

## 对后续协作的价值

v956-v965 把“请求外部 reviewed material”做成了 C++ 侧版本化证据。后续 Node/Java 如果真的进入 material intake，需要先提供真实 reviewed material 和明确计划；mini-kv 当前只提供可审查、可归档、可回放的只读请求清单，而不提前消费材料。
