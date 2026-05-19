# mini-kv v112 fake transport packet non-participation receipt

本版目标是给 Node v257 准备 mini-kv 侧的只读非参与证据。Node v255 已经把 fake transport adapter dry-run packet 固化成 request / response / timeout / cleanup / boundary 证据，Node v256 又完成 archive verification；mini-kv v112 只回显这些字段形状和边界，证明自己没有被这个 packet 带入任何真实执行链路。

它不是什么：不是 fake transport 的执行者，不重新运行 Node v255 行为，不创建 cleanup 目录或文件，不读取 credential value，不发送 external request，不写 storage，不执行 LOAD / COMPACT / RESTORE / SETNXEX，也不成为 managed audit storage backend。

## 入口和数据结构

`include/minikv/runtime_evidence_receipts.hpp` 新增：

```cpp
fake_transport_dry_run_packet_non_participation_receipt_digest(read_commands)
format_fake_transport_dry_run_packet_non_participation_receipt_json(read_commands)
```

实现位于 `src/runtime_evidence_receipts.cpp`。常量区记录 Node v255 packet 的 profile、state、packet mode、request id、operation、transport kind、credential/endpoint handle、timeout budget、cleanup count，以及 Node v256 archive verification 不重新执行 fake transport 的边界。

`src/command_response_formatters.cpp` 只负责把新 receipt 挂进 `SMOKEJSON`，并在 diagnostics/notes 里增加 Node v257 的消费提示。没有改 command dispatch，没有改 write/admin handler，没有触碰 WAL、snapshot、restore 或 store 语义。

## 控制面如何理解

`source_request_shape_field_count=8`、`source_response_shape_field_count=9`、`source_failure_mapping_count=6` 说明 mini-kv 只回显 Node v255 的 packet shape。`source_timeout_budget_ms=15000`、`source_budget_spent=false`、`source_timer_started=false` 说明这是预算字段验证，不是实际计时或真实连接。

cleanup 相关字段是本版重点：`source_cleanup_artifact_count=0`、`source_cleanup_in_memory_only=true`、`source_temporary_directory_created=false`、`source_temporary_file_created=false`、`source_archive_reruns_fake_transport_behavior=false`。这些字段告诉 Node v257：mini-kv 没有参与执行，也没有产生临时文件副作用。

mini-kv 自身边界继续为硬 false：`mini_kv_auto_start_allowed=false`、`storage_write_allowed=false`、`credential_value_read_allowed=false`、`external_request_sent=false`、`raw_endpoint_url_included=false`、`temporary_directory_created=false`、`temporary_file_created=false`、`managed_audit_storage_backend=false`、`restore_execution_allowed=false`、`load_restore_compact_executed=false`、`setnxex_execution_allowed=false`、`order_authoritative=false`。

## Fixture 和测试

新增 `fixtures/release/fake-transport-dry-run-packet-non-participation-receipt.json`，并把同一 receipt 嵌入 `fixtures/release/runtime-smoke-evidence.json` 和 `fixtures/release/verification-manifest.json`。嵌入时保留历史 v102/v111 字段结构，只追加 v112 提示和 receipt，避免破坏旧测试依赖的历史证据形状。

新增 `tests/fake_transport_dry_run_packet_non_participation_receipt_tests.cpp`，覆盖独立 fixture、runtime smoke fixture、manifest fixture 和真实 `CommandProcessor` 的 `SMOKEJSON` 输出。测试断言 request/response/failure mapping count、timeout budget、cleanup count、archive no-rerun、无 credential value、无 raw endpoint URL、无临时目录/文件、无 external request、无 mini-kv auto-start、无 storage write、无 restore/load/compact/SETNXEX。

## 验证和归档

本版全量构建和 CTest 使用 CLion bundled CMake/Ninja/MinGW toolchain。真实 smoke 仍然是 server/client 只读路径：`SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH`、`GET restore:real-read-token`、`QUIT`。归档写入 `c/112/图片/` 和 `c/112/解释/说明.md`。

一句话总结：v112 把 Node v255/v256 fake transport packet 的只读边界接入 mini-kv 证据链，让 Node v257 能验证三方一致性，同时 mini-kv 仍然只是 runtime evidence provider。
