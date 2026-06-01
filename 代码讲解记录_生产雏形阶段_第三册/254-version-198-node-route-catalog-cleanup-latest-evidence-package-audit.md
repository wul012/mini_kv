# mini-kv v198 node route catalog cleanup latest evidence package audit

## Goal and Boundary

v198 freezes v197 and audits `nodeRouteCatalogCleanupLatestEvidencePackage` as a package, rather than adding another consumer route or runtime behavior.

The latest Node plan checked was `docs/plans3/v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md`. Node v479 exposes archive verification through existing audit routes and explicitly treats Java/mini-kv as parallel work. v198 follows that line: it is a versioned package audit, not a route, router, write path, service launch, runtime probe, approval gate, or execution entry.

## Frozen Baseline

`fixtures/release/shard-readiness-v197.json` freezes the v197 package before the rolling fixture advances.

The frozen baseline records:

- `releaseVersion=v197`
- `status=node-route-catalog-cleanup-latest-evidence-package-read-only`
- `evidenceDigest=fnv1a64:836d9f0b3956fe30`
- `boundaryCatalogIndex.catalogVersion=read-only-boundary-fields.v25`
- `fieldCount=735`
- `groupCount=36`

The v198 rolling fixture reports digest `fnv1a64:8fcbe53d8b1700b7`, boundary catalog `read-only-boundary-fields.v26`, 756 fields, and 37 groups.

## Audit Shape

`include/minikv/shard_readiness_route_catalog_cleanup_catalog.hpp` adds `format_latest_route_catalog_cleanup_evidence_package_audit_json()`.

`src/shard_readiness_route_catalog_cleanup_catalog.cpp` renders the audit with:

- `expectedPackageReleaseCount=3`
- `observedPackageReleaseCount=3`
- `packageRangeStart=v194`
- `packageRangeEnd=v196`
- `releaseVersionsContiguous=true`
- `duplicatePackageVersionsDetected=false`
- `allPackagedEntriesReadOnly=true`
- `allPackagedFixturesVersioned=true`
- `allPackagedDigestsPresent=true`
- `latestPackagedDigestMatchesPackageSource=true`

The package source remains v196 with digest `fnv1a64:116ec6b912bb4026`; the audit source is the frozen v197 fixture with digest `fnv1a64:836d9f0b3956fe30`.

## Control-Plane Meaning

Control-plane readers can now check both layers: v197 says what the package contains, and v198 says the package is complete, contiguous, versioned, digest-backed, and still ready for archive verification.

The decisive no-runtime fields remain explicit: `rollingCurrentRejected=true`, `packageAuditOnly=true`, `runtimeExecutionPacketExecutable=false`, `startsMiniKvService=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, `loadRestoreCompactAllowed=false`, and `executionAllowed=false`.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` checks the in-process JSON, the frozen v197 fixture, the new package audit section, boundary catalog v26 counts, and the new audit boundary fields.

`tests/command_tests.cpp` checks that the public `SHARDJSON` command sees the same package audit and unchanged read-only/no-execution boundaries.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6471` returned v198 with digest `fnv1a64:8fcbe53d8b1700b7`, boundary catalog v26, 756 fields, 37 groups, `readOnly=True`, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v198 turns the v197 cleanup evidence package from a useful bundle into an auditable, frozen, no-runtime contract checkpoint.
