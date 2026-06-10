# v1226-v1260 production live capture archive verification non-participation

本版的角色很明确: 不是新的执行入口, 不是绕过契约的捷径, 也不是把 mini-kv 变成验证执行器. 它做的是把 Node v1657-v1686 的 archive verification module-split 计划冻结成一条版本化证据链, 让 mini-kv 继续只读地消费证据, 不读 verification endpoint, 不导入 payload, 不开 router, 不写, 不触 WAL, 不执行.

## 目标
这一轮把 `SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON` 做成稳定的只读证据命令, 并把 `shard-readiness.json` 的 current 版本滚到 v1260. 版本链本身分两段: v1226-v1255 对齐 Node 的 30 个拆分节点, v1256-v1260 做 mini-kv 自身的命令目录、fixture 导出、CLI/TCP smoke、归档说明和 closeout 加固.

## 文件职责
`include/minikv/shard_preview_production_live_capture_archive_verification_non_participation.hpp` 暴露四个入口: 完整 JSON、stage catalog、digest marker、status.

`src/shard_preview_production_live_capture_archive_verification_non_participation.cpp` 负责把最终证据 JSON 拼起来. 这里不做 I/O, 不启动服务, 不解析外部验证结果, 只把 source freeze、current stage、module split、diagnostics 和 validation 结果串成一个读模型.

`src/shard_preview_production_live_capture_archive_verification_non_participation_stages.cpp` 维护 35 个版本 stage. 前 30 个 stage 逐个对应 Node v1657-v1686 的拆分计划, 后 5 个 stage 负责 command catalog parity、fixture export parity、smoke parity、archive explanation parity 和 closeout summary.

`src/shard_preview_production_live_capture_archive_verification_non_participation_checks.cpp` 把 35 个检查点展开成结构化记录. 这些检查点只证明边界, 不做执行. 重点是 `nodeCaptureArchiveVerificationExecuted=false`, `nodeVerificationEndpointReadByMiniKv=false`, `archiveDigestRecomputedByMiniKv=false`, `activeRouterInstalled=false`, `writeRoutingAllowed=false`, `touchesWal=false`, `executionAllowed=false`.

`src/shard_preview_production_live_capture_archive_verification_non_participation_validation.cpp` 做最终判定. 它只接受四件事: source integrity 是否已经完整冻结到 30 stage, mini-kv 是否没有开启 verification 读取, 检查数是否与 stage 数对齐, 以及当前 published stage 是否已经到 35.

`src/command.cpp`, `src/command_contracts.cpp`, `src/line_editor.cpp`, `src/shard_readiness.cpp` 负责把新命令接进运行面、目录、补全和 SHARDJSON 聚合. 新命令仍然只走 RuntimeEvidence, 不是 write command, 也不是 admin 执行入口.

## 测试
`tests/production_live_capture_archive_verification_non_participation_stages_tests.cpp` 验证 v1226-v1260 的 stage 链连续, source frozen release 从 v1225 起步, 最后一 stage 落在 v1260.

`tests/production_live_capture_archive_verification_non_participation_checks_tests.cpp` 验证 35 条检查都能落成 JSON, 并且超出范围会抛 out_of_range.

`tests/production_live_capture_archive_verification_non_participation_validation_tests.cpp` 验证 source locked、verification closed、counts aligned 三个分支的真值关系.

`tests/production_live_capture_archive_verification_non_participation_tests.cpp` 验证主 JSON 里能看到 contract、command、sourceIntegrityStatus、release range、module split、diagnostics 和最终 validation pass.

`tests/production_live_capture_archive_verification_non_participation_fixture_tests.cpp` 验证 v1226-v1260 每个版本的 fixture 都能被读出来, 并且 current `shard-readiness.json` 已经滚到 v1260.

同时, `tests/command_tests.cpp`, `tests/shard_readiness_tests.cpp`, `tests/line_editor_tests.cpp` 和几组 route preview consumer tests 也一起回归, 证明当前状态已经切换到新的 archive verification closeout, 而不是停在 v1225.

## 运行验证
这轮先做了 CMake configure/build, 然后跑了 targeted CTest 和全量 CTest. smoke 部分因为 `SHARDJSON` 输出太长会碰到 client 单行上限, 所以最后改用 `CHECKJSON SHARDROUTEPRODUCTIONLIVECAPTUREARCHIVEVERIFYNONPARTICIPATIONJSON` 做 CLI/TCP smoke. 这个选择更小, 但边界更强: 它证明新命令能被解析, 但不会被当成可执行写命令.

## 这一版留下的东西
最有价值的不是“又多了一个命令”, 而是把 Node 的 35 版 module-split 计划冻结成 mini-kv 可读、可测、可归档的版本化证据链. 以后 Node/Java/mini-kv 再分拆或再对齐时, 这条链可以直接作为只读锚点继续接下去.
