# mini-kv v153: shard-readiness runtime execution artifact preflight

v153 的目标是回应 Node v395 之后的状态：Node v396 只有在六项真实 runtime execution artifacts 出现时才应继续。mini-kv 这版不生成这些 artifact，而是把 v152 declared operator lifecycle evidence 冻结下来，并在当前 `SHARDJSON` 中声明一个 blocked intake preflight，明确 artifact set 仍是 0/6。

本版不是什么：不是 runtime execution packet，不是 runtime live-read gate，不是 operator approval record，不是具体端口分配，不是 runtime probe，也不是 active shard router。它没有创建分片目录，没有启动服务作为证据动作，没有写存储，没有执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也没有让 mini-kv 成为订单或审计权威。

## 实现入口

`src/shard_readiness.cpp` 仍是 `SHARDJSON` 的实现入口。v153 把 `releaseVersion` 更新为 `v153`，把 `status` 更新为 `runtime-execution-artifact-intake-preflight-blocked-read-only`，并把历史 fixture 列表扩展到 `fixtures/release/shard-readiness-v152.json`。

`historicalFallback` 现在指向 v152 frozen fixture，`archiveCompatibility` 扩展到 Node v370-v395。这表示 Node v388-v395 已归档的 declared lifecycle / runtime gate / blocked preflight 链路都只作为历史事实保留；后续 Node v396 不能读取 rolling current 当历史 baseline。

## 新字段

`operatorServiceLifecycleEvidenceFreeze` 固定 v152 的 declared lifecycle evidence。它记录 v152 已声明 owner、startup command shape、port handle、GET-only smoke target、fail-closed behavior 和 cleanup responsibility，但同时冻结 `frozenRuntimeGateApproved=false`、`frozenStartsServices=false`、`frozenRuntimeProbeAllowed=false`、`frozenLiveReadAllowed=false`、`frozenRouterActivationAllowed=false`、`frozenWriteRoutingAllowed=false`、`frozenExecutionAllowed=false`。

`runtimeExecutionArtifactIntakePreflight` 是本版新增的控制面字段。它的 `preflightMode` 是 `blocked-missing-runtime-execution-artifacts`，并把真实 artifact 状态写成 `presentRuntimeExecutionArtifactCount=0`、`missingRuntimeExecutionArtifactCount=6`。六项缺口分别是 operator approval record、concrete loopback port assignment、GET-only smoke command、cleanup proof、service owner confirmation、process cleanup rules。

控制面读到这个字段时，应理解为“可以安全停止并等待 artifact”，不是“可以执行”。同一对象明确给出 `readyForRuntimeExecutionPacket=false`、`readyForRuntimeLiveReadGate=false`、`runtimeExecutionPacketPresent=false`、`runtimeExecutionPacketExecutable=false`、`executionAttempted=false`、`startsMiniKvService=false`、`runtimeProbeAllowed=false`、`liveReadAllowed=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`executionAllowed=false` 和 `failClosedOnMissingArtifacts=true`。

## Fixture

`fixtures/release/shard-readiness-v152.json` 保存 v152 的完整 declared lifecycle evidence，digest 为 `fnv1a64:55cd5b8db109c64f`。这份文件是 v153 的冻结 baseline。

current `fixtures/release/shard-readiness.json` 由真实 `minikv_cli.exe` 的 stdin 命令 `SHARDJSON` 生成，v153 digest 为 `fnv1a64:ca09095f3f3e4b2c`。测试会验证 current 不等于历史 v144-v152 fixture，防止 rolling current 被误用为冻结证据。

## 测试

`tests/shard_readiness_tests.cpp` 验证 current v153、历史 v144-v152 fixture、v152 frozen baseline、`operatorServiceLifecycleEvidenceFreeze` 和 `runtimeExecutionArtifactIntakePreflight`。断言重点是 artifact count 必须保持 0/6，且 runtime packet、probe、live read、router、write、execution 全部保持 false。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，确认命令层暴露的是同一份 v153 evidence。`CHECKJSON SHARDJSON extra` 继续证明 `SHARDJSON` 是只读元数据命令，不会因为参数变化进入写命令或执行路径。

验证闭环包括 CMake 配置与 fixture 生成、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6423 127.0.0.1`，用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`，确认 blocked preflight 字段通过真实服务路径可见；验证后服务已停止，端口无 LISTENING 残留。

一句话总结：v153 把“缺少真实 runtime execution artifacts”从口头阻塞变成可版本化、可回放、可 fail-closed 的 C++ evidence，继续保护 Node/Java/mini-kv 的松耦合边界。
