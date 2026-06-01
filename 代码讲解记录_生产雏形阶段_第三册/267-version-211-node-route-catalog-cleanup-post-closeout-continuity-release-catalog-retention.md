# mini-kv v211 node route catalog cleanup post-closeout continuity release catalog retention

## 本版目标

v211 的目标是把 v210 已审计的 post-closeout continuity release catalog 冻结成稳定输入，并向 current `SHARDJSON` 发布一个留存型阶段：`post-closeout-continuity-release-catalog-retention`。它回答的是控制面问题：下游如果在 Node v490 收口之后继续读取 mini-kv 证据，应该读到一个明确、版本化、可回放的 release-catalog 留存点，而不是依赖 rolling current 的隐含状态。

本版不是新的执行入口，不是 Java/Node 联调开关，不是 shard router，也不是 managed audit 存储后端。所有 runtime 权限仍然关闭：`readOnly=true`、`executionAllowed=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`startsServices=false`。

## 计划依据和边界

本版推进前只读读取了 Node 计划入口 `D:\nodeproj\orderops-node\docs\plans\README.md` 和最新计划 `D:\nodeproj\orderops-node\docs\plans3\v490-post-java-mini-kv-route-catalog-cleanup-verification-checklist-evidence-archive-verification-route-roadmap.md`。计划仍然把 mini-kv 放在推荐并行位置：Node v490 暴露 archive verifier 路由并收口批次，mini-kv 侧只需要提供可消费的只读证据，不需要修改 Node、Java，也不需要启动任何服务。

## 冻结来源

`fixtures/release/shard-readiness-v210.json` 是从 v210 的 rolling fixture 复制出来的冻结证据。它的 digest 是 `fnv1a64:1ccb2dd05cd63996`。v211 的 `nodeRouteCatalogCleanupPostCloseoutContinuity` 使用这些字段指回来源：

- `sourceFrozenReleaseVersion=v210`
- `sourceFrozenFixturePath=fixtures/release/shard-readiness-v210.json`
- `sourceFrozenDigest=fnv1a64:1ccb2dd05cd63996`
- `rollingCurrentRejected=true`

这四个字段一起防止下游把当前滚动文件误当成历史基线。后续 Node/Java 只要按 releaseVersion、fixture path 和 digest 校验，就能判断自己消费的是冻结证据，而不是临时构建输出。

## 代码入口

`src/shard_readiness_route_catalog_post_closeout.cpp` 是本版主要入口。它只更新阶段常量：`continuity_stage`、`stage_sequence`、`stage_release_version`、`stage_scope` 和 source frozen 三元组。JSON shape 不变，因此 `SHARDJSON` 的公开契约没有新增解析负担。

`src/shard_readiness_history.cpp` 负责滚动历史：diagnostics 里的 `nodeConsumer` 变为 v211，fixture parity 列表追加 `shard-readiness-v210.json`，historical fallback 指向 v210。这里的意义是把“最新可消费版本”和“上一版冻结来源”分开，避免下游把两种角色混在一起。

`src/shard_readiness.cpp` 只滚动 release seed 和 evidence digest 输入。它仍然聚合现有 formatter，不触碰 WAL、snapshot、restore、TCP server 行为，也不把 post-closeout evidence 变成命令路由。

## 字段怎么读

`continuityStage=post-closeout-continuity-release-catalog-retention` 表示本版是留存阶段，价值在于保存 audit 后的 release catalog，而不是再创建一个 catalog。`stageSequence=11` 表示 post-closeout lane 从 v201 起已经推进到第 11 个阶段。`trackedPostCloseoutRangeEnd=v211` 让下游可以做连续性检查：如果它看到范围断裂，就应该 fail closed。

边界字段仍然是最重要的保护：`nodeAddsEvidenceGate=false` 表示 Node 不需要因为本版新增入口；`runtimeExecutionPacketExecutable=false` 表示没有可执行包；`activeShardPrototypeEnabled=false` 和 `routerActivationAllowed=false` 表示没有活跃分片；`writeCommandsAllowed=false` 与 `loadRestoreCompactAllowed=false` 继续保护存储和恢复路径。

## 测试和真实 smoke

`tests/shard_readiness_tests.cpp` 和 `tests/command_tests.cpp` 同时钉住 v211 的 release、v210 冻结 fixture、source digest、stage name、sequence、range end 以及 no-router/no-write/no-execution 边界。本版还把 post-closeout continuity 的计划断言对齐到 Node v490 计划文件，避免测试只因为历史文本碰巧存在而通过。

验证结果：focused CTest 2/2 通过，full CTest 71/71 通过。TCP smoke 在 `127.0.0.1:6484` 启动真实 `minikv_server.exe`，通过 `minikv_client.exe` 读取 `SHARDJSON`，返回 v211、digest `fnv1a64:e116b578a611d13d`、fallback v210，并确认 read-only、no router、no write、no execution。smoke 后服务端进程已停止。

## 后续价值

v211 给 Node v491+ 或 Java echo 的价值是一个稳定留存点：他们可以消费 v211 rolling fixture，也可以回看 v210 frozen fixture，二者通过 digest 串起来。这样 mini-kv 继续提供证据链，而不承担订单权威、审计权威或运行时执行权。
