# mini-kv v170：Node v442 路由拆分窗口证据

本版目标是把 Node v442 的 credential resolver disabled runtime shell design draft body draft candidate route group split 纳入 mini-kv 的 `SHARDJSON` 只读证据链。它不是新的执行入口，不启动 Java 或 mini-kv 服务，不安装 active shard router，不放开写路由，也不让 LOAD / RESTORE / COMPACT 或任何 runtime execution 通过。

## 版本角色

v170 先把 v169 的 route-split window audit 冻结为 `fixtures/release/shard-readiness-v169.json`，再把当前 `nodeRouteSplitCompatibilityWindow` 从 Node v433-v441 扩展到 Node v433-v442。这样 Node 的新一轮 route registration 抽分可以被 mini-kv 作为兼容证据消费，但 mini-kv 仍只表达边界，不参与 Node 路由表实现。

## 核心实现

`src/shard_readiness_node_compatibility.cpp` 继续是 Node route split 兼容证据的入口。`route_split_compatibility_window_versions()` 新增 `Node v442`，`format_route_split_compatibility_window_json()` 将 `windowMode`、`sourceNodePlan`、`sourceFrozenReleaseVersion`、`sourceFrozenFixturePath`、`windowEndNodeVersion` 和 `splitRouteGroupCount` 同步到 v170 语义。

同一文件里的 `format_route_split_compatibility_window_audit_json()` 把审计计数从 9 提升到 10，并把冻结来源指向 v169。关键边界仍是 `runtimeGateApprovalPresent=false`、`runtimeExecutionPacketExecutable=false`、`startsJavaService=false`、`startsMiniKvService=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`writeCommandsAllowed=false`、`adminCommandsAllowed=false`、`loadRestoreCompactAllowed=false`、`executionAllowed=false`。

`src/shard_readiness.cpp` 负责版本级组合：`releaseVersion` 变为 `v170`，诊断提示改为 Node v442+ 消费 v170 证据，`fixtureParity` 增加 `shard-readiness-v169.json`，`historicalFallback` 指向 v169，归档兼容列表增加 Node v442。digest seed 和 digest 分片也同步到 v170，确保 fixture 是重新生成的真实版本证据。

`src/shard_readiness_boundary_fields.cpp` 没有增加新的字段或分组。`boundaryCatalogIndex` 仍保持 `read-only-boundary-fields.v9`、420 fields、22 groups，只把当前来源计划和冻结来源同步到 v442/v169，用来说明 catalog 边界继续沿用而不是膨胀。

## 测试保护

`tests/shard_readiness_tests.cpp` 验证 `SHARDJSON` 当前输出、滚动 fixture、冻结 v169 fixture、历史 fixture 链和只读边界。新增断言覆盖 v442 窗口、10 个版本计数、v169 冻结来源、v169 digest 继承、Node v442 preserved 标志，以及 catalog v9 计数不变。

`tests/command_tests.cpp` 走真实 `CommandProcessor`，证明 `SHARDJSON` 仍是 read command，`COMMANDSJSON` / `EXPLAINJSON` / `CHECKJSON` 没有把它变成写命令或执行命令。窗口和 audit 的 v442 字段也在命令层被验证，防止只改 fixture 不改运行输出。

## 后续价值

v170 给 Node v442 route registration 抽分补上 mini-kv 侧版本化证据，同时保持“版本化证据 + 只读边界 + 不破坏旧 schema + 不提前打开执行”的路线，方便后续 Node/Java/mini-kv 继续松耦合推进。
