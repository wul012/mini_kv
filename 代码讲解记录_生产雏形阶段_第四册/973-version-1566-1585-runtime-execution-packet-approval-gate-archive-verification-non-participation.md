# 第九百七十三篇代码讲解：runtime execution packet approval gate archive verification non-participation v1566-v1585

本版目标是把 Node v399 对 runtime execution packet approval gate archive verification 的判断，冻结成 mini-kv 自己可版本化、可回放、可测试的只读证据。它的角色不是审批系统、不是运行时执行入口、不是 Node/Java 的联动探针，也不是把 mini-kv 变成 router、writer、WAL replay 或部署回滚执行器。它只回答一件事：在 Node 还处于归档核验和审批门禁阶段时，mini-kv 已经明确不参与 approval input、runtime artifact、router、write、WAL 和 execution。

## 入口命令

本版新增的入口命令是：

```text
SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON
```

入口接入点有三处。`src/command.cpp` 负责把命令分发到证据 JSON formatter；`src/command_catalog.cpp` 负责让 `COMMANDS`、`COMMANDSJSON`、`EXPLAINJSON` 和 `CHECKJSON` 都能看到这个 read command；`src/shard_readiness.cpp` 负责把该证据作为当前 SHARDJSON 的 active section 发布，并把 release version 滚到 `v1585`。

命令仍然是 metadata read：`mutates_store=false`、`touches_wal=false`、`execution_allowed=false`。因此它可以被 CLI、测试和控制面读取，但不能触发 store 写入、WAL 写入、服务启动、runtime artifact 读取或审批执行。

## 响应结构

核心响应由四个模块拼起来：

```text
include/minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation.hpp
include/minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_stages.hpp
include/minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_checks.hpp
include/minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation.hpp
```

对应实现放在 `src/` 下同名四个 `.cpp` 文件里。这样拆分的原因很直接：20 个 staged release item、边界 check catalog、validation summary 和最终 JSON formatter 如果挤进一个文件，后续每版只会继续变长。拆开之后，版本阶段、边界断言、聚合校验和对外响应可以分别维护。

最外层 contract 是：

```text
shard-route-runtime-execution-packet-approval-gate-archive-verification-non-participation.v1
```

它记录了本版读取的 Node 计划路径：

```text
docs/plans3/v399-post-java-mini-kv-runtime-execution-packet-approval-gate-review-archive-verification-roadmap.md
```

也记录了 mini-kv 的来源冻结点：

```text
fixtures/release/shard-readiness-v1545.json
```

这意味着 v1566-v1585 不是重新解释旧证据，而是在 v1545 已完成的 precheck upstream receipt split non-participation 基础上，发布一层新的 archive-verification non-participation 判断。

## 核心流程

`runtime_execution_packet_approval_gate_archive_verification_non_participation_stages.cpp` 生成 20 个阶段条目。每个条目对应一个 v1566-v1585 的 release slot，表达当前阶段在 Node v399 archive-verification 方向下应该证明什么：只读、无审批输入、无 runtime artifact、无 active router、无 write、无 WAL、无 execution。

`runtime_execution_packet_approval_gate_archive_verification_non_participation_checks.cpp` 生成边界检查。这里最重要的不是数量，而是检查类别：它把 approval input、runtime artifact、service startup、raw endpoint、credential value、managed connection、router、store mutation、WAL touch、load/restore/compact 和 execution 全部压到 false。

`runtime_execution_packet_approval_gate_archive_verification_non_participation_validation.cpp` 汇总阶段和检查，计算 published/planned stage count、passed/expected check count、source freeze 是否完整、current release 是否落到 v1585。这个模块给 formatter 一个明确的 validation summary，避免 formatter 自己边拼 JSON 边承担业务判断。

最终 `runtime_execution_packet_approval_gate_archive_verification_non_participation.cpp` 只做一件事：把 stages、checks、validation 组合成稳定 JSON。这个分工让以后新增证据链时可以复用同样的工程形状，而不是继续复制一整段混合逻辑。

## 边界字段

本版最关键的字段是这些：

```text
readOnly=true
executionAllowed=false
orderAuthoritative=false
mutatesStore=false
touchesWal=false
approvalInputCount=0
runtimeArtifactCount=0
activeRouterInstalled=false
writeRoutingAllowed=false
writeCommandsAllowed=false
loadRestoreCompactAllowed=false
startsMiniKvService=false
liveReadAllowed=false
```

控制面读到这些字段时，应该把 mini-kv 理解成证据生产者和边界声明者，而不是执行者。`approvalInputCount=0` 和 `runtimeArtifactCount=0` 尤其重要：mini-kv 没有收集审批材料，也没有消费运行时包。它只是把“当前不能参与”的原因版本化。

`sourcePrecheckUpstreamReceiptVerificationSplitStatus` 保留 v1545 的历史来源状态，当前顶层 `status` 则滚到 v1585 的 runtime execution packet approval gate archive verification non-participation。这个区别修复了旧测试里“当前状态必须以 route-preview 开头”的脆弱假设。

## SHARDJSON 集成

`src/shard_readiness.cpp` 做了三件事：

1. `release_version` 滚到 `v1585`。
2. 当前 `status` 滚到 `runtime-execution-packet-approval-gate-archive-verification-non-participation-clean-ci-closeout-read-only`。
3. 在 SHARDJSON 中新增 `runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipation` section。

同时，当前命令目录统计从 88 条滚到 89 条，因为新增了一个 read command。这个变化同步进入 `commandCatalogQuality`，并由 fixture 和命令测试保护。

## 测试保护

本版新增五组核心测试：

```text
runtime_execution_packet_approval_gate_archive_verification_non_participation_stages_tests.cpp
runtime_execution_packet_approval_gate_archive_verification_non_participation_checks_tests.cpp
runtime_execution_packet_approval_gate_archive_verification_non_participation_validation_tests.cpp
runtime_execution_packet_approval_gate_archive_verification_non_participation_tests.cpp
runtime_execution_packet_approval_gate_archive_verification_non_participation_fixture_tests.cpp
```

stage tests 保护 20 个版本槽位连续、末尾落到 v1585。check tests 保护所有危险能力都保持关闭。validation tests 保护 published/planned count、source freeze 和 current release 一致。formatter tests 保护 command、contract、边界字段和 no true execution。fixture tests 保护 v1566-v1585 每个版本化 fixture 都可读且边界仍为只读。

旧测试也做了一次小重构：`tests/test_support.hpp` 新增 `assert_current_shard_readiness_status` 和 `assert_precheck_upstream_receipt_source_status`。这样旧模块测试不再把“route-preview 前缀”误当作当前永恒状态，而是同时确认当前顶层状态已滚到 v1585、历史 v1545 来源状态仍在。

本版实际验证包括 MinGW/CMake 构建、12 条定向测试、12 条旧断言修复回归测试、完整 308 条 CTest，以及 CLI smoke。全量测试证明这次只读证据扩展没有破坏 store、WAL、snapshot、TCP、restore 和历史 fixture 链。

## 上下游价值

对 Node 来说，本版给 v399 之后的 archive verification / approval gate 判断提供了一个稳定的 mini-kv 侧 evidence anchor。对 Java 来说，它明确 mini-kv 没有变成 order authority 或 runtime executor。对 mini-kv 自己来说，它把大主题拆成 core/stage/check/validation 四层，既保留了版本化证据密度，也避免继续堆出难维护的大文件。

一句话：v1566-v1585 让 Node/Java/mini-kv 的松耦合融合多了一层可验证的“mini-kv 不参与执行”证据，而不是多一个隐式执行能力。
