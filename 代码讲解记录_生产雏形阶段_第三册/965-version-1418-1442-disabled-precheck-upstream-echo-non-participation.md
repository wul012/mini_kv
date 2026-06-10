# mini-kv v1418-v1442 disabled precheck upstream echo non-participation

## 本版目标

v1418-v1442 的目标是把 Node v1867-v1878 的 disabled credential resolver precheck upstream echo module split，转换成 mini-kv 自己可发布、可回放、可审查的只读非参与证据。

这不是新的执行入口，不是 managed audit connection 的前置实现，也不是 C++ 开始消费 Node TypeScript 模块。Node 那边只是把一个过大的 upstream echo verification service 拆成 barrel、constants、reference builders、checks/messages、core loader 等内部模块；mini-kv 这一侧只证明：即使 Node 完成了这个拆分，C++ 仍然不 import Node module、不跑 Node check、不启动 sibling service、不读 endpoint、不解析 raw endpoint、不读 credential、不安装 active router、不写 WAL、不执行 load/restore/compact。

这个版本把上一批 v1417 的 fixture audit 作为冻结源。`sourceFixtureAuditReleaseVersion` 指向 `v1417`，`sourceFixtureAuditPublishedStageCount` 固定为 `12`，说明本批次不是重新解释历史，而是在一个已闭合的只读证据基础上继续加一层 disabled-precheck upstream echo 非参与证明。

## 模块结构

入口在 `include/minikv/shard_preview_disabled_precheck_upstream_echo_non_participation.hpp` 和 `src/shard_preview_disabled_precheck_upstream_echo_non_participation.cpp`。它暴露五个小接口：生成 JSON、生成 stage catalog、生成 digest marker、生成 current status，以及返回已发布阶段数。

`src/shard_preview_disabled_precheck_upstream_echo_non_participation_stages.cpp` 保存 v1418-v1442 的 25 个阶段。每个 `StageRecord` 都有 release version、source frozen release version、source fixture path 和 scope，链路从 `v1418` 读取 `v1417` 开始，最后收在 `v1442` 的 clean workspace / CI closeout。

`src/shard_preview_disabled_precheck_upstream_echo_non_participation_checks.cpp` 保存 25 条检查。它们不是运行 Node 的检查，而是把边界拆开写清楚：stable barrel no import、constants/reference no import、checks/messages no execution、core loader no execution、Node typecheck/Vitest/build 不由 mini-kv 运行、endpoint handle 不读、raw endpoint 不解析、credential/secret 不读、router/write/WAL/load/restore/compact 继续关闭。

`src/shard_preview_disabled_precheck_upstream_echo_non_participation_validation.cpp` 只做计数和边界聚合。它要求 source fixture audit 已完成 12 阶段、Node 不要求 fresh mini-kv evidence、mini-kv 不 import Node、不执行 Node check、不启动服务、不读 endpoint、不读 credential，并且 25 个 stage/check 全部发布后才给出 `disabledPrecheckUpstreamEchoNonParticipationValidationPassed=true`。

## 命令和 SHARDJSON

新增命令是 `SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON`。它在 `src/command.cpp` 中作为 runtime evidence read command 注册，在 `src/command_contracts.cpp` 中声明为 `category=read`、`mutates_store=false`、`touches_wal=false`。因此 `COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON` 和 `CHECKJSON` 都能看到它是只读命令，而不是写命令或 admin execution。

`src/shard_readiness.cpp` 将 current release 推到 `v1442`，并把 root `status` 切到：

```text
route-preview-disabled-precheck-upstream-echo-non-participation-clean-workspace-ci-closeout-read-only
```

同时新增 `shardPreviewDisabledPrecheckUpstreamEchoNonParticipation` section，并把 digest marker 加进 shard readiness digest material。这样 Node/Java 或人工审查者消费 `fixtures/release/shard-readiness.json` 时，会看到当前 mini-kv 证据链已经覆盖 disabled-precheck upstream echo 非参与，而不是仍停留在 v1417。

## 关键字段怎么读

`sourceNodePlan` 指向 `docs/plans3/v1878-disabled-precheck-upstream-echo-module-split-roadmap.md`，这只是 Node 计划路径，不代表 mini-kv 会打开该项目或运行 Node。`sourceNodeDisabledPrecheckUpstreamEchoReleaseRange` 是 `Node v1867-v1878`，用于说明 C++ 本次响应的是哪段 Node 内部拆分。

`sourceNodeRequiresFreshMiniKvEvidence=false` 表示 Node 不把本批次作为硬前置；mini-kv 继续独立推进自己的证据厚度。`sourceNodeRequiresSiblingStartup=false` 和 `sourceNodeNoServiceStartupConstraint=true` 表示不需要启动 Java 或 mini-kv 服务来配合 Node。

`nodeDisabledPrecheckStableBarrelImportedByMiniKv=false`、`nodeDisabledPrecheckCoreLoaderExecutedByMiniKv=false`、`nodeTypecheckExecutedByMiniKv=false`、`nodeVitestExecutedByMiniKv=false`、`nodeBuildExecutedByMiniKv=false` 是最重要的 Node 边界字段：mini-kv 只记录计划事实，不把 Node 的 TypeScript 内部实现带进 C++。

`endpointHandleRead=false`、`rawEndpointParsed=false`、`credentialValueRead=false`、`secretValueRead=false` 把 managed audit 连接相关风险继续关住。即使字段名里出现 endpoint / credential，也只是证明没有读取。

`activeRouterInstalled=false`、`writeRoutingAllowed=false`、`touchesWal=false`、`loadRestoreCompactAllowed=false`、`executionAllowed=false` 是存储边界字段。它们说明本批次不是 shard router、不是写路径、不是 WAL 操作、不是恢复命令、不是执行入口。

## 测试覆盖

新增 5 个测试文件：

- `disabled_precheck_upstream_echo_non_participation_stages_tests.cpp` 验证 v1418-v1442 的 release/source fixture 链连续。
- `disabled_precheck_upstream_echo_non_participation_checks_tests.cpp` 验证 25 条 check 的边界字段和越界保护。
- `disabled_precheck_upstream_echo_non_participation_validation_tests.cpp` 验证 source 未闭合、Node 要 fresh evidence、mini-kv 越界、计数不齐时都会让 validation failed。
- `disabled_precheck_upstream_echo_non_participation_tests.cpp` 验证 standalone JSON 命令的契约字段、source fixture audit、Node module split 边界和 no-execution 字段。
- `disabled_precheck_upstream_echo_non_participation_fixture_tests.cpp` 验证 v1418-v1442 versioned fixtures 和 current fixture 都含有新 section。

既有 `command_tests.cpp`、`line_editor_tests.cpp`、`shard_readiness_tests.cpp` 和上一批 fixture audit 测试也同步更新。这样新增命令不是孤立存在，而是被命令目录、补全、SHARDJSON current fixture 和历史 source fixture 共同约束。

## 归档和闭环

本批次的最终归档放在 `e/1442/图片/` 和 `e/1442/解释/说明.md`。归档截图覆盖 CMake 配置构建、定向测试、全量 CTest、CLI smoke、TCP smoke、diff/whitespace 检查。验证时 TCP server 只用于本地 standalone read command smoke，结束后停止，不作为长期服务保留。

一句话总结：v1418-v1442 把 Node disabled-precheck upstream echo 的内部拆分变成 mini-kv 可验证的只读非参与证据，同时通过 stage/check/validation 拆分让后续维护继续小步推进，不把 C++ 接成 Node 执行器或 managed audit runtime。
