# mini-kv

## Recent versions

The full version history is kept in [`docs/CHANGELOG.md`](docs/CHANGELOG.md). This README keeps only the latest ten entries so the build, run, architecture, and protocol notes remain easy to scan.

- v1623: formalizes the new Chinese project orientation guide under `项目通俗说明/`, adds a small CTest contract that reads the guide and verifies the architecture diagrams, command flow, WAL, Snapshot, TCP/RESP, CHECKJSON, SMOKEJSON, and boundary-field explanations stay present, and records the guide as an onboarding/maintenance surface without changing runtime commands, WAL/snapshot behavior, router authority, write authority, credential boundaries, managed-audit participation, or execution authority.
- v1622: closes the v1621 quality gaps by applying the changed-file clang-format gate locally, removing the three Markdown EOF blank-line issues reported by `git show --check`, replacing the Windows-local `cli_log_level_flag_smoke` stdin waiver with a CMake script that feeds `QUIT` into `minikv_cli --log-level debug`, and proving the closeout with format dry-run, whitespace checks, focused CTest, and full 338/338 local CTest while preserving TCP timeout behavior, archive paths, runtime commands, WAL/snapshot behavior, router authority, write authority, and execution authority.
- v1621: completes production-excellence K6 docs and release discipline by splitting the long README history into `docs/CHANGELOG.md`, adding `docs/SECURITY.md`, `docs/TESTING.md`, and `docs/CAPABILITY-SNAPSHOT.md`, documenting the CMake `0.102.0` vs git-tag `vNNNN` version policy, and adding opt-in TCP client idle/command timeout coverage with no default behavior change.
- v1620: completes production-excellence K5 archive retention by adding `docs/archive-retention-index.md`, a stdlib-only `scripts/archive_inventory.py`, and a non-failing CI archive inventory warning job; it also records the post-K4 Ubuntu coverage retest from run `27428515080`, where removing `--gcov-ignore-parse-errors negative_hits.warn_once_per_file` still failed on a gcov negative-hit parser bug in `src/command_catalog.cpp`, so the workaround remains intentionally protected while preserving all archive paths, fixtures, runtime commands, WAL/snapshot behavior, router authority, write authority, and execution authority.
- v1619: completes production-excellence K4 by splitting the 648-line `src/shard_readiness.cpp` into a tiny public fixture-path shim plus `src/shard_readiness_json.cpp`, `src/shard_readiness_core_digest.cpp`, `src/shard_readiness_boundary_validators.cpp`, and `src/shard_readiness_internal.hpp`; CMake now builds the shard-readiness formatter through focused translation units, fixture parity/full CTest/CLI/TCP smoke prove `SHARDJSON` remains read-only/no-router/no-write/no-WAL/no-execution, and K4 is now ready for planner review before any K5 work starts.
- v1618: continues production-excellence K4 by splitting command execution into four family executors: `src/command_string_ops.cpp`, `src/command_expiry_ops.cpp`, `src/command_persistence_ops.cpp`, and `src/command_introspection_ops.cpp`; `src/command_dispatch.cpp` is now the thin verb router, CMake and the Ubuntu coverage filter include the new command-family files, and command fixtures/full CTest/CLI/TCP smoke prove public command output, CHECKJSON semantics, WAL/snapshot behavior, and no-router/no-execution boundaries remain unchanged.
- v1617: starts production-excellence K4 with a command-dispatch translation-unit split: `CommandProcessor::execute_trimmed` moves from `src/command.cpp` to `src/command_dispatch.cpp`, shared parse/error helpers move to `src/command_parse_helpers.*`, the shared WAL command/compaction gate moves to `src/command_wal_gate.*`, and the Ubuntu coverage filter expands to include the new core command files so the K2 floor still protects moved logic; public command output, fixtures, WAL, snapshots, TCP/RESP, router authority, write authority, and execution authority remain unchanged.
- v1616: completes production-excellence K3 by adding a zero-dependency `minikv::Logger` with `error/warn/info/debug` filtering, UTC timestamp and thread id fields, wiring `--log-level` into `minikv_cli` and `minikv_server`, moving TCP server runtime logs to stderr while keeping server/WAL/evidence stdout unchanged, and proving the boundary with logger unit tests, CLI/server smoke tests, real TCP smoke, and full 337-test CTest.
- v1615: closes the post-K2 review GitHub Actions runtime advisory by moving every workflow checkout step from `actions/checkout@v4` to `actions/checkout@v6.0.3`, moving the coverage artifact upload from `actions/upload-artifact@v4` to `actions/upload-artifact@v7.0.1`, and extending the CI contract test so deprecated Node 20 action runtimes cannot silently return before GitHub's June 16, 2026 Node 24 enforcement, while preserving runtime commands, WAL, snapshots, TCP/RESP, fixtures, archive roots, write authority, router authority, and execution authority unchanged.
- v1614: completes production-excellence K2 coverage enforcement by freezing the green Ubuntu coverage artifact baseline at 2,292 core lines / 2,082 executed / 90% total line coverage, adding a required `gcovr --fail-under-line 88` gate for store/command/WAL/snapshot/RESP core modules, extending the CI contract test so the floor cannot be silently removed, and recording the Claude review checkpoint while preserving runtime commands, WAL, snapshots, TCP/RESP, fixtures, archive roots, write authority, router authority, and execution authority unchanged.

