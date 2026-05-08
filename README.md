# mini-kv

A C++20 practice project for building a small Redis-like key-value engine.

## Current version

Version 3 is a runnable in-memory KV service with TTL support:

- CMake project layout
- Thread-safe in-memory key-value store
- Interactive command-line client
- TCP server with one-command-per-line protocol
- TCP client for connecting to a running server
- Expiring keys with `EXPIRE` and `TTL`
- Minimal CTest coverage for store and command behavior

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

TCP server:

```powershell
.\build\Debug\minikv_server.exe 6379
```

TCP client:

```powershell
.\build\Debug\minikv_client.exe 127.0.0.1 6379
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
- `minikv_store_tests`
- `minikv_command_tests`

## CLI commands

```text
SET key value
GET key
DEL key
EXPIRE key seconds
TTL key
SIZE
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
DEL name
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

## Roadmap

1. Add WAL persistence.
2. Add snapshots.
3. Add benchmarking and stress tests.
4. Add a Redis RESP parser.
