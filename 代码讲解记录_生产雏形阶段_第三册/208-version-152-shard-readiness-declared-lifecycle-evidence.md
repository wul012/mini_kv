# mini-kv v152: shard-readiness declared operator lifecycle evidence

v152 的目标是接住 Node v387 之后的等待条件：Node 侧已经验证 v151 仍是 template-only，因此 mini-kv 这版把 v151 的 `operatorServiceLifecycleTemplate` 冻结为版本化 fixture，再在当前 `SHARDJSON` 中给出 declared operator lifecycle evidence。它补齐的是“声明证据”：谁负责、启动命令长什么样、端口由 operator 批准、GET-only smoke target 是什么、缺证据或端口冲突如何 fail closed、验证后谁清理。

本版不是什么：不是 runtime gate approval，不是 live-read 执行版，不是 runtime probe，不是 active shard router，也不是 write routing。`runtimeGateApproved=false` 是本版最重要的刹车字段。mini-kv 没有创建分片目录，没有安装活跃路由，没有写存储，没有执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也没有成为订单或审计权威。

## 实现入口

`src/shard_readiness.cpp` 仍然是唯一实现入口。`format_json()` 组装 `SHARDJSON`，`fixture_path()` 继续指向 rolling current fixture。v152 把 `releaseVersion` 改为 `v152`，把 `status` 改为 `declared-operator-lifecycle-no-runtime-read-only`，并把历史 fixture 列表扩展到 `fixtures/release/shard-readiness-v151.json`。

`historicalFallback` 现在指向 v151 frozen fixture，`archiveCompatibility` 扩展到 Node v370-v387。这样下游回看历史时必须读版本化文件，不会把 rolling `fixtures/release/shard-readiness.json` 当作旧版事实来源。

## 新字段

`operatorServiceLifecycleTemplateFreeze` 固定 v151 template-only 状态。它记录 `frozenReleaseVersion=v151`、`frozenFixturePath=fixtures/release/shard-readiness-v151.json`、`frozenStatus=operator-service-lifecycle-template-read-only`，并把 v151 未声明的 owner、startup command、port list、GET-only smoke target、cleanup responsibility、runtime probe 和 execution 全部固定为 false。

`operatorServiceLifecycleEvidence` 是本版新增的 declared evidence。它的 `evidenceMode` 是 `declared-lifecycle-no-runtime`，说明 v152 已经声明 operator-owned lifecycle 信息：`serviceOwnerHandle=mini-kv-local-operator`、`startupCommand=minikv_server.exe <operator-approved-loopback-port> 127.0.0.1`、`declaredPortHandles=["operator-approved-loopback-port"]`、`getOnlySmokeTarget=GET-only SHARDJSON evidence read`、`failClosedBehavior=missing evidence or port conflict blocks runtime gate`、`cleanupResponsibility=operator stops service and verifies no LISTENING port`。

控制面读到这些 true 字段时，只能理解为 lifecycle 证据形状已声明。它不能理解为可运行，因为同一对象还明确给出 `runtimeGateApproved=false`、`startsServices=false`、`runtimeProbeAllowed=false`、`liveReadAllowed=false`、`activeShardPrototypeEnabled=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`executionAllowed=false` 和 `requiresSeparateRuntimeGate=true`。

## Fixture

`fixtures/release/shard-readiness-v151.json` 保存 v151 的完整 template-only 证据，包含 `status=operator-service-lifecycle-template-read-only`、`operatorServiceLifecycleTemplate.evidenceMode=template-only-no-runtime`、`serviceOwnerDeclared=false`、`startupCommandDeclared=false`、`runtimeProbeAllowed=false`，digest 为 `fnv1a64:c9fb568ddff895e3`。

current `fixtures/release/shard-readiness.json` 由真实 `minikv_cli.exe` 的 `SHARDJSON` 输出生成，v152 digest 为 `fnv1a64:55cd5b8db109c64f`。测试会同时验证 current 不等于历史 v144-v151 fixture，防止 rolling current 被误用为冻结基线。

## 测试

`tests/shard_readiness_tests.cpp` 验证 current v152、历史 v144-v151 fixture、v151 frozen baseline、`operatorServiceLifecycleTemplateFreeze` 和 `operatorServiceLifecycleEvidence`。断言重点是 declared lifecycle 的 true 字段必须存在，同时所有 runtime gate、probe、live read、router、write、execution 字段仍保持 false。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，确认命令层暴露的是同一份 v152 evidence。`CHECKJSON SHARDJSON extra` 继续证明 `SHARDJSON` 是只读元数据命令，不会因为参数变化进入写命令或执行路径。

验证闭环包括 CMake 配置、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6422 127.0.0.1`，用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`，确认 v152 字段通过真实服务路径可见；验证后服务已停止，端口无 LISTENING 残留。

一句话总结：v152 把 operator lifecycle 从 template-only 推进到 declared evidence，但仍把 runtime/live-read/router/write/execution 全部锁在单独 gate 之后，给后续 Node/Java/mini-kv 松耦合融合留出清晰、可验证的边界。
