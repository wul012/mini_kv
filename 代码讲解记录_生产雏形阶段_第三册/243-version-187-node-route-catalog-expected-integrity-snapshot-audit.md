# mini-kv v187 - Node route catalog expected integrity snapshot audit

## Goal and boundary

v187 follows the latest Node plan `docs/plans3/v471-post-route-catalog-expected-integrity-snapshot-roadmap.md`. Node v471 moved the expected audit route catalog integrity snapshot factory into the catalog integrity module and made route-quality consume that factory. mini-kv only freezes the previous v186 evidence and publishes a read-only compatibility audit for that Node-side ownership cleanup.

This is not a new execution path. It does not make mini-kv compute Node route summaries, own Node catalog integrity snapshots, call Java, start mini-kv as a service, install a router, route writes, run LOAD/RESTORE/COMPACT, or relax any runtime execution gate.

## Evidence shape

`fixtures/release/shard-readiness-v186.json` is the frozen source for the new release. The rolling fixture remains `fixtures/release/shard-readiness.json`, but the historical baseline now points at v186 so Node can distinguish frozen evidence from the current mutable publication.

`src/shard_readiness_node_compatibility.cpp` extends the route/catalog maintenance window from Node v433 through Node v471. The formatter still computes the observed count from the shared vector, and the audit still parses the numeric start/end versions to prove that the count and span both equal 39.

`src/shard_readiness_route_catalog_closeout.cpp` adds `nodeRouteCatalogExpectedIntegritySnapshotAudit`. It records four Node-side cleanup steps:

- Node v468 removed residual anchor fields from catalog integrity and route quality.
- Node v469 introduced the typed route catalog summary builder.
- Node v470 centralized expected summary counts.
- Node v471 moved the expected integrity snapshot factory into catalog integrity and made route quality reuse it.

The same section keeps the boundary fields explicit: `miniKvComputesNodeRouteSummary=false`, `miniKvOwnsNodeRouteQualitySnapshot=false`, `miniKvOwnsNodeCatalogIntegritySnapshot=false`, `nodeConsumesFreshMiniKvEvidence=false`, `runtimeProbeAllowed=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, and `executionAllowed=false`.

## Catalog and tests

`src/shard_readiness_boundary_fields.cpp` advances the catalog to `read-only-boundary-fields.v15`. The catalog now has 502 fields and 26 groups, including the new expected integrity snapshot audit fields. Because the field list is generated from the same vector rendered as `readOnlyBoundaryFields`, the count and field names are tested together rather than maintained as disconnected constants.

`tests/shard_readiness_tests.cpp` checks the full SHARDJSON fixture: release version v187, frozen v186 path, Node v433-v471 window, 39-count numeric audit, v471 preservation flag, the new integrity snapshot audit, and the v15 boundary catalog. `tests/command_tests.cpp` repeats the command-level contract so the CLI path and command processor path stay aligned.

The version was validated with CMake rebuild, focused `command_tests` and `shard_readiness_tests`, full 71-test CTest, and a real TCP SHARDJSON smoke on `127.0.0.1:6460`. The smoke server was stopped after the check.

In one sentence: v187 lets Node v471 reuse mini-kv evidence as a versioned, read-only catalog-integrity ownership receipt while mini-kv remains a storage evidence producer, not a route authority or execution participant.
