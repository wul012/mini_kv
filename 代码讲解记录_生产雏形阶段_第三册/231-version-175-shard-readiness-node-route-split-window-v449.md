# mini-kv v175: Node v449 adapter implementation 窗口证据

本版目标是把 Node v449 的 managed audit adapter implementation route group split 纳入 mini-kv 的只读兼容窗口。它不是新的执行入口，不启动 Java 或 mini-kv 服务，不安装 active router，不放开写路由，不执行 LOAD / RESTORE / COMPACT，也不改变 mini-kv 的证据消费者边界。

## 版本角色

v175 先冻结 v174 为 `fixtures/release/shard-readiness-v174.json`，再把 `nodeRouteSplitCompatibilityWindow` 从 Node v433-v448 扩展到 Node v433-v449。Node v449 在计划中只抽出 managed audit adapter implementation precheck packet 和 disabled shell audit 的 JSON/Markdown route registrations，不新增 evidence gate，不改 API path，不启动 sibling 服务。

## 核心实现

`src/shard_readiness_node_compatibility.cpp` 仍是 route split window 的集中元数据入口。v175 只更新窗口 mode、source plan、frozen release、frozen fixture、window end 和 frozen digest，并把 `Node v449` 加入 shared vector。窗口 JSON 和 audit JSON 的版本数量继续由 shared vector 推导，因此 `splitRouteGroupCount`、`expectedWindowVersionCount` 和 `observedWindowVersionCount` 同步变为 17。

`src/shard_readiness.cpp` 同步版本组合：`releaseVersion=v175`，`fixtureParity` 增加 v174，`archiveCompatibility` 增加 Node v449，`historicalFallback` 指向 v174，并新增 `nodeV449ManagedAuditAdapterImplementationRouteSplitPreserved`。这些字段只描述只读证据链，不会触发 runtime probe、service startup 或 router/write 行为。

`src/shard_readiness_boundary_fields.cpp` 只同步 source plan 和 frozen source，catalog 继续保持 `read-only-boundary-fields.v9`、420 fields、22 groups。

## 测试保护

`tests/shard_readiness_tests.cpp` 覆盖 rolling fixture、冻结 v174 fixture、Node v433-v449 窗口、17 个版本计数、v174 digest baseline，以及 no router/no write/no execution 边界。

`tests/command_tests.cpp` 从命令处理器路径验证 `SHARDJSON`、`EXPLAINJSON` 和 `CHECKJSON` 仍将它作为 read-only metadata command，不把窗口证据变成写命令或执行命令。

## 后续价值

v175 让 mini-kv 的 route split compatibility window 追到 Node v449，同时保持版本化冻结和边界字段稳定，为后续 Node 继续拆 managed audit adapter 路由组提供可消费的 C++ 侧证据。
