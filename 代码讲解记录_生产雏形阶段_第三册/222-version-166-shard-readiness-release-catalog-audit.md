# mini-kv v166: shard readiness release catalog audit

v166 follows the Node v432 route-group split context. Node v432 is another maintainability-only route registration extraction, so mini-kv keeps its side focused on versioned, read-only evidence rather than runtime activation.

This is not an active shard router, not write routing, not runtime approval, not live read, and not execution. It does not change `SHARDJSON`, does not move the rolling fixture path, does not start services, does not run probes, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_release_catalog.cpp` now owns both the release catalog and its audit. `format_release_catalog_json()` publishes the v161-v164 catalog, while `format_release_catalog_audit_json()` verifies that the catalog is complete, versioned, digest-backed, and read-only.

`include/minikv/shard_readiness_release_catalog.hpp` exposes both formatter functions. This keeps catalog-related JSON out of the already-large top-level shard-readiness formatter.

`src/shard_readiness.cpp` bumps current evidence to `v166`, adds `shardReadinessReleaseCatalogAudit` after the catalog section, extends fixture parity through v165, and records Node v432 preservation in historical fallback and archive compatibility.

`src/shard_readiness_boundary_fields.cpp` bumps the shared boundary catalog to `read-only-boundary-fields.v7`, adds the `shard-readiness-release-catalog-audit` group, and adds catalog-audit fields to the shared read-only boundary vector.

## Evidence Contract

The current `releaseVersion` is `v166`, status is `shard-readiness-release-catalog-audit-read-only`, and `evidenceDigest` is `fnv1a64:4bd18a01790dc5f1`.

`fixtures/release/shard-readiness-v165.json` freezes v165 with digest `fnv1a64:5e94e2a9b438d2ad`. The rolling v166 fixture is regenerated from real `SHARDJSON` output.

`shardReadinessReleaseCatalogAudit` records `expectedReleaseCount=4`, `observedReleaseCount=4`, `releaseRangeStart=v161`, `releaseRangeEnd=v164`, `contiguousReleaseRange=true`, `duplicateReleasesDetected=false`, `missingCatalogEntriesDetected=false`, `allCatalogedDigestsPresent=true`, `allCatalogedFixturesVersioned=true`, `allCatalogedEntriesReadOnly=true`, `fieldCountsMonotonic=true`, `groupCountsMonotonic=true`, and `latestDigestMatchesFrozenSource=true`.

The audit remains catalog-only: runtime gate approval, runtime execution packet execution, Java/mini-kv service start, runtime probe, live read, active router, router activation, write routing, write/admin commands, load/restore/compact, and execution all remain false.

`boundaryCatalogIndex` now reports `catalogVersion=read-only-boundary-fields.v7`, `fieldCount=381`, and `groupCount=20`.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` verifies the v166 rolling fixture, the frozen v165 fixture, release catalog audit fields, Node v432 preservation, and boundary catalog v7 counts.

`tests/command_tests.cpp` verifies the same contract through `CommandProcessor`.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6439`. The TCP smoke reads v166 `SHARDJSON`, confirms the audit section remains no-router/no-write/no-execution, observes `(nil)` for `GET missing:v166`, stops the server, and verifies the port is clear.

In one sentence: v166 makes the release catalog auditable without turning catalog review into runtime routing, writes, probes, service startup, or execution.
