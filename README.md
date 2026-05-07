# mini-kv

A C++20 practice project for building a small Redis-like key-value engine.

## Current version

Version 1 is a runnable in-memory KV service:

- CMake project layout
- Thread-safe in-memory key-value store
- Interactive command-line client
- TCP server with one-command-per-line protocol
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
- `minikv_store_tests`
- `minikv_command_tests`

## CLI commands

```text
SET key value
GET key
DEL key
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
SIZE
DEL name
QUIT
```

The server keeps all data in memory. Data is lost when the process exits.

## Roadmap

1. Add a small client program.
2. Add TTL support.
3. Add WAL persistence.
4. Add snapshots.
5. Add benchmarking and stress tests.
6. Add a Redis RESP parser.
