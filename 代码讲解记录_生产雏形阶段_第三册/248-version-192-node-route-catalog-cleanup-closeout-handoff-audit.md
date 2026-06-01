# mini-kv v192 - Route catalog cleanup closeout handoff audit

## Goal and boundary

v192 still follows `docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md`. v191 created a read-only handoff manifest for the Node v472 route catalog cleanup closeout; v192 freezes that handoff and audits the handoff shape itself.

This is not a runtime integration. It adds no route, evidence gate, service startup, runtime probe, active router, write routing, admin command, LOAD/RESTORE/COMPACT behavior, or execution permission.

## Evidence shape

`fixtures/release/shard-readiness-v191.json` is the frozen source. `src/shard_readiness_route_catalog_evidence_chain.cpp` now publishes `nodeRouteCatalogCleanupCloseoutHandoffAudit`, which verifies the handoff included four expected sections, requires a versioned fixture, rejects rolling current as the baseline, requires read-only fixture semantics, preserves the v190 evidence-chain audit, and preserves Node v472 cleanup closeout validation.

The Node route/catalog compatibility window still covers Node v433-v472. Its frozen source now points at v191 and digest `fnv1a64:e6a06c9207144d9d`, so both the window and the new handoff audit are anchored to stable versioned evidence.

## Catalog and tests

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to `read-only-boundary-fields.v20`, with 625 fields and 31 groups. The new `node-route-catalog-cleanup-closeout-handoff-audit` group exposes the audit's included-section, versioned-fixture, rolling-current rejection, consumer-guidance, no-router, no-write, and no-execution fields.

`tests/shard_readiness_tests.cpp` verifies v192, the frozen v191 fixture, the handoff audit, Node v472 window parity, and boundary catalog growth. `tests/command_tests.cpp` mirrors the public `SHARDJSON` command path.

Validation covered a fresh CMake build, focused `command_tests` and `shard_readiness_tests`, full 71-test CTest, and a TCP SHARDJSON smoke on `127.0.0.1:6465`. The smoke server was stopped after verification.

In one sentence: v192 makes the v191 handoff auditable as versioned evidence while keeping mini-kv on the non-executing side of the boundary.
