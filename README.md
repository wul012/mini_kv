# mini-kv

A C++20 practice project for building a small Redis-like key-value engine.

## Current version

Version 81 is a runnable in-memory KV service with injected build version metadata, command catalog metadata, bounded JSON key inventory, digest-aware structured command risk explanation, read-only write-command execution contracts, a short-TTL token primitive with recovery evidence, release verification, runtime artifact rollback, runtime artifact bundle, restore compatibility handoff, restore dry-run operator package, artifact digest compatibility matrix, release artifact digest package, restore drill evidence, restore handoff checklist, restore evidence retention, retained restore artifact digest, restore approval boundary, restore boundary smoke manifests, structured runtime smoke evidence, SMOKEJSON failure taxonomy digest verification for imported real-read window results, an operator-window identity-neutral proof for archived real-read window checks, read-only CI/evidence hints for Node v201 manifest verification, and read-only artifact retention evidence for Node v203 cross-project retention gate checks, stable CHECKJSON write/read fixture samples, self-describing INFOJSON/STATSJSON/SMOKEJSON read-only runtime evidence, an INFOJSON/STATSJSON field guide for release evidence review, read-only storage evidence, and indexed restart recovery evidence with restart/replay cost and retention boundaries:

- CMake project layout
- Thread-safe in-memory key-value store
- Interactive command-line client
- TCP server with inline text and RESP request support
- TCP client for connecting to a running server
- Expiring keys with `EXPIRE` and `TTL`
- Optional write-ahead log persistence for `SET`, `DEL`, and `EXPIRE`
- WAL records include checksums while replay remains compatible with older plain records
- WAL replay reports applied, skipped, truncated, and checksum-failed records
- Manual WAL compaction rewrites the log from the current live store state
- Startup WAL repair can rebuild damaged logs with `--repair-wal`
- WAL maintenance reporting exposes log bytes, record count, live keys, and compact hints
- Optional automatic WAL compaction can rewrite long-running logs at startup and after WAL-backed writes
- WAL compaction thresholds are configurable, and `WALINFO` reports compact counters and saved work
- Runtime `STATS`, `STATSJSON`, and `HEALTH` commands expose live keys, WAL maintenance state, server connection counters, per-command counters, and latency counters
- Runtime `INFO` exposes injected build version, protocol, uptime, live keys, WAL mode, metrics mode, and request limit metadata
- Runtime `INFOJSON` exposes the same identity metadata as a schema-stable, read-only JSON object for external control planes
- Runtime `INFOJSON` and `STATSJSON` include `schema_version`, `read_only`, `execution_allowed`, `order_authoritative`, `evidence_type`, and diagnostics fields for real-read smoke checks
- Runtime `SMOKEJSON` combines version, identity, storage, WAL, command metrics, connection stats, real-read command list, forbidden command list, no-write diagnostics, failure taxonomy, taxonomy digest, verification sample, operator-window identity-neutral proof, CI evidence hints, and artifact retention evidence into one read-only JSON object for Node v203 cross-project retention gate checks
- Runtime `COMMANDS` and `COMMANDSJSON` expose supported command names, risk categories, mutation flags, WAL-touch flags, stability, and descriptions
- Runtime `EXPLAINJSON command` explains a command's parser validity, key target, TTL sensitivity, mutation risk, WAL risk, side effects, schema version, command digest, and side-effect count without executing it
- Runtime `CHECKJSON command` returns a read-only execution contract for a command, including parser, write-command, side-effect, command digest, and WAL durability checks without executing it
- Runtime `STORAGEJSON` returns read-only storage evidence with version, live key count, WAL status, snapshot capability, side effects, and diagnostic boundary notes for external control planes
- Runtime `SETNXEX key seconds value` atomically claims a missing key with a positive TTL for short token use cases, returning `1` on claim and `0` when a live token already exists
- Runtime `SETNXEX` remains a mini-kv token primitive only: `order_authoritative=false` still applies, and Java remains the order source of truth
- Repository fixture `fixtures/readonly/index.json` groups CHECKJSON, INFOJSON, STATSJSON, and runtime read field-guide evidence for Node v159 release evidence review preparation
- Repository fixture `fixtures/ttl-token/index.json` documents the `SETNXEX` token primitive, WAL record shape, repeat/expiry semantics, and non-order-authoritative boundary for Node v160 readiness review
- Repository fixture `fixtures/ttl-token/recovery-evidence.json` documents WAL, Snapshot, restart, expired-token, and compaction recovery boundaries for the TTL token primitive before Node v161 consumes it
- Repository fixture `fixtures/release/verification-manifest.json` documents the CMake configure/build, CTest, read-only SMOKEJSON/INFOJSON CI evidence hints, artifact retention evidence, runtime smoke, failure taxonomy digest, operator-window identity-neutral proof, fixture inventory, version evidence, rollback evidence sample, runtime artifact bundle manifest, restore compatibility handoff sample, restore dry-run operator package, artifact digest compatibility matrix, release artifact digest package, restore drill evidence, restore handoff checklist, restore evidence retention, retained restore artifact digest, restore approval boundary, restore boundary smoke manifest, and runtime smoke evidence expected before Node v203 checks cross-project artifact retention consistency
- Repository fixture `fixtures/release/runtime-artifact-rollback-evidence.json` documents binary/version, WAL, Snapshot, and fixture-version rollback boundaries without executing rollback or entering the Java transaction chain
- Repository fixture `fixtures/release/runtime-artifact-bundle-manifest.json` groups binary version, release manifest, rollback evidence, WAL/Snapshot compatibility, CTest, read-only smoke, and fixture inventory into one read-only bundle manifest
- Repository fixture `fixtures/release/restore-compatibility-handoff.json` records manual binary, WAL, Snapshot, and fixture compatibility confirmations for rollback window handoff without executing restore commands or creating Java order authority
- Repository fixture `fixtures/release/restore-dry-run-operator-package.json` records restore target, operator-recorded artifact digest placeholders, WAL/Snapshot compatibility, and CHECKJSON dry-run commands without executing restore commands or creating Java order authority
- Repository fixture `fixtures/release/artifact-digest-compatibility-matrix.json` records binary, WAL, Snapshot, and fixture digest compatibility placeholders for Node v168 production environment preflight without executing restore commands or creating Java order authority
- Repository fixture `fixtures/release/release-artifact-digest-package.json` records binary, WAL, Snapshot, and fixture digest placeholders plus a restore drill command profile for Node v170 deployment evidence intake without executing restore commands or creating Java order authority
- Repository fixture `fixtures/release/restore-drill-evidence.json` records restore target, digest comparison placeholder, operator confirmation, and CHECKJSON dry-run commands for Node v173 without executing restore commands or creating Java order authority
- Repository fixture `fixtures/release/restore-handoff-checklist.json` records restore operator checklist fields, artifact digest target, snapshot/WAL review placeholders, and CHECKJSON risk confirmation for Node v175 without executing LOAD, COMPACT, SETNXEX, or restore
- Repository fixture `fixtures/release/restore-evidence-retention.json` records restore checklist retention id, artifact digest placeholder, snapshot/WAL review retention placeholders, retention owner, and CHECKJSON risk evidence retention for Node v178 without executing LOAD, COMPACT, SETNXEX, or restore
- Repository fixture `fixtures/release/retained-restore-artifact-digest.json` records restore artifact digest placeholder, Snapshot review digest placeholder, WAL review digest placeholder, restore target placeholder, retention owner, and order-authoritative boundary for Node v180 without executing LOAD, COMPACT, SETNXEX, or restore
- Repository fixture `fixtures/release/restore-approval-boundary.json` records restore approver placeholder, restore target placeholder, artifact digest placeholder, Java transaction chain disconnection, and order-authoritative boundary for Node v182 without executing LOAD, COMPACT, SETNXEX, or restore
- Repository fixture `fixtures/release/restore-boundary-smoke-manifest.json` records the v75 real-read smoke command list, runtime target, client target, restore target placeholder, artifact digest placeholder, and no-write proof for Node v185 without executing LOAD, COMPACT, SETNXEX, or restore
- Repository fixture `fixtures/release/runtime-smoke-evidence.json` records the v81 SMOKEJSON real-read command target, Node v203 artifact retention gate boundary, forbidden command list, failure taxonomy digest, verification sample, operator-window proof, CI evidence path hint, artifact retention path/root/retention-days hints, no-restore proof, no-upload proof, no-production-window proof, and no-write proof without executing LOAD, COMPACT, SETNXEX, restore, CI upload, or production window opening
- Repository fixture `fixtures/readonly/infojson-empty-inline.json` captures a stable empty inline `INFOJSON` response with WAL disabled, metrics disabled, CI evidence path/no-restore hints, and artifact retention hints
- Repository fixture `fixtures/readonly/statsjson-empty-inline.json` captures a stable empty inline `STATSJSON` response before command accounting is recorded
- Repository fixture `fixtures/readonly/runtime-read-field-guide.json` explains INFOJSON/STATSJSON stable fields, dynamic fields, read-only smoke commands, and no-write boundaries
- Repository fixture `fixtures/recovery/index.json` lists recovery evidence fixtures, paths, digest, restart/replay sample cost, retention boundaries, consumer hints, and read-only boundaries for downstream smoke tests
- Repository fixture `fixtures/recovery/restart-recovery-evidence.json` captures a WAL plus snapshot restart recovery sample with before/after key count, digest, `recovered=true`, and read-only diagnostics
- Repository fixture `fixtures/checkjson/set-orderops-write-contract.json` captures the real `CHECKJSON SET orderops:1 value` response for external smoke tests
- Repository fixture `fixtures/checkjson/get-orderops-read-contract.json` captures the real `CHECKJSON GET orderops:1` response for read-command scenario matrix tests
- Runtime `RESETSTATS` clears command execution metrics without resetting data, WAL maintenance counters, or TCP connection counters
- Runtime `KEYS` returns a sorted one-line list of currently live keys for scripts and client cache refresh
- Runtime `KEYS prefix` returns a sorted filtered list without replacing the bundled client's full key cache
- Runtime `KEYSJSON [prefix]` returns a bounded JSON key inventory with prefix, key count, keys, truncation flag, and limit
- Optional `--metrics-interval-ms` emits periodic structured `event=server_metrics` connection, command, and latency counters
- Optional `--metrics-file` mirrors periodic server metrics and final stop metrics to a dedicated file for longer runs
- Optional `--metrics-file-format jsonl` writes metrics export records as schema-stable JSON Lines while keeping stdout lifecycle logs in text form
- Optional `--metrics-file-max-bytes` and `--metrics-file-keep` rotate and retain bounded metrics export files
- Manual snapshots with `SAVE` and `LOAD`
- Snapshot load rejects corrupt files without replacing the current store
- Atomic snapshot saves write a temporary file before replacing the target snapshot
- Benchmark executable for quick local throughput checks
- CTest coverage for store, command, WAL, snapshot, stress, RESP, TCP server, client history, line editor, and metrics file behavior
- Redis RESP parser and TCP response formatting
- Redis-style `PING [message]` command and RESP parser size limits
- Graceful TCP server stop support and structured `event=...` lifecycle logs
- TCP connection IDs plus active, total, and peak connection metrics in lifecycle logs
- Thread-safe server stdout logging for concurrent connection events
- Configurable TCP request byte limit and optional client socket timeout
- Broader TCP compatibility tests for `localhost` and address-family agnostic client resolution
- Configurable bundled TCP client connection retries and retry delay
- Bundled TCP client local session history with `:history`, `!!`, and `!N`
- Optional bundled TCP client persistent history via `--history-file`
- Optional bundled TCP client persistent key cache via `--key-cache-file`
- Bundled TCP client terminal line editing with Tab command completion, arrow-key history navigation, and cursor editing
- Bundled TCP client tracks keys learned from successful writes and offers Tab completion for key-oriented commands
- Bundled TCP client refreshes its key completion cache when `KEYS` returns a valid key list
- Command execution metrics track total, successful, error, per-command breakdown, total latency, average latency, and max latency
- External-client style RESP-over-TCP smoke coverage for pipelined requests
- RESP-over-TCP compatibility tests for null bulk replies, errors, `DEL`, `EXPIRE`, and `TTL`
- Concurrent RESP-over-TCP raw-socket client coverage with active and peak connection checks
- GitHub Actions CI for Linux, macOS, and Windows CMake build plus CTest

