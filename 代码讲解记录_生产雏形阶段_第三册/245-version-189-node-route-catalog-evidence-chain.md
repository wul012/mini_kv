# mini-kv v189 - Route catalog evidence chain

## Goal and boundary

v189 still follows the latest Node v471 plan, but the work is mini-kv evidence hygiene: split route catalog evidence-chain cataloging into its own module instead of continuing to grow the closeout formatter. The new section catalogs the v185-v188 route catalog evidence sequence from frozen fixtures.

This is not a Node router, not a route authority, and not runtime participation. It does not start services, probe runtime, enable live reads, install a router, route writes, run admin/LOAD/RESTORE/COMPACT commands, or allow execution.

## Module roles

`include/minikv/shard_readiness_route_catalog_evidence_chain.hpp` declares the new route catalog evidence-chain formatter. `src/shard_readiness_route_catalog_evidence_chain.cpp` owns the catalog payload for v185-v188:

- v185: `nodeRouteCatalogCloseoutSnapshot`
- v186: `nodeRouteCatalogAnchorRemovalAudit`
- v187: `nodeRouteCatalogExpectedIntegritySnapshotAudit`
- v188: `nodeRouteCatalogExpectedIntegritySnapshotFreeze`

Each entry records release version, status, fixture path, primary section, evidence digest, boundary catalog version, field count, group count, and closed boundary flags. The section also publishes catalog-level guarantees such as `versionedFixtureCatalogOnly=true`, `rollingCurrentUsedForFrozenCatalog=false`, `allCatalogedEntriesReadOnly=true`, and `allCatalogedDigestsPresent=true`.

`src/shard_readiness.cpp` wires the new section into `SHARDJSON` and changes v189 status to `node-route-catalog-evidence-chain-read-only`. `CMakeLists.txt` adds the new source file so the split is a real build boundary, not just a logical comment.

## Catalog and tests

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to `read-only-boundary-fields.v17`, with 547 fields and 28 groups. The new `node-route-catalog-evidence-chain` group adds explicit no-runtime, no-router, no-write, and no-execution fields for the catalog section.

`tests/shard_readiness_tests.cpp` verifies the full fixture, the frozen v188 file, the v185-v188 chain entries, the new catalog flags, and the v17 boundary catalog. `tests/command_tests.cpp` mirrors the command path so `SHARDJSON` through the command processor exposes the same evidence.

Validation covered a fresh CMake build, focused `command_tests` and `shard_readiness_tests`, full 71-test CTest, and a TCP SHARDJSON smoke on `127.0.0.1:6462`. The smoke server was stopped after verification.

In one sentence: v189 turns the v185-v188 route catalog evidence sequence into a versioned, read-only chain catalog while keeping mini-kv outside runtime routing and execution.
