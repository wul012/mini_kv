# v114 凭证解析器非参与回执

## 目标和边界

本版给 Node v261 增加一个 mini-kv 只读回显，用来对应 Node v260 的 sandbox endpoint credential resolver decision record。

它不是 credential resolver，不是 secret provider adapter，不是连接实现，也不是 managed audit 存储后端。它不会启动 mini-kv 或 Java，不会读取、加载、存储或包含 credential value，不会解析 raw endpoint URL，不会发送外部请求，不会写 approval ledger 或 managed audit state，也不会执行 schema migration、`LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。

## 版本位置

Node v260 在任何 credential resolver 演练存在之前，先记录一个 policy-only 的人工决策信封。mini-kv v114 把这个信封镜像成证据：endpoint handle、credential handle、resolver policy handle、approval marker、operator identity requirement、approval correlation requirement、redaction policy、fallback rotation plan、required decision fields 和 explicit no-go conditions。

本版有意继续放在现有 runtime sandbox receipt 模块里。加入 v114 后，`src/runtime_sandbox_receipts.cpp` 仍低于当前的大文件阈值，所以再拆一个新文件反而会让结构比改动本身更重。

## 代码模块

`src/runtime_sandbox_receipts.cpp` 现在负责 `credential_resolver_non_participation_receipt_digest` 和 `format_credential_resolver_non_participation_receipt_json`。formatter 回显 Node v260 的来源形状，记录来源计数，列出 required decision fields 和 no-go conditions，并硬编码 no credential、no resolver、no external request 边界。

`include/minikv/runtime_evidence_receipts.hpp` 通过和 v111-v113 相同的公共 receipt 接口暴露新的 digest 和 formatter。

`src/command_response_formatters.cpp` 把 `credential_resolver_non_participation_receipt` 加入 `SMOKEJSON`，把它放进 notes 数组，并新增 Node v261 diagnostics hint，同时保留更早的 v108/v112/v113/v111/v107/v102 消费提示。

## 契约字段

v114 receipt 回显了 Node v260 的这些上游事实：

- `source_required_decision_field_count=8`、`source_explicit_no_go_condition_count=9`、`source_check_count=20`。
- `endpoint_handle`、`credential_handle`、`resolver_policy_handle` 和 `approval_marker` 都只是名称或 marker。
- `operator_identity_required=true` 和 `approval_correlation_required=true` 是要求，不代表 mini-kv 推断操作者身份。
- `resolver_mode=policy-record-only-no-value-read` 和 `resolver_candidate_implementation=not-implemented` 保持 resolver 未实现。
- `source_credential_value_read=false`、`source_credential_value_loaded=false`、`source_credential_value_included=false`、`source_raw_endpoint_url_parsed=false`、`source_external_request_sent=false`、`source_automatic_upstream_start=false`。

控制面读取方应把 `read_only=true` 和 `execution_allowed=false` 理解为“仅证据”。`credential_resolver_implemented=false`、`credential_resolver_invoked=false`、`secret_provider_instantiated=false`、`mini_kv_auto_start_allowed=false`、`storage_write_allowed=false`、`managed_audit_storage_backend=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`order_authoritative=false` 才是 mini-kv 实际提供的边界证明。

## Fixture 和测试

`fixtures/release/credential-resolver-non-participation-receipt.json` 是 v114 独立 receipt。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 内嵌同一份 receipt，因此 Node 既可以消费独立 fixture，也可以消费 `SMOKEJSON` 聚合结果。

`tests/credential_resolver_non_participation_receipt_tests.cpp` 检查独立 fixture、runtime smoke fixture、release manifest、真实 `CommandProcessor` 的 `SMOKEJSON`，以及 `GET restore:real-read-token`。它会验证 Node v260 来源形状、decision record 字段、no-go conditions，以及 hard no-start/no-write/no-credential/no-resolver/no-execution 边界。

既有 runtime smoke、release manifest 和 command tests 只为新 key 和 diagnostics hint 做了扩展。它们继续守住历史 v102 runtime evidence 以及 v107-v113 receipt 契约。

## 总结

v114 让跨项目链路可以确认 credential resolver decision，但不让 mini-kv 成为 resolver、credential reader、endpoint parser、transport 或 audit store。
