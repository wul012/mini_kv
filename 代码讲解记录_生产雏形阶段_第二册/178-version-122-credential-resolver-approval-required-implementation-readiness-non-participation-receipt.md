# mini-kv v122 credential resolver approval-required implementation readiness non-participation receipt

## 本版目标

v122 的目标是承接 Node v281 的 `credential resolver approval-required implementation readiness review`，在 mini-kv 侧给出一份只读、不可执行的非参与回执，供后续 Node v282 与 Java v116 做联合验证。

它不是新的 resolver 实现，不是 secret provider，不读取 credential value，不解析 raw endpoint，不连接 managed audit，不执行 `LOAD`、`COMPACT`、`RESTORE`、`SETNXEX`，也不写 storage、approval ledger 或 schema。mini-kv 在这里仍然只是 runtime evidence provider。

## 入口与输出

核心入口在 `src/command_response_formatters.cpp`。`SMOKEJSON` 增加了：

```text
credential_resolver_approval_required_implementation_readiness_non_participation_receipt
```

同一个文件还把 `diagnostics.node_consumption` 的最新消费提示前置为 Node v282：Node v282 可以在 Node v281 readiness review 之后验证 mini-kv v122 的非参与回执。

公共声明在 `include/minikv/runtime_evidence_receipts.hpp`：

```cpp
credential_resolver_approval_required_implementation_readiness_non_participation_receipt_digest(...)
format_credential_resolver_approval_required_implementation_readiness_non_participation_receipt_json(...)
```

这两个函数分别负责 digest 和 JSON 格式化，避免 `SMOKEJSON` 直接拼复杂业务字段。

## 回执实现

实现文件是 `src/runtime_credential_resolver_implementation_readiness_receipts.cpp`。

这个文件把 Node v281 的 readiness review 固化为 mini-kv 可验证的只读证据：

- `source_profile_version` 指向 Node v281 readiness review profile。
- `source_node_v275` 保留上一层 approval-required boundary upstream echo 的来源。
- `readiness_review` 说明当前只是 pre-implementation echo-ready，runtime implementation 仍 blocked。
- `boundary_readiness` 列出六个 approval-required boundary code。
- `required_artifact_codes` 列出 18 个后续实现前必须具备的 artifact。
- 大量布尔字段保持 `false`，明确 mini-kv 没有 resolver、secret、endpoint、external request、schema migration、approval ledger 或 storage backend 参与。

关键控制面字段的含义：

- `read_only=true`：这是一份可读证据，不是执行入口。
- `execution_allowed=false`：不允许 Node 或 Java 借此触发任何动作。
- `real_resolver_implementation_allowed=false`：还没进入真实 resolver 实现。
- `ready_for_java_v116_mini_kv_v122_echo=true`：只表示可以做跨项目 echo 验证。
- `ready_for_managed_audit_resolver_implementation=false`：不能把 echo readiness 误解成实现授权。
- `load_restore_compact_executed=false` 和 `setnxex_execution_allowed=false`：mini-kv 没有把证据审查变成存储写入或恢复动作。
- `order_authoritative=false`：mini-kv 不成为 Java 订单权威。

## Fixture 与 Manifest

新增 fixture：

```text
fixtures/release/credential-resolver-approval-required-implementation-readiness-non-participation-receipt.json
```

同时更新：

```text
fixtures/release/runtime-smoke-evidence.json
fixtures/release/verification-manifest.json
```

`runtime-smoke-evidence.json` 让历史 runtime smoke fixture 能看到 v122 新回执。`verification-manifest.json` 把新 fixture、新测试目标和 read-only smoke 期望一起登记，便于后续按 release manifest 检查。

## 测试保护

新增 `tests/credential_resolver_approval_required_implementation_readiness_non_participation_receipt_tests.cpp`，它验证四类事情：

- 独立 fixture 包含 Node v281、Node v275、六个 boundary、18 个 required artifact。
- runtime smoke fixture 已嵌入同一份 v122 回执。
- release manifest 登记了新 fixture、新测试目标和 expected smoke string。
- 真实 `CommandProcessor().execute("SMOKEJSON")` 输出新回执，并且 `GET restore:real-read-token` 仍然是 `(nil)`。

同时补强了三处既有测试：

- `tests/command_tests.cpp` 检查 live `SMOKEJSON` 命令输出。
- `tests/runtime_smoke_evidence_tests.cpp` 检查 runtime smoke fixture 与 live response。
- `tests/release_verification_manifest_tests.cpp` 检查 release manifest 与 live response。

这样新回执不是单点自证，而是被命令、fixture、manifest、runtime smoke 同时约束。

## Smoke 与归档

本版使用 `cmake-build-v122` 完成配置、构建、目标测试、全量测试和真实 TCP smoke。真实 smoke 启动 `minikv_server` 后，通过 `minikv_client` 执行：

```text
SMOKEJSON
INFOJSON
STORAGEJSON
HEALTH
GET restore:real-read-token
QUIT
```

输出包含新回执 digest：

```text
fnv1a64:b6e1efa8878b64d6
```

并且 `GET restore:real-read-token` 返回 `(nil)`，说明只读 smoke 没有制造恢复 token 或写入痕迹。命令截图和说明从 v122 起写入新的同级归档目录 `d/122/`。

## 一句话总结

v122 把 Node v281 的实现前 readiness review 转成 mini-kv 可验证的只读非参与证据，让后续 Node v282/Java v116 能继续推进 credential resolver 边界验证，同时不让 mini-kv 越界成为执行者、存储后端或订单权威。
