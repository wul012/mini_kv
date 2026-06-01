# mini-kv v194 node route catalog cleanup closeout release catalog

## Goal and Boundary

v194 turns the v193 cleanup closeout handoff audit freeze into a frozen release fixture and adds a compact release catalog for the v190-v193 route catalog cleanup closeout chain.

This is not a new runtime entry point. It does not open a Node route, add an evidence gate, start Java or mini-kv services, probe live runtime state, activate a router, allow writes, run admin commands, run LOAD/RESTORE/COMPACT, or grant execution authority. The latest Node plan checked for this work was `docs/plans3/v473-post-java-mini-kv-route-catalog-cleanup-handoff-evidence-roadmap.md`; it recommends Java and mini-kv continue in parallel while Node consumes only frozen/tagged sibling evidence.

## Versioned Fixture

`fixtures/release/shard-readiness-v193.json` freezes the v193 rolling fixture before v194 changes the current `fixtures/release/shard-readiness.json`.

That frozen baseline records:

- `releaseVersion=v193`
- `status=node-route-catalog-cleanup-closeout-handoff-audit-freeze-read-only`
- `evidenceDigest=fnv1a64:0aad0fd5d2732af5`
- `boundaryCatalogIndex.catalogVersion=read-only-boundary-fields.v21`
- `fieldCount=648`
- `groupCount=32`

The current fixture now reports v194 with digest `fnv1a64:626f5a2a96a980c2`, boundary catalog `read-only-boundary-fields.v22`, 669 fields, and 33 groups.

## Implementation

`src/shard_readiness_route_catalog_evidence_chain.cpp` adds `format_route_catalog_cleanup_closeout_release_catalog_json()`.

The new section is `nodeRouteCatalogCleanupCloseoutReleaseCatalog`. It catalogs four versioned entries:

- v190: `nodeRouteCatalogEvidenceChainAudit`
- v191: `nodeRouteCatalogCleanupCloseoutHandoff`
- v192: `nodeRouteCatalogCleanupCloseoutHandoffAudit`
- v193: `nodeRouteCatalogCleanupCloseoutHandoffAuditFreeze`

Each entry carries the frozen fixture path, primary section, evidence digest, boundary catalog version, field count, group count, and the read-only/no-router/no-write/no-execution flags. The catalog-level flags also prove this is only a versioned fixture catalog: `versionedFixtureCatalogOnly=true`, `rollingCurrentUsedForFrozenCatalog=false`, `nodeConsumesFreshMiniKvEvidence=false`, `miniKvRuntimeContractChanged=false`, `nodeAddsEvidenceGate=false`, and all runtime/service/router/write/execution fields remain false.

`include/minikv/shard_readiness_route_catalog_evidence_chain.hpp` exposes the new formatter to the main `SHARDJSON` assembly.

`src/shard_readiness.cpp` rolls the release to v194, changes the top-level status to `node-route-catalog-cleanup-closeout-release-catalog-read-only`, includes the new section, and adds the v194 digest seed component.

`src/shard_readiness_history.cpp` extends fixture parity through `fixtures/release/shard-readiness-v193.json` and moves the historical fallback to the v193 frozen fixture.

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to `read-only-boundary-fields.v22`, adds a `node-route-catalog-cleanup-closeout-release-catalog` group, and registers the release-catalog fields that downstream consumers can audit without parsing the whole object by hand.

## Contract Reading

Control-plane readers should interpret v194 as a release catalog only:

- `readOnly=true` means the payload is evidence metadata.
- `executionAllowed=false` means the catalog cannot be used as a runtime approval packet.
- `routerActivationAllowed=false` and `writeRoutingAllowed=false` mean no active shard router exists here.
- `runtimeExecutionPacketExecutable=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, and `liveReadAllowed=false` keep the evidence chain out of execution and live service control.

The catalog helps Node/Java compare frozen mini-kv release evidence, but it does not make mini-kv order-authoritative or audit-authoritative.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` verifies the full in-process `SHARDJSON` contract, the new release catalog section, the v193 frozen fixture, fixture parity, boundary catalog version/counts, and the new boundary field names.

`tests/command_tests.cpp` verifies the public command surface sees the same v194 fields through `SHARDJSON` and still exposes no write, router, admin, load/restore/compact, or execution authority.

The v194 targeted CTest run passed `command_tests` and `shard_readiness_tests` 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6467` returned v194 with digest `fnv1a64:626f5a2a96a980c2`, boundary catalog v22, 669 fields, 33 groups, `readOnly=True`, and `executionAllowed=False`; the temporary server was stopped after verification.

One sentence: v194 gives downstream work a frozen, versioned catalog of the cleanup closeout mini-kv evidence chain while preserving the same no-router, no-write, no-execution boundary.
