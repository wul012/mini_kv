# 第七十二版：Restore Evidence Retention

本版目标是按 `v176-post-ci-evidence-hardening-roadmap.md` 的最新指导，给 mini-kv 增加一份恢复证据保留 fixture，让后续 Node v178 的 cross-project evidence retention gate 能读取 mini-kv v72 的恢复清单、artifact digest、Snapshot/WAL review 和 CHECKJSON 风险证据保留状态。

它不是什么：不是新的恢复执行入口，不执行 `LOAD` / `COMPACT` / `SETNXEX` / restore，不读取生产密钥，不启动 Node 或 Java，不进入 Java 订单事务链，也不把 mini-kv 变成订单权威存储。

## 1. 当前项目进度

v71 已经把恢复交接前的 operator、artifact digest target、Snapshot/WAL review placeholder 和 CHECKJSON 风险确认做成 `restore-handoff-checklist.json`。v72 往后再收一小步：不再新增执行能力，而是把“这些证据应如何被保留、由谁保留、保留多久、Node v178 怎么消费”固化成只读 fixture。

这版仍然不碰 WAL、Snapshot、restore、TCP 协议和命令执行器核心逻辑。它只新增证据保留样本、测试、manifest 入口、README 和讲解记录。

## 2. CMakeLists.txt

本版把 CMake 项目版本从 `0.71.0` 提升到 `0.72.0`。运行时 `INFOJSON` 仍通过生成的 `minikv/version.hpp` 暴露同一个版本号。

新增测试目标：

```text
minikv_restore_evidence_retention_tests
```

这个测试目标注入 `MINIKV_SOURCE_DIR`，读取仓库里的 retention fixture，并用真实 `CommandProcessor` 验证 `CHECKJSON` 只解释风险，不执行恢复相关命令。

## 3. restore-evidence-retention.json

新增文件：

```text
fixtures/release/restore-evidence-retention.json
```

它的角色是“恢复证据保留样本”，不是恢复执行器。核心边界字段是：

```text
read_only=true
execution_allowed=false
restore_execution_allowed=false
order_authoritative=false
consumer_hint=Node v178 cross-project evidence retention gate
```

控制面读到这些字段时应该理解为：mini-kv 只说明恢复相关证据需要如何保留，真实 artifact、operator identity、Snapshot/WAL review 仍由外部流程记录和审核，mini-kv 不保存生产秘密值，也不授权恢复。

`retention_target` 记录本版目标是 `v72`，引用 v71 的 `restore-handoff-checklist.json` 和当前 `verification-manifest.json`。关键字段包括：

```text
retention_id
artifact_digest_placeholder
snapshot_review_retention
wal_review_retention
retention_days
operator_identity_placeholder
```

这些字段给 Node v178 一个稳定读法：retention gate 可以检查证据是否被保留、保留字段是否齐全、是否仍然只是 placeholder，但不能据此执行真实恢复。

## 4. CHECKJSON 证据保留

本版使用的只读 smoke 命令是：

```text
INFOJSON
CHECKJSON LOAD data/retention-restore.snap
CHECKJSON COMPACT
CHECKJSON SETNXEX restore:retention-token 30 value
STORAGEJSON
HEALTH
GET restore:retention-token
QUIT
```

关键点仍是 `CHECKJSON`。`CHECKJSON LOAD` 只保留 `store_replace_from_snapshot` 风险证据，不执行 `LOAD`；`CHECKJSON COMPACT` 只保留 `wal_rewrite_when_enabled` 风险证据，不执行 `COMPACT`；`CHECKJSON SETNXEX` 只保留写入和 WAL side effect 说明，不声明 token。

所以 `GET restore:retention-token` 必须返回 `(nil)`。这证明 retention fixture 只记录风险证据保留要求，没有写入真实 token，也没有替换 store 状态。

## 5. verification-manifest 和 README

`fixtures/release/verification-manifest.json` 升级为 v72，新增：

```text
minikv_restore_evidence_retention_tests
fixtures/release/restore-evidence-retention.json
CHECKJSON LOAD data/retention-restore.snap
CHECKJSON SETNXEX restore:retention-token 30 value
GET restore:retention-token
```

README 同步更新 Current version、fixture 列表、CLion target 列表、release fixture 说明和 Roadmap。当前跨项目方向对齐为：Node v177 已完成 operator identity evidence，Java v63 与 mini-kv v72 推荐并行补 retention fixture，Node v178 等待三方证据后再做 cross-project evidence retention gate。

## 6. 测试和真实运行

`tests/restore_evidence_retention_tests.cpp` 分两层保护：

第一层保护 fixture 合同：确认 v72 版本、只读边界、retention id、artifact/Snapshot/WAL placeholder、retention owner、retention days、required confirmations、warnings、pause conditions 和输入 fixture 路径都存在。

第二层保护运行时解释语义：用真实 `CommandProcessor` 执行 `INFOJSON`、`CHECKJSON LOAD`、`CHECKJSON COMPACT`、`CHECKJSON SETNXEX`、`STORAGEJSON` 和 `GET restore:retention-token`，确认版本是 `0.72.0`，确认 `CHECKJSON` 不执行真实写入，确认 `STORAGEJSON` 仍然 `order_authoritative=false`，确认 token 查询仍是 `(nil)`。

已有 release evidence 测试里的运行时版本断言同步到 `0.72.0`。历史 fixture 自己的 `project_version`、`release_version` 和 consumer hint 不重写，避免把 v66-v71 的历史证据伪装成新版本。

## 7. 一句话总结

v72 把 mini-kv 从“恢复交接清单可被 review”推进到“恢复证据保留可被 Node retention gate 消费”，同时继续守住只读证据、外部人工审核、非订单权威和不执行恢复的边界。
