# mini-kv v163: slot table preview consistency audit

v163 follows the current Node v426 route-group split maintenance context. Node is still extracting route registrations without adding an evidence gate, changing API paths, or starting sibling services, so mini-kv continues independently with versioned read-only shard-readiness evidence.

This is not an active router, not write routing, not runtime approval, and not an execution path. It does not change `SHARDJSON` usage, does not move the rolling fixture path, does not create shard directories, does not start Java or mini-kv, does not run a runtime probe, does not enable live read, does not write storage, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_slot_preview.cpp` now renders both the preview and its consistency audit. The preview remains the concrete evidence shape, while `format_slot_table_preview_audit_json()` records how a consumer should read that shape: 16 expected and observed slots, range 0-15, no duplicate slots, no unassigned slots, one shard, and read-only preview-only assignments.

`include/minikv/shard_readiness_slot_preview.hpp` exposes the audit formatter next to the existing slot count, shard map, route sample, and slot table preview functions. Keeping the audit in the same module prevents drift between the preview data and the summary evidence.

`src/shard_readiness.cpp` still owns release identity, top-level ordering, frozen fixture history, and digest composition. It adds `slotTablePreviewAudit` immediately after `slotTablePreview` so consumers can read the detailed table and the consistency summary together.

`src/shard_readiness_boundary_fields.cpp` extends the shared boundary vector with `slotTablePreviewAudit.*` fields and bumps the index to `read-only-boundary-fields.v4`.

## Evidence Contract

The current `releaseVersion` is `v163` and status is `slot-table-preview-audit-read-only`.

`fixtures/release/shard-readiness-v162.json` freezes the v162 slot table preview with digest `fnv1a64:8c01a3657f07cfd1`. The current rolling fixture is regenerated from real `SHARDJSON` output and has digest `fnv1a64:f81c5b47cb396df6`.

`slotTablePreviewAudit` declares `expectedSlotCount=16`, `observedSlotCount=16`, `slotRangeStart=0`, `slotRangeEnd=15`, `contiguousSlotRange=true`, `duplicateSlotsDetected=false`, `unassignedSlotsDetected=false`, `allSlotsMapped=true`, `uniqueShardCount=1`, `allAssignmentsReadOnly=true`, and `allAssignmentsPreviewOnly=true`.

The audit keeps the runtime boundary explicit: active router, router activation, write routing, write/admin commands, load/restore/compact, service start, runtime probe, live read, and execution all remain false.

`boundaryCatalogIndex` now declares `catalogVersion=read-only-boundary-fields.v4`, `fieldCount=319`, and `groupCount=17`. The added group is `slot-table-preview-audit`, and the public `readOnlyBoundaryFields` list shares the same vector as the count.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` validates current v163, all historical shard-readiness fixtures through v162, the v162 frozen digest, the audit's slot count/range/no-duplicate/no-unassigned flags, and the catalog v4 count.

`tests/command_tests.cpp` verifies the same audit through `CommandProcessor`, preserving command behavior and fixture path parity.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6436`. The declared approval port `6424` remains unstarted, and the GET probe remains `(nil)`.

In one sentence: v163 turns the slot table preview into auditable, internally consistent evidence while keeping mini-kv out of active routing, writes, service startup, and execution.