## Build

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## CI

GitHub Actions runs the same CMake build and CTest flow on Linux, macOS, and Windows:

```text
.github/workflows/ci.yml
```

Each CI job configures a Debug build, builds every target, and runs the registered CTest suite with failure output enabled. The test suite is assert-driven, so CI intentionally keeps assertions enabled.

## Run

CLI:

```powershell
.\build\Debug\minikv_cli.exe
```

CLI with WAL:

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal
```

CLI with WAL repair:

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal --repair-wal
```

CLI with automatic WAL compaction:

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal --auto-compact-wal
```

CLI with tuned WAL compaction thresholds:

```powershell
.\build\Debug\minikv_cli.exe data\mini-kv.wal --auto-compact-wal --wal-compact-min-records 4 --wal-compact-record-ratio 2 --wal-compact-min-bytes 1048576
```

TCP server:

```powershell
.\build\Debug\minikv_server.exe 6379
```

TCP server with WAL:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal
```

TCP server with WAL repair:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal --repair-wal
```

TCP server with automatic WAL compaction:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal --auto-compact-wal
```

TCP server with tuned WAL compaction thresholds:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal --auto-compact-wal --wal-compact-min-records 4 --wal-compact-record-ratio 2 --wal-compact-min-bytes 1048576
```

TCP server with explicit limits:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --max-request-bytes 65536 --accept-poll-ms 200
```

