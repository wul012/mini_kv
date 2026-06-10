# 第 963 篇代码讲解：mini-kv v1386-v1405 type barrel split follow-up non-participation

本版目标是把 v1385 的 `shardPreviewTypeBarrelSplitNonParticipation` 证据冻结成后续 20 个版本的 source fixture，并继续证明 mini-kv 对 Node v1822-v1846 type-barrel/profile-boundary 拆分只做只读观察，不导入 Node TypeScript 类型，不读取 profile endpoint，不执行 Node route/renderer/typecheck/build/Vitest，不安装 router，不打开写路由，不触碰 WAL，也不获得 load/restore/compact 或运行时执行权。

它不是新的跨项目执行入口。`SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON` 只是一个 read/stable 元数据命令，作用是把“v1385 已经完成，后续只是在 mini-kv 侧守住证据和质量边界”写成可测试、可归档、可消费的 JSON。Node 的 type split 是否正确，仍由 Node 自己的 typecheck/build/Vitest 和路线图负责；mini-kv 只证明自己没有越界参与。

## 入口和挂接

根入口仍然是 `SHARDJSON`：

```text
src/shard_readiness.cpp
```

本版在根证据里做了四件事：

```text
releaseVersion -> v1405
status -> route-preview-type-barrel-split-follow-up-non-participation-clean-workspace-ci-closeout-read-only
evidenceDigest 增加 shardPreviewTypeBarrelSplitFollowUpNonParticipation marker
JSON 增加 shardPreviewTypeBarrelSplitFollowUpNonParticipation section
```

核心实现没有继续塞进 `shard_readiness.cpp`。新增模块拆成四类文件：

```text
include/minikv/shard_preview_type_barrel_split_follow_up_non_participation.hpp
include/minikv/shard_preview_type_barrel_split_follow_up_non_participation_stages.hpp
include/minikv/shard_preview_type_barrel_split_follow_up_non_participation_checks.hpp
include/minikv/shard_preview_type_barrel_split_follow_up_non_participation_validation.hpp
src/shard_preview_type_barrel_split_follow_up_non_participation.cpp
src/shard_preview_type_barrel_split_follow_up_non_participation_stages.cpp
src/shard_preview_type_barrel_split_follow_up_non_participation_checks.cpp
src/shard_preview_type_barrel_split_follow_up_non_participation_validation.cpp
```

命令挂接在：

```text
src/command.cpp
src/command_contracts.cpp
src/line_editor.cpp
```

`COMMANDS` / `COMMANDSJSON` / `HELP` / line editor completion 都能看到新命令，且命令契约声明为 `read`、`mutates_store=false`、`touches_wal=false`。命令表从 79 个命令扩展到 80 个命令。

## Source Freeze

本版 source 是 v1385，而不是更早的 v1360：

```text
sourceTypeBarrelSplitCommand=SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON
sourceTypeBarrelSplitContract=shard-route-preview-type-barrel-split-non-participation.v1
sourceTypeBarrelSplitReleaseVersion=v1385
sourceTypeBarrelSplitFixturePath=fixtures/release/shard-readiness-v1385.json
sourceTypeBarrelSplitPublishedStageCount=25
sourceTypeBarrelSplitComplete=true
```

控制面读到这些字段时，应理解为：v1361-v1385 的 type barrel split non-participation 链已经成为 frozen source。v1386-v1405 不重新解释 Node 的 type split，也不把 Node profile/type/export endpoint 变成 mini-kv 的输入源，只是在这个 source 之上补 follow-up 边界、fixture 链、归档质量和 current rollover 证据。

## Stage 目录

stage 目录在：

```text
src/shard_preview_type_barrel_split_follow_up_non_participation_stages.cpp
```

20 个 stage 从 v1386 到 v1405 连续递增。第一条冻结 v1385，最后一条完成 clean workspace / CI closeout：

```text
v1386 source fixture freeze
v1387 command catalog continuity
v1388 SHARDJSON current rollover
v1389 versioned fixture chain
v1390 profile endpoint no-read
v1391 Node type import guard
v1392 route renderer no-execution
v1393 Node validation no-execution
v1394 TCP standalone read-command smoke
v1395 large SHARDJSON payload CLI-only
v1396 rolling current test hardening
v1397 source digest pin
v1398 archive screenshot retention
v1399 walkthrough index continuity
v1400 generated validation cleanup
v1401 no service startup
v1402 router/write/WAL guard
v1403 current fixture parity
v1404 whitespace quality gate
v1405 clean workspace CI closeout
```

每条 stage 继续使用通用 `StageRecord`：

