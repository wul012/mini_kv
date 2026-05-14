# 第一百二十三次讲解：第六十七版 Restore Dry-Run Operator Package

本版目标是按最新跨项目计划从 rollback window readiness 继续推进到真实生产化缺口收敛的第一步。Java v57、mini-kv v66、Node v166 已经完成回滚窗口 readiness，下一步推荐并行 Java v58 和 mini-kv v67：Java 侧补 rollback SQL review gate sample，mini-kv 侧补 restore dry-run operator package。

它不是新的 server 命令，不新增 runtime API，不执行 `LOAD`、`COMPACT`、`SETNXEX`，不启动 Node 或 Java，不接生产数据库，也不把 mini-kv 写入 Java 订单一致性链路。它只是一份只读 operator package，让后续 Node v167 能消费 mini-kv 的 restore target、artifact digest 占位、WAL/Snapshot compatibility 和 CHECKJSON dry-run 命令。

本版来自最新跨项目计划：

```text
D:\nodeproj\orderops-node\docs\plans\v166-post-rollback-window-roadmap.md
```

计划要求当前轮到：

```text
推荐并行：Java v58 + mini-kv v67
mini-kv v67 做 restore dry-run operator package
记录 target release / binary digest / fixture digest
记录 WAL/Snapshot compatibility confirmation
记录 CHECKJSON dry-run 命令
明确 LOAD/COMPACT/SETNXEX 不执行
明确 restore package 不能产生 Java order authority
```

因此本版只推进 mini-kv / C++ 仓库，不修改 Node 或 Java。

## 项目阶段

mini-kv 仍处在“生产雏形阶段”，但本版从“回滚窗口 checklist 所需 handoff”继续靠近“执行前 preflight”。

v66 解决的是：

```text
回滚窗口前，人工应该确认 binary/WAL/Snapshot/fixture 兼容性哪些字段？
```

v67 解决的是：

```text
如果进入 rollback execution preflight，operator dry-run package 应该带哪些 target、digest 和 CHECKJSON 命令？
```

这个 package 不替 operator 做决定，也不做 restore。它只是把 dry-run 时要看的对象和命令固定成一份可被 Node v167 读取的样本。

## Operator Package Fixture

新增文件：

```text
fixtures/release/restore-dry-run-operator-package.json
```

核心字段：

```text
package_version=mini-kv-restore-dry-run-operator-package.v1
project=mini-kv
project_version=0.67.0
release_version=v67
path=fixtures/release/restore-dry-run-operator-package.json
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
consumer_hint=Node v167 rollback execution preflight contract
```

控制面读到这些字段时应该先知道：这是 operator dry-run package，不是自动恢复入口。`restore_execution_allowed=false` 继续说明，即便它列出了 restore target 和 dry-run 命令，也不能执行 `LOAD`、`COMPACT` 或 `SETNXEX`。

## Target 与 Digest

package 新增：

```text
restore_target
artifact_digests
fixture_inputs
```

`restore_target` 记录：

```text
target_release_version=v67
current_release_version=v67
previous_evidence=verification-manifest.json / runtime-artifact-bundle-manifest.json / restore-compatibility-handoff.json
operator_confirmation_required=true
```

它只说明“本次 dry-run 要核对哪个 release target”，不切流，不执行 restore。

`artifact_digests` 记录三类 operator 占位：

```text
binary-digest
fixture-digest
handoff-digest
```

这里使用：

```text
sha256:<operator-recorded-binary-digest>
sha256:<operator-recorded-fixture-digest>
sha256:<operator-recorded-handoff-digest>
```

这是有意的。当前版本不尝试伪造生产 artifact digest，而是明确这些 digest 需要 operator 在真实交付流程外部记录和验证。

`fixture_inputs` 列出 package 需要一起复核的输入：

```text
verification-manifest.json
infojson-empty-inline.json
ttl-token/recovery-evidence.json
```

这样 package 不是孤立样本，而是明确指向版本身份、release manifest 和 TTL/WAL 恢复边界证据。

## Compatibility 与 Dry-Run 命令

package 把兼容确认拆成：

```text
wal-compatibility
snapshot-compatibility
token-write-risk
```

`wal-compatibility` 记录 WAL format、checksum policy、`SETEXAT`、`EXPIREAT`、replay report、compact risk。它只能说明 WAL replay 风险，不能修 Java transaction state。

