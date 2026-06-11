# mini-kv v1506-v1525 代码讲解：blocked execution rehearsal split non-participation

## 本版目标和边界

本版把 Node 计划 `docs/plans3/v1968-v1982-blocked-execution-rehearsal-split-roadmap.md` 对应的 blocked execution rehearsal 拆分进度，固化为 mini-kv 侧的只读非参与证据。mini-kv 不是这轮 Node 拆分的执行者，也不消费实时 Node/Java 输出；它只记录控制面可读的边界事实：Node 已经把 public entrypoint、types、constants、references、core、policy、renderer、loader 等职责拆开，而 mini-kv 不导入这些模块、不运行这些检查、不启动任何服务。

这不是新的执行入口，不是 blocked execution rehearsal 的 C++ 实现，不会读取 endpoint、credential、历史 receipt、Java v90 文件或 mini-kv v99 WAL regression 文件，也不会回放 WAL、安装 router、写 store、触碰 WAL 或打开 managed audit connection。v1505 的 disabled fake harness contract upstream echo verification split non-participation 证据是本版冻结来源；v1506-v1525 是在它之后追加的一条版本化、只读、可回放的证据链。

## 模块拆分

`include/minikv/shard_preview_blocked_execution_rehearsal_split_non_participation.hpp` 和对应 `src/...cpp` 是主入口。它负责输出 `SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON` 的完整 JSON，并把本版挂进 `SHARDJSON` 的 rolling current。这里最关键的字段是 `sourceNodeBlockedExecutionRehearsalSplitCloseoutReleaseVersion:"Node v1982"`、`sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion:"v1505"`、`blockedExecutionRehearsalSplitNonParticipationReleaseVersion:"v1525"` 和 `blockedExecutionRehearsalSplitNonParticipationChainComplete:true`。

`shard_preview_blocked_execution_rehearsal_split_non_participation_stages.*` 保存 20 个版本阶段。v1506-v1520 对应 Node v1968-v1982 的拆分观察点，v1521-v1525 是 mini-kv 自己补上的硬边界：不解析 v99 WAL regression、不扫描 Java v90、不执行下游 intake、命令/fixture smoke、clean workspace/CI closeout。每个 stage 都带 `sourceFrozenReleaseVersion` 和 `sourceFrozenFixturePath`，保证历史 fixture 是一版一冻。

`shard_preview_blocked_execution_rehearsal_split_non_participation_checks.*` 保存 20 个检查项。check code 刻意对齐 stage 语义，例如 `java-v90-reference-no-scan`、`minikv-v99-wal-regression-no-replay`、`downstream-no-execution-lock`。每条检查都重复输出核心 false 字段，让局部 check 被单独消费时也能证明边界没有被打开。

`shard_preview_blocked_execution_rehearsal_split_non_participation_validation.*` 聚合来源、计划和计数。它要求 v1505 来源 stage count 为 15 且 complete，Node 不要求 fresh mini-kv evidence，mini-kv 没有导入 Node modules、没有执行 Node checks、没有启动服务、没有读 endpoint/credential、没有解析历史 receipt、没有扫描 Java evidence、没有 replay WAL regression，并且 published/planned/check count 全部对齐到 20。

## 命令和 SHARDJSON 接线

`src/command.cpp` 增加新命令的 dispatch、HELP 和 standalone 输出。`src/command_contracts.cpp` 把它登记成 read、no mutate、no WAL、stable 的命令。`src/line_editor.cpp` 增加补全项。`src/shard_readiness.cpp` 把 rolling release 推到 v1525，并把本版 digest marker 和 JSON section 合入根 `SHARDJSON`。

控制面读到这些字段时应这样理解：

- `readOnly:true` 和 `executionAllowed:false` 表示本版只提供证据，不提供执行能力。
- `miniKvImportsNodeModules:false`、`miniKvExecutesNodeChecks:false`、`miniKvStartsServices:false` 表示 C++ 没有跨到 Node 的运行时边界。
- `miniKvReadsHistoricalMiniKvV99WalRegressionFile:false` 和 `miniKvReplaysWalRegression:false` 表示 v99 只是历史引用，不是被重新解析或执行。
- `miniKvScansJavaV90ContextNormalizationEvidence:false` 表示 Java v90 只是计划中的冻结证据名，不是 mini-kv 去扫描 Java 项目。
- `activeRouterInstalled:false`、`writeRoutingAllowed:false`、`touchesWal:false` 表示没有打开 shard router、写路由或 WAL 权限。

## Fixture 和测试

`fixtures/release/shard-readiness-v1506.json` 到 `shard-readiness-v1525.json` 是本版的版本化 fixture 链，`fixtures/release/shard-readiness.json` 同步为 v1525 rolling current。历史 v1506-v1524 fixture 会裁剪 stage/check 数量并保留未完成状态；v1525 是真实 CLI 输出，作为最终 current。

新增测试分五层：stage 测试证明 v1506-v1525 连续且 source freeze 连续；check 测试证明 20 个检查和所有 false 边界字段存在；validation 测试证明好路径通过、来源不完整/fresh evidence/导入/服务/endpoint/credential/receipt/Java/WAL/count 任一打开都会失败；core 测试证明 standalone JSON 的合同、来源、边界和 status；fixture 测试证明 v1506-v1525 历史 fixture 和 rolling current 都包含本版段落。现有 `command_tests`、`line_editor_tests`、`shard_readiness_tests` 也同步覆盖命令目录、补全和 SHARDJSON。

本版验证顺序是：CMake build，目标 CTest，全量 296 条 CTest，CLI smoke，TCP smoke。TCP smoke 只发送 `COMMANDS` 和 standalone 新命令，避免把超长 `SHARDJSON` 塞进单行客户端；这仍然证明新命令在真实 TCP 读路径可见。

一句话总结：v1506-v1525 给 Node blocked execution rehearsal 拆分增加了一条 mini-kv 可读、可归档、不可执行的冻结证据链，后续 Node/Java/mini-kv 融合可以引用它，但不能借它打开服务、路由、写入或 WAL 执行权限。
