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

Current focus: **v1628 runtime receipts consolidation quality correction / no-network drift closure**. This version does not migrate another receipt formatter. It corrects the false shrink metric in `docs/receipts-consolidation-note.md`: line-count shrink is a review metric, not a stop-condition, so v1627's +116 diff-line abort/rollback growth is accepted only because byte parity, boundary closure, and builder testability improved. `runtime_receipt_json_builder_tests` now drives no-network drift to ground: both objects have 118 top-level fields, and the only raw-byte delta is the single `boundary` phrase `Node v323\u0027s` in the committed fixture versus `Node v323's` in the current formatter. No semantic field, digest, no-network/no-write/WAL/credential/execution boundary, fixture, or formatter migration changes in this version.

Version scheme note: the CMake project version is still `0.102.0` because several historical runtime receipts intentionally identify the frozen v102 runtime fixture. Git tags carry the high-level delivery cadence (`v1608`, `v1609`, and later). The generated `minikv/version.hpp` exposes the CMake version plus a configurable archive hint; changing the hint is a contract decision, not a routine tag bump.

## Where to look next

- `README.md` — overview, latest versions, build/run, protocol, and roadmap
- `docs/CHANGELOG.md` — full versioned project explanations
- `docs/CAPABILITY-SNAPSHOT.md` — dense capability and evidence-field snapshot
- `docs/receipts-consolidation-note.md` — v1625 necessity proof plus v1626-v1628 implementation notes for ordered JSON building, fixture-subobject parity, false shrink metric correction, and no-network drift closure
- `项目通俗说明/` — Chinese project orientation guides with diagrams and input/output explanations
- `src/` — core KV engine and command handling
- `tests/` — runtime and regression tests
- `a/` through `f/` — frozen versioned screenshots and evidence; do not move or rename historical roots
- `docs/production-excellence-progress.md` — current production-excellence gate tracking
- `治理计划/` — mini-kv local governance and archive-retention plans
