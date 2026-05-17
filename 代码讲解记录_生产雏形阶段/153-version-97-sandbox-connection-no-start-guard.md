# mini-kv v97 sandbox connection no-start guard receipt

本版目标是给 Node v231 的 manual sandbox connection preflight verification 增加一份只读的 `managed_audit_sandbox_connection_no_start_guard_receipt`。它不是新的连接入口，不会启动 Node、Java 或 mini-kv，不会打开 manual window，不会执行 sandbox connection，不会读取 credential value，不会运行 schema rehearsal，不会写 managed audit state，也不会让 mini-kv 成为 sandbox audit storage backend 或 Java order authority。

为什么做：v96 已经提供了 sandbox connection receipt echo marker，证明 mini-kv 只回声 Node v228 operator packet 的字段名。v97 在这个历史回声证据之上再补一层 preflight guard，让控制面能先确认“窗口默认关闭、没有任何自动启动权限”，再进入后续人工沙箱连接核对。

## 实现位置

`include/minikv/managed_audit_receipts.hpp` 新增 `sandbox_connection_no_start_guard_receipt_digest()` 和 `format_sandbox_connection_no_start_guard_receipt_json()`。这延续 v92 之后的边界：managed audit receipt 的 digest 和 JSON 格式化都在 receipt 模块里，不回流到 `src/command.cpp`。

`src/managed_audit_receipts.cpp` 新增 `RuntimeManagedAuditSandboxConnectionNoStartGuardReceipt`。它消费历史 v96 echo marker：`consumed_release_version="v96"`、`consumed_artifact_path_hint="c/96/"`、`consumed_marker_digest="fnv1a64:b9fc556875ea625b"`，并发布本版 guard digest `fnv1a64:9ff902c1a090c0b7`。这里保留历史 v96 digest 是关键点，v97 不是重写旧证据，而是在旧证据上声明 no-start preflight guard。

`src/command_response_formatters.cpp` 把 `managed_audit_sandbox_connection_no_start_guard_receipt` 同时加入 `INFOJSON` 和 `SMOKEJSON`。`SMOKEJSON` 的 diagnostics 推进到 Node v231，说明控制面可以先验证 no-start guard，再继续验证 v96 echo marker、v95 sandbox adapter non-participation receipt、v90/v89/v88/v87/v86 历史链，以及 live-read、binary provenance、retention、taxonomy、operator-window、CI 和 artifact-retention 证据。

## 字段含义

`manual_window_flag_name` 固定为 `ORDEROPS_MANAGED_AUDIT_MANUAL_SANDBOX_WINDOW_APPROVED`，但 `manual_window_open_by_default=false`。控制面读到这里时，应理解为 mini-kv 只公开 flag 名称给 preflight 校验，不打开窗口，也不把 flag 当作自动启动许可。

`node_auto_start_allowed=false`、`java_auto_start_allowed=false`、`mini_kv_auto_start_allowed=false` 是本版核心边界。Node v231 可以验证 mini-kv 已经运行且人工只读窗口已开启，但不能因为这份 receipt 去启动任何进程。

`connection_execution_allowed=false`、`credential_value_read_allowed=false`、`schema_rehearsal_execution_allowed=false`、`managed_audit_write_allowed=false`、`participates_in_sandbox_connection=false` 继续把 mini-kv 固定在 runtime evidence provider 的角色上。它不参与连接，不读 secret，不跑 rehearsal，不写状态。

## Fixture 和测试

`fixtures/readonly/infojson-empty-inline.json`、`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 同步到 v97 / Node v231。release fixture 现在明确记录 no-start guard 的 consumed v96 marker、v97 digest、manual window 默认关闭、Node/Java/mini-kv 不自动启动、schema rehearsal 不执行等字段。

`tests/managed_audit_receipts_tests.cpp` 直接保护 echo marker 和 no-start guard 的 JSON/digest。`tests/command_tests.cpp`、`tests/readonly_fixture_tests.cpp`、`tests/runtime_smoke_evidence_tests.cpp` 和 `tests/release_verification_manifest_tests.cpp` 保护真实 `INFOJSON` / `SMOKEJSON` 输出、fixture、diagnostics 和 no-execution 边界。

本版验证包括 CMake configure、CMake build、32 个 CTest 全部通过，以及真实 TCP smoke：`SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`。真实 smoke 证明新 guard 出现在运行输出中，`GET restore:real-read-token` 仍为 `(nil)`，没有执行 `SETNXEX`、`LOAD`、`COMPACT` 或 restore。

一句话总结：v97 给 Node v231 增加了可验证的 manual sandbox connection no-start guard，同时继续把 mini-kv 固定为只读证据提供方，为 Node/Java/mini-kv 的松耦合融合保留清晰边界。
