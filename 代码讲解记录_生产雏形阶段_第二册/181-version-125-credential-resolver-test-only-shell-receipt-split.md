# mini-kv v125 credential resolver test-only shell 回执拆分

## 本版目标

v125 是一次维护性拆分：把 v116 `test_only_resolver_shell_non_participation_receipt` 从 `src/runtime_credential_resolver_receipts.cpp` 独立到 `src/runtime_credential_resolver_test_only_shell_receipts.cpp`，并把 credential resolver receipt 共用的 digest/json 边界 helper 收到 `src/runtime_credential_resolver_receipt_utils.hpp`。

这不是新功能，不是新的执行入口，不是新的 credential resolver 实现，也不是对 `SMOKEJSON`、release fixture 或 Node/Java 边界的扩张。mini-kv 仍只提供只读运行证据，不自动启动上游，不读 credential value，不解析 raw endpoint URL，不写 managed audit 或本地存储，不执行 schema migration、restore、load、compact 或 `SETNXEX`。

## 为什么继续拆

v124 后 `src/command_response_formatters.cpp` 已经降到合理范围，但 `src/runtime_credential_resolver_receipts.cpp` 仍有 840 行，而且里面混放 v114 credential resolver decision、v115 disabled precheck、v116 test-only resolver shell 三段历史回执。继续把 v116 单独拆出是合理的，因为它有独立的 contract shape、request/response shape、failure mapping、guard conditions 和 fake resolver probe 元数据。

本版没有把 v115 也一起迁移，是为了控制改动面。v116 拆出后，原文件已经降到 511 行，后续是否继续拆 v115 可以按实际维护压力再决定。

## 模块职责

`src/runtime_credential_resolver_receipt_utils.hpp` 是 `src/` 内部私有 helper，不进入公开 include 契约。它只提供 credential resolver receipt 家族共用的：

- `field_string`：统一走 `runtime_evidence::json_string`。
- `receipt_digest`：统一走 `runtime_evidence::digest`。
- `format_common_credential_boundary_json`：保留 no-start、no-write、no-credential、no-restore、not-order-authoritative 等共同边界字段。
- `format_no_go_conditions_json`：保留 credential resolver 家族共同的 no-go 条件列表。

`src/runtime_credential_resolver_receipts.cpp` 现在聚焦 v114 credential resolver decision 和 v115 disabled precheck。它仍暴露原有 public functions，调用方和 header 不变。

`src/runtime_credential_resolver_test_only_shell_receipts.cpp` 专门承载 v116 test-only resolver shell 回执。这里保留 shell name、fake resolver kind、request/response 字段数量、failure mapping、guard condition 和 probe 元数据，并继续输出同一份 `test_only_resolver_shell_non_participation_receipt` JSON。

`CMakeLists.txt` 只把新 cpp 加入 `minikv` 库，没有新增 executable、test target 或运行入口。

## 契约边界

对控制面来说，`test_only_resolver_shell_non_participation_receipt` 仍然只能被理解为 v116 的只读非参与证明：

- `read_only=true` 表示它是证据输出，不是执行许可。
- `execution_allowed=false` 表示不能据此启动 resolver、连接 managed audit 或执行 schema migration。
- `credential_resolver_implemented=false`、`credential_resolver_invoked=false` 表示 mini-kv 没有实现也没有调用 resolver。
- `credential_value_read_allowed=false`、`raw_endpoint_url_parsed=false`、`external_request_sent=false` 表示 credential 和 endpoint 边界继续关闭。
- `storage_write_allowed=false`、`approval_ledger_write_allowed=false`、`order_authoritative=false` 表示 mini-kv 不是写入方、审批账本方或订单权威。

本版只是把这些字段搬到更合适的文件里，字段含义和下游消费方式不变。

## 测试与 smoke

针对性测试跑了 6 个：

- `test_only_resolver_shell_non_participation_receipt_tests`
- `disabled_credential_resolver_precheck_non_participation_receipt_tests`
- `credential_resolver_non_participation_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`
- `smokejson_command_receipt_tests`

这些测试覆盖 v114/v115/v116 回执的独立 fixture、runtime `SMOKEJSON` 聚合、release manifest 暴露和命令层输出。全量 `ctest` 52/52 通过，说明拆分没有影响其他命令、WAL、snapshot、TCP、restore 或 evidence tests。

真实 CLI smoke 输入 `SMOKEJSON`、`CHECKJSON SMOKEJSON`、`QUIT`，确认运行时输出仍能返回超长 `SMOKEJSON`，并且 `CHECKJSON SMOKEJSON` 仍说明它是只读命令而不是写命令。

## 行数结果

- `src/runtime_credential_resolver_receipts.cpp`: 840 行降到 511 行。
- `src/runtime_credential_resolver_test_only_shell_receipts.cpp`: 315 行。
- `src/runtime_credential_resolver_receipt_utils.hpp`: 41 行。

一句话总结：v125 把 test-only resolver shell 回执从历史 credential resolver 聚合文件中拆出来，让后续 Node/Java/mini-kv 的松耦合证据链更容易定位和维护，同时保持所有运行边界和字段契约不变。