TCP server with periodic metrics logs:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 10000
```

TCP server with metrics file export:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 10000 --metrics-file data\server.metrics.log
```

TCP server with JSONL metrics file export:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 10000 --metrics-file data\server.metrics.jsonl --metrics-file-format jsonl
```

TCP server with rotated metrics file export:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --metrics-interval-ms 10000 --metrics-file data\server.metrics.log --metrics-file-max-bytes 10485760 --metrics-file-keep 5
```

TCP server bound through hostname resolution:

```powershell
.\build\Debug\minikv_server.exe 6379 localhost --max-request-bytes 65536
```

TCP client:

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379
```

TCP client with socket timeout:

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379 5000
```

TCP client that waits for a server to come online:

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379 5000 --connect-retries 10 --retry-delay-ms 250
```

TCP client with persistent command history:

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379 --history-file data\client.history
```

TCP client with persistent key completion cache:

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379 --key-cache-file data\client.keys
```

TCP client local history commands:

```text
:history  Show commands sent during the current client session.
!!        Repeat the previous sent command.
!N        Repeat history entry N, using 1-based indexing.
```

When the bundled TCP client runs in an interactive terminal, it supports Tab command completion, Up/Down history navigation, Left/Right cursor movement, Home/End, Backspace, and Delete. Tab completes command prefixes such as `PI` to `PING `, `K` to `KEYS `, `R` to `RESETSTATS `, and `:h` to `:history `; ambiguous prefixes expand only to the longest shared prefix when that prefix grows. After successful `SET` commands, the client also learns key names and can complete the key argument for `GET`, `SET`, `DEL`, `EXPIRE`, and `TTL`. When `KEYS` returns a valid `key_count=N keys=...` response, the client replaces its known-key cache with that server list. With `--key-cache-file`, those learned or refreshed key names are loaded on startup and saved after cache-changing commands, so key completion can carry across client sessions. When stdin is redirected or piped, it keeps the original line-by-line input behavior.

Benchmark:

```powershell
.\build\Debug\minikv_benchmark.exe 100000 10000
```

For single-config generators, executables may be directly under `build`.

## CLion

Open this folder directly in CLion:

```text
D:\C\mini-kv
```

If CLion asks for a toolchain, choose one of these:

- MinGW / MSYS2 UCRT64
- Visual Studio Build Tools
- WSL toolchain

After CLion finishes loading CMake, run these targets:

- `minikv_cli`
- `minikv_server`
- `minikv_client`
- `minikv_benchmark`
- `minikv_store_tests`
- `minikv_command_tests`
- `minikv_wal_tests`
- `minikv_snapshot_tests`
- `minikv_stress_tests`
- `minikv_resp_tests`
- `minikv_tcp_server_tests`
- `minikv_tcp_resp_tests`
- `minikv_tcp_resp_compat_tests`
- `minikv_tcp_resp_concurrency_tests`
- `minikv_client_history_tests`
- `minikv_line_editor_tests`
- `minikv_metrics_file_tests`
- `minikv_recovery_evidence_tests`
- `minikv_recovery_fixture_index_tests`
- `minikv_ttl_token_recovery_tests`
- `minikv_release_verification_manifest_tests`
- `minikv_runtime_artifact_rollback_evidence_tests`
- `minikv_runtime_artifact_bundle_manifest_tests`
- `minikv_restore_compatibility_handoff_tests`
- `minikv_restore_dry_run_operator_package_tests`
- `minikv_artifact_digest_compatibility_matrix_tests`
- `minikv_release_artifact_digest_package_tests`
- `minikv_restore_drill_evidence_tests`
- `minikv_restore_handoff_checklist_tests`
- `minikv_restore_evidence_retention_tests`
- `minikv_retained_restore_artifact_digest_tests`
- `minikv_restore_approval_boundary_tests`
- `minikv_restore_boundary_smoke_manifest_tests`
- `minikv_runtime_smoke_evidence_tests`

## CLI commands

```text
PING [message]
SET key value
SETNXEX key seconds value
GET key
DEL key
EXPIRE key seconds
TTL key
SIZE
KEYS [prefix]
KEYSJSON [prefix]
SAVE path
LOAD path
COMPACT
WALINFO
STATS
STATSJSON
SMOKEJSON
RESETSTATS
HEALTH
INFO
INFOJSON
COMMANDS
COMMANDSJSON
EXPLAINJSON command
CHECKJSON command
STORAGEJSON
HELP
EXIT
QUIT
```

## TCP protocol

Start the server:

```powershell
.\cmake-build-debug\minikv_server.exe 6379
```

Connect with a TCP client, then send one inline command per line:

```text
SET name mini-kv
SETNXEX idem:token 30 claimed
PING
GET name
EXPIRE name 10
TTL name
SIZE
KEYS
KEYS user:
KEYSJSON
KEYSJSON user:
SAVE data\mini-kv.snapshot
DEL name
LOAD data\mini-kv.snapshot
COMPACT
WALINFO
STATS
STATSJSON
RESETSTATS
HEALTH
INFO
INFOJSON
COMMANDS
COMMANDSJSON
EXPLAINJSON SET orderops:1 value
CHECKJSON SET orderops:1 value
STORAGEJSON
QUIT
```

Or use the bundled client:

```powershell
.\cmake-build-debug\minikv_client.exe 127.0.0.1 6379
```

The TCP server also accepts Redis-style RESP request arrays:

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
*1\r\n$4\r\nPING\r\n
*2\r\n$3\r\nGET\r\n$4\r\nname\r\n
```

