# 第 1526-1545 版代码讲解：precheck upstream receipt verification split non-participation

## 最近250版讲解清算补记

本篇已纳入 v1316-v1565 最近250版讲解清算。清算参照 `D:\nodeproj\orderops-node\代码讲解记录\107-production-readiness-summary-v3-v103.md` 的 production-readiness walkthrough 模式；原文保留为历史正文，本节补齐统一判断口径。

- 清算范围：v1526-v1545（完整覆盖最近250版窗口）。
- 目标定位：本篇用于回看 mini-kv 只读证据、维护拆分或证据链闭环，不是新的运行入口。
- 不是什么：不打开 router/write/WAL/execution，不读取 credential/raw endpoint，不启动 Node/Java/mini-kv sibling 服务，不把 mini-kv 变成 order 或 audit authority。
- 入口和证据：以原文记录的 command surface、SHARDJSON/current fixture/versioned fixture、CTest、CLI/TCP smoke、归档说明为准；控制面只能按只读证据理解。
- 边界字段：阅读时优先核对 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers`、`diagnostics` 等字段是否继续表达只读、不可执行、非权威和不写入。
- 测试理解：测试应说明断言保护的边界行为；若原文仅列命令，本节将其清算为“命令证据必须服务于 no router / no write / no WAL / no execution 判断”。
- 清算结论：保留原位置，不搬迁；后续若重写正文，按治理模板补齐入口、结构、流程、边界字段、测试和一句话总结。

本版目标是把 Node v1983-v2002 的 `managedAuditManualSandboxConnectionPrecheckUpstreamReceiptVerification` 拆分计划，落成 mini-kv 侧的版本化、只读、不参与证据。它冻结的直接来源是 mini-kv v1525 的 blocked execution rehearsal split non-participation 证据；Node 计划中提到的 Java v99 receipt evidence 和 mini-kv v108 non-participation evidence 都只是 Node 侧已经消费过的历史引用，不要求 mini-kv 在本版重新扫描 Java、不要求读取旧 mini-kv fixture，也不要求启动任何服务。

它不是什么：不是新的执行入口，不导入 Node 模块，不解析 Node/Java 历史文件，不跑 TypeScript、Vitest、build 或 downstream guard，不变更 large-file inventory，不读取 endpoint 或 credential，不打开 managed audit connection，不安装 router，不写 store，不碰 WAL，也不允许 LOAD/RESTORE/COMPACT 作为证据生成的一部分。

## 入口和命令

本版新增的只读命令是：

```text
SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON
```

入口接在 `src/command.cpp`：

```cpp
return {shard_preview_precheck_upstream_receipt_verification_split_non_participation::
            format_precheck_upstream_receipt_verification_split_non_participation_json()};
```

命令表、HELP、补全和契约表同步加入了这个命令。`src/command_contracts.cpp` 中它的分类是 `read`，`mutates_store=false`，`touches_wal=false`，`stable=true`。这让 `COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 都能给出同一个结论：这是 metadata read，不是执行授权。

`SHARDJSON` 也滚动到 v1545。顶层 `status` 现在来自：

```cpp
precheck_upstream_receipt_verification_split_non_participation_status()
```

这表示当前 shard readiness 的最新证据层已经前进到 v1545，但 v1525 的 blocked execution rehearsal split 仍作为内嵌源证据保留，不被覆盖、不被重解释成运行时输入。

## 核心聚合模块

核心实现放在：

```text
src/shard_preview_precheck_upstream_receipt_verification_split_non_participation.cpp
include/minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation.hpp
```

它负责组装最终 JSON，包括：

- 合约名：`shard-route-precheck-upstream-receipt-verification-split-non-participation.v1`
- source Node plan：`docs/plans3/v1983-v2002-precheck-upstream-receipt-verification-split-roadmap.md`
- source Node range：`Node v1983-v2002`
- source closeout：`Node v2002`
- Node 历史引用：mini-kv v108 non-participation evidence、Java v99 receipt evidence
- mini-kv 冻结源：v1525 blocked execution rehearsal split non-participation
- 当前发布范围：v1526-v1545
- 当前阶段：`route-preview-precheck-upstream-receipt-verification-split-non-participation-clean-workspace-ci-closeout`

这里最重要的不是字段多，而是字段的边界语义明确。比如：

```text
readOnly=true
executionAllowed=false
writeRoutingAllowed=false
touchesWal=false
loadRestoreCompactAllowed=false
activeRouterInstalled=false
```

控制面读到这些字段时，应该把本证据理解成“可以展示、可以审计、可以作为跨项目计划完成度的只读依据”，而不是“可以启动 mini-kv、可以触发 Java/Node、可以执行写命令、可以恢复或压缩存储”。

另一个关键字段组是：

```text
miniKvImportsNodeModules=false
miniKvExecutesNodeChecks=false
miniKvReadsEndpoints=false
miniKvReadsCredentials=false
miniKvParsesHistoricalReceipts=false
miniKvScansJavaV99Evidence=false
miniKvExecutesDownstreamChecks=false
miniKvMutatesLargeFileInventory=false
```

