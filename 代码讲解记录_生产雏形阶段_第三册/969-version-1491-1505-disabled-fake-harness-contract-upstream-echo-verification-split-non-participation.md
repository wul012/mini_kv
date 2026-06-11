# mini-kv v1491-v1505 disabled fake harness contract upstream echo verification split non-participation

## 本版目标和边界

本版读取的上游计划是 Node `docs/plans3/v1953-v1967-disabled-fake-harness-contract-upstream-echo-verification-split-roadmap.md`。Node 这轮拆的是 disabled fake harness contract upstream echo verification：入口、常量、引用、policy、core、loader、验证汇总等职责被拆开，并继续消费历史 Java v122-v126 evidence 与历史 mini-kv v127 non-participation receipt。计划没有要求 mini-kv 启动服务、读取 endpoint、解析 credential、执行 Node 测试或重新读取历史 receipt 文件。

所以 mini-kv v1491-v1505 做的是 C++ 侧的只读非参与证据：冻结 v1490 release-window readiness packet split evidence 作为 source，发布 15 个版本化 fixture，把 Node v1953-v1967 的拆分进度记录成 evidence chain。它不是新的 fake harness 执行入口，不导入 Node entrypoint/constants/references，不执行 Node policy/core/loader/verification，不运行 typecheck/vitest/build，不读取 Java 或 mini-kv 历史文件，不打开 router、write、WAL、LOAD/RESTORE/COMPACT 或任何 execution 权限。

## 模块拆分

入口文件是 `src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation.cpp`，对外暴露命令：

```text
SHARDROUTEDISABLEDFAKEHARNESSCONTRACTUPSTREAMECHOVERIFICATIONSPLITNONPARTICIPATIONJSON
```

这个 formatter 汇总 source Node plan、v1490 source release-window evidence、当前 v1505 stage、15 个 check、validation 和边界字段。它只生成 JSON 证据，不进行文件读取或网络/runtime 调用。

stage catalog 拆到 `src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_stages.cpp`。15 个 stage 从 v1491 到 v1505，依次覆盖 public entrypoint boundary、constants boundary、Node v288 projection、Java reference、mini-kv v127 receipt reference、snippet/reference helper、core/summary/policy、focused tests、typecheck/build 和 closeout。每个 stage 都有连续的 `sourceFrozenReleaseVersion` 与 `sourceFrozenFixturePath`，fixture chain 可以说明每一版从上一版冻结而来。

check catalog 拆到 `src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_checks.cpp`。每个 check 都绑定一个 Node v1953-v1967 source 语境，并统一输出关键 false 字段：不导入 Node 模块、不执行 Node checks、不启动服务、不读 endpoint/credential、不解析历史 receipt、不打开 managed audit connection、不安装 active router、不允许 write routing、不 touch WAL、不允许 execution。

validation 拆到 `src/shard_preview_disabled_fake_harness_contract_upstream_echo_verification_split_non_participation_validation.cpp`。它把通过条件压成四组：v1490 source 是否完整、Node 是否不要求 fresh mini-kv evidence、mini-kv 边界是否关闭、planned/completed/published count 是否都对齐到 15。只要有人把 `miniKvExecutesNodeChecks`、`miniKvReadsEndpoints`、`miniKvParsesHistoricalReceipts` 或 count 改错，validation 就会变成 false。

## SHARDJSON 和命令面

`src/shard_readiness.cpp` 把根 `releaseVersion` 滚到 `v1505`，根 `status` 滚到：

```text
route-preview-disabled-fake-harness-contract-upstream-echo-verification-split-non-participation-clean-workspace-ci-closeout-read-only
```

同时新增 `shardPreviewDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipation` section，并把 digest marker 纳入 root evidence digest material。控制面读到这个 section 时，应理解为：Node 的 upstream echo verification split 已被 mini-kv 以只读方式观察和归档；mini-kv 不运行 Node，也不消费运行时 endpoint/credential/历史 receipt 文件。

`src/command.cpp`、`src/command_contracts.cpp` 和 `src/line_editor.cpp` 同步接入新命令。`COMMANDS`/`COMMANDSJSON`/`HELP`/line editor completion 都能看到它，contract 标记为 read、no mutate store、no WAL touch、stable。因为命令名很长，本版还把新增 CTest target 名缩短为 `dfh_echo_split_np_*`，避免 Windows/Ninja object dependency path 超限，这是本版顺带做的维护性优化。

## 关键字段怎么读

`sourceReleaseWindowReadinessPacketSplitReleaseVersion: "v1490"` 表示本版 source 是上一段 release-window readiness packet split 非参与证据，而不是直接重新读取 Node 或历史 runtime 文件。

`sourceNodeDisabledFakeHarnessContractUpstreamEchoVerificationSplitCloseoutReleaseVersion: "Node v1967"` 表示 mini-kv 记录的是 Node split closeout 的计划事实，不代表 C++ 执行了 Node 的 loader、core、policy 或 verification。

`sourceNodeConsumesHistoricalMiniKvReleaseVersion: "v127"` 和 `sourceNodeConsumesHistoricalJavaReleaseVersionRange: "Java v122-v126"` 是历史证据引用。mini-kv 本版只记录这些引用关系，不扫描 Java 证据，不读取 mini-kv v127 receipt 文件，也不打开 sibling 项目运行时。

`miniKvParsesHistoricalReceipts: false` 是本版新边界里很重要的一项。它把“Node 计划里提到历史 mini-kv receipt”和“mini-kv 现场解析历史 receipt 文件”分开，防止证据链从 read-only metadata 悄悄滑到 runtime file consumption。

`activeRouterInstalled: false`、`writeRoutingAllowed: false`、`touchesWal: false`、`executionAllowed: false` 继续保持 shard preview 的底线：证据可以增长，真实路由、写路径、WAL 和执行权限不能顺手打开。

## 测试和 fixture

新增 5 组短 target 测试：

```text
dfh_echo_split_np_stages_tests
dfh_echo_split_np_checks_tests
dfh_echo_split_np_validation_tests
dfh_echo_split_np_tests
dfh_echo_split_np_fixture_tests
```

stage tests 保护 v1491-v1505 的 stage 顺序、source freeze 连续性和 planned count。checks tests 保护 15 条边界检查及 count 越界失败。validation tests 覆盖 source 不完整、fresh evidence 要求打开、Node import/execution 边界打开、服务/endpoint/credential/receipt 边界打开、count 不齐时 validation 必须失败。核心 JSON tests 保护 standalone command 的字段语义。fixture tests 保护 `fixtures/release/shard-readiness-v1491.json` 到 `v1505.json` 以及 rolling current fixture 都包含新 section 和关键 false 边界。

旧测试也做了必要滚动：`command_tests` 认可 86 个命令和 standalone command；`line_editor_tests` 认可新命令补全；`shard_readiness_tests` 认可根 status 到 v1505 并检查新 section；旧 release-window fixture tests 继续证明 v1476-v1490 历史证据冻结，同时确认 current 已滚到 v1505。

本地验证结果：Debug 构建通过；针对性 CTest 9/9 通过；全量 CTest 291/291 通过。CLI/TCP smoke 作为归档截图落在 `e/1505/`，证明新命令和 `SHARDJSON` 都能通过真实命令面读到同一份只读 evidence。

## 版本价值

v1491-v1505 让 mini-kv 在 Node disabled fake harness upstream echo verification 继续拆分时保持清晰的非参与位置：Node 可以继续做自己的 contract verification，mini-kv 用版本化 fixture 证明自己没有被拖进 Node runtime、endpoint、credential、历史 receipt、router、write、WAL 或 execution 边界里。
