# mini-kv v169: Node route split window audit

v169 follows the Node v441 route-split context but does not extend runtime behavior. It freezes v168 and adds a read-only audit over the Node v433-v441 compatibility window.

This version is not an active shard router, not write routing, not runtime approval, not live read, and not execution. It does not change the `SHARDJSON` command, does not move the rolling fixture path, does not start services, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_node_compatibility.cpp` now publishes `format_route_split_compatibility_window_audit_json()`. The audit reuses the same local route-split version list as the window formatter, which keeps the observed count tied to the source list rather than manually duplicated.

`include/minikv/shard_readiness_node_compatibility.hpp` exposes the audit formatter alongside the existing compatibility formatters.

`src/shard_readiness.cpp` bumps current evidence to `v169`, freezes v168 as the previous consumed fixture, adds `nodeRouteSplitCompatibilityWindowAudit` after the window section, and updates diagnostics, historical fallback, digest input, status, and notes.

`src/shard_readiness_boundary_fields.cpp` advances the shared boundary catalog to `read-only-boundary-fields.v9`, adds the `node-route-split-compatibility-window-audit` group, and catalogs the audit's no-runtime fields.

## Evidence Contract

The current `releaseVersion` is `v169`, status is `node-route-split-window-audit-read-only`, and `evidenceDigest` is `fnv1a64:eaa587d1a50d8200`.

`fixtures/release/shard-readiness-v168.json` freezes v168 with digest `fnv1a64:d0d0ef2b2a0f5a0a`. The rolling v169 fixture is regenerated from real `SHARDJSON` output.

`nodeRouteSplitCompatibilityWindowAudit` records `expectedWindowVersionCount=9`, `observedWindowVersionCount=9`, `windowRangeStart=Node v433`, `windowRangeEnd=Node v441`, `contiguousNodeVersionWindow=true`, `duplicateWindowVersionsDetected=false`, `allWindowVersionsRouteRegistrationOnly=true`, `latestWindowMatchesFrozenSource=true`, and `windowAuditOnly=true`.

The audit keeps runtime gate approval, runtime execution packet execution, Java/mini-kv service start, runtime probe, live read, active shard prototype, router activation, write routing, write/admin commands, load/restore/compact, and execution all false.

`boundaryCatalogIndex` now reports `catalogVersion=read-only-boundary-fields.v9`, `fieldCount=420`, and `groupCount=22`.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` verifies the v169 rolling fixture, the frozen v168 fixture, the new audit fields, boundary catalog v9 counts, and the no-router/no-write/no-execution fields.

`tests/command_tests.cpp` verifies the same `SHARDJSON` contract through `CommandProcessor`.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6442`. The TCP smoke reads v169 `SHARDJSON`, confirms `nodeRouteSplitCompatibilityWindowAudit` and `observedWindowVersionCount=9`, observes `(nil)` for `GET missing:v169`, stops the server, and verifies the port is clear.

In one sentence: v169 makes the route-split compatibility window auditable without turning window review into mini-kv routing, writes, probes, service startup, or execution.
