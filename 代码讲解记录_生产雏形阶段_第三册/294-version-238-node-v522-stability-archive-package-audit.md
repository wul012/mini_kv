# mini-kv v238 Node v522 Stability Archive Package Audit

## Version Goal

This version freezes v237 as fixtures/release/shard-readiness-v237.json and advances the rolling SHARDJSON fixture to v238. The source plan is Node v522, which closed the Java / mini-kv route catalog cleanup twenty-version run and left Java and mini-kv on a recommended-parallel path.

The role of this version is evidence production. It gives a future Node v523+ consumer a versioned, tagged, read-only mini-kv fixture to inspect after the Node v522 closeout. It is not a new route, not a runtime approval, not a storage topology change, and not a permission to start Java, start mini-kv, probe TCP at runtime, install a router, or route writes.

## Files And Responsibilities

- src/shard_readiness_route_catalog_post_closeout_stage.cpp owns the current descriptor: stage name, sequence 38, release v238, frozen source fixture, and source digest.
- src/shard_readiness_route_catalog_post_closeout.cpp renders the stable JSON object and now points at the Node v522 plan and its 10/10 stability verifier context.
- src/shard_readiness.cpp carries the release version and digest inputs so the rolling fixture digest changes only when the explicit evidence contract changes.
- src/shard_readiness_history.cpp updates the historical fallback and fixture parity list so the old rolling fixture is consumed through shard-readiness-v237.json, never through mutable current output.
- tests/shard_readiness_post_closeout_continuity_tests.cpp keeps the new v233+ assertions out of the older large test file and verifies that SHARDJSON remains read-only through the command processor.

## Boundary Fields

The important contract fields remain readOnly=true, executionAllowed=false, routerActivationAllowed=false, and writeRoutingAllowed=false. sourceFrozenReleaseVersion=v237 tells consumers which immutable fixture to trust. stageSequence=38 and trackedPostCloseoutReleaseCount=38 show monotonic continuity from the v201 post-closeout start.  nodeBatchCloseoutVersion=Node v522 says this is aligned with the Node closeout plan, not with a live Node runtime call.

## Tests And Evidence

The targeted test set covers command_tests, shard_readiness_tests, and shard_readiness_post_closeout_continuity_tests. The full CTest run proves the change does not break the wider WAL, snapshot, TCP, restore, runtime evidence, credential resolver, and recovery tests. TCP smoke starts a local mini-kv server only for validation, reads SHARDJSON, confirms v238, and then stops the owned process.

## One-Line Value

v238 extends the Node v522-aligned mini-kv evidence chain while keeping the system inspectable, versioned, read-only, and non-executable.