`snapshot-compatibility` 记录 snapshot format、snapshot source、`load_replaces_store_acknowledged`、backup location、restore window。`LOAD` 仍然只能被 `CHECKJSON` 解释，不执行。

`token-write-risk` 记录 `SETNXEX` 写风险、目标 WAL 状态、TTL 边界。它用 `CHECKJSON SETNXEX dryrun:token 30 value` 解释风险，再用 `GET dryrun:token` 证明没有写入。

本版 dry-run 命令：

```text
INFOJSON
CHECKJSON LOAD data/dry-run-restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX dryrun:token 30 value
STORAGEJSON
HEALTH
GET dryrun:token
QUIT
```

## Release Manifest 跟随

更新文件：

```text
CMakeLists.txt
fixtures/release/verification-manifest.json
fixtures/readonly/infojson-empty-inline.json
```

版本从：

```text
0.66.0
```

推进到：

```text
0.67.0
```

`verification-manifest.json` 新增：

```text
fixtures/release/restore-dry-run-operator-package.json
minikv_restore_dry_run_operator_package_tests
```

同时保留 v64 rollback evidence、v65 bundle manifest、v66 restore compatibility handoff 作为历史来源，不把历史样本改写成 v67。

## 新增测试

新增文件：

```text
tests/restore_dry_run_operator_package_tests.cpp
```

新增 CTest 目标：

```text
minikv_restore_dry_run_operator_package_tests
```

测试分五层：

```text
1. 检查 package fixture 的版本、只读边界、restore_execution_allowed=false 和 consumer_hint
2. 检查 restore target、binary/fixture/handoff digest 占位都存在
3. 检查 WAL/Snapshot/token-write-risk 三类 compatibility confirmation
4. 用真实 CommandProcessor 执行 INFOJSON、CHECKJSON、STORAGEJSON、GET
5. 确认 LOAD、COMPACT、SETNXEX 只被解释，dryrun:token 仍然不存在
```

原有测试同步到当前运行版本 `0.67.0`，但旧 fixture 自己的 `project_version=0.64.0`、`0.65.0`、`0.66.0` 保持不变，因为它们代表历史证据来源。

## README 与 Roadmap

README 的当前版本更新为 Version 67，并新增：

```text
fixtures/release/restore-dry-run-operator-package.json
minikv_restore_dry_run_operator_package_tests
```

Roadmap 推进为：

```text
Java v58 + mini-kv v67 并行补 rollback SQL review / restore dry-run operator package
Node v167 等待两边完成后生成 rollback execution preflight contract
```

这和跨项目计划保持一致。

## 真实运行

本版真实 smoke 启动 mini-kv server，并执行：

```text
INFOJSON
CHECKJSON LOAD data/dry-run-restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX dryrun:token 30 value
STORAGEJSON
HEALTH
GET dryrun:token
QUIT
```

验证目标：

```text
INFOJSON -> version=0.67.0
CHECKJSON LOAD -> read_only=true, execution_allowed=false, side_effects=store_replace_from_snapshot
CHECKJSON COMPACT -> read_only=true, execution_allowed=false, side_effects=wal_rewrite_when_enabled
CHECKJSON SETNXEX -> read_only=true, execution_allowed=false, write_command=true
STORAGEJSON -> order_authoritative=false
GET dryrun:token -> (nil)
```

本轮 smoke 只运行 mini-kv，不启动 Java，不启动 Node，不执行真实 restore、rollback 或生产操作。

## 归档

运行调试归档继续写入：

```text
c/67/图片/
c/67/解释/说明.md
```

历史目录 `a/` 和 `b/` 保留，不搬迁。

## 边界

本版继续保持：

```text
不新增 runtime 命令
不修改 Node
不修改 Java
不启动 Node 或 Java
不打开 UPSTREAM_ACTIONS_ENABLED=true
不进入 Java 订单权威链路
不让 mini-kv 成为订单权威存储
不执行 LOAD / COMPACT / SETNXEX
不执行 restore 或 rollback
不伪造生产 artifact digest
```

如果未来 Node v167 发现 Java v58 和 mini-kv v67 的 preflight evidence 不一致，正确行为是暂停并让用户确认，而不是自动恢复或回滚。

## 一句话总结

v67 让 mini-kv 从“restore compatibility handoff 可人工核对”推进到“restore dry-run operator package 可被 preflight 消费”，为 Node v167 的跨项目 rollback execution preflight contract 补上了 C++ 侧的执行前材料。
