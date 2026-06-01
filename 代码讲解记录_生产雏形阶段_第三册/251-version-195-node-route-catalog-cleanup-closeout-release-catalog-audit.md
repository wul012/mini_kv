# mini-kv v195 node route catalog cleanup closeout release catalog audit

## Goal and Boundary

v195 freezes the v194 cleanup closeout release catalog and adds a consistency audit for that catalog. The latest Node plan checked was `docs/plans3/v474-post-java-mini-kv-route-catalog-cleanup-handoff-evidence-report-roadmap.md`, where Node exposes frozen Java/mini-kv handoff evidence through a JSON/Markdown report route without opening execution or service startup. mini-kv v195 stays in that same evidence-only lane.

This is not a new Node route, not a mini-kv router, not a service launcher, not a runtime approval gate, and not a write/admin/LOAD/RESTORE/COMPACT execution path.

## Frozen Baseline

`fixtures/release/shard-readiness-v194.json` captures the v194 catalog before the rolling fixture moves forward.

The frozen source records:

- `releaseVersion=v194`
- `status=node-route-catalog-cleanup-closeout-release-catalog-read-only`
- `evidenceDigest=fnv1a64:626f5a2a96a980c2`
- `boundaryCatalogIndex.catalogVersion=read-only-boundary-fields.v22`
- `fieldCount=669`
- `groupCount=33`

The new v195 rolling fixture reports digest `fnv1a64:36463a895469c4bc`, boundary catalog `read-only-boundary-fields.v23`, 693 fields, and 34 groups.

## Implementation

`src/shard_readiness_route_catalog_evidence_chain.cpp` adds `format_route_catalog_cleanup_closeout_release_catalog_audit_json()`.

The new `nodeRouteCatalogCleanupCloseoutReleaseCatalogAudit` section checks the v194 catalog as a frozen object:

- expected and observed cataloged release count are both 4
- v190-v193 is a contiguous four-version range
- release versions are not duplicated
- all cataloged entries are read-only
- all cataloged fixtures are versioned
- all cataloged digests are present
- field counts and group counts are monotonic
- latest digest matches the frozen v194 source
- rolling current is rejected as a historical baseline

`src/shard_readiness.cpp` advances the release to v195, changes the top-level status to `node-route-catalog-cleanup-closeout-release-catalog-audit-read-only`, publishes the new audit section, and updates the digest seed.

`src/shard_readiness_history.cpp` adds `fixtures/release/shard-readiness-v194.json` to fixture parity, records Node v473/v474 archived evidence preservation, and moves the historical fallback to the v194 release catalog.

`src/shard_readiness_node_compatibility.cpp` now points the route split compatibility source at the frozen v194 fixture and digest while keeping the route/catalog window itself at Node v433-v472.

`src/shard_readiness_boundary_fields.cpp` advances the generated boundary catalog to v23, adds the release-catalog-audit group, and registers the audit fields that consumers can verify without treating the audit as execution authority.

## Contract Reading

Consumers should read the new audit as a consistency proof over frozen metadata:

- `catalogOnlyAudit=true` means the audit is not a runtime command packet.
- `rollingCurrentRejected=true` means old evidence must be read from the versioned v194 fixture.
- `runtimeExecutionPacketExecutable=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, and `executionAllowed=false` keep this out of runtime behavior.

The audit improves downstream report confidence but does not make mini-kv order-authoritative or audit-authoritative.

## Tests and Smoke

`tests/shard_readiness_tests.cpp` verifies the in-process v195 JSON contract, the new audit fields, the v194 frozen fixture, Node v474 report-plan preservation, boundary catalog v23 counts, and read-only field publication.

`tests/command_tests.cpp` verifies the same public `SHARDJSON` surface through the command processor.

The focused CTest run passed 2/2. The full CTest suite passed 71/71. TCP smoke on `127.0.0.1:6468` returned v195 with digest `fnv1a64:36463a895469c4bc`, boundary catalog v23, 693 fields, 34 groups, `readOnly=True`, and `executionAllowed=False`; the server was stopped after verification.

One sentence: v195 gives downstream Node v474+ report work an audited, frozen mini-kv cleanup-closeout release catalog while preserving the read-only/no-router/no-write/no-execution boundary.
