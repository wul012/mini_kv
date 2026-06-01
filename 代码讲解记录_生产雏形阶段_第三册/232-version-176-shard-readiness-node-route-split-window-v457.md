# mini-kv v176: Node v457 manual sandbox connection 窗口追平

本版目标是把 Node v450-v457 的 managed audit local/sandbox adapter 和 manual sandbox connection route group split 纳入 mini-kv 的只读兼容窗口。它不是新的执行入口，不启动 Java 或 mini-kv 服务，不安装 active router，不放开写路由，不执行 LOAD / RESTORE / COMPACT，也不改变 mini-kv 的证据消费者边界。

## 版本角色

v176 先冻结 v175 为 `fixtures/release/shard-readiness-v175.json`，再把 `nodeRouteSplitCompatibilityWindow` 从 Node v433-v449 扩展到 Node v433-v457。Node v457 计划明确建议 mini-kv 将窗口从 v449 延伸到 v450-v457；这些 Node 版本都是 route-registration extraction，不新增 evidence gate，不改 API path，不启动 sibling 服务。

## 核心实现

`src/shard_readiness_node_compatibility.cpp` 仍是 route split window 的集中元数据入口。v176 更新窗口 mode、source plan、frozen release、frozen fixture、window end 和 frozen digest，并把 `Node v450` 到 `Node v457` 加入 shared vector。窗口 JSON 和 audit JSON 的版本数量继续由 shared vector 推导，因此 `splitRouteGroupCount`、`expectedWindowVersionCount` 和 `observedWindowVersionCount` 同步变为 25。

`src/shard_readiness.cpp` 同步版本组合：`releaseVersion=v176`，`fixtureParity` 增加 v175，`archiveCompatibility` 增加 Node v450-v457，`historicalFallback` 指向 v175，并新增 v450-v457 的 preserved flags。字段只描述只读证据链，不会触发 runtime probe、service startup 或 router/write 行为。

`src/shard_readiness_boundary_fields.cpp` 只同步 source plan 和 frozen source，catalog 继续保持 `read-only-boundary-fields.v9`、420 fields、22 groups。

## 测试保护

`tests/shard_readiness_tests.cpp` 覆盖 rolling fixture、冻结 v175 fixture、Node v433-v457 窗口、25 个版本计数、v175 digest baseline，以及 no router/no write/no execution 边界。

`tests/command_tests.cpp` 从命令处理器路径验证 `SHARDJSON`、`EXPLAINJSON` 和 `CHECKJSON` 仍将它作为 read-only metadata command，不把窗口证据变成写命令或执行命令。

## 后续价值

v176 让 mini-kv 的 route split compatibility window 追到 Node v457，同时保持版本化冻结和边界字段稳定，为后续 Node 完成 managed audit/manual sandbox connection 路由瘦身提供可消费的 C++ 侧证据。