## Current version

Current focus is tracked by the latest entry in `## Recent versions` and by `docs/production-excellence-progress.md`.
The long per-version history now lives in `docs/CHANGELOG.md`, and the detailed capability/evidence field snapshot lives in `docs/CAPABILITY-SNAPSHOT.md`.

Core surfaces:

- Thread-safe C++20 in-memory key-value store
- Inline text and RESP-over-TCP server/client flows
- WAL, snapshot, restore, and compaction evidence boundaries
- Runtime read-only JSON evidence through INFOJSON, STATSJSON, SMOKEJSON, STORAGEJSON, COMMANDSJSON, EXPLAINJSON, and CHECKJSON
- CI-backed CMake/CTest matrix with sanitizer, coverage, format, and archive-inventory lanes
- Explicit no-router, no-order-authority, no-write-expansion, and no-execution governance fields for Node/Java consumption

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

Dependabot is configured only for GitHub Actions:

```text
.github/dependabot.yml
```

It runs weekly, groups minor and patch action updates, and ignores semver-major automatic version updates. mini-kv does not currently configure npm, Maven, pip, cargo, Docker, vcpkg, or Conan updates; those should be added only after the repository actually adopts the corresponding dependency manager.

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

`KEYS` returns currently live key names in sorted order as `key_count=N keys=...`; expired keys are pruned before the list is produced. `KEYS prefix` returns only keys that start with that prefix as `key_count=N prefix=prefix keys=...`. The prefixed response intentionally differs from the full-list response so the bundled client does not replace its full local key cache with a filtered subset. `KEYSJSON [prefix]` returns the same live key inventory as JSON with `prefix`, `key_count`, `keys`, `truncated`, and `limit`; the default limit is `1000`, and the command never returns values. `SETNXEX key seconds value` is a short-TTL token primitive: it sets a missing or expired key with a value and positive TTL, returns `1` when the claim is accepted, returns `0` when a live token already exists, and does not make mini-kv an order-authoritative store. `STATS` returns a compact key-value status line with live key count, command counters, command latency counters, per-command breakdown, WAL maintenance details when enabled, and TCP connection counters when executed through the server. `STATSJSON` returns the same operational snapshot as one read-only JSON object, which is easier for scripts and log collectors to parse. It includes `schema_version=1`, `read_only=true`, `execution_allowed=false`, `order_authoritative=false`, `evidence_type=runtime_metrics`, and diagnostics showing `write_commands_executed=false`; latency counters remain dynamic runtime fields. `SMOKEJSON` returns a combined read-only runtime smoke object for external read adapters: it includes identity, version, storage, WAL, command metrics, connection stats, allowed real-read commands, forbidden commands (`LOAD`, `COMPACT`, `SETNXEX`, `RESTORE`), `live_read_session` fields for Node v205 execution packet preparation (`session_id_echo="mini-kv-live-read-v102"`, `server_uptime_bucket`, `read_command_list_digest="fnv1a64:5bef33f2fbe65cc5"`, `write_commands_allowed=false`, `auto_start_allowed=false`), failure taxonomy categories and stable `taxonomy_digest`, `operator_window.identity_neutral_proof=true`, CI evidence hints, artifact retention evidence, binary provenance digest `fnv1a64:d9776044d393ecbc`, retention provenance digest `fnv1a64:8cf7cc0df218035f`, replay marker digest `fnv1a64:6b9ef93f40cbd343`, command dispatch quality receipt `fnv1a64:0f2caa6931b482b8`, adapter shell non-storage guard receipt `fnv1a64:0174fa831d17cea4`, managed audit external adapter non-participation receipt `fnv1a64:9bacde73d6d07097`, managed audit sandbox adapter non-participation receipt `fnv1a64:f0cae7a4ce0674c2`, sandbox connection echo marker `fnv1a64:beb8dd6a0b102a11`, and sandbox connection no-start guard receipt `fnv1a64:69e377d01f15fd57`; diagnostics still show `write_commands_executed=false`, `admin_commands_executed=false`, and `runtime_write_observed=false`. `INFO` returns build and runtime identity as `version=... protocol=... uptime_seconds=... live_keys=... wal_enabled=... metrics_enabled=... max_request_bytes=...`; the version comes from the CMake project version generated into `minikv/version.hpp`. `INFOJSON` returns the same identity contract as read-only JSON: `schema_version`, `read_only`, `execution_allowed`, `order_authoritative`, `evidence_type`, `version`, `server.protocol`, `server.uptime_seconds`, `server.max_request_bytes`, `store.live_keys`, `wal.enabled`, `metrics.enabled`, `ci_evidence`, `artifact_retention`, managed audit receipts, `command_dispatch_quality_receipt`, `adapter_shell_non_storage_guard_receipt`, `managed_audit_external_adapter_non_participation_receipt`, `managed_audit_sandbox_adapter_non_participation_receipt`, and diagnostics. `COMMANDS` returns a compact text command catalog, and `COMMANDSJSON` returns the same catalog as `{"commands":[...]}` entries with `name`, `category`, `mutates_store`, `touches_wal`, `stable`, and `description`. Categories are `meta`, `read`, `write`, and `admin`; this is descriptive metadata, not an ACL or permission system. `EXPLAINJSON command` returns `schema_version`, `command_digest`, `command`, `category`, `mutates_store`, `touches_wal`, `key`, `requires_value`, `ttl_sensitive`, `allowed_by_parser`, `side_effects`, `side_effect_count`, and `warnings` for the supplied command line without executing it. `schema_version` is currently `1`; `command_digest` is a stable `fnv1a64:<hex>` evidence fingerprint over the normalized explain fields, not an authentication signature. `side_effects` uses stable labels such as `store_write`, `store_ttl_update`, `snapshot_file_write`, `store_replace_from_snapshot`, `wal_rewrite_when_enabled`, `metrics_reset`, `connection_close`, and `metadata_read`; `side_effect_count` mirrors the number of labels. `CHECKJSON command` wraps the same explain evidence into a read-only execution contract with `read_only=true`, `execution_allowed=false`, `checks`, `wal`, and contract warnings. Its `wal.durability` is `wal_backed`, `memory_only`, `not_wal_backed`, or `not_applicable` depending on the target command and whether this processor has a WAL. `STORAGEJSON` is also read-only and returns `schema_version`, `read_only`, `execution_allowed`, `version`, `store.live_keys`, `store.order_authoritative=false`, `wal`, `snapshot`, `side_effects`, `side_effect_count`, and `diagnostics`. Its diagnostic notes explicitly mark the evidence as `not_order_authoritative`, and the command reports WAL status without appending or compacting the WAL. `RESETSTATS` clears command execution metrics; a successful reset is not counted as a new command, so the next `STATS` or `STATSJSON` response starts from zero. `HEALTH` returns an `OK ...` line for quick liveness checks with the same WAL, command metric, and connection signals. `command_breakdown` uses `COMMAND:total/success/error/total_latency_ns/avg_latency_ns/max_latency_ns` entries separated by semicolons; unknown commands are grouped under `UNKNOWN`.

