# mini-kv v150: shard-readiness live-read gate prerequisite plan

v150 的目标是把 v149 的 `consumerHandoff` 证据冻结成 `fixtures/release/shard-readiness-v149.json`，然后在当前 `SHARDJSON` 中补一层 live-read gate 的前置计划证据。这个版本回应 Node v383 的暂停条件：后续如果要走 live read，必须先有服务 owner、端口、smoke target、fail-closed 和 cleanup 的明确计划；mini-kv 这一版只把这些要求结构化展示出来。

本版不是什么：不是 live-read gate 的执行版，不是 active shard router，不是 runtime probe，不是 write routing，也不是多进程分片服务。它没有创建分片目录，没有安装活跃路由，没有写存储，没有执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也没有让 mini-kv 成为订单或审计权威。

## 实现入口

`src/shard_readiness.cpp` 仍然保持窄出口：`fixture_path()` 返回 rolling current fixture 路径，`format_json()` 生成完整证据 JSON。v150 将 `releaseVersion` 更新为 `v150`，`status` 更新为 `live-read-gate-prerequisite-read-only`，并把历史 fixture 列表扩到 v144-v149。

`historicalFallback` 现在指向 `fixtures/release/shard-readiness-v149.json`。这说明后续控制面需要回看 v149 handoff 时，应读版本化冻结文件，而不是读滚动 current。`archiveCompatibility` 同步扩展到 Node v370-v383，表达 v150 不修改已有 Node 归档语义。

## 新字段

`consumerHandoff` 继续保留 frozen-evidence-only 语义，但本版把 `frozenReleaseVersion` 滚到 v149。它仍然声明 `startsServices=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`executionAllowed=false`，所以“可消费”并不等于“可运行”。

新增字段是 `liveReadGatePlan`。它的 `planMode` 是 `service-lifecycle-prerequisite-only`，并把 `liveReadGateAllowed`、`runtimeProbeAllowed`、`startsServices`、`routerActivationAllowed`、`writeRoutingAllowed`、`executionAllowed` 全部固定为 false。同时它要求后续计划必须提供 service owner、startup command、port list、read-only smoke target、fail-closed missing evidence behavior 和 cleanup responsibility。

`readOnlyBoundaryFields` 也新增了 `liveReadGatePlan.*` 的关键 false 字段。这样下游消费端不需要靠阅读说明文字来判断边界，而是可以直接验证结构化字段。

## Fixture

`fixtures/release/shard-readiness-v149.json` 保存 v149 的完整 handoff 证据，包括 `status=frozen-evidence-handoff-read-only`、`activePrototypePlanFreeze.frozenReleaseVersion=v148`、`consumerHandoff.handoffMode=frozen-evidence-only` 和 digest `fnv1a64:521fe6dee47f7f2c`。

current `fixtures/release/shard-readiness.json` 由真实 `minikv_cli.exe` 的 `SHARDJSON` 输出生成，v150 digest 为 `fnv1a64:b8b134f6aa527ca4`。这保证 fixture 与运行时 formatter 一致，而不是手写两份相似 JSON。

## 测试

`tests/shard_readiness_tests.cpp` 同时验证 current v150、历史 v144-v149 fixture、v149 frozen handoff 和 `liveReadGatePlan` 字段。重点断言是 current 不等于任何历史 fixture，且 live-read gate 相关执行字段全部为 false。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，确认命令层输出 v150、`consumerHandoff` 和 `liveReadGatePlan`。`CHECKJSON SHARDJSON extra` 继续证明 `SHARDJSON` 不是写命令，也不会因为多余参数进入执行路径。

验证闭环包括 CMake 配置、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6420 127.0.0.1`，用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`，确认 v150 字段通过真实服务路径可见；验证后服务已停止，端口无 LISTENING 残留。

一句话总结：v150 把 live-read gate 的服务生命周期要求变成可验证的只读证据，同时继续把真正的启动、探测、router、write path 和 execution 全部关在边界之外。
