# 141. v85 retention provenance replay marker

## 本版目标

v85 按 `D:\nodeproj\orderops-node\docs\plans\v211-post-managed-audit-packet-roadmap.md` 推进 mini-kv 侧任务：补一个 `retention_provenance_replay_marker`，让后续 Node v213 在生成 managed audit packet restore drill plan 前，能只读确认 Node v211 已经消费过 mini-kv v84 的 retention provenance evidence。

它不是什么同样重要：它不是 restore 执行记录，不是 managed audit 写入凭证，不会打开 `UPSTREAM_ACTIONS_ENABLED`，不会执行 `LOAD`、`COMPACT`、`SETNXEX`、`RESTORE`，也不会让 mini-kv 成为 Java 订单权威。mini-kv 在这里继续扮演 C++ 侧只读 evidence provider。

## 1. 版本和运行时入口

`CMakeLists.txt` 把工程版本推进到 `0.85.0`，所以 `INFOJSON`、`SMOKEJSON`、fixture 和 release manifest 都能明确标识当前 C++ 证据版本。

`src/command.cpp` 是本版核心入口。它保留 v84 已有的 `binary_provenance` 和 `retention_provenance_check`，再新增：

```text
RuntimeRetentionProvenanceReplayMarker
 -> consumer = Node v213 managed audit packet restore drill plan
 -> consumed_by = Node v211 managed audit identity approval provenance dry-run packet
 -> consumed_release_version = v84
 -> consumed_artifact_path_hint = c/84/
 -> consumed_check_digest = fnv1a64:357cc7e9eec3f223
 -> current_artifact_path_hint = c/85/
 -> replay_executed = false
 -> managed_audit_write_executed = false
```

这里刻意同时出现 `v84/c/84` 和 `v85/c/85`。前者表示被 Node v211 消费过的历史证据，后者表示本版生成这个消费回执的当前证据位置。这个区分是本版最关键的边界。

## 2. digest 契约

本版新增 `retention_provenance_replay_marker_digest()`，输入包括：

```text
mini-kv-retention-provenance-replay-marker
version = 0.85.0
consumed_by
consumed_release_version
consumed_artifact_path_hint
consumed_check_digest
current_artifact_path_hint
read_only / execution_allowed / replay_executed / managed_audit_write_executed
```

最终输出：

```text
marker_digest = fnv1a64:1ea4570c967cfdb1
```

控制面读到这个 digest 时，只能把它理解为“v85 对 v84 被消费事实的只读指纹”。它不是签名，不是安全认证，不代表 restore 已发生，更不代表 audit 状态已经落库。

## 3. INFOJSON / SMOKEJSON 给 Node 怎么读

`INFOJSON` 现在在 `retention_provenance_check` 后追加 `retention_provenance_replay_marker`。这让 Node 不需要发写命令，就能读到“v84 证据被 v211 消费”的回执。

`SMOKEJSON` 也追加同一节点，并把 `diagnostics.node_consumption` 推进到 Node v213：

```text
Node v213 may verify the v84 retention provenance consumption marker ... before managed audit packet restore drill planning
```

`diagnostics.notes` 增加 `retention_provenance_replay_marker`，让外部控制面可以在聚合 smoke 证据里识别这个节点。它与 `read_only=true`、`execution_allowed=false`、`replay_executed=false`、`managed_audit_write_executed=false` 一起表达边界：能读、能核对、能进入计划，但不能执行恢复或写审计。

## 4. fixture 和 release manifest

`fixtures/readonly/infojson-empty-inline.json` 精确同步 `INFOJSON` inline 空库输出，新增 replay marker 后仍保持无 BOM、无尾换行，保护外部只读样例不漂移。

`fixtures/release/runtime-smoke-evidence.json` 升级到 `mini-kv-runtime-smoke-evidence.v10`，面向 Node v213，新增以下验收点：

```text
retention_provenance_replay_marker.consumed_release_version equals v84
retention_provenance_replay_marker.consumed_artifact_path_hint equals c/84/
retention_provenance_replay_marker.consumed_check_digest equals fnv1a64:357cc7e9eec3f223
retention_provenance_replay_marker.current_artifact_path_hint equals c/85/
retention_provenance_replay_marker.marker_digest equals fnv1a64:1ea4570c967cfdb1
retention_provenance_replay_marker.replay_executed is false
```

`fixtures/release/verification-manifest.json` 同步记录 CMake、CTest、真实 TCP smoke 的预期字段，并新增边界 `retention provenance replay marker only`。README 也补充 v85 说明，强调这是 Node v213 restore drill plan 前的只读消费回执。

## 5. 测试保护了什么

`tests/command_tests.cpp` 直接执行 `SMOKEJSON`，保护运行时 JSON 必须吐出 replay marker，且必须保留 `consumed_release_version=v84`、旧 digest `fnv1a64:357cc7e9eec3f223` 和 `replay_executed=false`。

`tests/readonly_fixture_tests.cpp` 用真实 `INFOJSON` 输出精确对比 `fixtures/readonly/infojson-empty-inline.json`，防止外部控制面依赖的空库样例悄悄漂移。

`tests/runtime_smoke_evidence_tests.cpp` 同时检查 fixture 和真实 `SMOKEJSON`，保护 Node v213 需要的目标版本、消费字段、暂停条件、warning 和 no-write 语义。

`tests/release_verification_manifest_tests.cpp` 保护 release manifest 的真实 smoke 预期、边界列表、diagnostics notes 和运行时输出字段。

本轮本地验证结果：CMake 构建成功，CTest `31/31` 通过；真实 TCP smoke 使用 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`，确认 `marker_digest=fnv1a64:1ea4570c967cfdb1`，且 `GET restore:real-read-token` 返回 `(nil)`。

## 一句话总结

v85 把 mini-kv 从“能证明 v84 证据留存和来源对齐”推进到“能证明这份 v84 证据已被 Node v211 只读消费过”，为 Node v213 的恢复演练计划提供了更完整但仍严格不执行的 C++ 侧证据链。