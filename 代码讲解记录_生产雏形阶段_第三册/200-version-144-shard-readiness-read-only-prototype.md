# mini-kv v144: shard-readiness 只读雏形证据

v144 的目标是按 Node 最新计划给 mini-kv 补一份 `shard-readiness.v1` 只读证据。Node v370 需要先消费分片就绪字段，而 mini-kv 本版只负责说明“如果未来要做分片，控制面现在能读到哪些稳定字段”，不把这个雏形变成真实分片执行。

本版不是什么：不是实际分片路由，不是多目录存储，不是多进程启动，不是写路径改造，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，不接管订单或审计权威，也不让 Node 通过这份证据触发 mini-kv 的写入或管理命令。

`include/minikv/shard_readiness.hpp` 是本版新的窄接口。它只暴露两个函数：`fixture_path()` 返回稳定 fixture 路径，`format_json()` 返回运行时和 fixture 共用的 JSON 字符串。把它单独成文件，是为了避免继续把新证据塞进既有 runtime evidence 大文件里，也符合“不要形成难维护巨型文件”的维护规则。

`src/shard_readiness.cpp` 是核心实现。它固定 `contract=shard-readiness.v1`、`releaseVersion=v144`、`slotCount=16`，并给出三个样例 key 到 `shard-0` 的只读路由样本。这里的 slot table 是证据，不是激活的存储路由；`storagePath` 明确为 `not-created`，`writesAllowed=false`，`shardEnabled=false`，防止控制面把它误解成已经上线的分片能力。

这份 JSON 最重要的边界字段是几组布尔值。顶层 `readOnly=true` 和 `executionAllowed=false` 表示它只能被读取，不能触发执行。`boundaries.orderAuthoritative=false`、`auditAuthoritative=false` 防止 mini-kv 被误当成业务权威。`writeCommandsAllowed=false`、`adminCommandsAllowed=false`、`loadRestoreCompactAllowed=false`、`setnxexExecutionAllowed=false` 明确挡住写命令和危险管理命令。`multiProcessStarted=false`、`storageDirectoriesCreated=false` 则说明本版没有创建运行时拓扑。

`src/command.cpp` 把 `SHARDJSON` 接进 runtime evidence 命令家族：命令表数量从 29 增到 30，`HELP` 能看到新命令，真实 TCP/inline 命令会调用 `minikv::shard_readiness::format_json()`。这里同时拒绝多余参数，所以 `SHARDJSON extra` 不会被默默接受。

`src/command_contracts.cpp` 把 `SHARDJSON` 暴露给 `COMMANDS`、`COMMANDSJSON`、`EXPLAIN` 和 `CHECK`。它的分类是 `read`，`mutates=false`，`touches_wal=false`，副作用说明为 `metadata_read`。这样控制面和测试都能从命令目录层确认它不是写命令，也不碰 WAL。

`fixtures/release/shard-readiness.json` 是静态发布证据。`tests/shard_readiness_tests.cpp` 直接断言 fixture 内容必须等于 `format_json()` 的运行时输出，同时覆盖 contract、版本、slot 数、样例路由、边界字段和 digest。这个测试的价值在于防止运行时命令和发布 fixture 悄悄漂移。

`tests/command_tests.cpp` 补了命令侧回归：`COMMANDS`、`COMMANDSJSON`、`HELP` 都必须包含 `SHARDJSON`，真实 `CommandProcessor` 调用必须返回同一组 `readOnly`、`executionAllowed`、`shardEnabled`、`slotCount` 和 `evidenceDigest` 字段。它保护的是命令入口和命令目录，不只是 JSON formatter 本身。

验证闭环包括目标构建、定向 CTest、全量 CTest 和真实 TCP smoke。定向测试 2/2 通过，全量 CTest 71/71 通过；TCP smoke 临时启动 `minikv_server`，用真实 `minikv_client` 读取 `HEALTH`、`INFOJSON`、`STATSJSON`、`SHARDJSON` 后退出，并确认服务端没有残留 LISTENING 端口。截图和说明归档在 `e/144/图片/` 与 `e/144/解释/说明.md`。

一句话总结：v144 给后续 Node/Java/mini-kv 松耦合分片准备提供了可消费的只读契约面，同时把执行、写入、目录创建、多进程和业务权威边界继续锁住。
