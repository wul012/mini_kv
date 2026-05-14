# 第六十九版：Release Artifact Digest Package

## 本版目标

本版按最新跨项目计划 `D:\nodeproj\orderops-node\docs\plans\v169-post-production-environment-preflight-roadmap.md` 推进 mini-kv v69。计划里 Java v60 与 mini-kv v69 推荐并行：Java 侧补 production deployment runbook contract，mini-kv 侧补 release artifact digest package，随后 Node v170 才能合成 deployment evidence intake gate。

v69 的定位是把发布产物摘要证据打成一份只读 package。它把 binary、WAL、Snapshot、fixture digest placeholder，以及恢复演练命令 profile 放在同一份契约里，让 Node v170 后续可以读取“本次发布需要核对哪些产物、哪些 digest 需要 operator 记录、哪些命令只能 dry-run 解释”。

它不是什么：

```text
不是新的运行时命令
不是 restore 执行器
不执行 LOAD / COMPACT / SETNXEX
不读取生产 secret
不启动 Node 或 Java
不连接生产数据库
不把 mini-kv 放进 Java 订单一致性链路
```

所以本版继续保持 mini-kv 的边界：C++ 侧提供可读、可测、可归档的部署证据材料，不替代 Java 的订单权威，也不替 Node 做审批决策。

## 计划对齐

计划文件要求 mini-kv v69 落地：

```text
记录 binary digest
记录 WAL/Snapshot digest 或 checksum evidence
记录 fixture digest
记录 CHECKJSON restore drill command profile
明确 LOAD/COMPACT/SETNXEX 不执行
明确 mini-kv 仍不是 Java order authority
```

本版对应为：

```text
fixtures/release/release-artifact-digest-package.json
tests/release_artifact_digest_package_tests.cpp
minikv_release_artifact_digest_package_tests
verification-manifest v69
INFOJSON fixture version 0.69.0
README / 代码讲解 / c/69 归档
```

## 1. CMakeLists.txt

项目版本从：

```text
0.68.0
```

提升到：

```text
0.69.0
```

这会继续通过 `include/minikv/version.hpp.in` 注入到运行时，保证 `INFO`、`INFOJSON`、`STORAGEJSON` 和 release fixture 的当前版本一致。

新增测试目标：

```text
minikv_release_artifact_digest_package_tests
```

这个测试链接 `minikv` 静态库，并通过 `MINIKV_SOURCE_DIR` 读取仓库 fixture。这样它不只是检查 JSON 字符串，还会创建真实 `CommandProcessor`，验证 `CHECKJSON` 仍然只解释风险、不执行写入。

## 2. release-artifact-digest-package.json

新增主文件：

```text
fixtures/release/release-artifact-digest-package.json
```

身份字段：

```text
package_version=mini-kv-release-artifact-digest-package.v1
project=mini-kv
project_version=0.69.0
release_version=v69
consumer_hint=Node v170 deployment evidence intake gate
```

