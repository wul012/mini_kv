# mini-kv v193 - Route catalog cleanup closeout handoff audit freeze

## Goal and boundary

v193 continues under `docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md`. v192 audited the cleanup closeout handoff manifest; v193 freezes that audit as versioned evidence so downstream consumers have a stable fixture instead of relying on rolling current.

This is not an execution step. It adds no route, evidence gate, service startup, runtime probe, active router, write routing, admin command, LOAD/RESTORE/COMPACT behavior, or execution permission.

## Evidence shape

`fixtures/release/shard-readiness-v192.json` is the frozen source. `src/shard_readiness_route_catalog_evidence_chain.cpp` now publishes `nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze`, which records the v192 digest `fnv1a64:89bf4177b09f1ec0`, the frozen v192 status, the v20 boundary catalog counts, downstream-readiness preservation, rolling-current rejection, and Node v472 cleanup validation preservation.

The Node route/catalog compatibility window remains Node v433-v472. Its frozen source now points at v192 and digest `fnv1a64:89bf4177b09f1ec0`, keeping the window tied to a stable handoff audit fixture.

## Catalog and tests

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to `read-only-boundary-fields.v21`, with 648 fields and 32 groups. The new group exposes the freeze's preservation, no-router, no-write, and no-execution fields.

`tests/shard_readiness_tests.cpp` verifies v193, the frozen v192 fixture, the handoff audit freeze, Node v472 window parity, and boundary catalog growth. `tests/command_tests.cpp` mirrors the public `SHARDJSON` command path.

Validation covered a fresh CMake build, focused `command_tests` and `shard_readiness_tests`, full 71-test CTest, and a TCP SHARDJSON smoke on `127.0.0.1:6466`. The smoke server was stopped after verification.

In one sentence: v193 freezes the v192 handoff audit so the closeout chain remains stable, read-only, and non-executing.
