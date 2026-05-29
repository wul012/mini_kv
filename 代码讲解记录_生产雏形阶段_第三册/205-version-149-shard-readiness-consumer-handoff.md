# mini-kv v149: shard-readiness frozen evidence handoff

v149 的目标是把 v148 的 `activePrototypePlanFreeze` 证据从滚动 current 中冻结出来，形成 `fixtures/release/shard-readiness-v148.json`，再把当前 `SHARDJSON` 推进到一个更适合下游消费的 handoff 版本。它回答的是一个很具体的问题：Node/Java 接下来如果要读 mini-kv 的分片就绪证据，应读哪个稳定文件、校验哪些边界、遇到缺失证据时如何失败。

本版不是什么：不是 active shard router，不是 write routing，不是多进程服务编排，不是 live read gate 的执行版，也不是恢复、压缩、加载或订单/审计权威。它没有创建分片目录，没有安装活跃路由，没有写存储，没有执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也没有修改 Node 归档。

## 实现入口

`src/shard_readiness.cpp` 仍然维持 `fixture_path()` 和 `format_json()` 两个窄出口。v149 只沿着既有 formatter 继续扩展证据结构：`releaseVersion` 更新为 `v149`，`status` 更新为 `frozen-evidence-handoff-read-only`，`fixtureParity.historicalFixturePaths` 扩到 v144-v148，`historicalFallback.previousConsumedFixturePath` 指向 `fixtures/release/shard-readiness-v148.json`。

新增的核心字段是 `consumerHandoff`。它声明 `handoffMode=frozen-evidence-only`、`frozenReleaseVersion=v148`、`readyForNodeConsumption=true`，但同时把 `startsServices`、`activeShardPrototypeEnabled`、`routerActivationAllowed`、`writeRoutingAllowed`、`executionAllowed` 全部固定为 false。这让控制面读到 v149 后能清楚地区分两件事：证据可消费，运行能力没有打开。

`activePrototypePlanFreeze` 也向前滚动一格：它现在冻结的是 v148，路径为 `fixtures/release/shard-readiness-v148.json`，状态为 `active-prototype-plan-frozen-read-only`。v148 自身仍保留 v147 frozen baseline，因此证据链是版本化展开的，不依赖滚动 current 回看历史含义。

## Fixture

`fixtures/release/shard-readiness-v148.json` 保存 v148 的完整证据，包括 `activePrototypePlanFreeze.frozenReleaseVersion=v147`、`frozenRouterActivationAllowed=false`、`frozenWriteRoutingAllowed=false` 和 digest `fnv1a64:8270eeebbf1a7f29`。v149 当前 fixture 则新增 `consumerHandoff`，并生成 digest `fnv1a64:521fe6dee47f7f2c`。

这次的关键不是换一个状态字符串，而是把“下游如何安全消费”写成字段：读取版本化 fixture，不读未冻结 current；校验 release/status；校验 `readOnly=true` 和 `executionAllowed=false`；校验 no router 和 no write routing；冻结证据缺失时 fail closed。

## 测试

`tests/shard_readiness_tests.cpp` 同时保护 current v149、历史 v144-v148 fixture、v148 frozen baseline 和 `consumerHandoff` 字段。它确认 current 与 v144-v148 都不同，避免把滚动 current 当成历史证据；也确认 frozen v148 仍是只读、无 router、无 write routing、无执行授权。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，验证命令层暴露的不是另一份手写 fixture，而是同一套 v149 证据：`consumerHandoff` 存在且保持 startsServices/routerActivation/writeRouting/execution 全 false，同时 `CHECKJSON SHARDJSON extra` 继续证明这个命令不会被当作可执行写命令。

验证闭环包括 CMake 配置、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6419 127.0.0.1`，用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`，确认 v149 字段通过真实服务路径可见；验证后服务已停止，端口无 LISTENING 残留。

一句话总结：v149 把 v148 active-plan freeze 证据变成稳定 handoff 基线，让下游可以消费冻结文件，同时继续把 router、write path、service start 和 execution 全部挡在只读证据边界之外。
