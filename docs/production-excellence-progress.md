# Production Excellence Progress

Source plan: `D:\nodeproj\orderops-node\docs\plans\production-excellence-minikv-playbook.md`.

This file is the mini-kv local progress ledger. The Node playbook is read-only from this repo; mini-kv records milestone state here and keeps all edits inside `D:\C\mini-kv`.

## Deviations

- K0 keeps the configured `MINIKV_CURRENT_ARCHIVE_VERSION` default at `c/102/`. That value is not treated as the latest git tag; it is the frozen current-runtime fixture hint consumed by existing runtime receipts and tests. Moving it to CMake removes the hard-coded template value without changing evidence output or breaking downstream path/digest references.
- K1 starts with a diagnostic sanitizer lane instead of making sanitizer required in the same commit. The local CLion Windows MinGW toolchain does not ship `-lasan`/`-lubsan`; `MINIKV_SANITIZE=ON` therefore fails fast on that local toolchain with an explicit message, while the actual ASan/UBSan signal is collected by the Ubuntu CI job. Once that job is green, a follow-up version should remove `continue-on-error: true`.
- K2 also uses Ubuntu as the source of truth for coverage. The local CLion Windows MinGW toolchain can compile default Debug builds, but gcov instrumentation writes `.gcno` sidecar paths long enough to exceed the bundled Windows MinGW object-path limit in this repository. `MINIKV_COVERAGE=ON` therefore fails fast on Windows MinGW with an explicit message, while the Ubuntu coverage CI job builds, runs CTest, and publishes the core coverage report artifact.

## Progress

| Milestone | Version(s) | State | Evidence |
| --------- | ---------- | ----- | -------- |
| K0 | v1609 | completed | START_HERE refresh; CMake-configured archive hint with unchanged `c/102/` output; `metrics_file.hpp` pragma once; `.clang-format`; changed-file clang-format CI check; local configure/build/focused CTest/full CTest/CLI smoke |
| K1 | v1610-v1611 | completed | v1610 added `MINIKV_SANITIZE` and proved the Ubuntu sanitizer diagnostic job green; v1611 removes `continue-on-error: true`, tests that the sanitizer job cannot silently weaken, keeps Windows MinGW local sanitizer limitation as a fast configure failure, and preserves local default configure/build/334 CTest |
| K2 | v1612 | measuring baseline | `MINIKV_COVERAGE`; required Ubuntu coverage CI job; `gcovr` core-module report artifact; local Windows MinGW coverage fast-fail; local default configure/build/334 CTest |
| K3 | — | not started | |
| K4 | — | blocked on K1+K2 | |
| K5 | — | not started | |
| K6 | — | not started | |