RESP mode returns simple strings, integers, bulk strings, null bulk strings, or errors using RESP framing. Inline mode keeps the original one-line text responses.

`KEYS` returns currently live key names in sorted order as `key_count=N keys=...`; expired keys are pruned before the list is produced. `KEYS prefix` returns only keys that start with that prefix as `key_count=N prefix=prefix keys=...`. The prefixed response intentionally differs from the full-list response so the bundled client does not replace its full local key cache with a filtered subset. `KEYSJSON [prefix]` returns the same live key inventory as JSON with `prefix`, `key_count`, `keys`, `truncated`, and `limit`; the default limit is `1000`, and the command never returns values. `SETNXEX key seconds value` is a short-TTL token primitive: it sets a missing or expired key with a value and positive TTL, returns `1` when the claim is accepted, returns `0` when a live token already exists, and does not make mini-kv an order-authoritative store. `STATS` returns a compact key-value status line with live key count, command counters, command latency counters, per-command breakdown, WAL maintenance details when enabled, and TCP connection counters when executed through the server. `STATSJSON` returns the same operational snapshot as one read-only JSON object, which is easier for scripts and log collectors to parse. It includes `schema_version=1`, `read_only=true`, `execution_allowed=false`, `order_authoritative=false`, `evidence_type=runtime_metrics`, and diagnostics showing `write_commands_executed=false`; latency counters remain dynamic runtime fields. `SMOKEJSON` returns a combined read-only runtime smoke object for external read adapters: it includes identity, version, storage, WAL, command metrics, connection stats, allowed real-read commands, forbidden commands (`LOAD`, `COMPACT`, `SETNXEX`, `RESTORE`), failure taxonomy categories (`closed-window`, `connection-refused`, `timeout`, `invalid-json`, `read-command-failed`, `unsafe-surface`, and `unexpected-write-signal`), stable `taxonomy_digest` (`fnv1a64:f92fcba55feb26a2`) plus a verification sample for Node v196 imported-window checks, `operator_window.identity_neutral_proof=true` for Node v200 archived real-read window checks, `ci_evidence` fields for Node v201 manifest verification (`artifact_path_hint="c/80/"`, `no_restore_proof=true`, `upload_allowed=false`), and `artifact_retention` fields for Node v203 retention gate checks (`artifact_root="c/"`, `artifact_path_hint="c/81/"`, `retention_days=30`, `github_artifact_upload_attempted=false`, `production_window_allowed=false`); diagnostics still show `write_commands_executed=false`, `admin_commands_executed=false`, and `runtime_write_observed=false`. `INFO` returns build and runtime identity as `version=... protocol=... uptime_seconds=... live_keys=... wal_enabled=... metrics_enabled=... max_request_bytes=...`; the version comes from the CMake project version generated into `minikv/version.hpp`. `INFOJSON` returns the same identity contract as read-only JSON: `schema_version`, `read_only`, `execution_allowed`, `order_authoritative`, `evidence_type`, `version`, `server.protocol`, `server.uptime_seconds`, `server.max_request_bytes`, `store.live_keys`, `wal.enabled`, `metrics.enabled`, `ci_evidence`, `artifact_retention`, and diagnostics. `COMMANDS` returns a compact text command catalog, and `COMMANDSJSON` returns the same catalog as `{"commands":[...]}` entries with `name`, `category`, `mutates_store`, `touches_wal`, `stable`, and `description`. Categories are `meta`, `read`, `write`, and `admin`; this is descriptive metadata, not an ACL or permission system. `EXPLAINJSON command` returns `schema_version`, `command_digest`, `command`, `category`, `mutates_store`, `touches_wal`, `key`, `requires_value`, `ttl_sensitive`, `allowed_by_parser`, `side_effects`, `side_effect_count`, and `warnings` for the supplied command line without executing it. `schema_version` is currently `1`; `command_digest` is a stable `fnv1a64:<hex>` evidence fingerprint over the normalized explain fields, not an authentication signature. `side_effects` uses stable labels such as `store_write`, `store_ttl_update`, `snapshot_file_write`, `store_replace_from_snapshot`, `wal_rewrite_when_enabled`, `metrics_reset`, `connection_close`, and `metadata_read`; `side_effect_count` mirrors the number of labels. `CHECKJSON command` wraps the same explain evidence into a read-only execution contract with `read_only=true`, `execution_allowed=false`, `checks`, `wal`, and contract warnings. Its `wal.durability` is `wal_backed`, `memory_only`, `not_wal_backed`, or `not_applicable` depending on the target command and whether this processor has a WAL. `STORAGEJSON` is also read-only and returns `schema_version`, `read_only`, `execution_allowed`, `version`, `store.live_keys`, `store.order_authoritative=false`, `wal`, `snapshot`, `side_effects`, `side_effect_count`, and `diagnostics`. Its diagnostic notes explicitly mark the evidence as `not_order_authoritative`, and the command reports WAL status without appending or compacting the WAL. `RESETSTATS` clears command execution metrics; a successful reset is not counted as a new command, so the next `STATS` or `STATSJSON` response starts from zero. `HEALTH` returns an `OK ...` line for quick liveness checks with the same WAL, command metric, and connection signals. `command_breakdown` uses `COMMAND:total/success/error/total_latency_ns/avg_latency_ns/max_latency_ns` entries separated by semicolons; unknown commands are grouped under `UNKNOWN`.

