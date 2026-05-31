# mini-kv v165: shard readiness release catalog

v165 follows the current Node v431 route-group split context. Node v431 is still a maintainability-only route registration extraction: no evidence gate, no API path change, and no Java or mini-kv startup. mini-kv therefore keeps moving in the same lane by making its shard-readiness evidence chain easier to consume without changing runtime behavior.

This is not an active shard router, not write routing, not runtime approval, not live read, and not execution. It does not change `SHARDJSON`, does not move `fixtures/release/shard-readiness.json`, does not start services, does not run probes, does not create shard directories, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_release_catalog.cpp` is the new formatter module. It renders `shardReadinessReleaseCatalog`, a compact versioned catalog over v161-v164 shard-readiness evidence. Each entry records release version, status, fixture path, primary section, digest, boundary catalog version, field count, group count, and read-only/no-router/no-write/no-execution flags.

`include/minikv/shard_readiness_release_catalog.hpp` exposes only `format_release_catalog_json()`, keeping catalog ownership separate from the main shard-readiness formatter.

`src/shard_readiness.cpp` still owns the top-level JSON order, release identity, fixture parity, diagnostics, historical fallback, and digest composition. It now publishes `shardReadinessReleaseCatalog` after the slot preview audit maintenance section and bumps current release identity to `v165`.

`src/shard_readiness_boundary_fields.cpp` bumps the shared boundary catalog to `read-only-boundary-fields.v6`, adds the `shard-readiness-release-catalog` group, and adds release-catalog boundary fields to the same vector that powers `readOnlyBoundaryFields`.

## Evidence Contract

The current `releaseVersion` is `v165`, status is `shard-readiness-release-catalog-read-only`, and `evidenceDigest` is `fnv1a64:5e94e2a9b438d2ad`.

`fixtures/release/shard-readiness-v164.json` freezes v164 with digest `fnv1a64:9935ccb959b5a3b8`. The rolling v165 fixture is regenerated from real `SHARDJSON` output.

`shardReadinessReleaseCatalog` records four cataloged releases:

- v161 `boundaryCatalogIndex`, digest `fnv1a64:2193962195ba633a`, catalog v2, 290 fields, 15 groups.
- v162 `slotTablePreview`, digest `fnv1a64:8c01a3657f07cfd1`, catalog v3, 303 fields, 16 groups.
- v163 `slotTablePreviewAudit`, digest `fnv1a64:f81c5b47cb396df6`, catalog v4, 319 fields, 17 groups.
- v164 `slotTablePreviewAuditMaintenance`, digest `fnv1a64:9935ccb959b5a3b8`, catalog v5, 337 fields, 18 groups.

The section is catalog-only: `versionedFixtureCatalogOnly=true`, `rollingCurrentUsedForFrozenCatalog=false`, `publicShardJsonContractChanged=false`, `fixturePathChanged=false`, `shardJsonCommandChanged=false`, `nodeAddsEvidenceGate=false`, `runtimeExecutionPacketExecutable=false`, `startsJavaService=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, `liveReadAllowed=false`, `activeRouterInstalled=false`, `writeRoutingAllowed=false`, and `executionAllowed=false`.

`boundaryCatalogIndex` now reports `catalogVersion=read-only-boundary-fields.v6`, `fieldCount=357`, and `groupCount=19`.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` verifies the v165 rolling fixture, the frozen v164 fixture, all catalog entries, the v431 historical preservation flag, and the v6 boundary catalog counts.

`tests/command_tests.cpp` verifies the same evidence through `CommandProcessor`, so CLI/TCP command behavior remains aligned with the release fixture.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6438`. The TCP smoke reads v165 `SHARDJSON`, confirms the catalog section remains no-router/no-write/no-execution, observes `(nil)` for `GET missing:v165`, stops the server, and verifies the port is clear.

In one sentence: v165 turns the recent shard-readiness evidence chain into a structured, versioned catalog while keeping mini-kv strictly on the read-only side of the boundary.
