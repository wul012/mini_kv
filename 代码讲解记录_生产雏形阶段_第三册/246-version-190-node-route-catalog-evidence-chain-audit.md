# mini-kv v190 - Route catalog evidence chain audit

## Goal and boundary

v190 follows the latest Node plan `docs/plans3/v472-post-route-catalog-cleanup-closeout-roadmap.md`. Node v472 closes the v465-v471 route catalog cleanup batch and records full validation. mini-kv mirrors that as read-only evidence by freezing v189 and adding an audit over the v185-v188 route catalog evidence chain.

This is still not a runtime route. The audit adds no Node report route, no mini-kv service start, no Java start, no runtime probe, no active router, no write routing, no admin/LOAD/RESTORE/COMPACT command, and no execution permission.

## Evidence shape

`src/shard_readiness_node_compatibility.cpp` extends the route/catalog window through Node v472. The shared version vector now covers Node v433-v472, and the window audit computes a 40-version span.

`src/shard_readiness_route_catalog_evidence_chain.cpp` adds `nodeRouteCatalogEvidenceChainAudit`. The audit references the frozen v189 fixture and checks the chain shape: four cataloged releases, range v185-v188, contiguous releases, no duplicates, all digests present, all entries read-only, all fixtures versioned, and latest digest matching the frozen source.

The same audit records the Node v472 closeout validation result: full Vitest passed in shards with 394 files / 1222 tests, typecheck passed, and build passed. Those are evidence fields only; mini-kv does not execute Node and does not consume this as runtime approval.

## Catalog and tests

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to `read-only-boundary-fields.v18`, with 573 fields and 29 groups. The new `node-route-catalog-evidence-chain-audit` group gives downstream consumers direct names for the audit's no-runtime, no-router, no-write, and no-execution boundary fields.

`tests/shard_readiness_tests.cpp` verifies v190, the frozen v189 fixture, Node v472 window expansion, chain audit fields, and boundary catalog growth. `tests/command_tests.cpp` mirrors the command processor checks so `SHARDJSON` stays consistent.

Validation covered a fresh CMake build, focused `command_tests` and `shard_readiness_tests`, full 71-test CTest, and a TCP SHARDJSON smoke on `127.0.0.1:6463`. The smoke server was stopped after verification.

In one sentence: v190 closes the mini-kv side of the Node v472 route catalog cleanup by auditing the frozen evidence chain, not by opening any runtime behavior.
