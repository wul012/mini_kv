# mini-kv v1443-v1472 sandbox endpoint credential resolver upstream echo non-participation

## 最近250版讲解清算补记

本篇已纳入 v1316-v1565 最近250版讲解清算。清算参照 `D:\nodeproj\orderops-node\代码讲解记录\107-production-readiness-summary-v3-v103.md` 的 production-readiness walkthrough 模式；原文保留为历史正文，本节补齐统一判断口径。

- 清算范围：v1443-v1472（完整覆盖最近250版窗口）。
- 目标定位：本篇用于回看 mini-kv 只读证据、维护拆分或证据链闭环，不是新的运行入口。
- 不是什么：不打开 router/write/WAL/execution，不读取 credential/raw endpoint，不启动 Node/Java/mini-kv sibling 服务，不把 mini-kv 变成 order 或 audit authority。
- 入口和证据：以原文记录的 command surface、SHARDJSON/current fixture/versioned fixture、CTest、CLI/TCP smoke、归档说明为准；控制面只能按只读证据理解。
- 边界字段：阅读时优先核对 `read_only`、`execution_allowed`、`order_authoritative`、`mutates_store`、`touches_wal`、`warnings`、`blockers`、`diagnostics` 等字段是否继续表达只读、不可执行、非权威和不写入。
- 测试理解：测试应说明断言保护的边界行为；若原文仅列命令，本节将其清算为“命令证据必须服务于 no router / no write / no WAL / no execution 判断”。
- 清算结论：保留原位置，不搬迁；后续若重写正文，按治理模板补齐入口、结构、流程、边界字段、测试和一句话总结。

## 本版目标

v1443-v1472 的目标，是把 v1442 已经闭合的 `disabled-precheck upstream echo non-participation` 证据冻结成新的上游来源，然后围绕 Node v1879-v1903 的 sandbox endpoint credential resolver upstream echo 模块拆分，发布一条 mini-kv 自己可回放、可审计、可测试的 30 阶段非参与证据链。

这不是新的 endpoint resolver，不是 credential reader，不是 managed audit connection，也不是 C++ 开始执行 Node 的 TypeScript 检查。Node 那边本轮只是把 sandbox endpoint credential resolver upstream echo verification 的内部模块继续拆小，mini-kv 这里只证明：即使 Node 出现了 stable barrel、constants、reference builders、checks/messages、core loader、renderer/export、typecheck/build/Vitest 等内部结构，C++ 仍然不导入、不运行、不读取 endpoint handle、不解析 raw endpoint、不读取 credential/secret、不开启 active router、不写 WAL、不执行 load/restore/compact。

本版读取的计划是 `docs/plans3/v1903-sandbox-endpoint-credential-resolver-upstream-echo-module-split-roadmap.md`。计划本身合理：它明确 Node 消费的是历史 mini-kv v114 和 Java v105 证据，不要求 fresh mini-kv build，不要求 sibling service startup。因此 mini-kv 可以独立推进自己的版本化证据，而不是被 Node 计划牵着打开运行时边界。

## 模块结构

入口在 `include/minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.hpp` 和 `src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.cpp`。它暴露五个小接口：生成 standalone JSON、生成 stage catalog、生成 digest marker、生成 current status，以及返回已发布 stage count。这个接口形状延续上一版，避免继续把所有逻辑塞回 `shard_readiness.cpp`。

`src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages.cpp` 保存 v1443-v1472 的 30 个阶段。链路从 `v1443` 冻结 `fixtures/release/shard-readiness-v1442.json` 开始，之后每一版都指向前一版 fixture，最后收在 `v1472` 的 clean workspace / CI closeout。这里的 stage 不是运行步骤，而是证据发布步骤，用来证明版本化 fixture chain 是连续的。

`src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks.cpp` 保存 30 条边界检查。它们覆盖 source freeze、Node plan intake、command catalog、current SHARDJSON rollover、versioned fixture chain、stable barrel no import、constants/reference no import、checks/messages/core loader no execution、typecheck/build/Vitest boundary、no service startup、endpoint handle no read、raw endpoint no parse、credential secret no read、managed connection closed、no router、no write、no WAL/load/restore/compact、CLI/TCP standalone smoke、archive/walkthrough retention 和 clean workspace CI closeout。

`src/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation.cpp` 只做聚合判断：v1442 source 必须已完成 25 阶段，Node 不要求 fresh mini-kv evidence，mini-kv 不导入 Node、不执行 Node checks、不启动服务、不读 endpoint handle、不解析 raw endpoint、不读 credential，并且 30 个 stage/check 全部发布后才给出 `sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed=true`。

`src/shard_readiness.cpp` 把 current release 滚到 `v1472`，root status 更新为：

```text
route-preview-sandbox-endpoint-credential-resolver-upstream-echo-non-participation-clean-workspace-ci-closeout-read-only
```