Stable sample responses live under `fixtures/checkjson/`, `fixtures/readonly/`, `fixtures/ttl-token/`, `fixtures/release/`, and `fixtures/recovery/`.

`fixtures/checkjson/set-orderops-write-contract.json` is the exact no-WAL response for `CHECKJSON SET orderops:1 value`; `fixtures/checkjson/get-orderops-read-contract.json` is the exact read-command response for `CHECKJSON GET orderops:1`, including `side_effects=["store_read"]` and `side_effect_count=1`. `fixtures/readonly/index.json` groups those CHECKJSON contracts with `INFOJSON`, `STATSJSON`, and `runtime-read-field-guide.json` evidence for Node v159 release evidence review preparation while keeping `read_only=true`, `execution_allowed=false`, and `order_authoritative=false`.

`fixtures/ttl-token/index.json` documents the v61 `SETNXEX` token primitive for Node v160 readiness review, and `fixtures/ttl-token/recovery-evidence.json` documents the v62 TTL recovery boundary across WAL, Snapshot, restart, expired-token, and compaction paths.

`fixtures/release/verification-manifest.json` is the v81 release verification manifest for Node v203 cross-project CI artifact retention gate checks: it lists expected CMake configure/build, CTest, targeted tests, read-only `SMOKEJSON` and `INFOJSON` CI evidence hints, artifact retention evidence (`artifact_root="c/"`, `artifact_path_hint="c/81/"`, `retention_days=30`, `github_artifact_upload_attempted=false`, `production_window_allowed=false`), fixture inventory, version evidence, no-runtime-write boundary, failure taxonomy digest evidence, operator-window identity-neutral proof, rollback evidence sample, runtime artifact bundle manifest, restore compatibility handoff sample, restore dry-run operator package, artifact digest compatibility matrix, release artifact digest package, restore drill evidence, restore handoff checklist, restore evidence retention, retained restore artifact digest, restore approval boundary, restore boundary smoke manifest, and runtime smoke evidence.

`fixtures/release/release-artifact-digest-package.json` records binary, WAL, Snapshot, and fixture digest placeholders plus restore drill commands for Node v170; it uses `CHECKJSON LOAD data/release-artifact-drill.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX release:token 30 value` only as read-only package checks, and `GET release:token` must remain `(nil)`. `fixtures/release/restore-drill-evidence.json` records the v70 restore drill target, digest comparison placeholder, operator confirmation, and CHECKJSON dry-run commands for Node v173; it uses `CHECKJSON LOAD data/restore-drill.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:drill-token 30 value` only as read-only drill checks, and `GET restore:drill-token` must remain `(nil)`. `fixtures/release/restore-handoff-checklist.json` records the v71 restore handoff checklist for Node v175; it uses `CHECKJSON LOAD data/handoff-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:handoff-token 30 value` only as read-only risk confirmation, and `GET restore:handoff-token` must remain `(nil)`. `fixtures/release/restore-evidence-retention.json` records the v72 restore evidence retention fields for Node v178; it uses `CHECKJSON LOAD data/retention-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:retention-token 30 value` only as retained risk evidence, and `GET restore:retention-token` must remain `(nil)`. `fixtures/release/retained-restore-artifact-digest.json` records the v73 retained restore artifact digest fields for Node v180; it uses `CHECKJSON LOAD data/retained-digest-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:digest-token 30 value` only as retained digest evidence, and `GET restore:digest-token` must remain `(nil)`. `fixtures/release/restore-approval-boundary.json` records the v74 restore approval boundary fields for Node v182; it uses `CHECKJSON LOAD data/approval-boundary-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:approval-token 30 value` only as approval-boundary risk evidence, and `GET restore:approval-token` must remain `(nil)`. `fixtures/release/restore-boundary-smoke-manifest.json` records the v75 real-read smoke manifest for Node v185; it uses `CHECKJSON LOAD data/real-read-boundary-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:real-read-token 30 value` only as real-read risk evidence, and `GET restore:real-read-token` must remain `(nil)`. `fixtures/release/runtime-smoke-evidence.json` records the v81 `SMOKEJSON` read target for Node v203; it uses `SMOKEJSON`, `INFOJSON`, `STORAGEJSON`, `HEALTH`, and `GET restore:real-read-token` as read-only runtime evidence, adds failure taxonomy digest `fnv1a64:f92fcba55feb26a2`, a verification sample, `operator_window.identity_neutral_proof=true`, `ci_evidence` hints (`artifact_path_hint="c/80/"`, `no_restore_proof=true`, `upload_allowed=false`), and `artifact_retention` hints (`artifact_root="c/"`, `artifact_path_hint="c/81/"`, `retention_days=30`, `github_artifact_upload_attempted=false`, `production_window_allowed=false`); `GET restore:real-read-token` must remain `(nil)`.

