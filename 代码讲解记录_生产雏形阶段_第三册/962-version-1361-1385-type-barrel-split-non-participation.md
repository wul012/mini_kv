# 第 962 篇代码讲解：mini-kv v1361-v1385 type barrel split non-participation

本版目标是把 Node v1822-v1846 的 `controlled read-only shard preview type barrel split` 作为 mini-kv 的版本化只读证据冻结下来。它的角色是证据链，不是新的执行入口：mini-kv 不导入 Node 的 TypeScript type/barrel/profile boundary，不读取 profile endpoint 或 evidence endpoint，不执行 Node typecheck/build/Vitest，不安装 router，不写 WAL，不开启 load/restore/compact，也不获得任何跨项目审批权。

为什么要做这一版：最新 Node 计划把一个过大的 controlled read-only shard preview type aggregator 拆成稳定 barrel、profile-only type module、aggregate re-export module，以及 reads/preview graph/evidence endpoints 等命名 profile boundary。mini-kv 侧不需要新鲜证据才能让 Node 完成这个维护拆分，但需要把“我知道这件事，并且我没有参与运行时执行”的边界写成可测试 fixture，避免后续控制面误解为 mini-kv 已经消费了 Node 类型或 endpoint。

## 入口与挂接

主入口仍然是 `SHARDJSON`，位置在：

```text
src/shard_readiness.cpp
```

本版只在主聚合里做四件事：

```text
releaseVersion -> v1385
status -> route-preview-type-barrel-split-non-participation-clean-workspace-ci-closeout-read-only
evidenceDigest 增加 shardPreviewTypeBarrelSplitNonParticipation marker
JSON 增加 shardPreviewTypeBarrelSplitNonParticipation section
```

这点很重要：25 版的细节没有继续堆进 `shard_readiness.cpp`，而是挂到独立模块：

```text
include/minikv/shard_preview_type_barrel_split_non_participation.hpp
src/shard_preview_type_barrel_split_non_participation.cpp
```

此外，本版新增独立只读命令：

```text
SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON
```

命令接入点在：

```text
src/command.cpp
src/command_contracts.cpp
src/line_editor.cpp
```

它被声明为 read/stable/no WAL/no mutation 命令，并进入 `COMMANDS`、`COMMANDSJSON`、`HELP` 和客户端补全候选。

## Stage 目录

25 个 mini-kv 版本映射 Node v1822-v1846：

```text
src/shard_preview_type_barrel_split_non_participation_stages.cpp
```

每个 stage 记录：

```text
sequence
releaseVersion
stage
sourceFrozenReleaseVersion
sourceFrozenFixturePath
scope
```

第一条是 `v1361`，冻结来源是 `v1360`；最后一条是 `v1385`，冻结来源是 `v1384`。测试会逐项确认 release version、source frozen release、source frozen fixture path 连续递增。这样 Node 的 type-barrel 维护拆分可以被 mini-kv 作为“版本化观察链”保存，但每一步都仍然是只读证据，不是运行时消费。

## Check 目录

check catalog 在：

```text
src/shard_preview_type_barrel_split_non_participation_checks.cpp
```

它把 Node v1822-v1846 的重点拆成 25 个 check，例如：

```text
stable type barrel boundary
aggregate downstream type re-export module
profile-only module
reads profile boundary
preview graph profile boundary
evidence endpoint profile boundary
typecheck/build/Vitest
CI and clean workspace closeout
```

每条 check 都带着一组禁止项：

```text
nodeStableBarrelImportedByMiniKv=false
nodeProfileTypesImportedByMiniKv=false
nodeAggregateReexportsImportedByMiniKv=false
nodeProfileBoundaryEndpointReadByMiniKv=false
nodeTypecheckExecutedByMiniKv=false
nodeVitestExecutedByMiniKv=false
miniKvTypeBarrelSplitStarted=false
miniKvConsumesNodeTypes=false
profileBoundaryEndpointRead=false
typeRouterInstalled=false
writeRoutingAllowed=false
touchesWal=false
executionAllowed=false
```

控制面读到这些字段时，应理解为：mini-kv 只证明自己没有参与 Node 类型桶拆分执行，不证明 Node 类型本身正确，也不把 Node profile boundary 变成 mini-kv 的可调用接口。

## Validation

validation 模块在：

```text
src/shard_preview_type_barrel_split_non_participation_validation.cpp
```

它聚合三类条件：

```text
sourceLocked: v1360 catalog split source 已完成且为 30 stages
typeSplitClosed: mini-kv 没有启动 type split、没有导入 Node types、没有消费 profile boundaries、没有读 endpoint
countsAligned: 25 checks / 25 stages 完整发布
```

只有三类条件同时成立，`typeBarrelSplitNonParticipationValidationPassed=true`。专项 validation 测试还覆盖 source count 不足、closure flag 被打开、check count 不一致三个失败分支，避免这个字段只是 happy-path 字符串。

## Fixtures

本版新增：

```text
fixtures/release/shard-readiness-v1361.json
...
fixtures/release/shard-readiness-v1385.json
fixtures/release/shard-readiness.json
```

当前 fixture 来自新二进制实际 `SHARDJSON` 输出；历史 v1361-v1384 作为版本化冻结 fixture，递增更新 root release/status、嵌套 stage、source frozen pointer、published counts、stage catalog 和 check catalog。测试会逐版确认每个 fixture 都包含：

```text
root releaseVersion=v1361..v1385
shardPreviewTypeBarrelSplitNonParticipation section
typeBarrelSplitNonParticipationReleaseVersion 对应本版
sourceIntegrityReleaseVersion=v1360
sourceNodeTypeBarrelSplitCloseoutReleaseVersion=Node v1846
no Node type import / no endpoint read / no router / no write / no WAL / no execution
```

旧 v1331-v1360 catalog split fixture 没有被改写；它作为本版 source integrity 固定在 v1360。

## 测试与真实 smoke

新增专项测试：

```text
tests/type_barrel_split_non_participation_stages_tests.cpp
tests/type_barrel_split_non_participation_checks_tests.cpp
tests/type_barrel_split_non_participation_validation_tests.cpp
tests/type_barrel_split_non_participation_tests.cpp
tests/type_barrel_split_non_participation_fixture_tests.cpp
```

同时更新了 `command_tests`、`line_editor_tests`、`shard_readiness_tests` 和少量旧 fixture/current-status 断言。全量验证结果是：

```text
targeted CTest: 11/11 passed
full CTest: 256/256 passed
CLI smoke: passed
TCP smoke: passed for SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON and COMMANDS
git diff --check: exit 0
```

TCP smoke 没用完整 `SHARDJSON`，因为当前 `SHARDJSON` 体量已经超过 bundled TCP client 单行读取上限；本版 TCP 只验证新增独立只读命令，完整 `SHARDJSON` 由 CLI 和 CTest 覆盖。

一句话总结：v1361-v1385 把 Node type barrel/profile boundary 拆分变成 mini-kv 可审查、可回放、可测试的只读非参与证据，同时通过模块拆分控制了主聚合文件继续膨胀的风险。