```text
sequence
releaseVersion
stage
sourceFrozenReleaseVersion
sourceFrozenFixturePath
scope
```

`shard_route_preview_stage_chain` 会检查 sequence、releaseVersion、sourceFrozenReleaseVersion、sourceFrozenFixturePath 是否连续。这样后续消费方能知道这不是一个单点 JSON 字段，而是一条版本化证据链。

## Check 目录

check 目录在：

```text
src/shard_preview_type_barrel_split_follow_up_non_participation_checks.cpp
```

每条 check 都重复声明核心禁止项：

```text
nodeStableBarrelImportedByMiniKv=false
nodeProfileTypesImportedByMiniKv=false
nodeAggregateReexportsImportedByMiniKv=false
nodeProfileBoundaryEndpointReadByMiniKv=false
nodeRouteRendererTestsExecutedByMiniKv=false
nodeTypecheckExecutedByMiniKv=false
nodeVitestExecutedByMiniKv=false
nodeBuildExecutedByMiniKv=false
miniKvTypeBarrelFollowUpStarted=false
miniKvConsumesNodeTypes=false
profileBoundaryEndpointRead=false
typeRouterInstalled=false
writeRoutingAllowed=false
touchesWal=false
executionAllowed=false
```

这些字段的语义是“mini-kv 没有参与”。它们不代表 mini-kv 验证了 Node 类型本身的正确性，也不代表 mini-kv 可以读取 Node profile boundary。它只是一个 boundary receipt，告诉 Node/Java/控制面：这一侧没有启动 sibling service，没有消费 type endpoint，没有做路由或写入副作用。

## Validation

validation 模块在：

```text
src/shard_preview_type_barrel_split_follow_up_non_participation_validation.cpp
```

它聚合三类条件：

```text
sourceLocked: v1385 source stage count 为 25 且 source complete
followUpClosed: 不要求 fresh mini-kv evidence，不启动 follow-up，不导入 Node types，不读 profile endpoint，不执行 Node checks
countsAligned: 20 checks / 20 stages 完整发布
```

只有三类条件同时成立时，`typeBarrelSplitFollowUpNonParticipationValidationPassed=true`。测试里覆盖了 source count 不足、closure flag 被打开、check count 不足三个失败分支，避免 validation 只是 happy path 字符串。

## Fixtures

新增并更新的 release fixture 是：

```text
fixtures/release/shard-readiness-v1386.json
...
fixtures/release/shard-readiness-v1405.json
fixtures/release/shard-readiness.json
```

`shard-readiness.json` 与 `shard-readiness-v1405.json` 来自真实 CLI `SHARDJSON` 输出，保持 current runtime 与 current fixture 完全一致。v1386-v1404 是同一结构下的历史版本化快照，逐步减少 published stage/check 数量并调整 release/status/sourceFrozen 字段，保证历史链条可以被回看。

## 测试和 smoke

本版新增测试：

```text
tests/type_barrel_split_follow_up_non_participation_stages_tests.cpp
tests/type_barrel_split_follow_up_non_participation_checks_tests.cpp
tests/type_barrel_split_follow_up_non_participation_validation_tests.cpp
tests/type_barrel_split_follow_up_non_participation_tests.cpp
tests/type_barrel_split_follow_up_non_participation_fixture_tests.cpp
```

同步更新了：

```text
tests/command_tests.cpp
tests/line_editor_tests.cpp
tests/shard_readiness_tests.cpp
tests/type_barrel_split_non_participation_fixture_tests.cpp
```

最关键的本地验证是 Debug 全量 CTest，261 条测试全部通过。这里特意用 Debug，是因为项目里大量测试使用 `assert`，Release 下可能被 `NDEBUG` 关闭；Debug 全量 CTest 能真实验证 fixture parity、命令契约、stage/check/validation 和旧链路 current status rollover。

真实运行 smoke 覆盖了两类路径：

```text
CLI: SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON / SHARDJSON / COMMANDSJSON
TCP: SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON / COMMANDS
```

TCP smoke 不发送完整 `SHARDJSON`，因为当前 `SHARDJSON` payload 很大，客户端行长限制可能把“巨大证据读取”误判为 TCP 失败。本版 TCP 只证明 standalone read command 和 command catalog 元数据可通过服务端读取，且验证后立即停止临时 server。

一句话总结：v1386-v1405 把 v1385 type barrel split 证据变成稳定 source fixture，并用 20 个后续版本把 current rollover、命令目录、fixture 链、归档质量和 no-execution 边界补齐，为 Node/Java/mini-kv 后续松耦合协作提供了更耐维护的只读证据层。
