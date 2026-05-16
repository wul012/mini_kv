# 146. v90 external adapter non-participation receipt

## 本版目标

v90 的目标是给 Node v223 managed audit external adapter connection readiness review 一份 mini-kv 侧的只读回执：`managed_audit_external_adapter_non_participation_receipt`。Node 可以在准备真实外部 managed audit adapter 连接前读取 `INFOJSON` / `SMOKEJSON`，确认 mini-kv 只是 runtime evidence provider。

这不是新的执行入口，不是 external adapter storage backend，不读取 managed audit credentials，不执行 external adapter schema migration，不参与真实 external adapter，不写 managed audit state，不写 local dry-run records，不开放 `LOAD` / `COMPACT` / `SETNXEX` / `RESTORE`，也不改变写/admin/WAL/snapshot/restore handler。它只把“mini-kv 不参与真实外部 adapter”的边界变成可读、可测、可归档的运行证据。

## 核心代码入口

### `src/command.cpp`

本版新增 `RuntimeManagedAuditExternalAdapterNonParticipationReceipt`，字段分三类：

```text
消费链路: consumer, consumed_by, consumed_release_version, consumed_artifact_path_hint, consumed_receipt_digest
当前证据: current_artifact_path_hint, adapter_target, runtime_role, receipt_digest
边界布尔值: read_only, execution_allowed, external_adapter_storage_backend, participates_in_external_adapter, credential_read_allowed, migration_execution_allowed, schema_authoritative, write/admin/restore/write-state flags, order_authoritative
```

当前常量说明了 v90 消费 v89 adapter shell non-storage guard receipt：

```text
consumed_release_version = v89
consumed_artifact_path_hint = c/89/
consumed_receipt_digest = fnv1a64:76411286a0913dc8
current_artifact_path_hint = c/90/
adapter_target = real external managed audit adapter
runtime_role = runtime evidence provider only
receipt_digest = fnv1a64:0dfb07cd2f8de289
```

`managed_audit_external_adapter_non_participation_receipt_digest()` 把版本、上游 v89 回执、当前归档路径、adapter target、runtime role 和全部边界布尔值压成稳定 `fnv1a64` 指纹。`format_runtime_managed_audit_external_adapter_non_participation_receipt_json()` 把这些字段输出到 JSON。

### `INFOJSON` 和 `SMOKEJSON`

`INFOJSON` 新增 `managed_audit_external_adapter_non_participation_receipt`，让控制面在读 identity evidence 时就能看到外部 adapter 非参与边界。

`SMOKEJSON` 同步新增同一回执，并在 `notes` 中加入 `managed_audit_external_adapter_non_participation_receipt`。`diagnostics.node_consumption` 更新到 Node v223 语义：Node 可以核验 v90 外部 adapter 非参与回执、v89 adapter shell guard、v88 command dispatch receipt、历史 retention/restore/storage receipts、live-read session、taxonomy、CI 和 artifact retention evidence，然后再进入 external adapter connection readiness review。

## 关键字段怎么理解

`read_only=true` 和 `execution_allowed=false` 表示这份回执只能被读取，不能被解释成允许 mini-kv 执行 adapter、restore、migration 或 credential 行为。

`external_adapter_storage_backend=false` 是本版核心边界：mini-kv 不作为真实 external managed audit adapter 的存储后端。

`participates_in_external_adapter=false` 表示 mini-kv 不加入真实外部 adapter 运行链路，只提供运行证据。

`credential_required=false` 和 `credential_read_allowed=false` 表示 mini-kv 不需要也不允许读取生产 managed audit credentials。

`migration_required=false` 和 `migration_execution_allowed=false` 表示 mini-kv 不执行 external adapter schema migration。

`schema_authoritative=false`、`managed_audit_write_executed=false`、`local_dry_run_records_written=false` 和 `order_authoritative=false` 继续守住职责边界：mini-kv 不是 schema authority，不写 managed audit 状态，不写本地 adapter dry-run 记录，也不是 Java order authority。

## Fixture 和文档同步

`fixtures/readonly/infojson-empty-inline.json` 对齐当前 `INFOJSON`，新增 v90 回执并把当前版本字段推进到 `0.90.0` / `c/90/`。

`fixtures/release/runtime-smoke-evidence.json` 推进到 `mini-kv-runtime-smoke-evidence.v12`，消费目标改为 Node v223 managed audit external adapter connection readiness review，`node_must_verify` 明确要求核验 v90 回执的 consumed v89 digest、当前 c/90 路径、external adapter 非参与字段、credential/migration 禁止字段，以及历史证据链。

`fixtures/release/verification-manifest.json` 把 v90 的构建、测试、真实 smoke 预期、fixture inventory 和新回执收进统一验收清单。

`README.md` 更新当前版本说明，明确 v90 只提供外部 adapter 非参与证据，不扩大 mini-kv 的职责。

## 测试保护

`command_tests.cpp` 从真实 `SMOKEJSON` 输出验证新回执、digest、Node v223 消费说明和 notes 顺序。

`readonly_fixture_tests.cpp` 从真实 `INFOJSON` 输出验证新回执字段，确保 fixture 与运行态一致。

`runtime_smoke_evidence_tests.cpp` 保护 v12 runtime smoke fixture：它要求 `node_must_verify` 覆盖 consumed v89 receipt、external adapter 非参与、credential/migration 禁止、历史 receipts 和 no-write/no-restore 边界，同时检查真实 `SMOKEJSON` 也输出同样的回执。

`release_verification_manifest_tests.cpp` 保护 release manifest：CMake version、targeted tests、read-only smoke expectations、boundary notes 和 v90 回执必须同时存在。

一批历史 release fixture 测试继续更新当前运行态版本断言到 `0.90.0`，因为这些测试会读取当前 `INFOJSON` / `SMOKEJSON` 输出。

## 真实运行和归档

本版验证使用 CLion CMake/CTest 工具链：

```text
cmake -S . -B cmake-build-v90 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-v90 --parallel 2
ctest --test-dir cmake-build-v90 --output-on-failure
```

真实 TCP smoke 启动 `minikv_server 6429 127.0.0.1`，再用 `minikv_client` 发送：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

smoke 证明 `managed_audit_external_adapter_non_participation_receipt.receipt_digest=fnv1a64:0dfb07cd2f8de289`，并且 `external_adapter_storage_backend=false`、`participates_in_external_adapter=false`、`credential_read_allowed=false`、`migration_execution_allowed=false`、`managed_audit_write_executed=false`、`order_authoritative=false`，`GET restore:real-read-token` 仍为 `(nil)`。服务端验证后已停止。

归档文件在：

```text
c/90/图片/
c/90/解释/说明.md
```

## 一句话总结

v90 把“mini-kv 不参与真实外部 managed audit adapter，只提供只读运行证据”固化成可核验回执，为 Node/Java/mini-kv 后续松耦合融合继续守住外部 adapter、credential、migration 和 order authority 边界。
