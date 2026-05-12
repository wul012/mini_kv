# mini-kv

A C++20 practice project for building a small Redis-like key-value engine.

## Current version

Version 53 is a runnable in-memory KV service with injected build version metadata, command catalog metadata, bounded JSON key inventory, digest-aware structured command risk explanation, read-only write-command execution contracts, and stable CHECKJSON write/read fixture samples:

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
- Runtime `INFOJSON` exposes the same identity metadata as a schema-stable JSON object for external control planes
- Runtime `COMMANDS` and `COMMANDSJSON` expose supported command names, risk categories, mutation flags, WAL-touch flags, stability, and descriptions
- Runtime `EXPLAINJSON command` explains a command's parser validity, key target, TTL sensitivity, mutation risk, WAL risk, side effects, schema version, command digest, and side-effect count without executing it
- Runtime `CHECKJSON command` returns a read-only execution contract for a command, including parser, write-command, side-effect, command digest, and WAL durability checks without executing it
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

## CLI commands

```text
PING [message]
SET key value
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
RESETSTATS
HEALTH
INFO
INFOJSON
COMMANDS
COMMANDSJSON
EXPLAINJSON command
CHECKJSON command
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

`KEYS` returns currently live key names in sorted order as `key_count=N keys=...`; expired keys are pruned before the list is produced. `KEYS prefix` returns only keys that start with that prefix as `key_count=N prefix=prefix keys=...`. The prefixed response intentionally differs from the full-list response so the bundled client does not replace its full local key cache with a filtered subset. `KEYSJSON [prefix]` returns the same live key inventory as JSON with `prefix`, `key_count`, `keys`, `truncated`, and `limit`; the default limit is `1000`, and the command never returns values. `STATS` returns a compact key-value status line with live key count, command counters, command latency counters, per-command breakdown, WAL maintenance details when enabled, and TCP connection counters when executed through the server. `STATSJSON` returns the same operational snapshot as one JSON object, which is easier for scripts and log collectors to parse. `INFO` returns build and runtime identity as `version=... protocol=... uptime_seconds=... live_keys=... wal_enabled=... metrics_enabled=... max_request_bytes=...`; the version comes from the CMake project version generated into `minikv/version.hpp`. `INFOJSON` returns the same identity contract as JSON: `version`, `server.protocol`, `server.uptime_seconds`, `server.max_request_bytes`, `store.live_keys`, `wal.enabled`, and `metrics.enabled`. `COMMANDS` returns a compact text command catalog, and `COMMANDSJSON` returns the same catalog as `{"commands":[...]}` entries with `name`, `category`, `mutates_store`, `touches_wal`, `stable`, and `description`. Categories are `meta`, `read`, `write`, and `admin`; this is descriptive metadata, not an ACL or permission system. `EXPLAINJSON command` returns `schema_version`, `command_digest`, `command`, `category`, `mutates_store`, `touches_wal`, `key`, `requires_value`, `ttl_sensitive`, `allowed_by_parser`, `side_effects`, `side_effect_count`, and `warnings` for the supplied command line without executing it. `schema_version` is currently `1`; `command_digest` is a stable `fnv1a64:<hex>` evidence fingerprint over the normalized explain fields, not an authentication signature. `side_effects` uses stable labels such as `store_write`, `store_ttl_update`, `snapshot_file_write`, `store_replace_from_snapshot`, `wal_rewrite_when_enabled`, `metrics_reset`, `connection_close`, and `metadata_read`; `side_effect_count` mirrors the number of labels. `CHECKJSON command` wraps the same explain evidence into a read-only execution contract with `read_only=true`, `execution_allowed=false`, `checks`, `wal`, and contract warnings. Its `wal.durability` is `wal_backed`, `memory_only`, `not_wal_backed`, or `not_applicable` depending on the target command and whether this processor has a WAL. `RESETSTATS` clears command execution metrics; a successful reset is not counted as a new command, so the next `STATS` or `STATSJSON` response starts from zero. `HEALTH` returns an `OK ...` line for quick liveness checks with the same WAL, command metric, and connection signals. `command_breakdown` uses `COMMAND:total/success/error/total_latency_ns/avg_latency_ns/max_latency_ns` entries separated by semicolons; unknown commands are grouped under `UNKNOWN`.

Stable sample responses live under `fixtures/checkjson/`. `fixtures/checkjson/set-orderops-write-contract.json` is the exact no-WAL response for `CHECKJSON SET orderops:1 value`; `fixtures/checkjson/get-orderops-read-contract.json` is the exact read-command response for `CHECKJSON GET orderops:1`, including `side_effects=["store_read"]` and `side_effect_count=1`. `command_tests` compares both fixtures with real command output to catch drift before Node smoke tests consume them.

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
On an interactive terminal, the bundled client edits the current line locally with Tab completion, arrow keys, Home/End, Backspace, and Delete. Tab completes server command names including `KEYS`, `KEYSJSON`, `STATSJSON`, `RESETSTATS`, `COMMANDSJSON`, `EXPLAINJSON`, and `CHECKJSON`, plus local `:history`; it also completes remembered key names in the second token of `GET`, `SET`, `DEL`, `EXPIRE`, and `TTL`. Up/Down walk the same in-memory or loaded history that powers `:history`, `!!`, and `!N`. Redirected stdin still uses normal line-by-line reading for scripts and smoke tests.

TTL commands:

```text
EXPIRE key seconds  Set a positive second-based expiration for an existing key.
TTL key             Return -2 when the key is missing, -1 when it has no expiration, or remaining seconds otherwise.
```

`SET` replaces the value and clears any existing TTL for that key. The server keeps all data in memory. Data is lost when the process exits.

## WAL persistence

Pass a WAL path to `minikv_cli` or `minikv_server` to enable persistence. On startup, the program replays the WAL into memory. During runtime, successful write commands append records before applying the mutation:

```text
SET key value
DEL key
EXPIRE key seconds
```

`EXPIRE` is persisted as an internal absolute expiration record, so expired keys do not come back after restart. New WAL records are stored as `WAL2 <checksum> <record>` lines; replay still accepts older plain `SET` / `DEL` / `EXPIREAT` lines. WAL replay skips malformed records, skips checksum mismatches, detects a final unterminated record as a probable partial write, and reports applied, skipped, truncated, and checksum-failed counts during startup. `COMPACT` is available when a WAL path is configured; it rewrites the log to the current live key set using checksummed `SET` and `EXPIREAT` records, dropping overwritten, deleted, and expired history. Start `minikv_cli` or `minikv_server` with `--repair-wal` to replay the recoverable records and immediately rewrite the WAL to a clean compacted WAL2 file before serving commands. Startup also reports WAL bytes, physical record count, live keys, compact thresholds, compaction counters, and whether compaction is recommended; use `WALINFO`, `STATS`, `STATSJSON`, or `HEALTH` at runtime to inspect the same maintenance signal. Start with `--auto-compact-wal` to compact automatically when the same maintenance thresholds recommend it, both after startup replay and after WAL-backed write commands. Use `--wal-compact-min-records`, `--wal-compact-record-ratio`, and `--wal-compact-min-bytes` with a WAL path to tune those thresholds. Without a WAL path, mini-kv remains an in-memory-only service.

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

`stress_tests` runs multiple writer and eraser threads against one shared `Store`, then checks snapshot export/restore and final key consistency. `command_tests` verifies command parsing, command execution counters, per-command breakdown, unknown-command grouping, latency counters, full and prefix-filtered `KEYS`, bounded `KEYSJSON`, `STATS` / `STATSJSON` / `RESETSTATS` / `HEALTH`, injected-version `INFO` / `INFOJSON`, `COMMANDS` / `COMMANDSJSON`, `EXPLAINJSON` including schema, digest, side-effect coverage, `CHECKJSON` execution contracts, the stable CHECKJSON write/read fixture samples, and connection-stat provider formatting. `wal_tests` verifies checksummed WAL records, older plain-record replay compatibility, checksum mismatch skipping, truncated-tail detection, compacted WAL replay, WAL repair rewriting, WAL maintenance hints, automatic WAL compaction, configurable compact thresholds, compact counters, and `STATS` / `HEALTH` WAL reporting. `snapshot_tests` verifies snapshot save/load, corrupt snapshot rejection, and atomic overwrite behavior without leaving temporary snapshot files behind. `tcp_server_tests` starts servers on ephemeral ports, sends real inline TCP requests, verifies configurable request-limit rejection, covers `localhost` hostname resolution with address-family agnostic test sockets, requests stop through the server API, and checks the structured listen/accept/reject/close/stop log events plus periodic `event=server_metrics` logs, text and JSONL metrics exporter output, active, total, peak, command, error, breakdown, latency counters, `STATSJSON`, and `RESETSTATS` behavior. `tcp_resp_tests` uses a raw socket like an external client, sends pipelined RESP `PING` / `SET` / `GET` / `SIZE` / `QUIT` requests, and checks exact RESP frames. `tcp_resp_compat_tests` extends the same raw-socket coverage to null bulk replies, integer replies, command errors, protocol errors, `DEL`, `EXPIRE`, and `TTL`. `tcp_resp_concurrency_tests` holds multiple raw-socket RESP clients open at once, releases them together, verifies exact per-client responses, and checks active, total, and peak connection metrics. `client_history_tests` verifies the bundled client's local `:history`, `!!`, and `!N` behavior plus persistent history file load/save, persistent key cache load/save, duplicate filtering, capacity trimming, full cache replacement, full `KEYS` response parsing, and prefixed `KEYS` response rejection for cache refresh. `line_editor_tests` verifies the line editing buffer operations, command Tab completion including `KEYS`, `KEYSJSON`, `INFO`, `INFOJSON`, `COMMANDS`, `COMMANDSJSON`, `EXPLAINJSON`, and `CHECKJSON`, key-oriented Tab completion, and Up/Down history navigation used by the interactive bundled TCP client. `metrics_file_tests` verifies metrics file truncation, max-byte rotation, retained suffix files, discard-old mode, and invalid option rejection.

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

1. Add external control-plane smoke notes once Node consumes the CHECKJSON write/read fixtures in the v78 scenario matrix.
2. Consider additional CHECKJSON fixture samples only after Node proves it needs more command shapes for diagnostics.
