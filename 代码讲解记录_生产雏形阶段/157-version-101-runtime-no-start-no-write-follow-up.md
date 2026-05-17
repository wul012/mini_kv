# Version 101 Runtime No-Start / No-Write Follow-Up

## 本版目标

v101 的目标很窄：承接 Node 最新计划里对 mini-kv 的要求，为 Node v236 manual sandbox connection dry-run request envelope 补一份运行期 no-start / no-write follow-up evidence，并让 Node v237 readiness gate 可以只读消费它。

它不是什么：

- 不是新的连接执行入口。
- 不是 managed audit storage backend。
- 不是 Node、Java 或 mini-kv 自动启动 mini-kv 的许可。
- 不是凭证值读取、schema rehearsal、schema migration 或 managed audit state write。
- 不是 restore/load/compact 执行，也不让 mini-kv 进入 Java order authority 链路。

## 入口与运行时对象

运行时入口仍然是 `SMOKEJSON`。本版在 `src/command_response_formatters.cpp` 里增加了 `format_runtime_no_start_no_write_follow_up_json(...)`，再把结果挂到 `SMOKEJSON.runtime_no_start_no_write_follow_up`。

这个对象的角色是把 Node v236 envelope 里的字段句柄和 mini-kv 当前运行证据对齐。它包含当前版本 `0.101.0`、release `v101`、归档路径 `c/101/`、live-read echo `mini-kv-live-read-v101`，以及 binary provenance、retention、restore boundary、non-authoritative storage、command dispatch、adapter shell、external adapter、sandbox adapter、sandbox connection echo marker、sandbox no-start receipt 等 digest。

关键边界字段的含义：

- `read_only=true`：这是只读运行证据。
- `execution_allowed=false`：控制面不能把它解释成执行许可。
- `node_auto_start_allowed=false`、`java_auto_start_allowed=false`、`mini_kv_auto_start_allowed=false`：任何项目都不能据此自动启动 mini-kv。
- `connection_execution_allowed=false`：mini-kv 不参与 sandbox connection。
- `credential_value_read_allowed=false`：只回显字段句柄，不读取凭证值。
- `schema_rehearsal_execution_allowed=false`、`schema_migration_execution_allowed=false`：不执行 schema rehearsal/migration。
- `managed_audit_write_executed=false`、`storage_write_allowed=false`：不写 managed audit，也不是存储后端。
- `restore_execution_allowed=false`、`load_restore_compact_executed=false`：不执行 restore/load/compact。
- `order_authoritative=false`：mini-kv 仍不是 Java 订单权威。

## Fixture 与 Manifest

新增 `fixtures/release/runtime-no-start-no-write-follow-up.json`，把 v101 evidence 固化成文件级契约。它记录：

- `follow_up_version=mini-kv-runtime-no-start-no-write-follow-up.v1`
- `consumer_hint=Node v237 manual sandbox connection readiness gate`
- `source_envelope.producer=Node v236 manual sandbox connection dry-run request envelope`
- owner approval artifact、credential handle、schema rehearsal id、rollback path、timeout budget、manual abort marker 这些字段句柄
- 所有 no-start/no-write/no-execution/no-credential/no-schema/no-restore/order-authority 边界

`fixtures/release/runtime-smoke-evidence.json` 升到 v15 / v101，并把新对象纳入 runtime smoke evidence。`fixtures/release/verification-manifest.json` 增加新 fixture、targeted test 和 `runtime_no_start_no_write_follow_up` 段。`fixtures/release/current-runtime-fixture-rolling-guard.json` 继续承担 rolling guard，保持历史 consumed digest anchors 不漂移，只滚动当前版本、路径、session echo 和当前 digest。

## 测试保护什么

新增 `tests/runtime_no_start_no_write_follow_up_tests.cpp`，它保护四类行为：

- 新 fixture 必须存在，并且只能包含 credential handle / field names，不能包含 credential value。
- v101 当前 digest、版本、路径和 live-read echo 必须一致。
- 所有 no-start/no-write/no-execution 边界必须为 false。
- 真实 `SMOKEJSON` 输出必须包含 `runtime_no_start_no_write_follow_up`，并且 `GET restore:real-read-token` 仍为 `(nil)`。

既有测试同步更新到 v101，包括 manifest、runtime smoke、current rolling guard、readonly fixture、managed audit receipt、restore boundary 和 digest package 相关测试。这样 Node 后续消费时既能看见新对象，也不会误把历史 v84-v96 receipt chain 改写掉。

## 真实验证与归档

本版使用 CLion bundled CMake/CTest 工具链完成验证：

- CMake configure 成功。
- CMake build 成功。
- CTest `34/34` passed。
- 真实 TCP smoke 在 `127.0.0.1:6501` 运行，读取 `SMOKEJSON`、`INFOJSON`、`STORAGEJSON`、`HEALTH` 和 `GET restore:real-read-token`，结果仍为 `(nil)`。

归档截图放在 `c/101/图片/`，说明放在 `c/101/解释/说明.md`。验证结束后停止了本版启动的 server，不留下后台进程。

一句话总结：v101 让 Node v237 可以继续推进 manual sandbox connection readiness gate，但 mini-kv 仍只是只读证据提供者，不自启、不连接、不读凭证、不写 audit、不执行 restore/load/compact，也不进入 Java order authority。
