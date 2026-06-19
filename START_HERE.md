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

The README keeps the project overview and latest versions. Long history lives in `docs/CHANGELOG.md`, detailed capability notes live in `docs/CAPABILITY-SNAPSHOT.md`, and production-excellence progress is tracked in `docs/production-excellence-progress.md`.

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

Current focus: **v1625 runtime receipts consolidation design gate**. The production-excellence K4 stretch scope is now backed by `docs/receipts-consolidation-note.md`: 28 runtime receipt files / 788,811 bytes are inventoried, the 26-file credential-resolver family is selected as the first target, and future code migration must begin with an ordered zero-dependency builder plus fixture-subobject byte parity. v1625 changes no runtime formatter, fixture, digest, command, router, write path, credential/network boundary, or execution authority.

Version scheme note: the CMake project version is still `0.102.0` because several historical runtime receipts intentionally identify the frozen v102 runtime fixture. Git tags carry the high-level delivery cadence (`v1608`, `v1609`, and later). The generated `minikv/version.hpp` exposes the CMake version plus a configurable archive hint; changing the hint is a contract decision, not a routine tag bump.

## Where to look next

- `README.md` — overview, latest versions, build/run, protocol, and roadmap
- `docs/CHANGELOG.md` — full versioned project explanations
- `docs/CAPABILITY-SNAPSHOT.md` — dense capability and evidence-field snapshot
- `docs/receipts-consolidation-note.md` — v1625 necessity proof, ordered formatter design, byte-parity gate, and phased receipt migration plan
- `项目通俗说明/` — Chinese project orientation guides with diagrams and input/output explanations
- `src/` — core KV engine and command handling
- `tests/` — runtime and regression tests
- `a/` through `f/` — frozen versioned screenshots and evidence; do not move or rename historical roots
- `docs/production-excellence-progress.md` — current production-excellence gate tracking
- `治理计划/` — mini-kv local governance and archive-retention plans
