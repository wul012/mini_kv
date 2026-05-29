# mini-kv v151: shard-readiness operator service lifecycle template

v151 的目标是把 v150 的 `liveReadGatePlan` 证据冻结成 `fixtures/release/shard-readiness-v150.json`，然后在当前 `SHARDJSON` 中补一层 operator service lifecycle template。它接住 Node v385 的暂停条件：后续如果要进入 live read，必须有 operator-owned service owner、startup command、port list、GET-only smoke target、fail-closed behavior 和 cleanup responsibility。

本版不是什么：不是 operator 已批准的运行计划，不是 live-read gate 执行版，不是 runtime probe，不是 active shard router，不是 write routing，也不是多进程分片服务。它没有创建分片目录，没有安装活跃路由，没有写存储，没有执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也没有让 mini-kv 成为订单或审计权威。

## 实现入口

`src/shard_readiness.cpp` 继续保持一个窄出口：`format_json()` 组装 `SHARDJSON`，`fixture_path()` 指向 rolling current fixture。v151 将 `releaseVersion` 更新为 `v151`，`status` 更新为 `operator-service-lifecycle-template-read-only`，并把历史 fixture 列表扩到 v144-v150。

`historicalFallback` 现在指向 `fixtures/release/shard-readiness-v150.json`。这意味着下游回看 v150 live-read gate plan 时，必须读版本化冻结文件，而不是读当前 rolling fixture。`archiveCompatibility` 同步扩展到 Node v370-v385，表达 v151 不修改既有 Node 归档。

## 新字段

`liveReadGatePlanFreeze` 是 v151 的冻结锚点。它记录 `frozenReleaseVersion=v150`、`frozenFixturePath=fixtures/release/shard-readiness-v150.json`、`frozenStatus=live-read-gate-prerequisite-read-only`，并把 `frozenLiveReadGateAllowed`、`frozenRuntimeProbeAllowed`、`frozenStartsServices`、`frozenRouterActivationAllowed`、`frozenWriteRoutingAllowed`、`frozenExecutionAllowed` 全部固定为 false。

`operatorServiceLifecycleTemplate` 是本版的新模板字段。它的 `evidenceMode` 是 `template-only-no-runtime`，明确 `operatorOwnedServiceLifecycleRequired=true`，但 `serviceOwnerDeclared`、`startupCommandDeclared`、`portListDeclared`、`getOnlySmokeTargetDeclared`、`cleanupResponsibilityDeclared` 都是 false。这一点很重要：v151 不是伪造 operator evidence，而是把后续必须补齐的 evidence 形状写清楚。

模板同时继续声明 `startsServices=false`、`runtimeProbeAllowed=false`、`liveReadAllowed=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`executionAllowed=false`。控制面读到这些字段时，应理解为“还不能运行”，不是“可以开始 live read”。

## Fixture

`fixtures/release/shard-readiness-v150.json` 保存 v150 的完整 live-read gate prerequisite 证据，包括 `status=live-read-gate-prerequisite-read-only`、`liveReadGatePlan.planMode=service-lifecycle-prerequisite-only`、`liveReadGateAllowed=false`、`runtimeProbeAllowed=false`、`startsServices=false` 和 digest `fnv1a64:b8b134f6aa527ca4`。

current `fixtures/release/shard-readiness.json` 由真实 `minikv_cli.exe` 的 `SHARDJSON` 输出生成，v151 digest 为 `fnv1a64:c9fb568ddff895e3`。测试会同时验证 current 不等于历史 v144-v150 fixture，防止滚动 current 被误用作历史基线。

## 测试

`tests/shard_readiness_tests.cpp` 验证 current v151、历史 v144-v150 fixture、v150 frozen baseline、`liveReadGatePlanFreeze` 和 `operatorServiceLifecycleTemplate`。断言重点是模板字段不能声明 owner、startup、port、GET-only smoke 或 cleanup 已存在，同时所有运行/探测/写入/执行字段保持 false。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，确认命令层暴露的是同一份 v151 证据。`CHECKJSON SHARDJSON extra` 继续证明 `SHARDJSON` 是只读元数据命令，不会因为参数变化进入写命令或执行路径。

验证闭环包括 CMake 配置、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6421 127.0.0.1`，用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`，确认 v151 字段通过真实服务路径可见；验证后服务已停止，端口无 LISTENING 残留。

一句话总结：v151 把后续 live read 所需的 operator lifecycle evidence 形状固化下来，同时清楚地证明这些运行证据尚未声明，mini-kv 仍停留在只读、无探测、无启动、无写入的边界内。
