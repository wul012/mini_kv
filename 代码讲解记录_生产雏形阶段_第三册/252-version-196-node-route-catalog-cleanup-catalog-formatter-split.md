# mini-kv v196 node route catalog cleanup catalog formatter split

## Goal and Boundary

v196 is the refactoring version after several feature/evidence versions. It freezes v195 and moves cleanup closeout catalog formatting out of `shard_readiness_route_catalog_evidence_chain.cpp` into a dedicated `shard_readiness_route_catalog_cleanup_catalog` module.

The latest Node plan checked was `docs/plans3/v477-post-java-mini-kv-latest-route-catalog-cleanup-evidence-report-archive-roadmap.md`. Node v477 archives existing report output and explicitly stays away from dirty sibling workspaces or runtime startup. v196 follows that boundary: this is a formatter ownership split, not a runtime route, router, write path, service launch, live probe, approval gate, or execution packet.

## Frozen Baseline

`fixtures/release/shard-readiness-v195.json` freezes the v195 release catalog audit before v196 changes the rolling fixture.

The frozen baseline records:

- `releaseVersion=v195`
- `status=node-route-catalog-cleanup-closeout-release-catalog-audit-read-only`
- `evidenceDigest=fnv1a64:36463a895469c4bc`
- `boundaryCatalogIndex.catalogVersion=read-only-boundary-fields.v23`
- `fieldCount=693`
- `groupCount=34`

The v196 rolling fixture reports digest `fnv1a64:116ec6b912bb4026`, boundary catalog `read-only-boundary-fields.v24`, 714 fields, and 35 groups.

## Module Split

`include/minikv/shard_readiness_route_catalog_cleanup_catalog.hpp` declares the cleanup catalog surface:

- `format_route_catalog_cleanup_closeout_release_catalog_json()`
- `format_route_catalog_cleanup_closeout_release_catalog_audit_json()`
- `format_route_catalog_cleanup_closeout_catalog_maintenance_json()`

`src/shard_readiness_route_catalog_cleanup_catalog.cpp` now owns the v194 catalog formatter, the v195 catalog audit formatter, and the new v196 maintenance formatter.

`src/shard_readiness_route_catalog_evidence_chain.cpp` keeps the earlier evidence-chain, handoff, handoff-audit, and handoff-audit-freeze formatters. That keeps the file from becoming an ever-growing catch-all for every later cleanup-closeout layer.

`CMakeLists.txt` adds the new source file to the `minikv` library.

`src/shard_readiness.cpp` includes the new header, calls the new namespace for the catalog/audit sections, and publishes `nodeRouteCatalogCleanupCloseoutCatalogMaintenance`.

## Maintenance Contract

The new maintenance section records:

- `splitFrom=src/shard_readiness_route_catalog_evidence_chain.cpp`
- `splitInto=src/shard_readiness_route_catalog_cleanup_catalog.cpp`
- `extractedCatalogFormatter=true`
- `extractedCatalogAuditFormatter=true`
- `preservesReleaseCatalog=true`
- `preservesReleaseCatalogAudit=true`
- `publicCatalogSectionsChanged=false`

Control-plane readers should treat this as a code ownership proof. It does not alter the meaning of `nodeRouteCatalogCleanupCloseoutReleaseCatalog` or `nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit`.

The familiar boundary fields remain decisive: `runtimeExecutionPacketExecutable=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, and `executionAllowed=false`.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` checks the in-process JSON, the v195 frozen fixture, the new maintenance section, boundary catalog v24 counts, and the newly published maintenance boundary fields.

`tests/command_tests.cpp` checks the public `SHARDJSON` command surface sees the same split evidence and unchanged runtime boundaries.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6469` returned v196 with digest `fnv1a64:116ec6b912bb4026`, boundary catalog v24, 714 fields, 35 groups, `readOnly=True`, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v196 pays down formatter growth debt while preserving the versioned cleanup-closeout evidence chain and all read-only runtime boundaries.
