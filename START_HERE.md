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
- CMake builds with CTest coverage across the command, WAL, snapshot, RESP, TCP, fixture, and governance evidence suites

## Why it matters

This project demonstrates systems engineering discipline in C++:

- Proper thread-safe data structures with minimal critical sections
- Command dispatch architecture and a long-running plan to split oversized command/evidence files without changing public output
- WAL, snapshot, restore, auto-compaction, and evidence correctness
- CI and test coverage to ensure runtime contracts and read-only governance boundaries

The README contains the long project history. New production-excellence progress is tracked in `docs/production-excellence-progress.md` so the README does not absorb another governance chain.

## How to run it

```powershell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug --parallel
ctest --test-dir build -C Debug --output-on-failure --timeout 120 --progress
./minikv_server.exe 6506 127.0.0.1
./minikv_client.exe
```

Follow the README for detailed command usage and screenshots.

## Top technical highlights

1. WAL + atomic append mutation separation
2. Command dispatch table and planned command-family split
3. C++20 thread-safe data structures with snapshot/recovery support
4. Full test coverage with CTest and runtime JSON verification

## Latest version summary

Current focus: **v1617 production-excellence K4 command dispatch split, first slice**. K3 has passed planner review, so mini-kv has started the high-risk file-split milestone with the safest command-side cut: `CommandProcessor::execute_trimmed` now lives in `src/command_dispatch.cpp`, shared parse/error helpers live in `src/command_parse_helpers.*`, and the WAL command/compaction gate lives in `src/command_wal_gate.*`. Public command output, fixtures, WAL/snapshot/RESP behavior, router authority, write authority, and execution authority stay unchanged; K4 remains in progress until the remaining command-family and shard-readiness splits are complete.

Version scheme note: the CMake project version is still `0.102.0` because several historical runtime receipts intentionally identify the frozen v102 runtime fixture. Git tags carry the high-level delivery cadence (`v1608`, `v1609`, and later). The generated `minikv/version.hpp` exposes the CMake version plus a configurable archive hint; changing the hint is a contract decision, not a routine tag bump.

## Where to look next

- `README.md` — full versioned project explanations
- `src/` — core KV engine and command handling
- `tests/` — runtime and regression tests
- `a/` through `f/` — frozen versioned screenshots and evidence; do not move or rename historical roots
- `docs/production-excellence-progress.md` — current production-excellence gate tracking
- `治理计划/` — mini-kv local governance and archive-retention plans
