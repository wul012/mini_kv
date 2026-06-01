# mini-kv v200 node route catalog cleanup evidence batch closeout audit

## Goal and Boundary

v200 freezes v199 and adds `nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit`, a read-only consistency audit over the v194-v198 cleanup evidence batch closeout.

The latest Node plan checked was `docs/plans3/v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md`. That plan treats Node v480 as the route-catalog cleanup evidence batch closeout and leaves Java/mini-kv free to produce versioned evidence in parallel. v200 follows that direction on the C++ side by auditing the mini-kv closeout itself, not by opening a route, router, write path, service launch, runtime probe, approval gate, or execution entry.

## Frozen Baseline

`fixtures/release/shard-readiness-v199.json` freezes the v199 closeout before the rolling fixture advances.

The frozen baseline records:

- `releaseVersion=v199`
- `status=node-route-catalog-cleanup-evidence-batch-closeout-read-only`
- `evidenceDigest=fnv1a64:3a5716f6f09c2b3b`
- `boundaryCatalogIndex.catalogVersion=read-only-boundary-fields.v27`
- `fieldCount=778`
- `groupCount=38`

The v200 rolling fixture reports digest `fnv1a64:d1e889711b5d8574`, boundary catalog `read-only-boundary-fields.v28`, 800 fields, and 39 groups.

## Audit Shape

`src/shard_readiness_route_catalog_cleanup_catalog.cpp` now renders `nodeRouteCatalogCleanupEvidenceBatchCloseoutAudit`.

The audit records the closeout source section, frozen source fixture, frozen digest, expected and observed release counts, v194-v198 range, contiguous range result, duplicate-version result, per-entry read-only and versioned-fixture requirements, digest presence, monotonic field/group counts, latest closeout digest matching the frozen v199 source, Node v480 validation preservation, and archive verifier readiness.

This keeps the v199 closeout useful to a downstream control plane without asking that control plane to trust rolling current as historical evidence.

## Control-Plane Meaning

Control-plane readers should treat v200 as a closeout-audit checkpoint. It says that the v194-v198 closeout exists, is internally consistent, and can be consumed later from a versioned fixture.

The boundary fields keep the important denial semantics visible: `routerActivationAllowed=false`, `writeRoutingAllowed=false`, `executionAllowed=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, `loadRestoreCompactAllowed=false`, and `nodeConsumesFreshMiniKvEvidence=false`.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` checks the in-process JSON, the frozen v199 fixture, the new closeout audit section, boundary catalog v28 counts, and the closeout-audit boundary fields.

`tests/command_tests.cpp` checks that the public `SHARDJSON` command sees the same audit metadata and unchanged read-only/no-execution boundaries.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6473` returned v200 with digest `fnv1a64:d1e889711b5d8574`, boundary catalog v28, 800 fields, 39 groups, `readOnly=True`, router/write disabled, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v200 turns the cleanup evidence batch closeout into audited, versioned, downstream-ready metadata without letting that audit become runtime authority.
