# mini-kv v197 node route catalog cleanup latest evidence package

## Goal and Boundary

v197 freezes v196 and packages the latest cleanup catalog evidence set into one read-only section: `nodeRouteCatalogCleanupLatestEvidencePackage`.

The latest Node plan checked was `docs/plans3/v479-post-java-mini-kv-latest-route-catalog-cleanup-evidence-archive-verification-route-roadmap.md`. That plan keeps Node on archive verification output and does not require dirty sibling consumption or service startup. v197 follows the same boundary: this is a versioned evidence package, not a route, router, write path, service launch, runtime probe, approval gate, or execution entry.

## Frozen Baseline

`fixtures/release/shard-readiness-v196.json` freezes the v196 formatter-split evidence before the rolling fixture advances.

The frozen baseline records:

- `releaseVersion=v196`
- `status=node-route-catalog-cleanup-closeout-catalog-formatter-split-read-only`
- `evidenceDigest=fnv1a64:116ec6b912bb4026`
- `boundaryCatalogIndex.catalogVersion=read-only-boundary-fields.v24`
- `fieldCount=714`
- `groupCount=35`

The v197 rolling fixture reports digest `fnv1a64:836d9f0b3956fe30`, boundary catalog `read-only-boundary-fields.v25`, 735 fields, and 36 groups.

## Package Shape

`include/minikv/shard_readiness_route_catalog_cleanup_catalog.hpp` now declares `format_latest_route_catalog_cleanup_evidence_package_json()`.

`src/shard_readiness_route_catalog_cleanup_catalog.cpp` renders the package from three versioned cleanup catalog releases:

- v194: `nodeRouteCatalogCleanupCloseoutReleaseCatalog`
- v195: `nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit`
- v196: `nodeRouteCatalogCleanupCloseoutCatalogMaintenance`

Each entry carries its fixture path, digest, primary section, boundary catalog version, field count, and group count. The package also records `packageReleaseCount=3`, range `v194` through `v196`, latest source `v196`, and latest digest `fnv1a64:116ec6b912bb4026`.

## Control-Plane Meaning

Control-plane readers should treat v197 as an archive-verification package pointer. It lets a downstream consumer prove it is reading the same v194-v196 chain without treating `fixtures/release/shard-readiness.json` as a historical source.

The important boundary fields stay explicit: `versionedFixturePackageOnly=true`, `rollingCurrentUsedForFrozenPackage=false`, `allEntriesReadOnly=true`, `packageAllowsRuntimeRoute=false`, `packageAllowsWrite=false`, `packageAllowsRestoreLoadCompact=false`, and `packageAllowsExecution=false`.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` checks the in-process JSON, the frozen v196 fixture, the new package section, boundary catalog v25 counts, and the package boundary fields.

`tests/command_tests.cpp` checks the public `SHARDJSON` command sees the same package metadata and unchanged read-only/no-execution boundaries.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6470` returned v197 with digest `fnv1a64:836d9f0b3956fe30`, boundary catalog v25, 735 fields, 36 groups, `readOnly=True`, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v197 turns the latest cleanup catalog chain into a compact versioned package that Node/Java can audit later without mini-kv opening any runtime or write capability.
