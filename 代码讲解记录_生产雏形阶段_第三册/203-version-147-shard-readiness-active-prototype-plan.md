# mini-kv v147: shard-readiness active prototype prerequisite plan

v147 的目标接在 Node v378 之后：Node 已消费并冻结 mini-kv v146 的 shard readiness evidence，mini-kv 这边继续向前滚 current 证据，但不能让“active shard prototype”听起来像已经可以执行。于是本版先冻结 `fixtures/release/shard-readiness-v146.json`，再给 current `SHARDJSON` 增加 `activePrototypePlan` 前置计划字段。

本版不是什么：不是 active shard router，不是多目录存储，不是多进程服务编排，不是 write routing，不是 Java/Node 的执行授权，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，不写订单或审计状态，也不让 mini-kv 成为业务权威。

`src/shard_readiness.cpp` 仍然只向外暴露 `fixture_path()` 和 `format_json()`。内部继续按小 formatter 聚合 JSON：`format_fixture_parity_json()` 把历史路径扩展到 v144/v145/v146，`format_archive_compatibility_json()` 把 Node archived evidence 扩展到 v370-v378，`format_historical_fallback_json()` 指向 frozen v146，而新增的 `format_active_prototype_plan_json()` 专门表达 active shard prototype 仍是 prerequisite-only。

`activePrototypePlan` 的核心字段是给控制面读的：`activeShardPrototypeAllowed=false`、`routerActivationAllowed=false`、`shardDirectoryCreationAllowed=false`、`multiProcessStartAllowed=false`、`writeRoutingAllowed=false`。`requiredBeforeActivation` 列出后续若要真正进入 active prototype，必须先有独立计划、明确服务启动和清理责任、Java 只读 echo、Node consumer gate，以及 fail-closed smoke target。

current `SHARDJSON` 现在是 `releaseVersion=v147`、`status=active-prototype-prerequisite-read-only`。它仍然声明 `readOnly=true`、`executionAllowed=false`、`shardEnabled=false`、`shardCount=1`、`slotCount=16`，样例路由仍只指向 `shard-0`。这说明 slot table 还是 evidence，不是线上 routing table。

`readOnlyBoundaryFields` 新增了三条 active prototype 边界：`activePrototypePlan.activeShardPrototypeAllowed`、`activePrototypePlan.routerActivationAllowed`、`activePrototypePlan.shardDirectoryCreationAllowed`。这些字段方便 Node / Java 后续只读消费时明确断言“计划字段存在，但执行仍关闭”。

`fixtures/release/shard-readiness-v146.json` 保存了 v146 被 Node v378 消费时的完整 JSON，包括 `status=historical-fallback-hardened-read-only` 和 `evidenceDigest=fnv1a64:6847d87decb76fcb`。current v147 的 digest 是 `fnv1a64:e4a386fc9add4eaf`，测试会防止 frozen v146 与 rolling current 混用。

`tests/shard_readiness_tests.cpp` 同时保护 current v147、v144 historical prototype、v145 hardening fixture 和 v146 historical fallback fixture。`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径确认 `SHARDJSON` 暴露 v147、activePrototypePlan 和只读边界。测试仍是字符串断言风格，但本版范围很窄，字段风险集中在 release evidence JSON 的可见性和兼容性。

验证闭环包括 CMake 配置、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6417 127.0.0.1`，用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`，确认 activePrototypePlan 走真实命令路径可见；服务端验证后停止，端口无残留。截图和说明归档在 `e/147/图片/` 与 `e/147/解释/说明.md`。

一句话总结：v147 把 shard readiness 从“冻结历史 fallback”推进到“active prototype 前置计划可见”，但仍把所有分片执行能力关在只读证据边界之外。