这些字段把 Node v1983-v2002 的拆分成果和 mini-kv 的职责边界分开：mini-kv 只记录“Node 做了什么拆分、哪些行为仍归 Node 所有”，不把那些拆分模块变成 C++ 运行时依赖。

## 阶段目录

阶段目录拆到：

```text
src/shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages.cpp
include/minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_stages.hpp
```

20 个阶段对应 v1526-v1545。前 12 个阶段覆盖 Node v1983-v1994 的模块拆分：public loader/render exports、types、constants、Node v245 source adapter reference、Java v99 receipt reference、mini-kv v108 reference、private helpers、check policy、message policy、core assembly、renderer、entrypoint orchestration。

后 8 个阶段覆盖 Node v1995-v2002 的收尾和下游观察：code-health scan family、large-file inventory、direct tests forced fallback、downstream code-health、downstream rehearsal guard、typecheck/build、commit/tag/push/CI、clean workspace/no local process closeout。

每个阶段都携带 `sourceFrozenReleaseVersion` 和 `sourceFrozenFixturePath`。这不是运行时读取行为，而是版本化 fixture 链的解释线索。v1526 冻结 v1525，v1545 冻结 v1544，形成可回看、可审计的发布序列。

## Check 列表

check 列表拆到：

```text
src/shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks.cpp
include/minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_checks.hpp
```

这里的 20 个 check code 是控制面最容易直接消费的“边界摘要”。例如：

```text
public-loader-render-exports-preserved-no-import
node-v245-source-adapter-no-parse
java-v99-receipt-reference-no-scan
minikv-v108-non-participation-reference-no-parse
core-assembly-no-execution
renderer-no-execution
large-file-inventory-observed-only
typecheck-build-observed-only
clean-workspace-ci-closeout
```

每个 check 都重复携带关键 false 字段。这看起来有些冗余，但对证据消费是有价值的：即使控制面只截取 check 数组，也不会丢失“no router / no write / no WAL / no execution / no endpoint / no credential / no Java scan”的边界判断。

`format_precheck_upstream_receipt_verification_split_non_participation_checks_json()` 对 published count 做越界检查，防止历史 fixture 生成时把不存在的阶段误发布出去。

## Validation

validation 拆到：

```text
src/shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation.cpp
include/minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation.hpp
```

验证逻辑分成四类：

```text
source_locked
node_plan_observed_only
mini_kv_boundary_closed
counts_aligned
```

`source_locked` 要求 v1525 blocked execution rehearsal split 已发布 20 个阶段且完整；`node_plan_observed_only` 要求 Node 不需要新 mini-kv/Java 证据；`mini_kv_boundary_closed` 要求所有导入、执行、endpoint、credential、历史 receipt、Java scan、downstream check、inventory mutation 都是 false；`counts_aligned` 要求本版发布 20 个阶段、20 个 check。

只有这四个都成立，`precheckUpstreamReceiptVerificationSplitNonParticipationValidationPassed` 才是 true。历史 fixture v1526-v1544 会按阶段发布，因此 validation 故意保持 false；v1545 才是完整 closeout。

## Fixture

本版生成并保留：

```text
fixtures/release/shard-readiness-v1526.json
...
fixtures/release/shard-readiness-v1545.json
fixtures/release/shard-readiness.json
```

v1526-v1544 是阶段化历史 fixture：每个文件的 root `releaseVersion`、顶层 `status`、当前 section release version、stage sequence、source frozen path 都推进到对应版本；`stageCatalog` 和 check 数组只发布到对应阶段。

v1545 和 `shard-readiness.json` 来自真实运行时 `SHARDJSON` 输出，表示完整 20/20 的当前状态。这样 Node 或其他控制面既能看当前完整证据，也能回看每个中间版本如何一步步发布。

## 测试覆盖

新增测试包括：

```text
precheck_receipt_split_np_stages_tests
precheck_receipt_split_np_checks_tests
precheck_receipt_split_np_validation_tests
precheck_receipt_split_np_tests
precheck_receipt_split_np_fixture_tests
```

阶段测试保护 release version、source frozen version、fixture path 的连续性。check 测试保护 20 个 check code 和 false 边界字段不会漏。validation 测试不只测 happy path，还逐个把 source count、fresh evidence、Node import、endpoint、credential、Java scan、downstream execution、inventory mutation、count mismatch 改坏，确认 validation 会失败。主模块测试保护独立命令 JSON 的核心字段。fixture 测试逐个读取 v1526-v1545，确认版本化 fixture 链存在且边界字段稳定。

已有测试也同步更新：`command_tests` 保护命令目录和独立命令；`line_editor_tests` 保护补全；`shard_readiness_tests` 保护 SHARDJSON embedding；v1525 的 fixture 测试确认当前 fixture 已滚到 v1545，但旧 v1525 section 仍存在。

实际验证包括：

```text
定向 CTest：9/9 通过
完整 CTest：301/301 通过
CLI smoke：新命令、COMMANDS、CHECKJSON 通过
TCP smoke：临时 server + client 读取新命令通过，server 已停止
```

## 一句话总结

v1526-v1545 把 Node precheck upstream receipt verification split 的完成度变成 mini-kv 可审计的只读证据，同时继续守住 no Node import、no Java scan、no router、no write、no WAL、no execution 的松耦合边界。
