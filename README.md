# mini-kv

A C++20 practice project for building a small Redis-like key-value engine.

## Current version

Version 31 is a runnable in-memory KV service with bundled TCP client line editing:

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
- Runtime `STATS` and `HEALTH` commands expose live keys, WAL maintenance state, and server connection counters
- Optional `--metrics-interval-ms` emits periodic structured `event=server_metrics` connection counters
- Manual snapshots with `SAVE` and `LOAD`
- Snapshot load rejects corrupt files without replacing the current store
- Atomic snapshot saves write a temporary file before replacing the target snapshot
- Benchmark executable for quick local throughput checks
- CTest coverage for store, command, WAL, snapshot, stress, RESP, TCP server, client history, and line editor behavior
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
- Bundled TCP client terminal line editing with arrow-key history navigation and cursor editing
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

Each CI job configures the project, builds every target, and runs the registered CTest suite with failure output enabled.

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

TCP client local history commands:

```text
:history  Show commands sent during the current client session.
!!        Repeat the previous sent command.
!N        Repeat history entry N, using 1-based indexing.
```

When the bundled TCP client runs in an interactive terminal, it supports Up/Down history navigation, Left/Right cursor movement, Home/End, Backspace, and Delete. When stdin is redirected or piped, it keeps the original line-by-line input behavior.

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

## CLI commands

```text
PING [message]
SET key value
GET key
DEL key
EXPIRE key seconds
TTL key
SIZE
SAVE path
LOAD path
COMPACT
WALINFO
STATS
HEALTH
HELP
EXIT
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
SAVE data\mini-kv.snapshot
DEL name
LOAD data\mini-kv.snapshot
COMPACT
WALINFO
STATS
HEALTH
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

`STATS` returns a compact key-value status line with live key count, WAL maintenance details when enabled, and TCP connection counters when executed through the server. `HEALTH` returns an `OK ...` line for quick liveness checks with the same WAL and connection signals.

The server prints structured lifecycle logs using key-value fields:

```text
event=server_start host=127.0.0.1 port=6379 protocol=inline,resp max_request_bytes=65536 accept_poll_ms=200 metrics_interval_ms=0 auto_compact_wal=false
event=tcp_listen host=127.0.0.1 port=6379 max_request_bytes=65536
event=tcp_client_accepted host=127.0.0.1 port=6379 connection_id=1 active_connections=1 total_connections=1 peak_connections=1
event=server_metrics host=127.0.0.1 port=6379 active_connections=1 total_connections=1 peak_connections=1 metrics_interval_ms=10000
event=tcp_request_rejected host=127.0.0.1 port=6379 connection_id=1 reason=request_too_long pending_bytes=70000 max_request_bytes=65536
event=tcp_client_closed host=127.0.0.1 port=6379 connection_id=1 active_connections=0 total_connections=1 peak_connections=1
event=tcp_stop host=127.0.0.1 port=6379 active_connections=0 total_connections=1 peak_connections=1
event=server_stopped host=127.0.0.1 port=6379
```

Use `Ctrl+C` or `SIGTERM` to request a graceful server stop. The accept loop wakes periodically, observes the stop request, closes the listener, and exits `server.run()`.

TCP server options:

```text
minikv_server.exe [port] [host] [wal_path] [--repair-wal] [--auto-compact-wal]
                  [--wal-compact-min-records count] [--wal-compact-record-ratio ratio]
                  [--wal-compact-min-bytes bytes] [--max-request-bytes bytes]
                  [--accept-poll-ms ms] [--metrics-interval-ms ms]
```

`--max-request-bytes` limits the buffered bytes for one pending TCP request. Inline over-limit requests return `ERR line too long`; RESP over-limit requests return `-ERR request too long`.
`--metrics-interval-ms` enables periodic `event=server_metrics` logs. It is disabled by default and reports active, total, and peak connection counters when enabled.

TCP client options:

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
                  [--history-file path]
```

