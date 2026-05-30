# mini-kv v155: shard-readiness approval gate input precheck

v155 的目标是承接 Node v397-v399 的结论：mini-kv v154 已被 Node v397 接受为 candidate-only contribution，但 Node v398/v399 又明确证明 approval gate 仍然缺三项输入。mini-kv 这一版不进入 live integration，而是先把 v154 candidate 冻结成 `fixtures/release/shard-readiness-v154.json`，再在当前 `SHARDJSON` 中声明 approval-gate input precheck 仍然 blocked。

本版不是什么：不是 Node-approved runtime window，不是 correlated operator approval record，不是 complete cross-project runtime execution packet，不是 runtime live-read gate，也不是 cleanup proof。它不启动 Java，不启动 mini-kv candidate port `6424`，不运行 runtime probe，不创建 shard 目录，不安装 active router，不写存储，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不让 mini-kv 成为 order 或 audit authority。

## 实现入口

`src/shard_readiness.cpp` 仍是 `SHARDJSON` 的实现入口。v155 把 `releaseVersion` 更新为 `v155`，把 `status` 更新为 `runtime-execution-approval-gate-input-precheck-blocked-read-only`，并把历史 fixture 列表扩展到 `fixtures/release/shard-readiness-v154.json`。

`archiveCompatibility` 现在保留到 Node v399，`futureNodeConsumer` 写成 Node v400 或更晚版本只有在所有 approval gate input 同时存在时才能继续。`historicalFallback` 指向 v154 frozen fixture，并记录 Node v397 contribution review、Node v398 approval gate review、Node v399 archive verification 都只是历史 review/archive 上下文，不是 approval。

## 新字段

`miniKvRuntimeExecutionArtifactCandidateFreeze` 冻结 v154 的 candidate-only contribution。它保留候选行数量 4、candidate port `6424`，同时冻结 `frozenAcceptedRuntimeExecutionArtifactCount=0`、`frozenMissingAcceptedRuntimeExecutionArtifactCount=6`、`frozenRuntimeExecutionPacketExecutable=false`、`frozenStartsMiniKvService=false`、`frozenRuntimeProbeAllowed=false`、`frozenLiveReadAllowed=false`、`frozenRouterActivationAllowed=false`、`frozenWriteRoutingAllowed=false` 和 `frozenExecutionAllowed=false`。

`runtimeExecutionApprovalGateInputPrecheck` 是本版新增的控制面字段。它的 `precheckMode` 是 `blocked-missing-approval-gate-inputs`，并把三项缺口写成 `approvalGateInputCount=0`、`missingApprovalGateInputCount=3`。三项 required input 分别是 Node-approved runtime window、correlated operator approval record、complete cross-project runtime execution packet。

控制面读到这个对象时，应理解为“可以安全停止并等待 approval inputs”，不是“可以执行”。同一对象明确给出 `nodeApprovedRuntimeWindowPresent=false`、`correlatedOperatorApprovalRecordPresent=false`、`completeCrossProjectRuntimeExecutionPacketPresent=false`、`approvalGateInputsComplete=false`、`runtimeGateApprovalPresent=false`、`readyForRuntimeExecutionPacket=false`、`runtimeExecutionPacketExecutable=false`、`concreteLoopbackPortsAssigned=false`、`executionAttempted=false`、`startsJavaService=false`、`startsMiniKvService=false`、`runtimeProbeAllowed=false`、`liveReadAllowed=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false`、`executionAllowed=false` 和 `failClosedOnMissingApprovalInputs=true`。

## Fixture

`fixtures/release/shard-readiness-v154.json` 保存 v154 的完整 mini-kv runtime execution artifact candidate，digest 是 `fnv1a64:edbe9c546c0d780a`。它是 v155 的冻结 baseline。

current `fixtures/release/shard-readiness.json` 由真实 `minikv_cli.exe` 的 stdin 命令 `SHARDJSON` 生成，v155 digest 是 `fnv1a64:edb1c4d8534461ab`。测试会验证 current 不等于历史 v144-v154 fixture，并确认 v154 candidate 只能作为冻结来源，不能被当成当前批准记录。

## 测试

`tests/shard_readiness_tests.cpp` 验证 current v155、历史 v144-v154 fixture、v154 frozen candidate、`miniKvRuntimeExecutionArtifactCandidateFreeze` 和 `runtimeExecutionApprovalGateInputPrecheck`。断言重点是 approval gate input 必须保持 0/3，runtime artifact 仍是 0/6，且 Node-approved window、operator approval、cross-project packet、concrete ports、Java start、mini-kv start、probe、live read、router、write、execution 全部为 false。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，确认命令层暴露的是同一份 v155 evidence。`CHECKJSON SHARDJSON extra` 继续证明 `SHARDJSON` 是只读元数据命令，即使带错参数也不会进入写命令或执行路径。

验证闭环包括 CMake 配置与 fixture 生成、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6426 127.0.0.1`，而不是候选端口 `6424`；随后用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`。日志证明 precheck 字段可通过真实服务路径读取，同时 `6424` 全程无 LISTENING socket，`6426` 验证结束后也被清理。

一句话总结：v155 把“approval gate 三项输入仍缺失”做成可版本化、可测试、可回放的 C++ evidence，让后续 Node/Java/mini-kv 继续只在完整审批输入出现后才可能进入 runtime。
