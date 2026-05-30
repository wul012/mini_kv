# mini-kv v156: shard-readiness final mini-kv approval input

v156 的目标是承接 Node v400 的输入契约：Node 已经把下一步拆成 owner-by-owner handoff，并明确 mini-kv v155 只是 precheck-only。mini-kv 这一版把 v155 precheck 冻结成 `fixtures/release/shard-readiness-v155.json`，再在当前 `SHARDJSON` 中声明 mini-kv 本侧 final approval gate input 已完整。

本版不是什么：不是 Node-approved runtime window，不是 correlated operator approval record，不是 complete cross-project runtime execution packet，不是 runtime approval，也不是 cleanup proof after approved start。它不启动 Java，不启动 mini-kv 声明端口 `6424`，不运行 runtime probe，不创建 shard 目录，不安装 active router，不写存储，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不让 mini-kv 成为 order 或 audit authority。

## 实现入口

`src/shard_readiness.cpp` 仍是 `SHARDJSON` 的实现入口。v156 把 `releaseVersion` 更新为 `v156`，把 `status` 更新为 `mini-kv-final-approval-gate-input-no-runtime-read-only`，并把历史 fixture 列表扩展到 `fixtures/release/shard-readiness-v155.json`。

`archiveCompatibility` 现在保留到 Node v400，`futureNodeConsumer` 写成 Node v401 或更晚版本只有在 final mini-kv input、Node runtime window、correlated approval 和 cross-project packet 同时存在时才能继续。`historicalFallback` 指向 v155 frozen fixture，并记录 Node v400 只把 v155 当作 precheck-only input contract evidence。

## 新字段

`runtimeExecutionApprovalGateInputPrecheckFreeze` 冻结 v155 的 0/3 approval-gate precheck。它保留 `frozenApprovalGateInputCount=0`、`frozenMissingApprovalGateInputCount=3`、`frozenNodeApprovedRuntimeWindowPresent=false`、`frozenCorrelatedOperatorApprovalRecordPresent=false`、`frozenCompleteCrossProjectRuntimeExecutionPacketPresent=false`、`frozenRuntimeExecutionPacketExecutable=false`、`frozenStartsJavaService=false`、`frozenStartsMiniKvService=false`、`frozenRuntimeProbeAllowed=false`、`frozenLiveReadAllowed=false`、`frozenRouterActivationAllowed=false`、`frozenWriteRoutingAllowed=false` 和 `frozenExecutionAllowed=false`。

`miniKvFinalApprovalGateInput` 是本版新增的控制面字段。它的 `inputMode` 是 `mini-kv-final-approval-gate-input-no-runtime`，并把 mini-kv 本侧四项输入写成完整：concrete mini-kv loopback port assignment、GET-only SHARDJSON smoke command、mini-kv service owner confirmation、stop-only-owned-process cleanup rules。具体端口是 `127.0.0.1:6424`，启动命令形状是 `minikv_server.exe 6424 127.0.0.1`，读取命令是 `minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream`，owner handle 是 `mini-kv-local-operator`。

控制面读到这个对象时，应理解为“mini-kv 本侧输入可供 intake”，不是“全局 runtime 可以执行”。同一对象明确给出 `cleanupProofPresent=false`、`cleanupProofRequiresApprovedRuntimeStart=true`、`nodeApprovedRuntimeWindowPresent=false`、`correlatedOperatorApprovalRecordPresent=false`、`completeCrossProjectRuntimeExecutionPacketPresent=false`、`runtimeGateApprovalPresent=false`、`readyForRuntimeExecutionPacket=false`、`runtimeExecutionPacketExecutable=false`、`concreteLoopbackPortsAssignedByCrossProjectPacket=false`、`executionAttempted=false`、`startsJavaService=false`、`startsMiniKvService=false`、`runtimeProbeAllowed=false`、`liveReadAllowed=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false` 和 `executionAllowed=false`。

## Fixture

`fixtures/release/shard-readiness-v155.json` 保存 v155 的完整 approval-gate input precheck，digest 是 `fnv1a64:edb1c4d8534461ab`。它是 v156 的冻结 baseline。

current `fixtures/release/shard-readiness.json` 由真实 `minikv_cli.exe` 的 stdin 命令 `SHARDJSON` 生成，v156 digest 是 `fnv1a64:f240d1fe1b25fab7`。测试会验证 current 不等于历史 v144-v155 fixture，并确认 v155 precheck 只能作为冻结来源，不能被当成当前 final input。

## 测试

`tests/shard_readiness_tests.cpp` 验证 current v156、历史 v144-v155 fixture、v155 frozen precheck、`runtimeExecutionApprovalGateInputPrecheckFreeze` 和 `miniKvFinalApprovalGateInput`。断言重点是 mini-kv 本侧 input 可以完整，但 Node runtime window、operator approval、cross-project packet、cleanup proof、Java start、mini-kv start、probe、live read、router、write、execution 全部为 false。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，确认命令层暴露的是同一份 v156 evidence。`CHECKJSON SHARDJSON extra` 继续证明 `SHARDJSON` 是只读元数据命令，即使带错参数也不会进入写命令或执行路径。

验证闭环包括 CMake 配置与 fixture 生成、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6427 127.0.0.1`，而不是声明端口 `6424`；随后用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`。日志证明 final input 字段可通过真实服务路径读取，同时 `6424` 全程无 LISTENING socket，`6427` 验证结束后也被清理。

一句话总结：v156 把 mini-kv 本侧最终 approval-gate input 做成可版本化、可测试、可回放的 C++ evidence，同时继续把真正的 runtime 执行锁在 Node/operator/cross-project 三方输入之后。
