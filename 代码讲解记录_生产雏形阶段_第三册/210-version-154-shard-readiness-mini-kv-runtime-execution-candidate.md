# mini-kv v154: shard-readiness mini-kv runtime execution candidate

v154 的目标是承接 Node v396 给出的六项 runtime execution artifact 缺口，但仍只推进 mini-kv / C++ 侧。上一版 v153 已经证明 runtime execution artifact intake 仍是 0/6、必须 fail closed；本版先把这份 blocked preflight 冻结成 `fixtures/release/shard-readiness-v153.json`，再在 rolling current 中声明 mini-kv 可以提供的候选行：候选 loopback 端口、GET-only smoke 命令、service owner 和 process cleanup rules。

本版不是什么：不是 runtime execution packet，不是 operator approval record，不是 Node runtime window approval，不是 Java/mini-kv 联合端口绑定，不是 cleanup proof，也不是 live-read gate。它没有把候选端口 `6424` 启动为服务，没有运行 runtime probe，没有创建 shard 目录，没有安装 active router，没有执行写命令，也没有让 mini-kv 成为 order 或 audit authority。

## 实现入口

`src/shard_readiness.cpp` 仍是 `SHARDJSON` 的唯一实现入口。v154 把 `releaseVersion` 更新为 `v154`，把 `status` 更新为 `mini-kv-runtime-execution-artifact-candidate-no-runtime-read-only`，并把历史 fixture 列表扩展到 `fixtures/release/shard-readiness-v153.json`。

`historicalFallback` 现在指向 v153 frozen fixture，说明 Node v397 或更晚版本若要继续，必须等待完整 runtime execution packet。`archiveCompatibility` 扩展到 Node v396，并把 future consumer 写成 “Node v397 or later after complete runtime execution packet”，避免把 v154 的候选声明误读成可执行授权。

## 新字段

`runtimeExecutionArtifactIntakePreflightFreeze` 冻结 v153 的 blocked preflight。它保留 `frozenPresentRuntimeExecutionArtifactCount=0`、`frozenMissingRuntimeExecutionArtifactCount=6`、`frozenReadyForRuntimeExecutionPacket=false`、`frozenRuntimeExecutionPacketExecutable=false`、`frozenExecutionAttempted=false`、`frozenStartsMiniKvService=false`、`frozenRuntimeProbeAllowed=false`、`frozenLiveReadAllowed=false`、`frozenRouterActivationAllowed=false`、`frozenWriteRoutingAllowed=false` 和 `frozenExecutionAllowed=false`。控制面读到这个对象时，应理解为“旧版阻塞证据已冻结”，不是“冻结证据可以被当作当前可执行包”。

`miniKvRuntimeExecutionArtifactCandidate` 是本版新增的候选对象。它声明 `candidateMode=mini-kv-side-candidate-no-runtime`，候选行数量为 4，分别是 mini-kv loopback port candidate、GET-only smoke command candidate、service owner candidate 和 process cleanup rules candidate。具体候选端口是 `127.0.0.1:6424`，候选启动命令是 `minikv_server.exe 6424 127.0.0.1`，候选读取命令是 `minikv_client.exe 127.0.0.1 6424 < GET-only SHARDJSON command stream`，owner handle 是 `mini-kv-local-operator`。

同一对象也把未批准状态写死：`acceptedRuntimeExecutionArtifactCount=0`、`missingAcceptedRuntimeExecutionArtifactCount=6`、`acceptedRuntimeExecutionArtifactsComplete=false`、`operatorApprovalRecordPresent=false`、`nodeRuntimeWindowApproved=false`、`crossProjectRuntimeExecutionPacketPresent=false`、`javaPortBindingPresent=false`、`miniKvLoopbackPortOperatorApproved=false`、`getOnlySmokeCommandOperatorApproved=false`、`serviceOwnerOperatorConfirmed=false`、`cleanupProofPresent=false`、`runtimeExecutionPacketExecutable=false`、`startsMiniKvService=false`、`startsServices=false`、`runtimeProbeAllowed=false`、`liveReadAllowed=false`、`routerActivationAllowed=false`、`writeRoutingAllowed=false` 和 `executionAllowed=false`。

这些字段给 Node 控制面的解释是：mini-kv 已经把本侧候选材料准备好，但它们还没有被 operator、Node runtime window 和跨项目 packet 接受；因此只能被 intake、展示和 fail-closed 判断消费，不能驱动服务启动或 live read。

## Fixture

`fixtures/release/shard-readiness-v153.json` 保存 v153 的完整 blocked preflight，digest 是 `fnv1a64:ca09095f3f3e4b2c`。它是 v154 的冻结 baseline。

current `fixtures/release/shard-readiness.json` 由真实 `minikv_cli.exe` 的 stdin 命令 `SHARDJSON` 生成，v154 digest 是 `fnv1a64:edbe9c546c0d780a`。测试会验证 current 不等于历史 v144-v153 fixture，并确认 rolling current 不能被误当作冻结证据。

## 测试

`tests/shard_readiness_tests.cpp` 验证 current v154、历史 v144-v153 fixture、v153 frozen baseline、`runtimeExecutionArtifactIntakePreflightFreeze` 和 `miniKvRuntimeExecutionArtifactCandidate`。断言重点是候选行可以存在，但 accepted artifact 仍是 0/6；operator approval、Node runtime window、cross-project packet、Java binding、cleanup proof、runtime executable、probe、live read、router、write 和 execution 全部必须保持 false。

`tests/command_tests.cpp` 从真实 `CommandProcessor` 路径读取 `SHARDJSON`，确认命令层暴露的是同一份 v154 evidence。`CHECKJSON SHARDJSON extra` 继续证明 `SHARDJSON` 是只读元数据命令，即使多传参数也不会进入写命令或执行路径。

验证闭环包括 CMake 配置与 fixture 生成、定向 CTest 2/2、全量 CTest 71/71，以及真实 TCP smoke。TCP smoke 临时启动 `minikv_server.exe 6425 127.0.0.1`，而不是候选端口 `6424`；随后用 `minikv_client.exe` 读取 `HEALTH`、`COMMANDSJSON`、`EXPLAINJSON SHARDJSON`、`CHECKJSON SHARDJSON extra` 和 `SHARDJSON`。日志证明 candidate 字段可通过真实服务路径读取，同时 `6424` 全程无 LISTENING socket，`6425` 验证结束后也被清理。

一句话总结：v154 把 mini-kv 能提供的 runtime execution packet 候选材料做成可版本化、可测试、可回放的 C++ evidence，同时继续守住 no approval、no router、no write、no execution 的松耦合边界。
