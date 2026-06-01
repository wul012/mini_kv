# mini-kv v199 node route catalog cleanup evidence batch closeout

## Goal and Boundary

v199 freezes v198 and adds `nodeRouteCatalogCleanupEvidenceBatchCloseout`, a read-only closeout over the mini-kv cleanup evidence sequence v194-v198.

The latest Node plan checked was `docs/plans3/v480-post-java-mini-kv-route-catalog-cleanup-evidence-batch-closeout-roadmap.md`. Node v480 closes its v473-v479 cleanup evidence batch and says later work can start from clean Java/mini-kv tags. v199 follows that shape on the C++ side: close the mini-kv evidence batch without adding a route, router, write path, service launch, runtime probe, approval gate, or execution entry.

## Frozen Baseline

`fixtures/release/shard-readiness-v198.json` freezes the v198 package audit before the rolling fixture advances.

The frozen baseline records:

- `releaseVersion=v198`
- `status=node-route-catalog-cleanup-latest-evidence-package-audit-read-only`
- `evidenceDigest=fnv1a64:8fcbe53d8b1700b7`
- `boundaryCatalogIndex.catalogVersion=read-only-boundary-fields.v26`
- `fieldCount=756`
- `groupCount=37`

The v199 rolling fixture reports digest `fnv1a64:3a5716f6f09c2b3b`, boundary catalog `read-only-boundary-fields.v27`, 778 fields, and 38 groups.

## Closeout Shape

`src/shard_readiness_route_catalog_cleanup_catalog.cpp` now renders `nodeRouteCatalogCleanupEvidenceBatchCloseout`.

The closeout records five mini-kv entries:

- v194: cleanup closeout release catalog
- v195: release catalog audit
- v196: catalog formatter-split maintenance
- v197: latest evidence package
- v198: latest evidence package audit

Each entry keeps fixture path, primary section, digest, boundary catalog version, field count, group count, read-only state, and execution denial.

## Control-Plane Meaning

Control-plane readers should treat v199 as a batch checkpoint. It says the v194-v198 mini-kv evidence chain is complete enough for later Node/Java consumption from clean tags, while the rolling current fixture is still not a historical baseline.

The no-runtime contract remains explicit: `rollingCurrentUsedForFrozenCloseout=false`, `closeoutOnly=true`, `nodeConsumesFreshMiniKvEvidence=false`, `runtimeExecutionPacketExecutable=false`, `startsMiniKvService=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, and `executionAllowed=false`.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` checks the in-process JSON, the frozen v198 fixture, the new closeout section, boundary catalog v27 counts, and the closeout boundary fields.

`tests/command_tests.cpp` checks that the public `SHARDJSON` command sees the same closeout metadata and unchanged read-only/no-execution boundaries.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6472` returned v199 with digest `fnv1a64:3a5716f6f09c2b3b`, boundary catalog v27, 778 fields, 38 groups, `readOnly=True`, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v199 gives the cleanup evidence batch a clean, versioned stopping point without letting that closeout become runtime authority.
