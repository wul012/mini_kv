# mini-kv v167: Node route split compatibility window

v167 follows the Node v440 plan. Node v433 through v440 are route-registration refactors in the Node project, and v440 explicitly says it does not change API paths, add an evidence gate, or start Java or mini-kv.

This version is not an active shard router, not write routing, not a live-read gate, not runtime approval, and not execution. It does not change the `SHARDJSON` command, does not move the rolling fixture path, does not start services, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_node_compatibility.cpp` now owns two Node-compatibility records. The existing `format_route_group_split_compatibility_json()` keeps the v159 Node v418 compatibility evidence. The new `format_route_split_compatibility_window_json()` publishes the v433-v440 compatibility window and lists every route-split version covered by this read-only evidence.

`include/minikv/shard_readiness_node_compatibility.hpp` exposes the new formatter without pulling Node-specific strings back into the main shard-readiness formatter.

`src/shard_readiness.cpp` bumps current evidence to `v167`, freezes v166 as the previous consumed fixture, adds the `nodeRouteSplitCompatibilityWindow` section, extends fixture parity and archive compatibility through Node v440, and updates the digest seed so the rolling fixture is a distinct release.

`src/shard_readiness_boundary_fields.cpp` advances the shared boundary catalog to `read-only-boundary-fields.v8`, adds the `node-route-split-compatibility-window` group, and catalogs the new no-runtime fields from the same shared vector used by `readOnlyBoundaryFields`.

## Evidence Contract

The current `releaseVersion` is `v167`, status is `node-route-split-window-compatibility-read-only`, and `evidenceDigest` is `fnv1a64:caf300915d4e988c`.

`fixtures/release/shard-readiness-v166.json` freezes v166 with digest `fnv1a64:4bd18a01790dc5f1`. The rolling v167 fixture is regenerated from real `SHARDJSON` output.

`nodeRouteSplitCompatibilityWindow` records `windowStartNodeVersion=Node v433`, `windowEndNodeVersion=Node v440`, `splitRouteGroupCount=8`, and `allChangesRouteRegistrationOnly=true`. It also keeps `nodeApiPathChanged=false`, `nodeResponseShapeChanged=false`, `nodeAddsEvidenceGate=false`, `nodeStartsJavaService=false`, `nodeStartsMiniKvService=false`, `miniKvFixturePathChanged=false`, `miniKvShardJsonCommandChanged=false`, `runtimeGateApprovalPresent=false`, `runtimeExecutionPacketExecutable=false`, `runtimeProbeAllowed=false`, `liveReadAllowed=false`, `routerActivationAllowed=false`, `writeRoutingAllowed=false`, and `executionAllowed=false`.

`boundaryCatalogIndex` now reports `catalogVersion=read-only-boundary-fields.v8`, `fieldCount=401`, and `groupCount=21`.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` verifies the v167 rolling fixture, the frozen v166 fixture, v433-v440 preservation flags, the new compatibility window fields, and boundary catalog v8 counts.

`tests/command_tests.cpp` verifies the same contract through `CommandProcessor`.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6440`. The TCP smoke reads v167 `SHARDJSON`, confirms `nodeRouteSplitCompatibilityWindow` is present while execution and write routing are false, observes `(nil)` for `GET missing:v167`, stops the server, and verifies the port is clear.

In one sentence: v167 lets downstream consumers recognize the Node v433-v440 route-split window without turning route-table maintenance into mini-kv routing, writes, probes, service startup, or execution.
