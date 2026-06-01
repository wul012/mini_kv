# mini-kv v188 - Expected integrity snapshot freeze

## Goal and boundary

v188 keeps following the latest Node plan, `docs/plans3/v471-post-route-catalog-expected-integrity-snapshot-roadmap.md`, but this version is mini-kv-side closure work rather than a new Node feature. v187 introduced the read-only `nodeRouteCatalogExpectedIntegritySnapshotAudit`; v188 freezes that evidence as `fixtures/release/shard-readiness-v187.json` so downstream consumers can rely on a versioned baseline instead of rolling current.

This is not runtime activation. It does not start Java or mini-kv services, does not probe runtime, does not install a router, does not permit write routing, does not run admin/LOAD/RESTORE/COMPACT commands, and does not make mini-kv a Node catalog integrity owner.

## Evidence shape

`src/shard_readiness_node_compatibility.cpp` keeps the Node v433-v471 window at 39 versions, but the window source now points at `fixtures/release/shard-readiness-v187.json` with digest `fnv1a64:f4459b7beacceedd`. This makes the route window evidence chain explicit: v188 reads the frozen v187 audit instead of interpreting the latest rolling fixture as a historical baseline.

`src/shard_readiness_route_catalog_closeout.cpp` adds `nodeRouteCatalogExpectedIntegritySnapshotFreeze`. The section records the frozen release, fixture path, digest, status, window end, route group count, boundary catalog version, and frozen field count. Its key boundary fields stay closed: `rollingCurrentUsedForFrozenBaseline=false`, `miniKvOwnsNodeCatalogIntegritySnapshot=false`, `nodeConsumesFreshMiniKvEvidence=false`, `runtimeProbeAllowed=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, and `executionAllowed=false`.

`src/shard_readiness_history.cpp` adds the v187 fixture to fixture parity and moves historical fallback to the v187 frozen evidence. That keeps the current fixture free to advance while preserving the prior consumer baseline.

## Catalog and tests

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to `read-only-boundary-fields.v16`. The new freeze group increases the catalog to 525 fields and 27 groups, including explicit freeze boundary names for runtime, router, write, and execution denial.

`tests/shard_readiness_tests.cpp` validates the full fixture, the added frozen v187 file, the new freeze section, the v16 catalog, and the historical fixture chain. `tests/command_tests.cpp` mirrors the command-facing checks to prove the `SHARDJSON` command path publishes the same evidence.

Validation covered a fresh CMake build, focused `command_tests` and `shard_readiness_tests`, full 71-test CTest, and a TCP SHARDJSON smoke on `127.0.0.1:6461`. The smoke server was stopped after verification.

In one sentence: v188 turns v187's Node v471 catalog-integrity audit into a stable versioned baseline without opening runtime, router, write, or execution authority.
