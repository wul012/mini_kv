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
- Independent OSFS course-design filesystem with an on-disk image, MFD/UFD directories, persistent users, permissions, descriptor offsets, direct/single-indirect blocks, read-only FSCK, and USERADD/PASSWD
- CMake builds with 352 registered tests across command, WAL, snapshot, RESP, TCP, OSFS, fixture, archive, and governance evidence suites

## Why it matters

This project demonstrates systems engineering discipline in C++:

- Proper thread-safe data structures with minimal critical sections
- Command dispatch architecture and a long-running plan to split oversized command/evidence files without changing public output
- WAL, snapshot, restore, auto-compaction, and evidence correctness
- CI and test coverage to ensure runtime contracts and read-only governance boundaries
- A separate teaching filesystem that exposes disk layout, allocation, authentication, permissions, offsets, and consistency checks without pretending to be the KV engine's production storage path

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
4. OSFS MFD/UFD, indirect-block, FSCK, and user-administration mechanisms with corruption and reopen tests
5. 352 registered tests plus runtime JSON verification

## Latest version summary

Current focus: **v1663 OSFS teaching clarity and resilience evidence**. The bounded elegance program is externally reviewed PASS and closed. Eight mechanism-first Chinese tutorials now connect commands, in-memory state, disk changes, source paths, outputs, and tests; a separate resilience target closes fd-session, group-permission, directory-growth, sparse-range, and inode-bitmap FSCK gaps without changing the disk format or public commands.

Version scheme note: the CMake project version is still `0.102.0` because several historical runtime receipts intentionally identify the frozen v102 runtime fixture. Git tags carry the high-level delivery cadence (`v1608`, `v1609`, and later). The generated `minikv/version.hpp` exposes the CMake version plus a configurable archive hint; changing the hint is a contract decision, not a routine tag bump.

## Where to look next

- `README.md` — overview, latest versions, build/run, protocol, and roadmap
- `docs/CHANGELOG.md` — full versioned project explanations
- `docs/CAPABILITY-SNAPSHOT.md` — dense capability and evidence-field snapshot
- `docs/receipts-consolidation-note.md` — v1625 necessity proof plus v1626-v1628 implementation notes for ordered JSON building, fixture-subobject parity, false shrink metric correction, and no-network drift closure
- `docs/project-docs-honesty-matrix.md` — current requirement-to-evidence map for receipt completion, OSFS mechanisms, archive growth, and brief closure
- `docs/minikv-track-final-evidence.md` — E1-E10 gate map, exact deviation ledger, reproduction commands, and external-review status
- `src/osfs_*.cpp`, `include/minikv/osfs/`, and `tests/osfs_tests.cpp` — independent OSFS implementation and focused executable evidence
- `课程设计交付/v1636-osfs-final/` — current OSFS coursework report, demo, transcript, diagrams, and requirement matrix
- `OSFS课程设计通俗教程/` — student-facing mechanism tutorials, end-to-end command flow, test map, and defense preparation pointers
- `项目通俗说明/` — Chinese project orientation guides with diagrams and input/output explanations
- `src/` — core KV engine and command handling
- `tests/` — runtime and regression tests
- `a/` through `f/` — frozen versioned screenshots and evidence; do not move or rename historical roots
- `docs/production-excellence-progress.md` — current production-excellence gate tracking
- `治理计划/` — mini-kv local governance and archive-retention plans
