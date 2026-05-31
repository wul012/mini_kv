# mini-kv v164: slot preview audit formatter split

v164 follows the current Node v430 route-registration split context. Node v430 is a maintainability refactor for signed human approval artifact audit JSON/Markdown route registrations: it adds no evidence gate, changes no API path, and starts neither Java nor mini-kv. mini-kv therefore advances with a matching maintenance version: keep the public shard-readiness evidence stable while reducing formatter coupling.

This is not an active router, not write routing, not a live-read gate, not runtime approval, and not execution. It does not change `SHARDJSON`, does not move `fixtures/release/shard-readiness.json`, does not start Java or mini-kv, does not run a runtime probe, does not create shard directories, does not write storage, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_slot_preview.cpp` now owns only the slot/shard preview data: `slot_count()`, shard map JSON, route sample JSON, and `slotTablePreview`. This keeps the concrete preview table small and focused.

`src/shard_readiness_slot_preview_audit.cpp` is the new audit formatter module. It renders the existing `slotTablePreviewAudit` and the new `slotTablePreviewAuditMaintenance` section. The audit still derives its expected slot count from the preview module, so the split does not create a second source of truth.

`include/minikv/shard_readiness_slot_preview_audit.hpp` exposes only the two audit formatter functions. `include/minikv/shard_readiness_slot_preview.hpp` no longer advertises audit formatting, which makes the ownership boundary visible at compile time.

`src/shard_readiness.cpp` still owns release identity, JSON section order, frozen fixture history, diagnostics, and digest composition. It bumps current evidence to `v164`, keeps historical fixture parity through v163, and publishes `slotTablePreviewAuditMaintenance` after the existing audit.

`src/shard_readiness_boundary_fields.cpp` extends the shared boundary catalog to `read-only-boundary-fields.v5` with a new `slot-table-preview-audit-maintenance` group. The public field list and the count still come from the same vector.

## Evidence Contract

The current `releaseVersion` is `v164`, status is `slot-table-preview-audit-maintenance-read-only`, and `evidenceDigest` is `fnv1a64:9935ccb959b5a3b8`.

`fixtures/release/shard-readiness-v163.json` freezes the v163 slot preview audit with digest `fnv1a64:f81c5b47cb396df6`. The rolling fixture is regenerated from real `SHARDJSON` output, not hand-edited.

`slotTablePreviewAuditMaintenance` records that the audit formatter was extracted, the slot preview module still owns assignments, both `slotTablePreview` and `slotTablePreviewAudit` are still published, and the public `SHARDJSON` contract, fixture path, command name, and Node evidence-gate posture did not change.

The boundary fields stay explicit: `runtimeGateApprovalPresent=false`, `runtimeExecutionPacketPresent=false`, `runtimeExecutionPacketExecutable=false`, `startsJavaService=false`, `startsMiniKvService=false`, `runtimeProbeAllowed=false`, `liveReadAllowed=false`, `activeRouterInstalled=false`, `writeRoutingAllowed=false`, `writeCommandsAllowed=false`, `adminCommandsAllowed=false`, `loadRestoreCompactAllowed=false`, `executionAllowed=false`, and `readOnly=true`.

`boundaryCatalogIndex` now declares `catalogVersion=read-only-boundary-fields.v5`, `fieldCount=337`, and `groupCount=18`. The new group is `slot-table-preview-audit-maintenance`.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` verifies the v164 rolling fixture, frozen v163 fixture, digest history, new maintenance section, Node v430 preservation flags, and catalog v5 count.

`tests/command_tests.cpp` verifies the same contract through `CommandProcessor`, so command output and fixture parity stay aligned.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6437`. The smoke reads `SHARDJSON`, observes the v164 digest and maintenance fields, confirms `GET missing:v164` returns `(nil)`, stops the server, and verifies the port is no longer listening.

In one sentence: v164 makes the slot preview audit easier to maintain without changing the downstream evidence contract or opening router, write, service-start, live-read, or execution paths.
