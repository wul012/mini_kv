# mini-kv v212 node route catalog cleanup post-closeout continuity release catalog retention audit

## 本版目标

v212 冻结 v211 的留存型 post-closeout continuity evidence，并把当前阶段推进到 `post-closeout-continuity-release-catalog-retention-audit`。它不是重新发布 catalog，而是审计“留存点本身是否可以作为后续消费基线”：source fixture 是否版本化、digest 是否固定、post-closeout range 是否连续、下游是否仍只能只读消费。

本版仍然不是执行入口，不授权 Node/Java runtime probe，不启动 mini-kv 服务作为联调依赖，也不打开 active shard router。所有关键边界继续保持：`readOnly=true`、`executionAllowed=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`loadRestoreCompactAllowed=false`。

## 计划依据

推进前只读读取了 `D:\nodeproj\orderops-node\docs\plans\README.md` 和 `D:\nodeproj\orderops-node\docs\plans3\v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`。该计划描述 Node v490 的 archive verifier 路由和批次收口，Java/mini-kv 被推荐并行推进。mini-kv 因此只提供可冻结、可审计、可回放的 evidence，不修改 Node 或 Java。

## 冻结来源和连续性

`fixtures/release/shard-readiness-v211.json` 保存 v211 的 rolling fixture，source digest 是 `fnv1a64:e116b578a611d13d`。v212 的 current fixture digest 是 `fnv1a64:1649e22a22a59fd8`。这形成两层关系：v211 是本版审计的输入，v212 是本版输出的可消费证据。

`historicalFallback.previousConsumedReleaseVersion=v211` 表示下游可以回退到上一版冻结证据；`fixtureParity.historicalFixturePaths` 追加 `shard-readiness-v211.json` 表示历史文件已经纳入 current evidence 的可见目录；`trackedPostCloseoutRangeEnd=v212` 和 `trackedPostCloseoutReleaseCount=12` 则让控制面能检查 v201 之后的 post-closeout lane 没有断链。

## 模块职责

`src/shard_readiness_route_catalog_post_closeout.cpp` 继续承载 post-closeout continuity 小节，负责 stage、source frozen 三元组、range、archive verifier 计数和边界字段。它只改变 metadata，不改变 `SHARDJSON` 命令入口。

`src/shard_readiness_history.cpp` 维护 diagnostics、fixture parity 和 historical fallback。这里的设计重点是把“当前 release 给谁消费”和“当前 release 从哪版冻结而来”拆开，避免 Node/Java 把 rolling current 当成历史基线。

`tests/shard_readiness_tests.cpp` 保护完整 JSON 输出，`tests/command_tests.cpp` 保护命令入口输出。两边都断言 v212 release、v211 source fixture、`post-closeout-continuity-release-catalog-retention-audit`、sequence 12、fallback v211 以及 no-router/no-write/no-execution。

## 下游如何理解

当 Node v491+ 或 Java echo 看到本版时，应该把它理解为“v211 留存点的审计结果”。它可以证明 mini-kv 的证据链还在继续，但不能证明 runtime 可以启动，也不能替代 operator approval、credential handle、raw endpoint、managed audit connection 或 deployment/rollback 审批。

## 验证结果

Focused CTest 2/2 通过，full CTest 71/71 通过。TCP smoke 在 `127.0.0.1:6485` 读取真实 server 返回的 `SHARDJSON`，确认 release v212、digest `fnv1a64:1649e22a22a59fd8`、fallback v211、readOnly=True、executionAllowed=False、router/write 仍关闭。验证后服务端已停止。
