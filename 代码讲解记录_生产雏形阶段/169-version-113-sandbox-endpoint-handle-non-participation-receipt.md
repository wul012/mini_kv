# v113 沙箱端点句柄非参与回执

## 目标和边界

本版给 Node v259 增加一个只读上游回显点：mini-kv 现在通过 `sandbox_endpoint_handle_non_participation_receipt` 暴露 Node v258 沙箱端点句柄预检审查的非参与证据。

它不是连接实现，不是凭证解析器，不是 managed audit 存储后端，也不是新的执行入口。它不会启动 mini-kv 或 Java，不会解析 raw endpoint URL，不会读取 credential value，不会发送外部请求，不会写 approval ledger 或 managed audit state，也不会执行 `LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。

## 版本位置

Node v258 只审查 endpoint handle、credential handle，以及周边的 network allowlist、TLS policy、redaction policy 和关闭状态的 operator window。mini-kv v113 把这个形状镜像成证据，让后续 Node v259 的 upstream echo 能确认 mini-kv 仍然在沙箱连接之外。

本版也顺手控制了重构节奏：`src/runtime_evidence_receipts.cpp` 已经增长到 1100 行以上，所以较新的 sandbox receipt 实现被移到 `src/runtime_sandbox_receipts.cpp`。对外公开头文件仍然是 `include/minikv/runtime_evidence_receipts.hpp`。

## 代码模块

`src/runtime_sandbox_receipts.cpp` 负责 v111、v112、v113 这类 sandbox 风格 receipt 的 JSON formatter 和 digest 函数。对 v113 来说，它定义了 Node v258 的来源字段、审查计数、handle 名称、policy handle、边界布尔值，以及 `sandbox_endpoint_handle_non_participation_receipt_digest`。

`src/runtime_evidence_receipts.cpp` 保留核心 runtime evidence 链：live-read session、failure taxonomy、operator-window proof、CI/artifact retention hints、binary provenance、retention check、retention replay marker，以及更早的 manual sandbox command/precheck receipts。

`src/command_response_formatters.cpp` 把新 receipt 接入 `SMOKEJSON` 和 diagnostics notes。diagnostics 字符串现在说明 Node v259 可以在 Node v258 endpoint handle preflight review 之后验证 mini-kv v113 receipt，同时保留既有的 Node v246/v257/v254/v244/v239 提示。

## 契约字段

v113 receipt 回显了 Node v258 的这些上游事实：

- `source_required_review_item_count=7`、`source_completed_review_item_count=7`、`source_forbidden_operation_count=7`、`source_check_count=19`。
- `endpoint_handle` 和 `credential_handle` 都只是 handle 名称。
- `network_allowlist_review`、`tls_policy_review`、`redaction_policy`、`operator_window` 都只表示已审查，不暴露 raw host、CIDR、certificate material、private key、raw endpoint URL 或 credential value。
- `source_ready_for_managed_audit_sandbox_adapter_connection=false`、`source_external_request_sent=false`、`source_raw_endpoint_url_parsed=false`、`source_credential_value_read=false`、`source_automatic_upstream_start=false`。

控制面读取方应把 `read_only=true` 和 `execution_allowed=false` 理解为“仅证据”。`mini_kv_auto_start_allowed=false`、`storage_write_allowed=false`、`managed_audit_storage_backend=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`order_authoritative=false` 是硬边界，不是运行时开关。

## Fixture 和测试

`fixtures/release/sandbox-endpoint-handle-non-participation-receipt.json` 是 v113 独立 receipt。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 内嵌同一份 receipt，因此 Node 既可以消费独立 fixture，也可以消费 `SMOKEJSON` 聚合结果。

`tests/sandbox_endpoint_handle_non_participation_receipt_tests.cpp` 检查独立 fixture、runtime smoke fixture、release manifest、真实 `CommandProcessor` 的 `SMOKEJSON`，以及 `GET restore:real-read-token`。测试会拒绝意外出现的 credential value 或 raw endpoint URL 字段，并验证 no-start、no-write、no-execution 和 non-authoritative 标志。

既有 runtime smoke、release manifest 和 command tests 只为新 key 和 diagnostics hint 做了扩展。它们继续守住历史 v102 runtime evidence 以及 v107-v112 receipt 契约。

## 总结

v113 给 Node/Java/mini-kv 集成链提供了干净的 endpoint-handle preflight echo，同时不让 mini-kv 成为 endpoint、credential reader、transport 或 audit store。
