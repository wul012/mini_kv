# mini-kv v836 draft text package comparison closeout audit source freeze

## 本版目标

v836 的目标是把 v835 的 draft text package review closeout audit 固化成新的 comparison closeout audit 起点。Node 最新计划已经走到 signed approval capture artifact draft text package comparison preflight closeout，但 mini-kv 在这一版仍然只提供离线、只读、不可执行的证据目录。

这版不是什么也要讲清楚：它不是 draft text package parser，不是 package acceptance gate，不是 approval grant emitter，也不是 runtime executor。它不会打开 active router，不会增加 write routing，不会触碰 WAL，不会读取运行时 archive，不会启动 sibling service，也不会把任何 operator value 写入 mini-kv。

## 入口和命令

新增命令是：

```text
SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON
```

入口注册在 `src/command.cpp`，命令契约补在 `src/command_contracts.cpp`，交互补全补在 `src/line_editor.cpp`。命令返回 JSON，而不是执行动作；如果传入额外参数，仍走 usage 错误，保持 metadata read 的命令形态。

`SHARDJSON` 也嵌入了新分支：

```text
shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftTextPackageComparisonCloseoutAudit
```

这让 Node 或其他控制面可以在一个聚合证据里读到 comparison closeout audit 状态，但这个字段仍然只是证据字段，不是执行开关。

## 模块拆分

本版没有把所有内容塞进一个巨型文件，而是拆成六个小模块：

- `...comparison_closeout_audit.cpp`：核心 JSON 聚合，负责命令字段、来源冻结、边界字段、stage chain、validation 和 diagnostics。
- `...comparison_closeout_audit_stages.cpp`：25 阶段 catalog，从 v836 到 v860，v836 只发布第 1 阶段。
- `...comparison_closeout_audit_checks.cpp`：comparison check catalog，记录 identity lane、digest binding、signature metadata、runtime payload rejection、write route rejection 等检查项。
- `...comparison_closeout_audit_blockers.cpp`：acceptance control/blocker catalog，明确哪些情况必须 fail closed。
- `...comparison_closeout_audit_profiles.cpp`：每个阶段的 profile，把维护视角从 check/blocker 里拆出来。
- `...comparison_closeout_audit_validation.cpp`：只做目录数量、已发布阶段和来源 closeout chain 的结构校验。

测试文件名刻意压短成 `tests/text_package_comparison_closeout_audit_*_tests.cpp`。原因是 Windows + MinGW 的 `.obj.d` 路径长度会被超长文件名顶爆；源模块保留完整语义名，测试文件和 CMake target 做维护性缩短。

## 关键字段怎么理解

核心来源字段：

- `sourceDraftTextPackageReviewCloseoutAuditReleaseVersion="v835"`：comparison audit 的来源不是 rolling current，而是已经冻结的 v835。
- `sourceDraftTextPackageReviewCloseoutAuditFixturePath="fixtures/release/shard-readiness-v835.json"`：控制面应从版本化 fixture 理解来源，不应回读当前 fixture 当作历史证据。
- `sourceDraftTextPackageReviewCloseoutAuditChainComplete=true`：v811-v835 review closeout audit 链已完整，v836 才开始 comparison closeout audit。

comparison 边界字段：

- `draftTextPackageComparisonCloseoutAuditOnly=true`：本分支只描述 comparison closeout audit。
- `submittedDraftTextPackageParsed=false`：不会解析已提交 draft text package。
- `detachedSignaturePayloadParsed=false`：不会解析 detached signature payload。
- `uncomparedPackageAccepted=false`：未比较的 package 不能被接受。
- `unacceptablePackageAccepted=false`：不可接受材料不能被接受。
- `approvalGrantEmitted=false`：不发 approval grant。
- `submittedOperatorValueCount=0`、`acceptedOperatorValueCount=0`、`importedEvidenceValueCount=0`、`persistedOperatorValueCount=0`：mini-kv 不接收、不导入、不持久化 operator value。
- `writeRoutingAllowed=false`、`touchesWal=false`、`executionAllowed=false`：这三项是最重要的硬边界，表示没有写路由、没有 WAL 触碰、没有执行入口。

validation 字段：

- `comparisonCatalogsAligned=true`：checks、acceptance controls、profiles 数量对齐。
- `completedComparisonCheckCount=1`：v836 只发布第 1 个 source freeze 检查，不假装已经完成 v836-v860 全链。
- `readyForDraftTextPackageComparison=false`：这不是“可以比较并接受 package”的信号，只是 comparison closeout audit 的只读证据。

## 测试覆盖

新增 focused 测试覆盖六个 comparison 模块：

- stages 测试检查 v836-v860 阶段连续、v836 来源冻结到 v835、当前 published count 为 1。
- checks/blockers/profiles 测试检查第 1 阶段只发布 source freeze，超出 count 时才显示完整 catalog。
- validation 测试检查 source review closeout chain complete、catalog aligned 和 published checks guarded。
- 核心 JSON 测试检查 source fixture、comparison-only、no parse、no accept、no grant、no runtime、no write、no execution。

既有 `command_tests`、`shard_readiness_tests`、`line_editor_tests` 也同步扩展，保护命令入口、`SHARDJSON` 嵌入、补全和顶层 status。两个早期模块测试里原本硬编码了顶层 `SHARDJSON.status`，本版同步改为 v836 status；它们自己的模块契约没有改变。

## 真实运行证明

本版归档在 `e/836/图片/` 和 `e/836/解释/说明.md`：

- CMake configure 成功。
- CMake build 成功。
- focused CTest 9/9 通过。
- full CTest 143/143 通过。
- CLI smoke 通过，确认新命令和 `SHARDJSON` 都能读到 v836 comparison closeout audit。
- TCP smoke 通过，临时 server 验证后停止，临时 WAL 删除。
- `git -c core.autocrlf=false diff --check` 通过，修掉了 README 和讲解索引首行 CRLF 引起的 trailing whitespace 提示。

一句话总结：v836 把 v835 review closeout audit 变成 comparison closeout audit 的版本化起点，并把 no parse、no accept、no grant、no runtime、no write、no execution 继续压在 C++ 侧的可测试证据里。
