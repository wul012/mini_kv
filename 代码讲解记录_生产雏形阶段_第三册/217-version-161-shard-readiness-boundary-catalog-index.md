# mini-kv v161: boundary catalog index and count

v161 follows the current Node v420 route-group split maintenance context. Node is still reorganizing route registrations without changing API paths or starting sibling services, so mini-kv uses this version to harden its own shard-readiness evidence rather than adding runtime behavior.

This is not runtime approval, not a new gate, and not active shard routing. It does not change `SHARDJSON` usage, does not move the rolling fixture path, does not start Java, does not start mini-kv on the declared approval port, does not run a runtime probe, does not enable live read, does not route writes, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_boundary_fields.cpp` now owns a shared `read_only_boundary_field_names()` vector. `format_read_only_boundary_fields_json()` renders the public `readOnlyBoundaryFields` array from that vector, and `format_catalog_index_json()` uses the same vector to compute `fieldCount`.

That small refactor matters because v160 had already moved the catalog out of the main `SHARDJSON` formatter, but the next maintenance risk was count drift: a human could add a field to the public array and forget to update the explanatory index. In v161, the index and the public list share the same source.

`include/minikv/shard_readiness_boundary_fields.hpp` exposes the new `format_catalog_index_json()` function. `src/shard_readiness.cpp` keeps top-level ordering and digest ownership, adding `boundaryCatalogIndex` before `readOnlyBoundaryFields`.

## Evidence Contract

The current `releaseVersion` is `v161` and status is `boundary-field-catalog-index-read-only`.

`fixtures/release/shard-readiness-v160.json` freezes the v160 boundary catalog split with digest `fnv1a64:c96ae9497cfd6386`. The current rolling fixture is regenerated from real `SHARDJSON` output and has digest `fnv1a64:2193962195ba633a`.

`boundaryCatalogIndex` declares `catalogVersion=read-only-boundary-fields.v2`, `fieldCount=290`, and `groupCount=15`. It also lists the catalog groups, including top-level read-only fields, historical fallback, live-read gate, approval inputs, Node route-group compatibility, boundary maintenance, and the index itself.

The same object keeps the boundary flags explicit: public contract, fixture path, command shape, evidence gate, runtime gate approval, executable runtime packet, service start, runtime probe, live read, router activation, write routing, write/admin commands, load/restore/compact, and execution all remain false.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` now validates current v161, all historical shard-readiness fixtures through v160, `fieldCount=290`, `groupCount=15`, shared-vector generation, and the new index boundary fields.

`tests/command_tests.cpp` verifies the same index through `CommandProcessor`, keeping the command surface and fixture path honest.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6432`. The declared approval port `6424` remains unstarted, and the GET probe remains `(nil)`.

In one sentence: v161 makes the boundary field catalog easier to audit without loosening any mini-kv runtime, router, write, or execution boundary.
