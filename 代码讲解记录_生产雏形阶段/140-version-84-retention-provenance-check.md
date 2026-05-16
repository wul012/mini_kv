# 140. v84 runtime evidence retention / provenance check

## 本版目标

v84 继续按 `D:\nodeproj\orderops-node\docs\plans\v209-managed-audit-dry-run-roadmap.md` 推进 mini-kv 侧任务：给运行时只读证据补一个 `retention_provenance_check` 节点，让 Node v211 在做 managed audit identity approval provenance dry-run packet 前，可以同时核对“证据留存路径”和“二进制来源 digest”。

它不是什么也很重要：它不是新的写入口，不生成真实 audit record，不打开 `UPSTREAM_ACTIONS_ENABLED`，不执行 `LOAD`、`COMPACT`、`SETNXEX`、`RESTORE`，也不把 mini-kv 放进 Java 订单事务链。mini-kv 在这里仍然只提供可读证据。

## 1. 版本和运行时字段

`CMakeLists.txt` 把工程版本升到 `0.84.0`。这个版本会进入生成的 `minikv/version.hpp`，再由 `INFOJSON`、`SMOKEJSON`、`HEALTH`、fixture 测试一起消费。

`src/command.cpp` 新增 `RuntimeRetentionProvenanceCheck`：

```text
consumer = Node v211 managed audit identity approval provenance dry-run packet
artifact_path_hint = c/84/
release_manifest_path = fixtures/release/verification-manifest.json
runtime_smoke_evidence_path = fixtures/release/runtime-smoke-evidence.json
retention_source_path_hint = c/81/
provenance_source = binary_provenance.provenance_digest
read_only = true
execution_allowed = false
managed_audit_write_executed = false
```

这些字段的控制面含义是：Node 可以把它当成 dry-run 前的核对材料，但不能把它当成生产审计写入凭证。`retention_source_path_hint=c/81/` 指向历史 artifact retention 证据来源，`artifact_path_hint=c/84/` 指向本版运行调试归档，二者通过 `expected_binary_provenance_digest` 和 `check_digest` 串起来。

## 2. digest 契约

本版保留 v83 的 `binary_provenance` 节点，但把版本、构建目录和归档目录推进到 v84：

```text
source_version = 0.84.0
artifact_path_hint = c/84/
runtime_binary_hint = cmake-build-v84/minikv_server and cmake-build-v84/minikv_client from the current CMake build
provenance_digest = fnv1a64:c682f9c827129e40
production_binary_claimed = false
```

新增的 `retention_provenance_check` 再引用这个 digest：

```text
expected_binary_provenance_digest = fnv1a64:c682f9c827129e40
check_digest = fnv1a64:357cc7e9eec3f223
managed_audit_write_executed = false
```

`check_digest` 的输入包含项目版本、本版归档路径、release manifest 路径、runtime smoke evidence 路径、历史 retention source path，以及当前 binary provenance digest。它是证据指纹，不是签名，也不是安全认证。

## 3. INFOJSON / SMOKEJSON 怎么给 Node 读

`INFOJSON` 现在在 `ci_evidence`、`artifact_retention`、`binary_provenance` 后面追加 `retention_provenance_check`。这让 Node 不必跑写命令，就能读到 v84 的路径和 digest 对齐信息。

`SMOKEJSON` 同样追加该节点，并把 `diagnostics.node_consumption` 更新到 Node v211：

```text
Node v211 may verify runtime evidence retention, binary provenance digest alignment, live-read session echo...
```

`diagnostics.notes` 新增 `retention_provenance_check`，表示这是 runtime smoke 的一部分，但仍然是只读聚合证据。它和 `read_only=true`、`execution_allowed=false`、`order_authoritative=false`、`managed_audit_write_executed=false` 一起构成边界。

## 4. fixture 和 README

`fixtures/readonly/infojson-empty-inline.json` 是精确 `INFOJSON` inline 样本。本版同步加入 `retention_provenance_check`，并清理为无 BOM、无尾换行，避免精确比较因为隐藏字节失败。

`fixtures/release/runtime-smoke-evidence.json` 升到 `mini-kv-runtime-smoke-evidence.v9`，面向 Node v211，补充：

```text
retention_provenance_check.expected_binary_provenance_digest
retention_provenance_check.check_digest
retention_provenance_check.managed_audit_write_executed
```

`fixtures/release/verification-manifest.json` 同步记录 v84 的 CMake、CTest、真实 smoke 目标和 retention provenance check 断言。`README.md` 补充 v84 manifest 与 runtime smoke evidence 的说明，强调它是 Node v211 dry-run packet 前的只读核对材料。

## 5. 测试保护了什么

`tests/command_tests.cpp` 直接执行 `SMOKEJSON`，保护运行时 JSON 必须包含 v84 的 `binary_provenance` 和 `retention_provenance_check`，并且 `managed_audit_write_executed=false`。

`tests/readonly_fixture_tests.cpp` 用真实 `INFOJSON` 输出精确对比 fixture，保护外部控制面依赖的样本形状不漂移。

`tests/runtime_smoke_evidence_tests.cpp` 同时检查 fixture 和真实 `SMOKEJSON`，保护 Node v211 所需字段、暂停边界、no-write 语义和 digest 字段都存在。

`tests/release_verification_manifest_tests.cpp` 保护 release manifest 中的命令、目标测试、真实 smoke 期望和 v84 digest 契约。

本轮本地验证结果是 CMake 构建成功、CTest `31/31` 通过，真实 TCP smoke 也通过。真实 smoke 只执行 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`，其中 `GET restore:real-read-token` 返回 `(nil)`，说明没有创建 restore token。

## 一句话总结

v84 把 mini-kv 的运行时证据从“能说明二进制来源”推进到“能说明证据留存路径和二进制来源已对齐”，为 Node v211 的 managed audit dry-run packet 提供了更完整但仍然只读的 C++ 侧证据。
