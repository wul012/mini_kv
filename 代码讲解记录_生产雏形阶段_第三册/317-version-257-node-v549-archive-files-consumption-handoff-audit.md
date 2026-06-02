# mini-kv v257 Node v549 Archive Files Consumption Handoff Audit

## Version Goal

This version freezes v256 as fixtures/release/shard-readiness-v256.json and advances the rolling SHARDJSON fixture to v257. The source plan is Node v549, which verifies the archived output of the latest sibling live-smoke archive verification route and keeps Java and mini-kv on a recommended-parallel path.

The role of this mini-kv version is evidence production. It is not a new route, not a runtime approval, not a storage topology change, and not permission to start Java, start mini-kv, probe TCP at runtime, install a router, or route writes.

## Files And Responsibilities

- src/shard_readiness_route_catalog_post_closeout_stage.cpp owns the current descriptor: stage name, sequence 57, release v257, frozen source fixture, and source digest.
- src/shard_readiness_route_catalog_post_closeout.cpp renders the stable JSON object and points at the Node v549 plan with 3 of 3 archive-verification checks.
- src/shard_readiness.cpp carries the release version and digest inputs so the rolling fixture digest changes only when the explicit evidence contract changes.
- src/shard_readiness_history.cpp updates historical fallback and fixture parity so v256 is consumed through shard-readiness-v256.json, never through mutable current output.
- tests/shard_readiness_post_closeout_continuity_tests.cpp verifies SHARDJSON through the command processor and proves the store is not mutated.

## Boundary Fields

The important contract fields remain readOnly=true, executionAllowed=false, routerActivationAllowed=false, and writeRoutingAllowed=false. sourceFrozenReleaseVersion=v256 tells consumers which immutable fixture to trust. stageSequence=57 and trackedPostCloseoutReleaseCount=57 show monotonic continuity from the v201 post-closeout start. nodeBatchCloseoutVersion=Node v549 says this is aligned with the latest Node archive-verification plan, not with a live Node runtime call.

## Tests And Evidence

The targeted test set covers command_tests, shard_readiness_tests, and shard_readiness_post_closeout_continuity_tests. The full CTest run proves the change does not break the wider WAL, snapshot, TCP, restore, runtime evidence, credential resolver, and recovery tests. TCP smoke starts a local mini-kv server only for validation, reads SHARDJSON, confirms v257, and then stops the owned process.

The archived screenshots under e/257 show configure/build, targeted CTest, full CTest, TCP smoke, and evidence summary. The generated Markdown records also pass whitespace and control-character hygiene checks.

## One-Line Value

v257 extends the Node v549-aligned mini-kv evidence chain while keeping the system inspectable, versioned, read-only, and non-executable.
