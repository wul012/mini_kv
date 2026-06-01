# mini-kv v191 - Route catalog cleanup closeout handoff

## Goal and boundary

v191 follows the same current Node plan, `docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md`. Node v472 closed the route catalog cleanup batch; mini-kv v190 audited the frozen evidence chain. v191 turns that audit into a downstream handoff manifest by freezing v190 as a versioned fixture.

This is not a runtime bridge. It does not add a report route, service startup, runtime probe, active shard router, write routing, admin command, LOAD/RESTORE/COMPACT path, or execution permission.

## Evidence shape

`fixtures/release/shard-readiness-v190.json` is the frozen input for the new handoff. `src/shard_readiness_route_catalog_evidence_chain.cpp` now publishes `nodeRouteCatalogCleanupCloseoutHandoff`, which tells consumers to read the versioned v190 fixture before rolling current, verify v190 release/status, verify the route catalog evidence chain audit, treat Node v472 validation as documentation evidence only, and fail closed if read-only or execution boundaries drift.

The existing Node route/catalog window still ends at Node v472 with 40 covered versions. Its frozen source now points at v190 and digest `fnv1a64:b24193bae62876e6`, so the compatibility window is tied to a stable audit fixture rather than the moving current file.

## Catalog and tests

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to `read-only-boundary-fields.v19`, with 597 fields and 30 groups. The new `node-route-catalog-cleanup-closeout-handoff` group exposes the handoff's downstream-consumption, versioned-fixture, no-runtime, no-router, no-write, and no-execution fields.

`tests/shard_readiness_tests.cpp` checks the v191 release/status, the frozen v190 fixture, the handoff manifest, Node v472 window parity, and boundary catalog growth. `tests/command_tests.cpp` mirrors the public `SHARDJSON` path so the command processor returns the same contract.

Validation covered a fresh CMake build, focused `command_tests` and `shard_readiness_tests`, full 71-test CTest, and a TCP SHARDJSON smoke on `127.0.0.1:6464`. The smoke server was stopped after verification.

In one sentence: v191 gives downstream consumers a stable v190 cleanup closeout handoff while keeping mini-kv firmly in read-only evidence mode.
