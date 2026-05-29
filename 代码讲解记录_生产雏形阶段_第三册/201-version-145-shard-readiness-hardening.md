# mini-kv v145: SHARDJSON 只读 hardening

v145 的目标是沿着 Node v373 之后的新口径，继续并行加固 mini-kv 的 shard readiness 证据。Node v374 负责收束 v370-v373 归档链；mini-kv 这边不等待它串行批准，而是在不改 Node 已归档证据的前提下，产出新的 v145 只读证据，后续 Node 版本再选择消费。

本版不是什么：不是分片上线，不是 active router，不是多目录存储，不是多进程拓扑，不是写路径改造，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，不写订单或审计状态，也不把 mini-kv 变成业务权威。

`src/shard_readiness.cpp` 是核心改动。v144 的 `format_json()` 已经能输出基本字段，但继续追加字段会让它变成难维护的长字符串，所以 v145 把内部 formatter 拆成 `format_shard_map_json()`、`format_boundaries_json()`、`format_diagnostics_json()`、`format_command_catalog_json()`、`format_fixture_parity_json()` 和 `format_archive_compatibility_json()`。外部接口仍然只有 `fixture_path()` 和 `format_json()`，不会扩大调用面。

新的 `SHARDJSON` 仍然保留 `contract=shard-readiness.v1`、`shardEnabled=false`、`shardCount=1`、`slotCount=16` 和三条样例 key 路由，但 `releaseVersion` 滚到 `v145`，`status` 变为 `hardened-read-only`。这表示它是同一契约的加固版，不是实际路由能力上线。

控制面最该看的新增字段有四组。`commandCatalog` 明确 `SHARDJSON` 是 `read` 命令，`mutatesStore=false`、`touchesWal=false`、`extraArgsAllowed=false`、`sideEffects=["metadata_read"]`。`fixtureParity` 明确当前 runtime 输出要等于 `fixtures/release/shard-readiness.json`，并保留 `fixtures/release/shard-readiness-v144.json` 作为历史证据。`archiveCompatibility` 明确 v145 不改 Node v370、v371、v372、v373 已归档证据。`readOnlyBoundaryFields` 列出下游最应该做防回归检查的边界字段。

`fixtures/release/shard-readiness.json` 是当前 v145 fixture，`fixtures/release/shard-readiness-v144.json` 是 v144 历史快照。这样做的好处是后续 Node 可以消费新证据，但回看 v144 时不用从 git 历史里捞旧 JSON；同时也避免把“当前证据滚动”误解成“重写 Node v370-v373 归档链”。

`tests/shard_readiness_tests.cpp` 现在覆盖三条线：当前 fixture 必须等于 runtime `format_json()`；历史 v144 fixture 必须保留且 digest 不变；真实 `CommandProcessor` 对 `SHARDJSON`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON` 和 `CHECKJSON SHARDJSON extra` 的返回都必须符合只读语义。`tests/command_tests.cpp` 则补充主命令回归，确保 `COMMANDS/COMMANDSJSON/HELP` 仍能暴露 `SHARDJSON`，并且响应里出现 v145 hardening 字段。

验证闭环包括定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 用 `minikv_client` 读取 `COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 和 `SHARDJSON`，证明 v145 字段走真实命令路径可见；服务端 smoke 后立即停止，没有端口残留。截图和说明归档在 `e/145/图片/` 与 `e/145/解释/说明.md`。

一句话总结：v145 让 mini-kv 的 shard readiness 证据更适合后续控制面消费，同时继续把写入、执行、分片上线和 Node 归档链重写这些边界锁住。
