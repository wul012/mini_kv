# 第七十一版：Restore Handoff Checklist

本版目标是按 `v173-post-release-window-readiness-roadmap.md` 的最新指导，给 mini-kv 增加一份恢复前人工交接清单，让后续 Node v175 的 release handoff readiness review 能引用 mini-kv v71 的恢复交接准备状态。

它不是什么：不是新的恢复执行入口，不执行 `LOAD` / `COMPACT` / `SETNXEX` / restore，不绕过人工审批，不读取生产密钥，不启动 Node 或 Java，不连接 Java 订单事务链，也不把 mini-kv 变成 Java 订单权威库。

## 1. 当前项目进度

v70 已经有 `restore-drill-evidence.json`，能说明“恢复演练风险已经通过 CHECKJSON 解释过”。v71 往前再收一小步：把真正交接前需要人工确认的 operator、artifact digest target、Snapshot/WAL review placeholder、CHECKJSON 风险确认字段固化成 fixture。

这一步仍然很谨慎。mini-kv 没有改 WAL、Snapshot、restore、命令执行器和 TCP 协议，只新增一份可被控制面读取的证据清单，以及保护它的测试。

## 2. CMakeLists.txt

本版把 CMake 项目版本从 `0.70.0` 提升到 `0.71.0`。这个版本号继续由 `include/minikv/version.hpp.in` 生成，运行时的 `INFOJSON` 和测试里的 `minikv::version` 都会读到同一个版本。

新增测试目标：

```text
minikv_restore_handoff_checklist_tests
```

它和已有 release evidence 测试一样注入 `MINIKV_SOURCE_DIR`，用于读取仓库 fixture，并用真实 `CommandProcessor` 验证 CHECKJSON 仍然只是解释命令风险。

## 3. restore-handoff-checklist.json

新增文件：

```text
fixtures/release/restore-handoff-checklist.json
```

这份 fixture 的角色是“恢复交接清单”，不是恢复执行器。核心边界字段是：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
consumer_hint=Node v175 release handoff readiness review
```

控制面读到这些字段时应该理解为：mini-kv 只提供人工交接前的可审核证据，真实恢复仍然必须在外部流程里由操作员确认，且 mini-kv 仍不是 Java 订单一致性链路的一部分。

`handoff_target` 记录本版目标是 `v71`，并引用 v70 restore drill、v69 release artifact digest package、v68 artifact digest compatibility matrix 作为前置证据。`artifact_digest_target`、`snapshot_review_placeholder`、`wal_review_placeholder` 都是占位符，表示摘要和审查结果由操作员或外部流程记录，fixture 不保存真实生产值。

`handoff_checklist` 固化交接字段：

```text
restore_operator_id
artifact_digest_target_reviewed_at
snapshot_review_recorded_at
wal_review_recorded_at
checkjson_risk_confirmed
restore_boundary_reviewed
handoff_ready_for_node_review
```

这些字段给 Node v175 一个明确读法：清单是否齐全可以被 review，但清单本身不会批准或执行恢复。

## 4. CHECKJSON 风险确认

本版使用的只读风险确认命令是：

```text
INFOJSON
CHECKJSON LOAD data/handoff-restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX restore:handoff-token 30 value
STORAGEJSON
HEALTH
GET restore:handoff-token
QUIT
```

关键仍然在 `CHECKJSON`。`CHECKJSON LOAD` 只返回 `store_replace_from_snapshot` 风险说明，不执行 `LOAD`；`CHECKJSON COMPACT` 只返回 `wal_rewrite_when_enabled` 风险说明，不执行 `COMPACT`；`CHECKJSON SETNXEX` 只返回写入和 WAL side effect 说明，不声明 token。

因此 `GET restore:handoff-token` 必须保持 `(nil)`。这个结果是本版边界的核心证据：控制面可以看到恢复相关命令风险，但 mini-kv 没有真的写数据或替换存储状态。

## 5. verification-manifest 和 README

`fixtures/release/verification-manifest.json` 升级为 v71，新增：

```text
minikv_restore_handoff_checklist_tests
fixtures/release/restore-handoff-checklist.json
CHECKJSON LOAD data/handoff-restore.snap
CHECKJSON SETNXEX restore:handoff-token 30 value
GET restore:handoff-token
```

README 同步更新 Current version、fixture 列表、CLion target 列表、stable sample 说明和 Roadmap。当前跨项目方向对齐为：Node v174 已完成 dry-run envelope，Java v62 与 mini-kv v71 推荐并行补 handoff checklist，Node v175 等待三者完成后做 release handoff readiness review。

## 6. 测试和真实运行

`tests/restore_handoff_checklist_tests.cpp` 分两层保护：

第一层保护 fixture 合同：确认 v71 版本、只读边界、operator checklist 字段、artifact/Snapshot/WAL placeholder、required confirmations、warnings、pause conditions 和所有输入 fixture 路径都存在。

第二层保护运行时解释语义：用真实 `CommandProcessor` 执行 `INFOJSON`、`CHECKJSON LOAD`、`CHECKJSON COMPACT`、`CHECKJSON SETNXEX`、`STORAGEJSON` 和 `GET restore:handoff-token`，确认版本是 `0.71.0`，确认 `CHECKJSON` 不执行真实写入，确认 `STORAGEJSON` 仍然 `order_authoritative=false`，确认 token 查询仍是 `(nil)`。

已有 release evidence 测试里的运行时版本断言同步到 `0.71.0`，但 v66-v70 历史 fixture 自己的 `project_version`、`release_version` 和 consumer hint 不改，避免重写历史证据。

## 7. 一句话总结

v71 把 mini-kv 从“恢复演练证据”推进到“恢复交接清单可被 Node handoff review 消费”的状态，同时继续守住只读证据、人工审核、非订单权威和不执行恢复的边界。
