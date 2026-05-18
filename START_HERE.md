# Start Here: mini_kv

## What this project does

This repository implements a Redis-like in-memory key-value store written in C++20.

Features:
- Thread-safe in-memory store
- WAL write-ahead logging
- TCP client-server communication
- RESP protocol support
- Runtime JSON evidence for commands and snapshots
- Command execution audit with SMOKEJSON/INFOJSON/KEYSJSON outputs
- CMake + Ninja build with CTest

## Why it matters

This project demonstrates systems engineering discipline in C++:

- Proper thread-safe data structures with minimal critical sections
- Command dispatch architecture and code refactoring (v106 table-driven command dispatch)
- WAL, snapshot, restore, auto-compaction, and evidence correctness
- CI and test coverage to ensure runtime contracts

The README contains full walkthroughs of how each version evolved and why v106 introduces a dispatch table for clarity and maintainability.

## How to run it

```powershell
mkdir build && cd build
cmake -G Ninja ..
ninja
ctest --output-on-failure
./minikv_server.exe 6506 127.0.0.1
./minikv_client.exe
```

Follow the README for detailed command usage and screenshots.

## Top technical highlights

1. WAL + atomic append mutation separation
2. Command dispatch table and `CommandDispatchVerb` switch
3. C++20 thread-safe data structures with snapshot/recovery support
4. Full test coverage with CTest and runtime JSON verification

## Latest version summary

Current focus: **v106 table-drive command dispatch**. Converts the long `if (command == ...)` chain into a dispatch table plus enum-based switch. Behavior unchanged but easier to audit and extend.

## Where to look next

- `README.md` — full versioned project explanations
- `src/` — core KV engine and command handling
- `tests/` — runtime and regression tests
- `c/` — versioned screenshots and evidence
- `docs/` — optional detailed command and runtime explanation