`SHARDROUTE key` and `SHARDROUTEJSON key` are read-only shard route preview commands. They compute the preview slot and `shard-0` assignment used by the current single-shard readiness prototype, preserve the historical sample slots from `SHARDJSON`, and never install an active router, create shard directories, touch WAL, write storage, execute admin commands, or make mini-kv order/audit authoritative.

Stable sample responses live under `fixtures/checkjson/`, `fixtures/readonly/`, `fixtures/ttl-token/`, `fixtures/release/`, and `fixtures/recovery/`.

`fixtures/checkjson/set-orderops-write-contract.json` is the exact no-WAL response for `CHECKJSON SET orderops:1 value`; `fixtures/checkjson/get-orderops-read-contract.json` is the exact read-command response for `CHECKJSON GET orderops:1`, including `side_effects=["store_read"]` and `side_effect_count=1`. `fixtures/readonly/index.json` groups those CHECKJSON contracts with `INFOJSON`, `STATSJSON`, and `runtime-read-field-guide.json` evidence for Node v159 release evidence review preparation while keeping `read_only=true`, `execution_allowed=false`, and `order_authoritative=false`.

`fixtures/ttl-token/index.json` documents the v61 `SETNXEX` token primitive for Node v160 readiness review, and `fixtures/ttl-token/recovery-evidence.json` documents the v62 TTL recovery boundary across WAL, Snapshot, restart, expired-token, and compaction paths.

