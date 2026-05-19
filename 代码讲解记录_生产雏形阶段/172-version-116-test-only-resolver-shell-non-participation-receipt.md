# v116 Test-Only Resolver Shell Non-Participation Receipt

## 目标和边界

本版给 Node v265 增加 mini-kv 的只读回显，用来对应 Node v264 的 credential resolver test-only shell contract。它的角色是让控制面确认：上游现在只有一个 test-only fake resolver shell 合同，mini-kv 仍然不参与 resolver、secret provider、credential value、raw endpoint、managed audit connection、storage write 或 restore/load/compact 执行链。

它不是什么：不是新的命令入口，不是 credential resolver，不是 resolver client，不是 secret provider，不是 endpoint parser，不是 fake resolver 执行器，不是 managed audit storage backend，也不是 Java order authority。它不启动 Node、Java 或 mini-kv，不读 credential value，不解析 raw endpoint URL，不发外部请求，不写本地 storage、approval ledger、managed audit state 或 production record，也不执行 schema migration、`LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。

## 版本位置

Node v263 已经证明 Java v106 和 mini-kv v115 都能回显 disabled credential resolver precheck 的非参与边界。Node v264 在此基础上只做 test-only shell contract：请求仍然是 handle-only，响应只暴露 side-effect booleans，failure mapping 和 guard condition 都是为了证明“停在假壳”，不是为了打开真实 resolver。

mini-kv v116 做的事很克制：把 Node v264 的 9 个 request field、13 个 response field、7 个 failure mapping、10 个 guard condition，以及 fake resolver probe 元数据镜像成回执。这个回执服务于 Node v265 的上游 echo verification，但本身不执行 probe，也不引入任何新写路径。

## 代码模块

`include/minikv/runtime_evidence_receipts.hpp` 继续是公共契约入口，本版新增 `test_only_resolver_shell_non_participation_receipt_digest` 和 `format_test_only_resolver_shell_non_participation_receipt_json`。

`src/runtime_credential_resolver_receipts.cpp` 负责 v114、v115 和 v116 的 credential-resolver 相关 receipt。v116 的 formatter 主要输出这些结构：

- `source_contract_*`：说明来源是 Node v264 test-only shell contract，route/profile/state/mode/name/kind 都是只读合同元数据。
- `resolver_shell_contract`：明确 real resolver 和 real secret provider 都没有打开，当前只是 fake-in-memory test shell。
- `request_shape`：列出 `requestId`、`operation`、`credentialHandle`、`endpointHandle`、`resolverPolicyHandle`、`approvalMarker`、`approvalCorrelationId`、`dryRun`、`fakeResolverOnly`，并强调不接受 credential value 或 raw endpoint URL。
- `response_shape`：列出 13 个响应字段，并把 resolver client、secret provider、credential value、raw endpoint、external request、managed audit connection、schema migration、production record 全部固定为 false。
- `failure_mapping` 和 `guard_conditions`：把 Node v264 的失败分类和保护条件回显出来，供 Node v265 做一致性校验。
- `fake_resolver_probe`：只记录 probe 的 request id、fake resolver kind 和预期响应，不代表 mini-kv 执行了 probe。

`src/command_response_formatters.cpp` 把 `test_only_resolver_shell_non_participation_receipt` 加进 `SMOKEJSON` 聚合对象、diagnostics 和 notes。这样 Node 可以读独立 fixture，也可以在真实 runtime smoke 里读同一份边界证明。

## 契约字段

控制面读到 `read_only=true`、`execution_allowed=false`、`test_only_resolver_shell_contract_only=true`、`test_only_shell=true`、`fake_resolver_only=true` 和 `handle_only_request=true` 时，应理解为：mini-kv 只提供证据，不提供执行能力。

关键硬边界是这些 false 字段：`resolver_client_instantiated=false`、`secret_provider_instantiated=false`、`credential_value_read_allowed=false`、`credential_value_loaded=false`、`credential_value_stored=false`、`credential_value_included=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`connection_execution_allowed=false`、`storage_write_allowed=false`、`production_record_written=false`、`schema_migration_execution_allowed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`managed_audit_storage_backend=false` 和 `order_authoritative=false`。

`fake_resolver_probe_executed=false` 也很重要：Node v264 可以声明 test-only fake resolver probe 的形状，但 mini-kv v116 只做回显，不运行 probe，也不把自己变成 fake resolver 执行环境。

## Fixture 和测试

`fixtures/release/test-only-resolver-shell-non-participation-receipt.json` 是 v116 独立 receipt。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 内嵌同一份 receipt，保证独立文件、manifest 和真实 `SMOKEJSON` 看到的是同一套字段。

`tests/test_only_resolver_shell_non_participation_receipt_tests.cpp` 检查独立 fixture、runtime smoke fixture、release manifest、真实 `CommandProcessor` 的 `SMOKEJSON`，以及 `GET restore:real-read-token`。它覆盖 Node v264 合同形状、Node v263 来源链、request/response shape、failure mapping、guard condition、fake resolver probe 元数据、receipt digest，以及 hard no-start/no-write/no-credential/no-resolver/no-execution 边界。

已有 `command_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 也同步检查新的 SMOKEJSON key、diagnostics hint、manifest test 名称和 fixture 路径，避免运行时聚合和仓库证据漂移。

## 总结

v116 让跨项目链路可以继续推进到 Node v265 的 test-only resolver shell upstream echo verification，但 mini-kv 仍然只是只读证据提供者，不成为 resolver、credential reader、endpoint parser、transport、managed audit store 或 Java order authority。
