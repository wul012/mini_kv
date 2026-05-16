# 145. v89 adapter shell non-storage guard receipt

## 本版目标

v89 的目标是给 Node v221 managed audit local adapter candidate dry-run 一份 mini-kv 侧的只读边界回执：Node 可以读取 `INFOJSON` / `SMOKEJSON`，确认 mini-kv 不是 `ManagedAuditAdapter disabled shell` 的存储后端，也不会写入 local dry-run records、managed audit state、restore 结果或 Java 订单权威状态。

这不是新的执行入口，不是 adapter storage backend，不是 managed audit store，不开放 `LOAD` / `COMPACT` / `SETNXEX` / `RESTORE`，不改写/admin/WAL/snapshot/restore handler，也不把 mini-kv 接入 Java transaction chain。它只是把“mini-kv 只能提供运行时证据，不能替 Node adapter 承担存储职责”这件事变成可核验字段。

## 本版所处阶段

mini-kv 当前已经进入生产雏形阶段：它有 TCP/RESP、WAL、Snapshot、运行时 evidence、release fixture、真实 smoke 和跨项目消费契约。v89 继续沿用小版本节奏，只在 runtime evidence 层补一个边界 receipt，让后续 Node v221 做本地 adapter 候选 dry-run 前，可以先确认数据写入职责不落到 mini-kv。

## 核心代码入口

### 1. `src/command.cpp`

本版新增 `RuntimeAdapterShellNonStorageGuardReceipt`，并定义当前运行时回执：

```text
consumer = Node v221 managed audit local adapter candidate dry-run
consumed_by = Node v220 managed audit adapter interface and disabled shell
consumed_release_version = v88
consumed_artifact_path_hint = c/88/
consumed_receipt_digest = fnv1a64:4aa6d12fb067e2a6
current_artifact_path_hint = c/89/
adapter_shell = ManagedAuditAdapter disabled shell
receipt_digest = fnv1a64:76411286a0913dc8
```

`adapter_shell_non_storage_guard_receipt_digest()` 用版本号、上游 v88 回执、当前归档路径、adapter shell 名称和所有边界布尔值计算 `fnv1a64` 指纹。这样 Node 读到字段后，不只是看到一句说明文字，还能把当前二进制/fixture 的边界状态压成一个稳定 digest。

`format_runtime_adapter_shell_non_storage_guard_receipt_json()` 负责把回执输出为 JSON，并挂入 `INFOJSON` 和 `SMOKEJSON`。这和 v88 的 `command_dispatch_quality_receipt` 是串联关系：v88 证明 runtime evidence command dispatch 没碰写/admin/restore 路径；v89 继续消费 v88 回执，证明 adapter shell dry-run 的存储职责也没有落到 mini-kv。

### 2. `SMOKEJSON` 聚合

`SMOKEJSON` 的 `notes` 新增 `adapter_shell_non_storage_guard_receipt`，`diagnostics.node_consumption` 改成 Node v221 语义。控制面读到它时应理解为：

```text
mini-kv may be read as runtime evidence
mini-kv must already be running inside an explicit read-only window
Node may verify the v89 adapter shell non-storage guard receipt
Node must not auto-start mini-kv or write candidate records into mini-kv
```

这仍然是 read adapter target，不是 execution gate。`GET restore:real-read-token` 继续用于证明 forbidden restore token 没有被写出来。

## 关键字段怎么理解

`read_only=true` 和 `execution_allowed=false` 表示这份回执只能被读取，不能解释成允许执行。

`adapter_shell_storage_backend=false` 和 `storage_backend_allowed=false` 是本版最核心的边界：Node v221 可以用 mini-kv 证明“我不是 adapter shell 的 storage backend”，不能把 mini-kv 当成本地候选 dry-run 的记录落点。

`local_dry_run_records_written=false` 和 `managed_audit_write_executed=false` 说明 mini-kv 没有写入 Node adapter 的候选记录，也没有写入 managed audit 状态。

`write_handler_changed=false`、`admin_handler_changed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false` 说明本版没有打开写命令、admin 命令、restore 或 load/compact 执行路径。

`order_authoritative=false` 继续把 mini-kv 排除在 Java order authority 之外。它可以给 Node/Java 提供证据，但不能成为订单事实来源。

## Fixture 和文档同步

`fixtures/readonly/infojson-empty-inline.json` 对齐当前 `INFOJSON`，新增 `adapter_shell_non_storage_guard_receipt`，并把 binary/retention/replay/restore/storage/dispatch 证据的当前版本推进到 `0.89.0` 和 `c/89/`。

`fixtures/release/runtime-smoke-evidence.json` 把消费目标推进到 Node v221 managed audit local adapter candidate dry-run，记录 `node_must_verify` 中必须检查的新回执字段，并保留 v84-v88 的历史证据链。

`fixtures/release/verification-manifest.json` 把 v89 的构建、测试、fixture、runtime smoke 和 receipt digest 收到同一份总验收清单里，方便后续 Node v221 只读消费。

`README.md` 同步说明 v89 的边界：mini-kv 不是 managed audit store，也不是 adapter shell storage backend；不写 local dry-run records，不写 managed audit state，不开放 restore/admin/write 路径。

## 测试保护

`command_tests.cpp` 从真实 `INFOJSON` / `SMOKEJSON` 输出检查新回执、digest、Node v221 诊断文字和 `notes` 顺序。

`readonly_fixture_tests.cpp` 保护 `INFOJSON` fixture 和真实命令输出一致，尤其是 adapter shell guard 的只读布尔字段。

`runtime_smoke_evidence_tests.cpp` 保护 release runtime smoke fixture，确认 `node_must_verify` 先要求 Node 核验 adapter shell guard，再核验 binary provenance、retention provenance、restore boundary 和 historical receipts。

`release_verification_manifest_tests.cpp` 保护总验收清单，确保 CMake 版本、runtime fixture、manifest、回执 digest 和 Node v221 目标一致。

一批历史 release fixture 测试继续更新当前运行时版本断言到 `0.89.0`，因为这些测试会读取当前 `INFOJSON` / `SMOKEJSON` 输出，版本字段必须跟当前构建一致。

## 真实运行和归档

本版验证使用 CLion CMake/CTest 工具链：

```text
cmake -S . -B cmake-build-v89 -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-v89 --parallel 2
ctest --test-dir cmake-build-v89 --output-on-failure
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

smoke 证明 `adapter_shell_non_storage_guard_receipt.receipt_digest=fnv1a64:76411286a0913dc8`、`adapter_shell_storage_backend=false`、`storage_backend_allowed=false`、`local_dry_run_records_written=false`、`managed_audit_write_executed=false`，并且 `GET restore:real-read-token` 仍然是 `(nil)`。服务端验证后停止。

归档文件在：

```text
c/89/图片/
c/89/解释/说明.md
```

## 一句话总结

v89 把“mini-kv 不承担 Node adapter shell 存储职责”变成了可读、可测、可归档的证据回执，为 Node/Java/mini-kv 的松耦合融合继续守住只读证据边界。
