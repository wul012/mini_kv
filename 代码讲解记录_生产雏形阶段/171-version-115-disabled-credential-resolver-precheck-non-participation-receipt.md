# v115 禁用凭证解析器预检非参与回执

## 目标和边界

本版给 Node v263 增加一个 mini-kv 只读回显，用来对应 Node v262 的 disabled credential resolver precheck。它的角色是让控制面确认“凭证解析器仍然默认关闭，而且 mini-kv 不参与解析链路”。

它不是新的执行入口，不是 credential resolver，不是 resolver client，不是 secret provider adapter，不是 endpoint parser，不是 transport，也不是 managed audit 存储后端。它不会启动 mini-kv 或 Java，不会读取、加载、存储或包含 credential value，不会解析 raw endpoint URL，不会发送外部请求，不会写 approval ledger、managed audit state 或本地 storage，也不会执行 schema migration、`LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。

## 版本位置

Node v262 已经把 disabled resolver precheck 做成显式包：6 个 env handle、2 个 opt-in gate、7 个 failure class、12 个 dry-run response field，以及继承自 v114 的 9 个 no-go condition。mini-kv v115 只把这个包镜像成证据，不提供任何真实 resolver 实现。

本版顺手做了一个合理拆分：v114 以后继续往 `src/runtime_sandbox_receipts.cpp` 塞 credential resolver 逻辑，会让 sandbox 文件重新接近大文件区间。v115 把 v114 和 v115 的 credential resolver 收据集中到 `src/runtime_credential_resolver_receipts.cpp`，让 sandbox receipt 文件继续只承担 sandbox echo 方向。

## 代码模块

`include/minikv/runtime_evidence_receipts.hpp` 继续是公共契约入口，新增 `disabled_credential_resolver_precheck_non_participation_receipt_digest` 和 `format_disabled_credential_resolver_precheck_non_participation_receipt_json`。

`src/runtime_credential_resolver_receipts.cpp` 现在负责 v114 credential resolver decision receipt 和 v115 disabled resolver precheck receipt。v115 formatter 回显 Node v262 的 env handle、gate、failure taxonomy、dry-run response shape、inherited no-go conditions、digest 和边界字段；digest 也把这些来源计数、禁用状态和 read command list 纳入稳定指纹。

`src/runtime_sandbox_receipts.cpp` 被收窄为 sandbox/manual connection 相关 receipt。拆分后它是 665 行，新的 credential resolver 文件是 555 行，核心 `src/runtime_evidence_receipts.cpp` 是 752 行，`src/command_response_formatters.cpp` 是 735 行；这比继续堆到单个 1000 行文件里更好维护。

`src/command_response_formatters.cpp` 把 `disabled_credential_resolver_precheck_non_participation_receipt` 加入 `SMOKEJSON` 聚合对象、notes 和 diagnostics，让 Node v263 可以从真实 runtime smoke 里读取同一份边界证明。

`include/minikv/client_limits.hpp` 给 bundled TCP client 增加 `client_max_response_line_bytes=512 KiB`。这是本版真实 TCP smoke 暴露出的配套修复：当前 `SMOKEJSON` 已超过旧的 64 KiB 客户端响应行上限，如果不调高，上游控制面可以通过 fixture 看见证据，但本项目自己的 `minikv_client` 无法读取同一条 runtime evidence。这个改动只影响客户端响应读取，不改变服务端 `--max-request-bytes` 请求限制，也不触碰 write/admin/WAL/snapshot/restore 行为。

## 契约字段

v115 receipt 回显了 Node v262 的这些上游事实：

- `source_required_env_handle_count=6`、`source_opt_in_gate_count=2`、`source_failure_class_count=7`、`source_dry_run_response_field_count=12`、`source_inherited_no_go_condition_count=9`。
- env handle 只有名称，不包含 credential value 或 raw endpoint URL。
- opt-in gate 的当前默认值是 `false`，precheck 会把启用状态视为阻塞项，而不是绕过审批。
- dry-run response shape 明确暴露 resolver client、secret provider、credential value、raw endpoint、external request、managed audit connection 和 schema migration 都没有发生。

控制面读取方应把 `read_only=true`、`execution_allowed=false` 和 `disabled_credential_resolver_precheck_only=true` 理解为“仅证据”。`resolver_client_instantiated=false`、`secret_provider_instantiated=false`、`credential_value_read_allowed=false`、`credential_value_load_allowed=false`、`credential_value_store_allowed=false`、`credential_value_include_allowed=false`、`raw_endpoint_url_parse_allowed=false`、`external_request_allowed=false`、`storage_write_allowed=false`、`schema_migration_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`managed_audit_storage_backend=false` 和 `order_authoritative=false` 才是 mini-kv 提供给 Node/Java 的硬边界。

## Fixture 和测试

`fixtures/release/disabled-credential-resolver-precheck-non-participation-receipt.json` 是 v115 独立 receipt。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 内嵌同一份 receipt，因此 Node 既可以消费独立 fixture，也可以消费 `SMOKEJSON` 聚合结果。

`tests/disabled_credential_resolver_precheck_non_participation_receipt_tests.cpp` 检查独立 fixture、runtime smoke fixture、release manifest、真实 `CommandProcessor` 的 `SMOKEJSON`，以及 `GET restore:real-read-token`。它会验证 Node v262 来源形状、禁用 gate、failure taxonomy、dry-run response shape，以及 hard no-start/no-write/no-credential/no-resolver/no-execution 边界。

既有 `command_tests` 和 `runtime_smoke_evidence_tests` 也扩展了新 key 和 diagnostics hint；`runtime_smoke_evidence_tests` 还确认 runtime smoke fixture 已经超过旧 64 KiB 上限但低于新的 `client_max_response_line_bytes`。完整 CTest 覆盖 44 个测试，真实 TCP smoke 通过 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH` 和 `GET restore:real-read-token` 再确认 runtime 读侧可消费、restore token 仍为 `(nil)`。

## 总结

v115 让跨项目链路可以确认 disabled credential resolver precheck 已经就位，但 mini-kv 仍只提供只读证据，不成为 resolver、credential reader、endpoint parser、transport、audit store 或 Java order authority。