`command_tests` compares both CHECKJSON fixtures with real command output and checks the TTL token fixture index, `readonly_fixture_tests` compares the read-only fixture package with real `CHECKJSON`, `INFOJSON`, and `STATSJSON` output plus the runtime read field guide, `recovery_evidence_tests` and `recovery_fixture_index_tests` protect restart evidence, `ttl_token_recovery_tests` exercises TTL recovery, and the release tests protect the manifest, rollback evidence, bundle manifest, restore handoff, dry-run operator package, digest matrix, release artifact digest package, restore drill evidence, restore handoff checklist, restore evidence retention, retained restore artifact digest, restore approval boundary, restore boundary smoke, runtime smoke no-execution boundaries, SMOKEJSON failure taxonomy digest/sample fields, operator-window identity-neutral proof, CI evidence hints, and artifact retention evidence.

The server prints structured lifecycle logs using key-value fields:

```text
event=server_start host=127.0.0.1 port=6379 protocol=inline,resp max_request_bytes=65536 accept_poll_ms=200 metrics_interval_ms=0 metrics_file="none" metrics_file_format=text metrics_file_max_bytes=0 metrics_file_keep=0 auto_compact_wal=false
event=tcp_listen host=127.0.0.1 port=6379 max_request_bytes=65536
event=tcp_client_accepted host=127.0.0.1 port=6379 connection_id=1 active_connections=1 total_connections=1 peak_connections=1
event=server_metrics host=127.0.0.1 port=6379 active_connections=1 total_connections=1 peak_connections=1 total_commands=10 successful_commands=9 error_commands=1 total_latency_ns=900000 avg_latency_ns=90000 max_latency_ns=250000 command_breakdown=GET:4/4/0/160000/40000/70000;PING:5/5/0/50000/10000/15000;UNKNOWN:1/0/1/690000/690000/690000 metrics_interval_ms=10000
event=tcp_request_rejected host=127.0.0.1 port=6379 connection_id=1 reason=request_too_long pending_bytes=70000 max_request_bytes=65536
event=tcp_client_closed host=127.0.0.1 port=6379 connection_id=1 active_connections=0 total_connections=1 peak_connections=1
event=tcp_stop host=127.0.0.1 port=6379 active_connections=0 total_connections=1 peak_connections=1 total_commands=10 successful_commands=9 error_commands=1 total_latency_ns=900000 avg_latency_ns=90000 max_latency_ns=250000 command_breakdown=GET:4/4/0/160000/40000/70000;PING:5/5/0/50000/10000/15000;UNKNOWN:1/0/1/690000/690000/690000
event=server_stopped host=127.0.0.1 port=6379
```

Use `Ctrl+C` or `SIGTERM` to request a graceful server stop. The accept loop wakes periodically, observes the stop request, closes the listener, and exits `server.run()`.

TCP server options:

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--auto-compact-wal]
                  [--wal-compact-min-records count] [--wal-compact-record-ratio ratio]
                  [--wal-compact-min-bytes bytes] [--max-request-bytes bytes]
                  [--accept-poll-ms ms] [--metrics-interval-ms ms] [--metrics-file path]
                  [--metrics-file-format text|jsonl]
                  [--metrics-file-max-bytes bytes] [--metrics-file-keep count]
```

`--max-request-bytes` limits the buffered bytes for one pending TCP request. Inline over-limit requests return `ERR line too long`; RESP over-limit requests return `-ERR request too long`.
`--metrics-interval-ms` enables periodic `event=server_metrics` logs. It is disabled by default and reports active, total, and peak connection counters plus command totals, error totals, latency counters, and per-command breakdown when enabled.
`--metrics-file` writes the same periodic `event=server_metrics` lines, plus the final `event=tcp_stop` snapshot, to a dedicated file. The file is truncated when the server starts and flushed after each exported metrics line.
`--metrics-file-format` controls only the metrics file output format. `text` is the default and preserves the existing key-value lines. `jsonl` writes one JSON object per exported metrics event with `event`, `host`, `port`, `connection_stats`, `commands`, and `metrics_interval_ms` on periodic samples. Stdout lifecycle logs stay in the original text format. The option requires `--metrics-file`.
`--metrics-file-max-bytes` rotates the metrics file before the next exported line would exceed the configured size. `--metrics-file-keep` controls how many rotated files are retained as `<path>.1`, `<path>.2`, and so on; it defaults to 3 when a metrics file is configured and may be set to 0 to discard old metrics instead of keeping rotated files. Both options require `--metrics-file`.

TCP client options:

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
                  [--history-file path] [--key-cache-file path]
```

