# mini-kv

A C++20 practice project for building a small Redis-like key-value engine.

## Current version

Version 22 is a runnable in-memory KV service with atomic snapshot saves:

- CMake project layout
- Thread-safe in-memory key-value store
- Interactive command-line client
- TCP server with inline text and RESP request support
- TCP client for connecting to a running server
- Expiring keys with `EXPIRE` and `TTL`
- Optional write-ahead log persistence for `SET`, `DEL`, and `EXPIRE`
- WAL replay reports applied, skipped, and truncated records
- Manual snapshots with `SAVE` and `LOAD`
- Snapshot load rejects corrupt files without replacing the current store
- Atomic snapshot saves write a temporary file before replacing the target snapshot
- Benchmark executable for quick local throughput checks
- CTest coverage for store, command, WAL, snapshot, stress, RESP, and TCP server behavior
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

TCP server:

```powershell
.\build\Debug\minikv_server.exe 6379
```

TCP server with WAL:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 data\mini-kv.wal
```

TCP server with explicit limits:

```powershell
.\build\Debug\minikv_server.exe 6379 127.0.0.1 --max-request-bytes 65536 --accept-poll-ms 200
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

The server prints structured lifecycle logs using key-value fields:

```text
event=server_start host=127.0.0.1 port=6379 protocol=inline,resp
event=tcp_listen host=127.0.0.1 port=6379 max_request_bytes=65536
event=tcp_client_accepted host=127.0.0.1 port=6379 connection_id=1 active_connections=1 total_connections=1 peak_connections=1
event=tcp_request_rejected host=127.0.0.1 port=6379 connection_id=1 reason=request_too_long pending_bytes=70000 max_request_bytes=65536
event=tcp_client_closed host=127.0.0.1 port=6379 connection_id=1 active_connections=0 total_connections=1 peak_connections=1
event=tcp_stop host=127.0.0.1 port=6379 active_connections=0 total_connections=1 peak_connections=1
event=server_stopped host=127.0.0.1 port=6379
```

Use `Ctrl+C` or `SIGTERM` to request a graceful server stop. The accept loop wakes periodically, observes the stop request, closes the listener, and exits `server.run()`.

TCP server options:

```text
minikv_server.exe [port] [host] [wal_path] [--max-request-bytes bytes] [--accept-poll-ms ms]
```

`--max-request-bytes` limits the buffered bytes for one pending TCP request. Inline over-limit requests return `ERR line too long`; RESP over-limit requests return `-ERR request too long`.

TCP client options:

```text
minikv_client.exe [host] [port] [timeout_ms] [--connect-retries count] [--retry-delay-ms ms]
                  [--history-file path]
```

`timeout_ms` sets both receive and send socket timeouts for the bundled client.
`--connect-retries` controls how many times the client retries after the initial connection attempt fails. `--retry-delay-ms` controls the delay between retries and defaults to 250 ms.
The bundled client also handles local session history commands before sending data to the server: `:history`, `!!`, and `!N`. When `--history-file` is set, the client loads existing commands from that file at startup and saves the bounded history after each successful sent command.

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

`EXPIRE` is persisted as an internal absolute expiration record, so expired keys do not come back after restart. WAL replay skips malformed records, detects a final unterminated record as a probable partial write, and reports applied, skipped, and truncated counts during startup. Without a WAL path, mini-kv remains an in-memory-only service.

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

`stress_tests` runs multiple writer and eraser threads against one shared `Store`, then checks snapshot export/restore and final key consistency. `snapshot_tests` verifies snapshot save/load, corrupt snapshot rejection, and atomic overwrite behavior without leaving temporary snapshot files behind. `tcp_server_tests` starts servers on ephemeral ports, sends real inline TCP requests, verifies configurable request-limit rejection, covers `localhost` hostname resolution with address-family agnostic test sockets, requests stop through the server API, and checks the structured listen/accept/reject/close/stop log events plus active, total, and peak connection metrics. `tcp_resp_tests` uses a raw socket like an external client, sends pipelined RESP `PING` / `SET` / `GET` / `SIZE` / `QUIT` requests, and checks exact RESP frames. `tcp_resp_compat_tests` extends the same raw-socket coverage to null bulk replies, integer replies, command errors, protocol errors, `DEL`, `EXPIRE`, and `TTL`. `tcp_resp_concurrency_tests` holds multiple raw-socket RESP clients open at once, releases them together, verifies exact per-client responses, and checks active, total, and peak connection metrics. `client_history_tests` verifies the bundled client's local `:history`, `!!`, and `!N` behavior plus persistent history file load/save and capacity trimming.

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

1. Add interactive line editing for the bundled TCP client.
2. Add WAL checksums for stronger recovery guarantees.
