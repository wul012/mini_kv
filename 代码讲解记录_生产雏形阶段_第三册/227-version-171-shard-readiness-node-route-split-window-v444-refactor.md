# mini-kv v171：Node v444 路由拆分窗口与元数据收敛

本版目标是把 Node v443/v444 的 managed audit route group split 纳入 mini-kv 的只读兼容窗口，同时收敛 route split window 的重复元数据。它不是新的运行入口，不启动 Java 或 mini-kv，不安装 active router，不放开写路由，不执行 LOAD / RESTORE / COMPACT，也不把 mini-kv 变成订单或审计权威。

## 版本角色

v171 先冻结 v170 为 `fixtures/release/shard-readiness-v170.json`，再把 `nodeRouteSplitCompatibilityWindow` 从 Node v433-v442 扩展到 Node v433-v444。新增的 Node v443 和 v444 都是 route registration extraction：一个覆盖 managed audit adapter 路由组，一个覆盖 managed audit persistence 路由组。mini-kv 只记录“这些变化不会要求 mini-kv 改 API、改 fixture path、启动服务或执行 runtime probe”。

## 实现拆分

`src/shard_readiness_node_compatibility.cpp` 仍是核心入口。本版在匿名命名空间中增加 `route_split_window_mode`、`route_split_window_source_node_plan`、`route_split_window_source_frozen_release_version`、`route_split_window_source_frozen_fixture_path`、`route_split_window_start_node_version`、`route_split_window_end_node_version` 和 `route_split_window_source_frozen_digest` 等局部常量。

这样 `format_route_split_compatibility_window_json()` 和 `format_route_split_compatibility_window_audit_json()` 不再各自散落同一组版本来源字段。窗口与审计都从同一组常量读源计划、冻结版本、冻结 fixture、窗口起止和冻结 digest；版本数量继续由 `route_split_compatibility_window_versions().size()` 推导，避免手动计数漂移。

`src/shard_readiness.cpp` 负责把 v171 组合进整体 `SHARDJSON`：`releaseVersion=v171`，`fixtureParity` 增加 v170，`archiveCompatibility` 增加 Node v443/v444，`historicalFallback` 指向 v170，并新增 `nodeV443ManagedAuditAdapterRouteSplitPreserved` 与 `nodeV444ManagedAuditPersistenceRouteSplitPreserved`。这些字段仍只说明历史和边界，不会触发执行。

`src/shard_readiness_boundary_fields.cpp` 没有新增 field 或 group。catalog 仍是 `read-only-boundary-fields.v9`、420 fields、22 groups，只把当前来源计划同步到 Node v444 和冻结 v170，说明这是兼容窗口维护，不是边界字段扩张。

## 测试保护

`tests/shard_readiness_tests.cpp` 继续验证 rolling fixture、冻结 fixture 链、v170 冻结内容、v171 当前输出和只读边界。重点断言包括：窗口结束于 Node v444，`splitRouteGroupCount=12`，audit 期望/观察数量同为 12，source frozen fixture 指向 v170，source frozen digest 是 v170 的 digest，catalog 仍是 v9/420/22。

`tests/command_tests.cpp` 从命令处理器路径验证同样的契约，确保 `SHARDJSON` 仍是 read-only metadata command，`EXPLAINJSON` / `CHECKJSON` 不会把它解释成写命令或执行命令。

## 后续价值

v171 让 route split window 继续追上 Node v444，同时把窗口元数据集中，降低后续每次扩窗时漏改 source plan、fixture、digest 或计数的风险。