`timeout_ms` sets both receive and send socket timeouts for the bundled client.
`--connect-retries` controls how many times the client retries after the initial connection attempt fails. `--retry-delay-ms` controls the delay between retries and defaults to 250 ms.
The bundled client also handles local session history commands before sending data to the server: `:history`, `!!`, and `!N`. When `--history-file` is set, the client loads existing commands from that file at startup and saves the bounded history after each successful sent command.
When `--key-cache-file` is set, the client loads remembered key names at startup, saves newly learned keys after successful `SET`, removes deleted keys after `DEL`, clears the cache after successful `LOAD`, and replaces the cache after a successful `KEYS` response.
On an interactive terminal, the bundled client edits the current line locally with Tab completion, arrow keys, Home/End, Backspace, and Delete. Tab completes server command names including `KEYS`, `KEYSJSON`, `STATSJSON`, `SMOKEJSON`, `STORAGEJSON`, `RESETSTATS`, `COMMANDSJSON`, `EXPLAINJSON`, and `CHECKJSON`, plus local `:history`; it also completes remembered key names in the second token of `GET`, `SET`, `DEL`, `EXPIRE`, and `TTL`. Up/Down walk the same in-memory or loaded history that powers `:history`, `!!`, and `!N`. Redirected stdin still uses normal line-by-line reading for scripts and smoke tests.

TTL commands:

```text
SETNXEX key seconds value  Claim a missing key with value and positive TTL, returning 1 on claim and 0 on duplicate.
EXPIRE key seconds         Set a positive second-based expiration for an existing key.
TTL key                    Return -2 when the key is missing, -1 when it has no expiration, or remaining seconds otherwise.
```

`SET` replaces the value and clears any existing TTL for that key. `SETNXEX` does not replace a live key; once the token expires and is pruned, the key can be claimed again. This is useful for short token experiments, not for order-authoritative storage. The server keeps all data in memory unless WAL or snapshots are used.

## WAL persistence

Pass a WAL path to `minikv_cli` or `minikv_server` to enable persistence. On startup, the program replays the WAL into memory. During runtime, successful write commands append records before applying the mutation:

```text
SET key value
SETNXEX key seconds value
DEL key
EXPIRE key seconds
```

`EXPIRE` is persisted as an internal absolute expiration record, so expired keys do not come back after restart. `SETNXEX` uses a single internal `SETEXAT key epoch_millis value` WAL record for a successful claim, so replay can restore the value and expiration together or drop the token if the expiration time is already past. Snapshots also preserve unexpired token TTLs and skip expired token entries during restore. A restart can load a snapshot baseline and replay later WAL token claims; compacted live tokens replay from `SET` plus `EXPIREAT`, still without reviving expired tokens. New WAL records are stored as `WAL2 <checksum> <record>` lines; replay still accepts older plain `SET` / `DEL` / `EXPIREAT` lines and now accepts `SETEXAT` records. WAL replay skips malformed records, skips checksum mismatches, detects a final unterminated record as a probable partial write, and reports applied, skipped, truncated, and checksum-failed counts during startup. `COMPACT` is available when a WAL path is configured; it rewrites the log to the current live key set using checksummed `SET` and `EXPIREAT` records, dropping overwritten, deleted, and expired history. Start `minikv_cli` or `minikv_server` with `--repair-wal` to replay the recoverable records and immediately rewrite the WAL to a clean compacted WAL2 file before serving commands. Startup also reports WAL bytes, physical record count, live keys, compact thresholds, compaction counters, and whether compaction is recommended; use `WALINFO`, `STATS`, `STATSJSON`, or `HEALTH` at runtime to inspect the same maintenance signal. Start with `--auto-compact-wal` to compact automatically when the same maintenance thresholds recommend it, both after startup replay and after WAL-backed write commands. Use `--wal-compact-min-records`, `--wal-compact-record-ratio`, and `--wal-compact-min-bytes` with a WAL path to tune those thresholds. Without a WAL path, mini-kv remains an in-memory-only service.

`WALINFO` includes:

```text
wal_bytes wal_records live_keys compact_recommended
compact_min_records compact_record_ratio compact_min_bytes
manual_compactions auto_compactions repair_compactions
compacted_keys records_removed bytes_saved
```

## Snapshots

Use snapshots to save the current in-memory dataset to a compact file and load it later:

```text
SAVE path  Save all currently live keys to a snapshot file.
LOAD path  Replace the current in-memory dataset with a snapshot file.
```

Snapshot files preserve values and absolute expiration times. Expired keys are not saved, and expired records are skipped when loading. Corrupt snapshot files are rejected before replacing the current in-memory store. `SAVE` writes a temporary file in the target directory and then replaces the target snapshot, avoiding direct truncation of the previous snapshot while the new one is still being written. `LOAD` is a manual state replacement command; pair it with WAL intentionally if you want both baseline snapshots and later write replay.

## Benchmarks and stress tests

Run a quick in-process benchmark:

```powershell
.\cmake-build-debug\minikv_benchmark.exe 100000 10000
```

Arguments are optional:

```text
minikv_benchmark.exe [operations] [key_count]
```

The benchmark prints elapsed time and operations per second for direct `Store` operations and command-layer `SET` / `GET` operations. It is a lightweight local signal, not a replacement for full production-grade benchmarking.

The stress test is registered with CTest:

```powershell
ctest --test-dir cmake-build-debug --output-on-failure
```