同时新增 `shardPreviewSandboxEndpointCredentialResolverUpstreamEchoNonParticipation` section，并把 digest marker 纳入 shard readiness digest material。这样当前 `SHARDJSON` 能直接说明 mini-kv 已经覆盖 Node v1879-v1903 的 sandbox endpoint credential resolver upstream echo 拆分边界，但没有把这个拆分变成 C++ 执行能力。

## 命令契约

新增命令是：

```text
SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON
```

它在 `src/command.cpp` 中作为只读 evidence command 注册，在 `src/command_contracts.cpp` 中声明为 `category=read`、`mutates_store=false`、`touches_wal=false`。`src/line_editor.cpp` 也补了命令补全，所以 `COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON`、`CHECKJSON` 和交互式补全看到的是同一份只读命令目录。

这个命令的 JSON 中，`sourceDisabledPrecheckReleaseVersion=v1442` 和 `sourceDisabledPrecheckPublishedStageCount=25` 说明上游来源已经锁定；`sourceNodeSandboxEndpointCredentialResolverUpstreamEchoCloseoutReleaseVersion=Node v1903` 说明本版响应的是 Node 的内部拆分收尾；`sourceNodeConsumesHistoricalMiniKvReleaseVersion=v114` 和 `sourceNodeConsumesHistoricalJavaReleaseVersion=Java v105` 说明 Node 计划没有要求当前 mini-kv 或 Java 参与新运行。

## 关键边界字段

`nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv=false`、`nodeSandboxEndpointCredentialResolverConstantsImportedByMiniKv=false`、`nodeSandboxEndpointCredentialResolverReferencesImportedByMiniKv=false` 表示 C++ 没有把 Node 模块结构导入进来。它们是模块拆分边界，不是编译链接边界。

`nodeSandboxEndpointCredentialResolverChecksExecutedByMiniKv=false`、`nodeSandboxEndpointCredentialResolverMessagesExecutedByMiniKv=false`、`nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv=false`、`nodeTypecheckExecutedByMiniKv=false`、`nodeVitestExecutedByMiniKv=false`、`nodeBuildExecutedByMiniKv=false` 表示 mini-kv 不执行 Node 检查、消息构造、loader、typecheck、Vitest 或 build。mini-kv 记录计划事实，但不替 Node 做工作。

`miniKvReadsEndpointHandles=false`、`miniKvParsesRawEndpoint=false`、`miniKvReadsCredentials=false`、`endpointHandleRead=false`、`rawEndpointParsed=false`、`credentialValueRead=false`、`secretValueRead=false` 是本版最重要的安全边界。字段名里出现 endpoint / credential，不代表 C++ 开始读这些东西；恰好相反，它们证明所有这些读取仍然关闭。

`managedAuditConnectionOpened=false`、`schemaMigrationExecuted=false`、`ledgerWriteExecuted=false`、`externalRequestSent=false` 把 managed audit connection 的风险口继续关住。本版没有连接、没有 schema migration、没有 ledger write、没有外部请求。

`activeRouterInstalled=false`、`writeRoutingAllowed=false`、`touchesWal=false`、`loadRestoreCompactAllowed=false`、`executionAllowed=false` 则是存储和执行边界。它们保证这个证据命令不会变成 shard router、写路径、WAL 操作、恢复命令或执行入口。

## 测试覆盖

新增 5 个测试文件：

- `sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages_tests.cpp` 验证 v1443-v1472 release/source fixture 连续性。
- `sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_tests.cpp` 验证 30 条 check、边界字段和越界保护。
- `sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_tests.cpp` 验证 source 未闭合、Node 要 fresh evidence、mini-kv 越界、raw endpoint 解析、计数不齐时都会失败。
- `sandbox_endpoint_credential_resolver_upstream_echo_non_participation_tests.cpp` 验证 standalone JSON 的契约、来源、Node/Java historical reference、30 阶段发布和 no-execution 字段。
- `sandbox_endpoint_credential_resolver_upstream_echo_non_participation_fixture_tests.cpp` 验证 v1443-v1472 versioned fixtures 和当前 `shard-readiness.json` 都含有本版 section。

同时更新了 `command_tests.cpp`、`line_editor_tests.cpp`、`shard_readiness_tests.cpp` 和若干直接读取 current `shard_readiness::format_json()` 的旧测试。更新原则是：v1442 模块自己的测试继续断言 v1442 status；所有表示当前 rolling SHARDJSON 的断言滚到 v1472。

## 归档和闭环

本版最终归档放在 `e/1472/图片/` 和 `e/1472/解释/说明.md`。归档覆盖 CMake configure/build、targeted CTest、full CTest、CLI smoke、TCP smoke 和 diff/whitespace 检查。验证时 TCP server 只用于本地 standalone read smoke，结束后停止，不作为后台服务保留。

一句话总结：v1443-v1472 把 Node sandbox endpoint credential resolver upstream echo 的内部拆分，转化为 mini-kv 自己版本化、可测试、可归档的只读非参与证据，同时把 endpoint、credential、managed connection、router、write、WAL 和 execution 边界继续锁死。