`fixtures/release/verification-manifest.json` is the v102 release verification manifest for Node v239 manual sandbox connection operator window evidence verification: it lists expected CMake configure/build, CTest, targeted tests, read-only `SMOKEJSON` live-read session hints (`session_id_echo="mini-kv-live-read-v102"`, `read_command_list_digest="fnv1a64:5bef33f2fbe65cc5"`, `write_commands_allowed=false`, `auto_start_allowed=false`), INFOJSON/SMOKEJSON CI evidence hints, artifact retention evidence, fixture inventory, version evidence, no-runtime-write boundary, failure taxonomy digest evidence, operator-window identity-neutral proof, retained restore and rollback evidence samples, runtime smoke evidence, binary provenance digest `fnv1a64:d9776044d393ecbc`, retention provenance check digest `fnv1a64:8cf7cc0df218035f`, replay marker digest `fnv1a64:6b9ef93f40cbd343`, managed audit adapter restore boundary digest `fnv1a64:1177efbaceb52c1c`, managed audit adapter non-authoritative storage digest `fnv1a64:667590b83f510a58`, command dispatch quality digest `fnv1a64:0f2caa6931b482b8`, adapter shell non-storage guard digest `fnv1a64:0174fa831d17cea4`, external adapter non-participation digest `fnv1a64:9bacde73d6d07097`, sandbox adapter non-participation digest `fnv1a64:f0cae7a4ce0674c2`, sandbox connection receipt echo marker digest `fnv1a64:beb8dd6a0b102a11`, and sandbox connection no-start guard receipt digest `fnv1a64:69e377d01f15fd57`. The connection marker consumes v95 sandbox adapter evidence (`consumed_release_version="v95"`, `consumed_artifact_path_hint="c/95/"`, `consumed_receipt_digest="fnv1a64:ceaed265f7f9560c"`) and echoes Node v228 operator packet field handles without allowing mini-kv to connect, read credential values, run schema rehearsal, write managed audit state, execute restore/load/compact, or become Java order authority.

`fixtures/release/release-artifact-digest-package.json` records binary, WAL, Snapshot, and fixture digest placeholders plus restore drill commands for Node v170; it uses `CHECKJSON LOAD data/release-artifact-drill.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX release:token 30 value` only as read-only package checks, and `GET release:token` must remain `(nil)`. `fixtures/release/restore-drill-evidence.json` records the v70 restore drill target, digest comparison placeholder, operator confirmation, and CHECKJSON dry-run commands for Node v173; it uses `CHECKJSON LOAD data/restore-drill.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:drill-token 30 value` only as read-only drill checks, and `GET restore:drill-token` must remain `(nil)`. `fixtures/release/restore-handoff-checklist.json` records the v71 restore handoff checklist for Node v175; it uses `CHECKJSON LOAD data/handoff-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:handoff-token 30 value` only as read-only risk confirmation, and `GET restore:handoff-token` must remain `(nil)`. `fixtures/release/restore-evidence-retention.json` records the v72 restore evidence retention fields for Node v178; it uses `CHECKJSON LOAD data/retention-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:retention-token 30 value` only as retained risk evidence, and `GET restore:retention-token` must remain `(nil)`. `fixtures/release/retained-restore-artifact-digest.json` records the v73 retained restore artifact digest fields for Node v180; it uses `CHECKJSON LOAD data/retained-digest-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:digest-token 30 value` only as retained digest evidence, and `GET restore:digest-token` must remain `(nil)`. `fixtures/release/restore-approval-boundary.json` records the v74 restore approval boundary fields for Node v182; it uses `CHECKJSON LOAD data/approval-boundary-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:approval-token 30 value` only as approval-boundary risk evidence, and `GET restore:approval-token` must remain `(nil)`. `fixtures/release/restore-boundary-smoke-manifest.json` records the v75 real-read smoke manifest for Node v185; it uses `CHECKJSON LOAD data/real-read-boundary-restore.snap`, `CHECKJSON COMPACT`, and `CHECKJSON SETNXEX restore:real-read-token 30 value` only as real-read risk evidence, and `GET restore:real-read-token` must remain `(nil)`. `fixtures/release/runtime-smoke-evidence.json` records the v102 `SMOKEJSON` read target for Node v239 manual sandbox connection operator window evidence verification; it uses `SMOKEJSON`, `INFOJSON`, `STORAGEJSON`, `HEALTH`, and `GET restore:real-read-token` as read-only runtime evidence, adds `live_read_session` hints (`session_id_echo="mini-kv-live-read-v102"`, `read_command_list_digest="fnv1a64:5bef33f2fbe65cc5"`, `write_commands_allowed=false`, `auto_start_allowed=false`), failure taxonomy digest `fnv1a64:f92fcba55feb26a2`, operator-window/CI/artifact-retention hints, the v95 receipt chain through sandbox adapter non-participation digest `fnv1a64:f0cae7a4ce0674c2`, the v96 sandbox connection receipt echo marker digest `fnv1a64:beb8dd6a0b102a11`, and the no-start guard receipt digest `fnv1a64:69e377d01f15fd57`; `GET restore:real-read-token` must remain `(nil)` and mini-kv must not become an external adapter storage backend, sandbox audit storage backend, sandbox connection participant, credential value reader, schema rehearsal executor, local dry-run writer, managed audit writer, or Java order authority.

`fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` also keep the historical managed audit adapter non-authoritative storage receipt for Node v217, the v88 command dispatch quality receipt for Node v219, the v89 adapter shell non-storage guard receipt for Node v221, the v90 external adapter non-participation receipt for Node v223, and the v95 sandbox adapter non-participation receipt for Node v225. Version 102 keeps the historical no-start guard receipt for Node v231 in the current evidence chain and updates `fixtures/release/current-runtime-fixture-rolling-guard.json` so Node v239 can accept the rolling current fixture (`0.102.0`, `v102`, `c/102/`, `mini-kv-live-read-v102`, and current receipt digests) while the historical consumed digest anchors from v84, v85, v86, v87, v88, v89, v90, v95, and v96 remain fixed; it proves the manual window is closed by default while mini-kv is not auto-started by Node, Java, or itself, does not execute sandbox connection, does not read credential values, does not run schema rehearsal, does not write managed audit state, and still leaves INFOJSON/SMOKEJSON read-only contracts, CHECKJSON/EXPLAINJSON field contracts, KEYS/STATS/HEALTH/STORAGEJSON/WALINFO response contracts, write responses, WAL append-before-mutation order, no-op miss behavior, admin handlers, snapshot, restore, and Java order authority untouched. Version 102 carries forward `fixtures/release/runtime-no-start-no-write-follow-up.json`, keeps the runtime `SMOKEJSON.runtime_no_start_no_write_follow_up` object for Node v239 evidence verification, and adds `fixtures/release/operator-window-no-start-no-write-receipt.json` plus `SMOKEJSON.operator_window_no_start_no_write_receipt` so Node can verify the Node v238 checklist echo without mini-kv auto-start, connection execution, storage writes, credential value reads, schema rehearsal, managed audit writes, restore/load/compact/SETNXEX execution, or order authority.

`command_tests` compares both CHECKJSON fixtures with real command output and checks the TTL token fixture index, `readonly_fixture_tests` compares the read-only fixture package with real `CHECKJSON`, `INFOJSON`, and `STATSJSON` output plus the runtime read field guide, `recovery_evidence_tests` and `recovery_fixture_index_tests` protect restart evidence, `ttl_token_recovery_tests` exercises TTL recovery, and the release tests protect the manifest, rollback evidence, bundle manifest, restore handoff, dry-run operator package, digest matrix, release artifact digest package, restore drill evidence, restore handoff checklist, restore evidence retention, retained restore artifact digest, restore approval boundary, restore boundary smoke, runtime smoke no-execution boundaries, SMOKEJSON live-read session hints, failure taxonomy digest/sample fields, operator-window identity-neutral proof, CI evidence hints, artifact retention evidence, managed audit restore-boundary receipts, managed audit non-authoritative storage receipts, command dispatch quality receipts, adapter shell non-storage guard receipts, external adapter non-participation receipts, sandbox adapter non-participation receipts, sandbox connection echo markers, sandbox connection no-start guard receipts, and the current runtime fixture rolling guard.

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
minikv_benchmark.exe [--evidence-json] [operations] [key_count]
```

The benchmark prints elapsed time and operations per second for direct `Store` operations and command-layer `SET` / `GET` operations. With `--evidence-json`, it emits one JSON line for CI or local smoke checks. That JSON is still a lightweight local signal: it mutates only an ephemeral in-process store, does not enable WAL, does not start networking, does not execute snapshot/restore/load/compact, does not read credentials, and does not open a managed audit connection.

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
12. Java v73 and mini-kv v82 prepare live-read friendly runtime fields before Node v205 runs the three-project real-read execution packet.