`stress_tests` runs multiple writer and eraser threads against one shared `Store`, then checks snapshot export/restore and final key consistency. `command_tests` verifies command parsing, command execution counters, per-command breakdown, unknown-command grouping, latency counters, full and prefix-filtered `KEYS`, bounded `KEYSJSON`, `SETNXEX` token claim/duplicate/expiry behavior, `STATS` / `STATSJSON` / `SMOKEJSON` / `STORAGEJSON` / `RESETSTATS` / `HEALTH`, injected-version `INFO` / `INFOJSON`, `COMMANDS` / `COMMANDSJSON`, `EXPLAINJSON` including schema, digest, side-effect coverage, `CHECKJSON` execution contracts, the stable CHECKJSON write/read fixture samples, INFOJSON/STATSJSON/SMOKEJSON read-only self-description fields, and connection-stat provider formatting. `readonly_fixture_tests` verifies the read-only fixture index, existing CHECKJSON fixture links, exact empty inline `INFOJSON` and `STATSJSON` sample responses, no-write boundaries, dynamic field notes, Node v159 consumer hint, runtime read diagnostics, and runtime field-guide interpretation notes. `recovery_evidence_tests` verifies the restart recovery evidence formatter, stable store digest, snapshot baseline save/load, WAL replay report, exact fixture response, and the `recovered=true` / `digests_match=true` contract. `recovery_fixture_index_tests` verifies the recovery fixture index, stable relative path, expected digest, Node v107 consumer hint, restart/replay cost fields, retention boundary fields, and read-only boundaries. `wal_tests` verifies checksummed WAL records, older plain-record replay compatibility, `SETEXAT` replay for `SETNXEX`, checksum mismatch skipping, truncated-tail detection, compacted WAL replay, WAL repair rewriting, WAL maintenance hints, automatic WAL compaction, configurable compact thresholds, compact counters, and `STATS` / `HEALTH` WAL reporting. `snapshot_tests` verifies snapshot save/load, corrupt snapshot rejection, and atomic overwrite behavior without leaving temporary snapshot files behind. `tcp_server_tests` starts servers on ephemeral ports, sends real inline TCP requests, verifies configurable request-limit rejection, covers `localhost` hostname resolution with address-family agnostic test sockets, requests stop through the server API, and checks the structured listen/accept/reject/close/stop log events plus periodic `event=server_metrics` logs, text and JSONL metrics exporter output, active, total, peak, command, error, breakdown, latency counters, `STATSJSON`, and `RESETSTATS` behavior. `tcp_resp_tests` uses a raw socket like an external client, sends pipelined RESP `PING` / `SET` / `GET` / `SIZE` / `QUIT` requests, and checks exact RESP frames. `tcp_resp_compat_tests` extends the same raw-socket coverage to null bulk replies, integer replies, command errors, protocol errors, `DEL`, `EXPIRE`, and `TTL`. `tcp_resp_concurrency_tests` holds multiple raw-socket RESP clients open at once, releases them together, verifies exact per-client responses, and checks active, total, and peak connection metrics. `client_history_tests` verifies the bundled client's local `:history`, `!!`, and `!N` behavior plus persistent history file load/save, persistent key cache load/save, duplicate filtering, capacity trimming, full cache replacement, full `KEYS` response parsing, and prefixed `KEYS` response rejection for cache refresh. `line_editor_tests` verifies the line editing buffer operations, command Tab completion including `SETNXEX`, `KEYS`, `KEYSJSON`, `INFO`, `INFOJSON`, `COMMANDS`, `COMMANDSJSON`, `EXPLAINJSON`, `CHECKJSON`, `SMOKEJSON`, and `STORAGEJSON`, key-oriented Tab completion, and Up/Down history navigation used by the interactive bundled TCP client. `metrics_file_tests` verifies metrics file truncation, max-byte rotation, retained suffix files, discard-old mode, and invalid option rejection.

## RESP protocol

The RESP parser handles Redis-style request arrays made of bulk strings:

```text
*3\r\n$3\r\nSET\r\n$4\r\nname\r\n$7\r\nmini-kv\r\n
```

It returns the parsed arguments and the number of bytes consumed, so the TCP server can process pipelined input. The server auto-detects RESP requests when a buffered request starts with `*`; otherwise it keeps using the inline text protocol.

Protocol hardening:

```text
Maximum RESP arguments: 1024
Maximum RESP bulk string length: 65536 bytes
```

Oversized RESP requests return a RESP error instead of waiting indefinitely for huge payloads.

## Roadmap

1. Node v179 has completed the production release pre-approval packet without creating an approval decision, writing an approval ledger, or executing release/deployment/rollback/restore.
2. Java v64 and mini-kv v73 completed release operator signoff and retained restore artifact digest fixtures without executing deployment, rollback SQL, LOAD, COMPACT, SETNXEX, or restore.
3. Node v180 completed the approval decision prerequisite gate, and Node v181 completed the approval ledger dry-run envelope without creating a real approval decision, writing the real ledger, or publishing.
4. Java v65 and mini-kv v74 completed rollback approver and restore approval boundary fixtures before Node v182 rehearsed the release approval decision packet.
5. Let Java v66 and mini-kv v75 progress in parallel on release approval rehearsal aggregation and restore boundary read-only smoke manifests before Node v185 performs real-read rehearsal intake.
6. Java v67 and mini-kv v76 prepare real read adapter rehearsal evidence before Node v191 connects to Java HTTP and mini-kv TCP in a disabled-by-default read-only window.
7. Java v68 and mini-kv v77 prepare read-only failure taxonomy evidence before Node v193 classifies real-read adapter failures.
8. Java v69 and mini-kv v78 prepare imported-window verification hints before Node v196 consumes manual real-read window results.
9. Java v70 and mini-kv v79 prepare operator-window identity-neutral proof before Node continues the audit handoff and CI archive manifest work.
10. Java v71 and mini-kv v80 prepare read-only CI/evidence hints before Node v201 verifies the CI artifact manifest.
11. Java v72 and mini-kv v81 prepare read-only artifact retention evidence before Node v203 checks cross-project CI artifact retention.
