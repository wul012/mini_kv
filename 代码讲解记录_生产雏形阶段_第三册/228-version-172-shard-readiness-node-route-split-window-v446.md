# mini-kv v172：Node v446 managed audit restore drill 窗口证据

本版目标是把 Node v445/v446 的 managed audit route group split 纳入 mini-kv 的只读兼容窗口。它不是新的执行入口，不启动 Java 或 mini-kv 服务，不安装 active router，不放开写路由，不执行 LOAD / RESTORE / COMPACT，也不让 mini-kv 成为订单或审计权威。

## 版本角色

v172 先冻结 v171 为 `fixtures/release/shard-readiness-v171.json`，再把 `nodeRouteSplitCompatibilityWindow` 从 Node v433-v444 扩展到 Node v433-v446。新增的 Node v445 是 managed audit identity approval route group split，Node v446 是 managed audit restore drill route group split；两者在 Node 计划里都被限定为 route registration extraction，不新增 evidence gate，不改 API path，不启动 sibling 服务。

## 核心实现

`src/shard_readiness_node_compatibility.cpp` 继续使用 v171 引入的局部元数据常量。v172 只更新这些常量的窗口模式、来源计划、冻结版本、冻结 fixture、窗口终点和冻结 digest，并把 `Node v445`、`Node v446` 加入 `route_split_compatibility_window_versions()`。

`format_route_split_compatibility_window_json()` 和 `format_route_split_compatibility_window_audit_json()` 的计数仍由 shared vector 推导，所以窗口从 12 个版本变成 14 个版本时，不需要手工维护两套数字。审计继续发布 `contiguousNodeVersionWindow=true`、`duplicateWindowVersionsDetected=false`、`allWindowVersionsRouteRegistrationOnly=true`，并保持 runtime/write/router 相关字段全部为 false。

`src/shard_readiness.cpp` 负责把 v172 组合进整体 `SHARDJSON`：`fixtureParity` 增加 v171，`archiveCompatibility` 增加 Node v445/v446，`historicalFallback` 指向 v171，并新增 `nodeV445ManagedAuditIdentityApprovalRouteSplitPreserved` 与 `nodeV446ManagedAuditRestoreDrillRouteSplitPreserved`。

`src/shard_readiness_boundary_fields.cpp` 只同步当前来源计划和冻结来源，catalog 仍是 `read-only-boundary-fields.v9`、420 fields、22 groups，没有新增边界字段。

## 测试保护

`tests/shard_readiness_tests.cpp` 验证 rolling fixture、冻结 v171 fixture、窗口覆盖 Node v433-v446、`splitRouteGroupCount=14`、audit 期望/观察数量同为 14、source frozen digest 来自 v171，并继续断言 no router、no write、no execution。

`tests/command_tests.cpp` 从真实命令处理器路径验证 `SHARDJSON` 的同一组契约，保证 CLI/TCP 读到的是运行时代码生成的证据，而不是只更新了文件。

## 后续价值

v172 让 mini-kv 继续跟上 Node v446 的 route-table 拆分进度，同时保持版本化冻结证据和只读边界不变，为后续 Java/Node 消费链保留稳定、可审计的兼容窗口。