边界字段仍在最外层：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
```

控制面读到这些字段时，应该理解为：这份 package 可以被 intake gate 读取和展示，但不能被当作执行授权，不能触发 restore，也不能证明 mini-kv 是订单事实来源。

## 3. release_inputs

`release_inputs` 记录本次发布包的上下文：

```text
target_release_version=v69
previous_release_version=v68
previous_evidence=verification-manifest / artifact-digest-compatibility-matrix / restore-dry-run-operator-package / runtime-artifact-bundle-manifest
operator_confirmation_required=true
```

这里把 v68 的 artifact digest compatibility matrix 纳入来源，是为了让 v69 package 能接在上一版矩阵之后：v68 定义“哪些 digest 要核对”，v69 把这些 digest 和恢复演练命令 profile 打成 Node v170 可消费的一包。

## 4. artifact_digests

`artifact_digests` 拆成四类：

```text
binary-digest
wal-checksum-evidence
snapshot-digest-evidence
fixture-digest
```

`binary-digest` 对应 release build 产物：

```text
minikv_server/minikv_cli/minikv_client
sha256:<operator-recorded-binary-digest>
```

它要求 operator 在 mini-kv 外部记录真实 digest。fixture 只固定字段和占位格式，不伪造生产 digest。

`wal-checksum-evidence` 指向：

```text
fixtures/ttl-token/recovery-evidence.json
```

它说明 WAL2 checksum、`SETEXAT` replay、`EXPIREAT` replay、`COMPACT` dry-run profile 都要被一起考虑。但这仍然只是 C++ 侧持久化证据，不能修复 Java transaction state。

`snapshot-digest-evidence` 指向：

```text
fixtures/recovery/restart-recovery-evidence.json
```

它说明 Snapshot SAVE/LOAD、过期项跳过、恢复演练 profile 需要被人工核对。package 本身不执行 `LOAD`。

`fixture-digest` 指向 package 自己：

```text
fixtures/release/release-artifact-digest-package.json
sha256:<operator-recorded-fixture-digest>
```

这保证 Node v170 读取的 package 也需要进入 digest 核对链，而不是只核对二进制产物。

## 5. restore_drill_command_profile

本版新增的关键字段是：

```text
restore_drill_command_profile
```

它列出恢复演练中允许被只读解释的命令：

```text
INFOJSON
CHECKJSON LOAD data/release-artifact-drill.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX release:token 30 value
STORAGEJSON
HEALTH
GET release:token
QUIT
```

这里仍然靠 `CHECKJSON` 维持边界。`CHECKJSON LOAD` 只解释 `LOAD` 会产生 `store_replace_from_snapshot` 风险，不读取 snapshot 文件，不替换 store。`CHECKJSON COMPACT` 只解释 WAL rewrite 风险。`CHECKJSON SETNXEX` 只解释 token 写风险，而 `GET release:token` 必须返回 `(nil)`，证明没有真的写 token。

## 6. operator_confirmation

`operator_confirmation` 记录 operator 需要填的字段：

```text
release_operator_id
binary_digest_recorded_at
wal_snapshot_digest_recorded_at
fixture_digest_recorded_at
restore_drill_profile_reviewed
artifact_matrix_cross_checked
```

这些字段不是审批通过，也不是执行授权。它们只表示部署证据 intake 前，哪些人工核对动作需要留下记录。真实生产 digest 和审批仍在 mini-kv 外部完成。

## 7. verification-manifest v69

`fixtures/release/verification-manifest.json` 同步到：

```text
project_version=0.69.0
release_version=v69
consumer_hint=Node v170 deployment evidence intake gate
```

新增 fixture：

```text
fixtures/release/release-artifact-digest-package.json
```

新增 targeted test：

```text
minikv_release_artifact_digest_package_tests
```

read-only smoke 改为端口 6409，并围绕 release artifact drill profile：

```text
INFOJSON
CHECKJSON LOAD data/release-artifact-drill.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX release:token 30 value
STORAGEJSON
HEALTH
GET release:token
QUIT
```

manifest 新增边界：

```text
release artifact digest package only
does not read production secrets
does not execute LOAD/COMPACT/SETNXEX
```

## 8. 测试保护什么

新增测试文件：

```text
tests/release_artifact_digest_package_tests.cpp
```

它保护三件事。

第一，package 身份和边界不能漂移：`read_only=true`、`execution_allowed=false`、`restore_execution_allowed=false`、`order_authoritative=false` 必须一直存在。

第二，digest package 必须真的包含四类摘要占位：

```text
binary-digest
wal-checksum-evidence
snapshot-digest-evidence
fixture-digest
```

并且这些 digest 都是 `operator-recorded` placeholder，不在 fixture 中伪造真实生产值。

第三，真实 `CommandProcessor` 仍保持只读解释边界：

```text
INFOJSON -> version=0.69.0
CHECKJSON LOAD -> 解释 LOAD 风险但不执行
CHECKJSON COMPACT -> 解释 COMPACT 风险但不执行
CHECKJSON SETNXEX -> 解释写风险但不写 token
STORAGEJSON -> order_authoritative=false
GET release:token -> (nil)
```

原有历史证据测试也同步到当前运行版本 `0.69.0`，但历史 fixture 自己的版本仍保留：

```text
v64 rollback evidence
v65 bundle manifest
v66 restore compatibility handoff
v67 restore dry-run operator package
v68 artifact digest compatibility matrix
```

这说明 v69 binary 能继续解释旧证据，而不是重写旧证据。

## 9. README 和归档

README 当前版本更新为 Version 69，并新增：

```text
fixtures/release/release-artifact-digest-package.json
minikv_release_artifact_digest_package_tests
```

Roadmap 同步为：

```text
Java v60 + mini-kv v69 并行补 deployment runbook / release artifact digest package
Node v170 等待两边完成后生成 deployment evidence intake gate
Node v171 做 deployment evidence verification
Java v61 + mini-kv v70 后续补 rollback approval / restore drill evidence
Node v172 形成 release window readiness packet
```

运行截图和解释继续放在：

```text
c/69/图片/
c/69/解释/说明.md
```

## 10. 一句话价值

v69 让 mini-kv 从“artifact digest compatibility matrix 可被生产前置检查核对”推进到“release artifact digest package 可被 deployment evidence intake gate 消费”，为 Node v170 补上了 C++ 侧二进制、WAL、Snapshot、fixture digest 与恢复演练 profile 的只读部署证据包。
