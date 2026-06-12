# Production Excellence Progress

Source plan: `D:\nodeproj\orderops-node\docs\plans\production-excellence-minikv-playbook.md`.

This file is the mini-kv local progress ledger. The Node playbook is read-only from this repo; mini-kv records milestone state here and keeps all edits inside `D:\C\mini-kv`.

## Deviations

- K0 keeps the configured `MINIKV_CURRENT_ARCHIVE_VERSION` default at `c/102/`. That value is not treated as the latest git tag; it is the frozen current-runtime fixture hint consumed by existing runtime receipts and tests. Moving it to CMake removes the hard-coded template value without changing evidence output or breaking downstream path/digest references.

## Progress

| Milestone | Version(s) | State | Evidence |
| --------- | ---------- | ----- | -------- |
| K0 | v1609 | completed | START_HERE refresh; CMake-configured archive hint with unchanged `c/102/` output; `metrics_file.hpp` pragma once; `.clang-format`; changed-file clang-format CI check; local configure/build/focused CTest/full CTest/CLI smoke |
| K1 | — | not started | |
| K2 | — | not started | baseline: __% |
| K3 | — | not started | |
| K4 | — | blocked on K1+K2 | |
| K5 | — | not started | |
| K6 | — | not started | |
