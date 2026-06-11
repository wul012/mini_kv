# mini-kv v1476-v1490 release-window readiness packet split non-participation

## 最近250版讲解清算补记

本篇已纳入 v1316-v1565 最近250版讲解清算。清算参照 `D:\nodeproj\orderops-node\代码讲解记录\107-production-readiness-summary-v3-v103.md` 的 production-readiness walkthrough 模式；原文保留为历史正文，本节补齐统一判断口径。

- 清算范围：v1476-v1490（完整覆盖最近250版窗口）。
- 目标定位：本篇用于回看 mini-kv 只读证据、维护拆分或证据链闭环，不是新的运行入口。
- 不是什么：不打开 router/write/WAL/execution，不读取 credential/raw endpoint，不启动 Node/Java/mini-kv sibling 服务，不把 mini-kv 变成 order 或 audit authority。
- 入口和证据：以原文记录的 command surface、SHARDJSON/current fixture/versioned fixture、CTest、CLI/TCP smoke、归档说明为准；控制面只能按只读证据理解。
- 边界字段：阅读时优先核对 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers`、`diagnostics` 等字段是否继续表达只读、不可执行、非权威和不写入。
- 测试理解：测试应说明断言保护的边界行为；若原文仅列命令，本节将其清算为“命令证据必须服务于 no router / no write / no WAL / no execution 判断”。
- 清算结论：保留原位置，不搬迁；后续若重写正文，按治理模板补齐入口、结构、流程、边界字段、测试和一句话总结。

## 本版目标和边界

本版把 Node 最新计划 `docs/plans3/v1935-v1937-release-window-readiness-packet-split-roadmap.md` 中的 release-window readiness packet split 变成 mini-kv 的只读非参与证据。Node 侧这轮做的是 release window readiness packet 的 Types / Evidence / Policy / loader / renderer 拆分，并且计划明确消费历史 Java v61 rollback approval fixture 和历史 mini-kv v70 restore drill evidence，不要求 mini-kv 提供新的实时服务、端点、凭据或执行结果。

所以 mini-kv v1476-v1490 做的不是打开 release-window 执行入口，也不是运行 Node 的检查，更不是读取 Node endpoint、credential、managed audit connection 或 restore drill runtime。它只冻结 v1475 的 implementation-plan upstream echo closeout evidence，发布 15 个版本化 fixture，证明 mini-kv 在 Node 这轮拆分中保持 read-only、no router、no write、no WAL、no execution。

本版范围和 Node 计划一致：Node 继续拥有 release-window packet 的真实实现和验证；mini-kv 只做 C++ 侧证据链、命令暴露、SHARDJSON 嵌入、fixture 和测试闭环。

## 模块拆分

新增入口是 `SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON`，核心 formatter 放在 `src/shard_preview_release_window_readiness_packet_split_non_participation.cpp`。这个文件负责组装最终 JSON：source Node plan、历史 Java/mini-kv 引用、v1475 source fixture、当前 v1490 stage、15 个 checks、validation 和边界字段。

阶段目录拆到 `src/shard_preview_release_window_readiness_packet_split_non_participation_stages.cpp`。这里定义 v1476 到 v1490 的 15 个 stage，从 `source-freeze` 开始，到 `clean-workspace-ci-closeout` 结束。每个 stage 都有 `sourceFrozenReleaseVersion` 和 `sourceFrozenFixturePath`，因此 fixture 链可以说明自己从哪个前一版本冻结而来。

检查目录拆到 `src/shard_preview_release_window_readiness_packet_split_non_participation_checks.cpp`。这里不是简单列名字，而是把每个 check 都绑定到一个 source 和 protects 说明，并统一输出 false 边界字段：不导入 Node packet stable entrypoint、types、evidence、policy，不执行 loader / renderer / checks，不跑 Node typecheck / vitest / build，不启动服务，不读 endpoint 或 credential，不重放 restore drill，不安装 router，不写 WAL。

validation 拆到 `src/shard_preview_release_window_readiness_packet_split_non_participation_validation.cpp`。它把通过条件压成几个明确判断：v1475 source 是否完整、Node 是否不要求 fresh mini-kv evidence、mini-kv 边界是否关闭、planned/completed/published counts 是否都是 15。这样未来如果有人误把 count 改成 14、或把 `miniKvExecutesNodeChecks` 之类字段打开，validation 会直接变成 false。

## SHARDJSON 和命令面

`src/shard_readiness.cpp` 把根 releaseVersion 滚到 `v1490`，根 status 改成：

```text
route-preview-release-window-readiness-packet-split-non-participation-clean-workspace-ci-closeout-read-only
```

同时新增 `shardPreviewReleaseWindowReadinessPacketSplitNonParticipation` section，并把 digest material 加入 root evidence digest。控制面读到这个 section 时，可以把它理解成：Node v1935-v1937 的 release-window packet split 已被 mini-kv 只读观察，但 mini-kv 没有导入 Node module、没有执行 Node check、没有启动服务、没有读 endpoint/credential，也没有把 restore/load/compact 或 write routing 打开。

`src/command.cpp`、`src/command_contracts.cpp`、`src/line_editor.cpp` 同步接入新命令。命令目录计数从 84 变成 85，`COMMANDS` / `COMMANDSJSON` / `HELP` / line editor completion 都能看到新命令；命令 contract 标为 read、no mutate store、no WAL touch、stable。

## 关键字段怎么读

`sourceImplementationPlanReleaseVersion: "v1475"` 表示本版不是凭空开始，而是从上一版 implementation-plan closeout fixture 冻结而来。

`sourceNodeReleaseWindowReadinessPacketSplitCloseoutReleaseVersion: "Node v1937"` 表示 mini-kv 记录的是 Node 已完成 packet split closeout 的上下文，不代表 mini-kv 执行了 Node 的 packet builder、renderer 或 checks。

`sourceNodeConsumesHistoricalMiniKvReleaseVersion: "v70"` 和 `sourceNodeConsumesHistoricalJavaReleaseVersion: "Java v61"` 是历史证据引用。它们强调 Node 本轮消费的是既有 fixture/evidence，不要求 mini-kv 或 Java 新启动服务。

`releaseWindowReadinessPacketSplitNonParticipationOnly: true` 是本版的角色定位。它说明 mini-kv 只是非参与证据，不是 release-window packet 的真实生产者、执行者或审批者。

`miniKvExecutesReleaseWindowPacket: false`、`miniKvReplaysRestoreDrillEvidence: false`、`nodeProductionReleaseDryRunEnvelopeExecutedByMiniKv: false` 是这版最重要的运行边界。它们把“读到 release window”与“执行 release window”明确分开。

`activeRouterInstalled: false`、`writeRoutingAllowed: false`、`touchesWal: false`、`executionAllowed: false` 继续维持 mini-kv 的 shard preview 纪律：证据可以增长，实际路由和写路径不能被顺手打开。

## 测试和 fixture

新增 5 组测试：

```text
release_window_readiness_packet_split_non_participation_stages_tests
release_window_readiness_packet_split_non_participation_checks_tests
release_window_readiness_packet_split_non_participation_validation_tests
release_window_readiness_packet_split_non_participation_tests
release_window_readiness_packet_split_non_participation_fixture_tests
```

stage tests 保护 v1476-v1490 的顺序、source freeze 连续性和 planned count。checks tests 保护 15 条边界检查都存在，并且 check count 不能越界。validation tests 保护 source/count/boundary 任一条件不满足时 validation 必须失败。核心 JSON tests 保护 standalone command 的字段语义。fixture tests 保护 `fixtures/release/shard-readiness-v1476.json` 到 `v1490.json` 以及 rolling current fixture 都包含新 section 和关键 false 边界。

旧测试也做了必要滚动：`command_tests` 认可 85 个命令和新命令 contract；`line_editor_tests` 认可新命令补全；`shard_readiness_tests` 认可根 status 已经从 v1475 滚到 v1490；旧 value-supply 相关 current status 断言同步更新，但它们自己的历史模块语义没有被改写。

本地验证结果是 Debug 构建通过、针对性测试 9/9 通过、全量 CTest 286/286 通过。真实 CLI/TCP smoke 归档会作为本版截图证据补在 `e/1490/`。

## 版本价值

v1476-v1490 给 mini-kv 增加了一条更薄但更硬的跨项目证据链：Node 可以继续拆 release-window readiness packet，而 mini-kv 用版本化 fixture 证明自己没有被拖进 Node runtime、endpoint、credential、router、write、WAL 或 execution 边界里。
