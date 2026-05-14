# 第六十八版：Artifact Digest Compatibility Matrix

## 本版目标

本版按最新跨项目计划 `D:\nodeproj\orderops-node\docs\plans\v166-post-rollback-window-roadmap.md` 推进 mini-kv v68。计划里 Java v59 与 mini-kv v68 推荐并行：Java 侧补 production secret source contract，mini-kv 侧补 artifact digest compatibility matrix，随后 Node v168 才能合成 production environment preflight checklist。

v68 的角色很窄：把 binary、WAL、Snapshot、fixture 的 digest / checksum evidence 固化成一份只读矩阵，让后续控制面知道“哪些产物需要 operator 记录 digest、哪些版本证据需要一起核对、哪些不允许自动执行”。

它不是什么：

```text
不是新的 server 命令
不是 restore 执行器
不执行 LOAD / COMPACT / SETNXEX
不读取生产 secret
不启动 Node 或 Java
不连接生产数据库
不把 mini-kv 放进 Java 订单一致性链路
```

所以本版继续保持 mini-kv 的位置：C++ 侧提供可读、可测、可归档的 evidence，而不是替代 Java 的订单权威或 Node 的审批控制面。

## 计划对齐

计划文件当前写明：

```text
下一步推荐并行：Java v59 + mini-kv v68
mini-kv v68 做 artifact digest compatibility matrix
记录 binary digest
记录 WAL/Snapshot digest 或 checksum evidence
记录 fixture digest
明确 digest matrix 不执行 restore
明确 mini-kv 仍不是 Java order authority
```

本版对应落地为：

```text
fixtures/release/artifact-digest-compatibility-matrix.json
tests/artifact_digest_compatibility_matrix_tests.cpp
minikv_artifact_digest_compatibility_matrix_tests
verification-manifest v68
INFOJSON fixture version 0.68.0
README / 代码讲解 / c/68 归档
```

## 1. CMakeLists.txt

本版把项目版本从：

```text
0.67.0
```

提升到：

```text
0.68.0
```

这会继续通过 `include/minikv/version.hpp.in` 注入运行时版本，所以 `INFO`、`INFOJSON`、`STORAGEJSON` 读到的版本会跟 CMake project version 保持一致。

同时新增测试目标：

```text
minikv_artifact_digest_compatibility_matrix_tests
```

它链接 `minikv` 静态库，并注入 `MINIKV_SOURCE_DIR`，这样测试既能读 repository fixture，也能创建真实 `CommandProcessor` 校验 `INFOJSON` / `CHECKJSON` / `STORAGEJSON` 的当前行为。

## 2. artifact-digest-compatibility-matrix.json

新增主文件：

```text
fixtures/release/artifact-digest-compatibility-matrix.json
```

核心身份字段：

```text
matrix_version=mini-kv-artifact-digest-compatibility-matrix.v1
project=mini-kv
project_version=0.68.0
release_version=v68
consumer_hint=Node v168 production environment preflight checklist
```

