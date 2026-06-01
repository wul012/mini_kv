# mini-kv v168: Node route split window through v441

v168 follows the Node v441 plan. Node v441 is another route-registration extraction for the disabled runtime shell design draft body preparation-plan routes. The plan says this is maintainability-only: no API path change, no evidence gate, and no Java or mini-kv startup.

This version is not an active shard router, not write routing, not runtime approval, not live read, and not execution. It does not change the `SHARDJSON` command, does not move the rolling fixture path, does not add boundary fields, and does not execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

## Module Roles

`src/shard_readiness_node_compatibility.cpp` now keeps the route-split compatibility window versions in one local helper. `format_route_split_compatibility_window_json()` renders the covered version list from that helper, so the count and list move together when the Node window advances.

`src/shard_readiness.cpp` bumps current evidence to `v168`, freezes v167 as the previous consumed fixture, extends archive compatibility through Node v441, and records the new historical fallback flag for the preparation-plan route split.

`src/shard_readiness_boundary_fields.cpp` keeps the same field and group schema: `read-only-boundary-fields.v8`, `fieldCount=401`, and `groupCount=21`. Only the source plan and frozen source fixture move forward to v441/v167.

## Evidence Contract

The current `releaseVersion` is `v168`, status is `node-route-split-window-preparation-plan-read-only`, and `evidenceDigest` is `fnv1a64:d0d0ef2b2a0f5a0a`.

`fixtures/release/shard-readiness-v167.json` freezes v167 with digest `fnv1a64:caf300915d4e988c`. The rolling v168 fixture is regenerated from real `SHARDJSON` output.

`nodeRouteSplitCompatibilityWindow` now records `windowStartNodeVersion=Node v433`, `windowEndNodeVersion=Node v441`, and `splitRouteGroupCount=9`. The covered versions are Node v433 through Node v441, and the section still keeps API path, response shape, evidence gate, Java start, mini-kv start, runtime approval, runtime packet execution, runtime probe, live read, router activation, write routing, and execution all false.

## Tests And Smoke

`tests/shard_readiness_tests.cpp` verifies the v168 rolling fixture, the frozen v167 fixture, the Node v441 preservation flag, the nine-version compatibility window, and unchanged boundary catalog v8 counts.

`tests/command_tests.cpp` verifies the same `SHARDJSON` contract through `CommandProcessor`.

The closure loop includes focused 2-test CTest, full 71-test CTest, and a TCP smoke on temporary port `6441`. The TCP smoke reads v168 `SHARDJSON`, confirms `windowEndNodeVersion=Node v441`, observes `(nil)` for `GET missing:v168`, stops the server, and verifies the port is clear.

In one sentence: v168 advances the Node route-split compatibility window one step while keeping mini-kv a read-only evidence publisher rather than a router, writer, service starter, or executor.