`timeout_ms` sets both receive and send socket timeouts for the bundled client.
`--connect-retries` controls how many times the client retries after the initial connection attempt fails. `--retry-delay-ms` controls the delay between retries and defaults to 250 ms.
The bundled client also handles local session history commands before sending data to the server: `:history`, `!!`, and `!N`. When `--history-file` is set, the client loads existing commands from that file at startup and saves the bounded history after each successful sent command.
On an interactive terminal, the bundled client edits the current line locally with arrow keys, Home/End, Backspace, and Delete. Up/Down walk the same in-memory or loaded history that powers `:history`, `!!`, and `!N`. Redirected stdin still uses normal line-by-line reading for scripts and smoke tests.

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

`EXPIRE` is persisted as an internal absolute expiration record, so expired keys do not come back after restart. New WAL records are stored as `WAL2 <checksum> <record>` lines; replay still accepts older plain `SET` / `DEL` / `EXPIREAT` lines. WAL replay skips malformed records, skips checksum mismatches, detects a final unterminated record as a probable partial write, and reports applied, skipped, truncated, and checksum-failed counts during startup. `COMPACT` is available when a WAL path is configured; it rewrites the log to the current live key set using checksummed `SET` and `EXPIREAT` records, dropping overwritten, deleted, and expired history. Start `minikv_cli` or `minikv_server` with `--repair-wal` to replay the recoverable records and immediately rewrite the WAL to a clean compacted WAL2 file before serving commands. Startup also reports WAL bytes, physical record count, live keys, compact thresholds, compaction counters, and whether compaction is recommended; use `WALINFO`, `STATS`, or `HEALTH` at runtime to inspect the same maintenance signal. Start with `--auto-compact-wal` to compact automatically when the same maintenance thresholds recommend it, both after startup replay and after WAL-backed write commands. Use `--wal-compact-min-records`, `--wal-compact-record-ratio`, and `--wal-compact-min-bytes` with a WAL path to tune those thresholds. Without a WAL path, mini-kv remains an in-memory-only service.

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

`stress_tests` runs multiple writer and eraser threads against one shared `Store`, then checks snapshot export/restore and final key consistency. `wal_tests` verifies checksummed WAL records, older plain-record replay compatibility, checksum mismatch skipping, truncated-tail detection, compacted WAL replay, WAL repair rewriting, WAL maintenance hints, automatic WAL compaction, configurable compact thresholds, compact counters, and `STATS` / `HEALTH` WAL reporting. `snapshot_tests` verifies snapshot save/load, corrupt snapshot rejection, and atomic overwrite behavior without leaving temporary snapshot files behind. `tcp_server_tests` starts servers on ephemeral ports, sends real inline TCP requests, verifies configurable request-limit rejection, covers `localhost` hostname resolution with address-family agnostic test sockets, requests stop through the server API, and checks the structured listen/accept/reject/close/stop log events plus periodic `event=server_metrics` logs and active, total, and peak connection metrics surfaced through `STATS` / `HEALTH`. `tcp_resp_tests` uses a raw socket like an external client, sends pipelined RESP `PING` / `SET` / `GET` / `SIZE` / `QUIT` requests, and checks exact RESP frames. `tcp_resp_compat_tests` extends the same raw-socket coverage to null bulk replies, integer replies, command errors, protocol errors, `DEL`, `EXPIRE`, and `TTL`. `tcp_resp_concurrency_tests` holds multiple raw-socket RESP clients open at once, releases them together, verifies exact per-client responses, and checks active, total, and peak connection metrics. `client_history_tests` verifies the bundled client's local `:history`, `!!`, and `!N` behavior plus persistent history file load/save and capacity trimming. `line_editor_tests` verifies the line editing buffer operations and Up/Down history navigation used by the interactive bundled TCP client.

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

1. Add command execution counters and error counters.
2. Add interactive command completion for the bundled TCP client.
