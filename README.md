# mini-kv

A C++20 practice project for building a small Redis-like key-value engine.

## Current version

Version 6 is a runnable in-memory KV service with benchmarks and stress tests:

- CMake project layout
- Thread-safe in-memory key-value store
- Interactive command-line client
- TCP server with one-command-per-line protocol
- TCP client for connecting to a running server
- Expiring keys with `EXPIRE` and `TTL`
- Optional write-ahead log persistence for `SET`, `DEL`, and `EXPIRE`
- Manual snapshots with `SAVE` and `LOAD`
- Benchmark executable for quick local throughput checks
- CTest coverage for store, command, WAL, snapshot, and stress behavior

## Build

```powershell
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

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

TCP client:

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379
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

## CLI commands

```text
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

Connect with a TCP client, then send one command per line:

```text
SET name mini-kv
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

`EXPIRE` is persisted as an internal absolute expiration record, so expired keys do not come back after restart. Without a WAL path, mini-kv remains an in-memory-only service.

## Snapshots

Use snapshots to save the current in-memory dataset to a compact file and load it later:

```text
SAVE path  Save all currently live keys to a snapshot file.
LOAD path  Replace the current in-memory dataset with a snapshot file.
```

Snapshot files preserve values and absolute expiration times. Expired keys are not saved, and expired records are skipped when loading. `LOAD` is a manual state replacement command; pair it with WAL intentionally if you want both baseline snapshots and later write replay.

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

`stress_tests` runs multiple writer and eraser threads against one shared `Store`, then checks snapshot export/restore and final key consistency.

## Roadmap

1. Add a Redis RESP parser.
