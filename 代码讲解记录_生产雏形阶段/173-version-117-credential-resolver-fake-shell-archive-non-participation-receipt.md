# v117 Credential Resolver Fake-Shell Archive Non-Participation Receipt

## 目标和边界

本版给 Node v267 增加 mini-kv 的只读回执，用来对应 Node v266 的 credential resolver fake-shell archive verification。它的角色是让控制面确认：Node v264 的 test-only fake resolver shell contract 和 Node v265 的 upstream echo verification 已经被 Node v266 归档核验，但 mini-kv 仍然只提供证据，不参与 resolver、secret provider、credential value、raw endpoint、managed audit connection、storage write 或 restore/load/compact 执行链。

它不是什么：不是新的命令入口，不是 archive reader，不是 credential resolver，不是 resolver client，不是 secret provider，不是 endpoint parser，不会重跑 fake resolver shell 行为，不会读取 Node 归档文件，不会启动 Node、Java 或 mini-kv，不会读 credential value，不会解析 raw endpoint URL，不会发送外部请求，不会写本地 storage、approval ledger、managed audit state 或 production record，也不会执行 schema migration、`LOAD`、`COMPACT`、`RESTORE` 或 `SETNXEX`。

## 版本位置

Node v266 已经只读核验了 v264/v265 的归档证据：archive file count 是 9，required/matched snippet count 是 24，check count 是 28，并明确 `archiveVerificationRerunsFakeShellBehavior=false`。mini-kv v117 做的是把这些归档事实镜像成非参与回执，供 Node v267 做上游 echo verification。

这个范围是合理的，因为它没有推进真实 resolver、真实 secret provider、真实 sandbox connection 或写路径，只把已经存在的 Node 归档证据接入 mini-kv 的 `SMOKEJSON` 证据链。

## 代码模块

`include/minikv/runtime_evidence_receipts.hpp` 继续是公共契约入口，本版新增 `credential_resolver_fake_shell_archive_non_participation_receipt_digest` 和 `format_credential_resolver_fake_shell_archive_non_participation_receipt_json`。

`src/runtime_credential_resolver_archive_receipts.cpp` 是新拆出的实现文件。这样 v117 的 archive echo 不继续塞进 v114-v116 的 `src/runtime_credential_resolver_receipts.cpp`，大文件压力被压住，也方便以后单独审计 archive 类回执。

formatter 主要输出这些结构：

- `source_archive_*`: 说明来源是 Node v266 archive verification，包含 profile、route、state、archive digest 和 evidence span。
- `source_node_v264`: 摘要回显 test-only fake resolver shell contract，锁定 9 个 request field、13 个 response field、7 个 failure mapping、10 个 guard condition。
- `source_node_v265`: 摘要回显 upstream echo verification，锁定 Node v264、Java v107、mini-kv v116、Java v109 optimization context 都已对齐。
- `archived_evidence`: 回显 9 个 archive 文件元数据、v264/v265 archive root、24 个 snippet 的分组匹配结果。
- `archive_verification`: 回显 v264/v265 route digest 关系，并保留 `archive_verification_reruns_fake_shell_behavior=false`。
- `checks` 和 `summary`: 锁定 28/28 checks、0 blocker、1 warning、2 recommendation。

`src/command_response_formatters.cpp` 把 `credential_resolver_fake_shell_archive_non_participation_receipt` 加进 `SMOKEJSON` 聚合对象、notes 和 diagnostics。控制面既可以读取独立 fixture，也可以从真实 runtime smoke 中读取同一份证据。

## 契约字段

控制面读到 `read_only=true`、`execution_allowed=false`、`archive_verification_only=true` 和 `credential_resolver_fake_shell_archive_non_participation_receipt_only=true` 时，应理解为：mini-kv 只回显 Node v266 已经形成的 archive verification facts。

关键硬边界是这些 false 字段：`archive_files_read_by_mini_kv=false`、`archive_verification_reruns_fake_shell_behavior=false`、`resolver_client_instantiated=false`、`secret_provider_instantiated=false`、`credential_value_read_allowed=false`、`credential_value_loaded=false`、`credential_value_stored=false`、`credential_value_included=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false`、`connection_execution_allowed=false`、`storage_write_allowed=false`、`production_record_written=false`、`schema_migration_execution_allowed=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`managed_audit_storage_backend=false` 和 `order_authoritative=false`。

这里最容易误读的是 archive：Node v266 可以检查归档文件，但 mini-kv v117 不在运行时读取 Node 归档文件。mini-kv 的职责只是把 Node v266 的归档核验结果做成稳定、可消费的 runtime evidence。

## Fixture 和测试

`fixtures/release/credential-resolver-fake-shell-archive-non-participation-receipt.json` 是 v117 独立回执。`fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json` 内嵌同一份 receipt，保证独立文件、manifest 和真实 `SMOKEJSON` 看到的是同一套字段。

`tests/credential_resolver_fake_shell_archive_non_participation_receipt_tests.cpp` 检查独立 fixture、runtime smoke fixture、release manifest、真实 `CommandProcessor` 的 `SMOKEJSON`，以及 `GET restore:real-read-token`。它覆盖 Node v266 archive 状态、v264/v265 摘要、archive 文件元数据、snippet count、receipt digest，以及 hard no-start/no-write/no-credential/no-resolver/no-execution 边界。

已有 `command_tests`、`runtime_smoke_evidence_tests` 和 `release_verification_manifest_tests` 也同步检查新 `SMOKEJSON` key、diagnostics hint、manifest test 名称和 fixture 路径，避免运行时聚合与仓库证据漂移。

## 总结

v117 让跨项目链路可以继续推进到 Node v267 的 fake-shell archive upstream echo verification，但 mini-kv 仍然只是只读证据提供者，不成为 resolver、credential reader、endpoint parser、transport、managed audit store 或 Java order authority。