边界字段仍然放在最外层：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
```

控制面读到这些字段时，应该理解为：这份矩阵可以被 preflight 读取和展示，但不能被当作执行授权，不能触发 restore，也不能证明 mini-kv 是订单事实来源。

## 3. release_inputs

`release_inputs` 说明矩阵对应的目标版本和历史证据来源：

```text
target_release_version=v68
previous_release_version=v67
previous_evidence=verification-manifest / restore-dry-run-operator-package / runtime-artifact-bundle / restore-compatibility-handoff
operator_confirmation_required=true
```

这段不是“回滚目标选择器”，而是“operator 需要核对哪些材料”的索引。它不会切流，不会做 restore，也不会改任何运行时状态。

## 4. digest_matrix

矩阵主体拆成四类：

```text
binary-digest
wal-checksum-evidence
snapshot-digest-evidence
fixture-digest
```

`binary-digest` 代表 release build 产物，例如：

```text
minikv_server/minikv_cli/minikv_client
sha256:<operator-recorded-binary-digest>
```

它与：

```text
CMake project version 0.68.0
INFOJSON runtime version 0.68.0
```

绑定在一起。意思是 digest 不能脱离运行版本看，控制面至少要知道“这个二进制 digest 对应的是哪个 runtime version”。

`wal-checksum-evidence` 指向 WAL2 replay 和 checksum 证据，来源是：

```text
fixtures/ttl-token/recovery-evidence.json
```

它强调 `SETEXAT`、`EXPIREAT`、WAL2 checksum replay 这些兼容边界，但仍然只是证据，不是修复 Java transaction state 的工具。

`snapshot-digest-evidence` 指向 snapshot SAVE/LOAD 证据，来源是：

```text
fixtures/recovery/restart-recovery-evidence.json
```

它说明 snapshot digest 需要和手动 SAVE/LOAD、过期项跳过等行为一起核对，但矩阵本身不执行 `LOAD`。

`fixture-digest` 指向本次新增的 matrix fixture 自己，说明后续 Node v168 消费时要能确认：读到的是被 operator 记录过 digest 的 fixture，而不是飘在仓库里的未确认样本。

## 5. compatibility_checks

`compatibility_checks` 不是新的命令实现，而是把矩阵里的复核点绑定到现有只读命令：

```text
binary-version-match -> INFOJSON
wal-matrix-review -> CHECKJSON COMPACT / STORAGEJSON
snapshot-matrix-review -> CHECKJSON LOAD data/digest-matrix.snap / STORAGEJSON
fixture-matrix-review -> CHECKJSON SETNXEX digest:token 30 value / GET digest:token
```

这里的关键是 `CHECKJSON`。它解释真实命令如果执行会有什么风险，但它自己不会执行对应命令。

例如：

```text
CHECKJSON LOAD data/digest-matrix.snap
```

只返回 `LOAD` 的 parser / write_command / side_effects / WAL durability 信息，不会读取 snapshot 文件，也不会替换 store。

```text
CHECKJSON SETNXEX digest:token 30 value
GET digest:token
```

组合起来证明：矩阵解释了 token 写风险，但并没有真的 claim token，`GET digest:token` 必须仍然是 `(nil)`。

## 6. verification-manifest v68

`fixtures/release/verification-manifest.json` 同步到：

```text
project_version=0.68.0
release_version=v68
consumer_hint=Node v168 production environment preflight checklist
```

新增 fixture：

```text
fixtures/release/artifact-digest-compatibility-matrix.json
```

新增 targeted test：

```text
minikv_artifact_digest_compatibility_matrix_tests
```

read-only smoke 改为端口 6408，并围绕 digest matrix 命令：

```text
INFOJSON
CHECKJSON LOAD data/digest-matrix.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX digest:token 30 value
STORAGEJSON
HEALTH
GET digest:token
QUIT
```

manifest 的 boundary 新增：

```text
artifact digest compatibility matrix only
```

这保证 v68 被解释成“生产前置 evidence 的一部分”，不是“恢复执行入口”。

## 7. 测试保护什么

新增测试文件：

```text
tests/artifact_digest_compatibility_matrix_tests.cpp
```

它做三类断言。

第一类是 fixture 身份断言：确认 `matrix_version`、`project_version`、`release_version`、`consumer_hint`、`read_only`、`execution_allowed`、`restore_execution_allowed`、`order_authoritative` 没被改坏。

第二类是矩阵内容断言：确认四类 digest 占位存在：

```text
binary-digest
wal-checksum-evidence
snapshot-digest-evidence
fixture-digest
```

并确认这些占位都是 operator-recorded placeholder，而不是伪造真实生产 digest。

第三类是真实命令断言：测试创建 `CommandProcessor`，执行：

```text
INFOJSON
CHECKJSON LOAD data/digest-matrix.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX digest:token 30 value
STORAGEJSON
GET digest:token
```

它保护的是运行时边界：`INFOJSON` 能读到 `0.68.0`，`CHECKJSON` 不执行写命令，`STORAGEJSON` 仍然 `order_authoritative=false`，`GET digest:token` 仍然 `(nil)`。

## 8. 历史样本如何处理

本版没有把历史 fixture 改写成 v68：

```text
runtime-artifact-rollback-evidence.json -> v64
runtime-artifact-bundle-manifest.json -> v65
restore-compatibility-handoff.json -> v66
restore-dry-run-operator-package.json -> v67
```

这些样本代表历史 evidence 来源，应该保留自己的版本身份。测试只把当前运行时版本从 `0.67.0` 同步到 `0.68.0`，证明新 binary 仍然能解释旧 evidence 的只读命令。

## 9. README 和归档

README 当前版本更新为 Version 68，并新增：

```text
fixtures/release/artifact-digest-compatibility-matrix.json
minikv_artifact_digest_compatibility_matrix_tests
```

Roadmap 同步为：

```text
Java v59 + mini-kv v68 并行补 secret source / artifact digest matrix
Node v168 等待两边完成后生成 production environment preflight checklist
Node v169 再做 post-v166 readiness summary
```

运行截图和解释继续放在：

```text
c/68/图片/
c/68/解释/说明.md
```

## 10. 一句话价值

v68 让 mini-kv 从“restore dry-run package 可以被 preflight 读取”推进到“artifact digest compatibility matrix 可以被 production environment preflight 核对”，为 Node v168 补上了 C++ 侧二进制、WAL、Snapshot、fixture digest 的只读生产前置证据。
