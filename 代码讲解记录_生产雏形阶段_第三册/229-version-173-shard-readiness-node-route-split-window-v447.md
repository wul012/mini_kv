# mini-kv v173：Node v447 dry-run adapter 窗口证据

本版目标是把 Node v447 的 managed audit dry-run adapter route group split 纳入 mini-kv 的只读兼容窗口。它不是新的执行入口，不启动 Java 或 mini-kv 服务，不安装 active router，不放开写路由，不执行 LOAD / RESTORE / COMPACT，也不改变 mini-kv 的证据消费者边界。

## 版本角色

v173 先冻结 v172 为 `fixtures/release/shard-readiness-v172.json`，再把 `nodeRouteSplitCompatibilityWindow` 从 Node v433-v446 扩展到 Node v433-v447。Node v447 在计划中只抽出 dry-run adapter、archive verification 和 production-hardening readiness gate 的 JSON/Markdown route registrations，不新增 evidence gate，不改 API path，不启动 sibling 服务。

## 核心实现

`src/shard_readiness_node_compatibility.cpp` 继续沿用集中化的 route split window 元数据常量。v173 只更新窗口 mode、source plan、frozen release、frozen fixture、window end 和 frozen digest，并把 `Node v447` 加入 shared vector。窗口 JSON 和 audit JSON 的版本数量都由 shared vector 推导，因此 `splitRouteGroupCount`、`expectedWindowVersionCount` 和 `observedWindowVersionCount` 同步变为 15。

`src/shard_readiness.cpp` 同步版本组合：`releaseVersion=v173`，`fixtureParity` 增加 v172，`archiveCompatibility` 增加 Node v447，`historicalFallback` 指向 v172，并新增 `nodeV447ManagedAuditDryRunAdapterRouteSplitPreserved`。这些字段只描述只读证据链，不会触发 runtime probe、service startup 或 router/write 行为。

`src/shard_readiness_boundary_fields.cpp` 只同步 source plan 和 frozen source，catalog 继续保持 `read-only-boundary-fields.v9`、420 fields、22 groups。

## 测试保护

`tests/shard_readiness_tests.cpp` 覆盖 rolling fixture、冻结 v172 fixture、Node v433-v447 窗口、15 个版本计数、v172 digest baseline，以及 no router/no write/no execution 边界。

`tests/command_tests.cpp` 从命令处理器路径验证 `SHARDJSON`、`EXPLAINJSON` 和 `CHECKJSON` 仍将它作为 read-only metadata command，不把窗口证据变成写命令或执行命令。

## 后续价值

v173 让 mini-kv 的 route split compatibility window 追到 Node v447，同时保持版本化冻结和边界字段稳定，为后续 Node 路由表继续瘦身提供可消费的 C++ 侧证据。
