# mini-kv v146: shard-readiness historical fallback

v146 的目标来自 Node v376 之后的质量规则：历史证据不能继续读滚动 current 文件。Node 已经消费 mini-kv v145，所以 mini-kv 这边先把 v145 冻结成 `fixtures/release/shard-readiness-v145.json`，再把 current `fixtures/release/shard-readiness.json` 滚到 v146。这样后续 Node 做历史 fallback 时，可以读固定文件，而不是误读最新 current。

本版不是什么：不是分片上线，不是 active router，不是多目录存储，不是多进程拓扑，不改写 Node v370-v376 归档，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，不写订单或审计状态，也不让 mini-kv 变成业务权威。

`src/shard_readiness.cpp` 继续保持一个窄接口：外部只看到 `fixture_path()` 和 `format_json()`。内部把证据拆成多个小 formatter，其中 v146 新增 `format_historical_fallback_json()`，专门表达 `previousConsumedReleaseVersion=v145`、`previousConsumedFixturePath=fixtures/release/shard-readiness-v145.json`、`rollingCurrentUsedForHistoricalBaseline=false`、`nodeV376ConsumptionPreserved=true`、`nodeV377ReadsUnfinishedUpstream=false`。这些字段是给控制面看的护栏，不会触发任何执行。

current `SHARDJSON` 现在是 `releaseVersion=v146`、`status=historical-fallback-hardened-read-only`。它仍然声明 `shardEnabled=false`、`shardCount=1`、`slotCount=16`，样例路由仍然只指向 `shard-0`。这说明 slot table 仍是证据，不是上线后的活跃路由。

`fixtureParity` 从单个历史路径扩展为 `historicalFixturePaths`，同时包含 v144 和 v145。`archiveCompatibility` 扩展到 Node v370-v376，表示 v146 不改这些已经归档或消费的证据。`readOnlyBoundaryFields` 增加 `archiveCompatibility.changesArchivedNodeEvidence` 和 `historicalFallback.rollingCurrentUsedForHistoricalBaseline`，方便后续消费者把“不能改归档、不能用 current 当旧版本基线”写成明确断言。

`fixtures/release/shard-readiness-v145.json` 是本版最重要的新增文件。它保存了 v145 被 Node v376 消费时的完整 JSON，包括 `evidenceDigest=fnv1a64:ebe4c7e1a2704482`。current v146 则使用新 digest `fnv1a64:6847d87decb76fcb`，两者不同，测试会防止它们混淆。

`tests/shard_readiness_tests.cpp` 现在同时验证三份证据：current v146 必须等于 runtime `format_json()`；v144 历史 prototype fixture 必须仍保留原 digest；v145 frozen fixture 必须仍保留 v145 digest 和 hardening 字段。`tests/command_tests.cpp` 继续覆盖真实命令响应，确保 `SHARDJSON` 暴露 v146、historical fallback 和只读边界字段。

验证闭环包括定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 通过 `minikv_client` 读取 `COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 和 `SHARDJSON`，证明 v146 字段走真实命令路径可见；服务端 smoke 后停止，没有端口残留。截图和说明归档在 `e/146/图片/` 与 `e/146/解释/说明.md`。

一句话总结：v146 把 shard readiness 从“有 current 证据”推进到“历史 fallback 不会误读滚动 current”，同时继续保持只读、非执行、非权威的边界。
