# mini-kv v162: read-only slot table preview

v162 follows the current Node v425 route-group split maintenance context. Node is still extracting JSON/Markdown route registrations without changing API paths, adding gates, or starting Java/mini-kv, so mini-kv can keep advancing its own versioned evidence in parallel.

This is not an active shard router, not write routing, not runtime approval, and not a new execution path. It does not change `SHARDJSON` usage, does not move the rolling fixture path, does not create shard directories, does not start Java or mini-kv, does not run a runtime probe, does not enable live read, does not write storage, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_slot_preview.cpp` now owns the small shard-map and slot-preview formatter surface. It renders the existing `shardMap`, the existing `keyRoutingSamples`, and the new `slotTablePreview` object from one local module instead of letting `src/shard_readiness.cpp` keep growing at the top.

`include/minikv/shard_readiness_slot_preview.hpp` exposes four narrow functions: `slot_count()`, `format_shard_map_json()`, `format_route_samples_json()`, and `format_slot_table_preview_json()`. The main `src/shard_readiness.cpp` still owns release identity, top-level JSON ordering, historical fallback, and digest composition.

`src/shard_readiness_boundary_fields.cpp` keeps the read-only field catalog as the boundary audit source. v162 extends it with `slotTablePreview.*` no-router/no-write/no-execution fields and bumps the catalog index to `read-only-boundary-fields.v3`.

## Evidence Contract

The current `releaseVersion` is `v162` and status is `slot-table-preview-read-only`.

`fixtures/release/shard-readiness-v161.json` freezes the v161 boundary catalog index with digest `fnv1a64:2193962195ba633a`. The current rolling fixture is regenerated from real `SHARDJSON` output and has digest `fnv1a64:8c01a3657f07cfd1`.

`slotTablePreview` materializes all 16 slots as evidence-only assignments to `shard-0`. The section declares `slotTablePreviewOnly=true`, `readOnlyShardMapPreview=true`, `activeRouterInstalled=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, `startsServices=false`, `runtimeProbeAllowed=false`, `liveReadAllowed=false`, and `executionAllowed=false`.

`boundaryCatalogIndex` now declares `catalogVersion=read-only-boundary-fields.v3`, `fieldCount=303`, and `groupCount=16`. The added group is `slot-table-preview`, and the public `readOnlyBoundaryFields` array includes the preview boundary fields from the same shared vector used for the count.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` validates current v162, all historical shard-readiness fixtures through v161, the 16-slot preview, v161 frozen digest, `fieldCount=303`, `groupCount=16`, and the new `slotTablePreview.*` boundary fields.

`tests/command_tests.cpp` verifies the same preview through `CommandProcessor`, keeping the CLI command path and the fixture path aligned with the library formatter.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6435`. The declared approval port `6424` remains unstarted, and the GET probe remains `(nil)`.

In one sentence: v162 gives downstream consumers a concrete read-only slot table shape while keeping mini-kv firmly out of active routing, writes, service startup, and execution.
