# mini-kv v218 - Node v498 Consumer Readiness Handshake Package Audit

## Goal and Role

v218 audits the Node v498 consumer-readiness handshake package introduced in v217. v217 moved current post-closeout evidence to the Node v498 plan. v218 freezes v217 and proves that the v498-aligned package remains read-only, digest-pinned, and explicitly non-runtime.

The Node plan read for this version is docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md. That plan archives Node v497 batch closeout route output and says Java and mini-kv may proceed in parallel without fresh sibling runtime evidence. v218 respects that boundary.

This version is not a route, not an evidence gate, not a service startup, not a runtime probe, not a router activation, not a write/admin command path, and not a LOAD, RESTORE, or COMPACT execution path. It is an audit of a read-only handshake package.

## Frozen Source Baseline

v218 freezes the v217 current fixture as fixtures/release/shard-readiness-v217.json. The current output reports:

- releaseVersion: v218
- sourceNodePlan: docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md
- nodeBatchCloseoutVersion: Node v498
- sourceFrozenReleaseVersion: v217
- sourceFrozenFixturePath: fixtures/release/shard-readiness-v217.json
- sourceFrozenDigest: fnv1a64:c335f2b6ab5014b7
- continuityStage: post-closeout-continuity-node-v498-consumer-readiness-handshake-package-audit
- stageSequence: 18
- trackedPostCloseoutRangeEnd: v218
- trackedPostCloseoutReleaseCount: 18

This keeps v217 as an immutable source and avoids rolling-current history.

## Implementation Shape

src/shard_readiness_route_catalog_post_closeout_stage.cpp updates the descriptor to v218, sequence 18, source fixture v217, and source digest fnv1a64:c335f2b6ab5014b7. The descriptor now names the audit stage.

src/shard_readiness_route_catalog_post_closeout.cpp keeps the same sourceNodePlan v498 and nodeBatchCloseoutVersion Node v498 introduced in v217. That is important: v218 audits the v498 package instead of moving to another plan.

src/shard_readiness.cpp updates releaseVersion and digest seed strings to v218 and v217. This makes evidenceDigest sensitive to the audit stage and its frozen source.

src/shard_readiness_history.cpp adds fixtures/release/shard-readiness-v217.json to fixture parity and points historicalFallback to v217. The nodeConsumer sentence now says Node v499+ may consume v218 as consumer readiness handshake audit evidence only.

README.md records digest fnv1a64:34d57ad0a803bc2c and repeats the no-runtime boundary.

## Field Semantics for Consumers

sourceNodePlan and nodeBatchCloseoutVersion still identify Node v498. A consumer should interpret that as plan alignment for the archived closeout package, not as proof that Node already executed against mini-kv.

continuityStage is post-closeout-continuity-node-v498-consumer-readiness-handshake-package-audit. The audit checks package continuity and boundary preservation.

sourceFrozenReleaseVersion and sourceFrozenDigest point to v217. Consumers validate v218 by first validating v217.

stageSequence and trackedPostCloseoutReleaseCount are both 18. This continues the stage/range invariant.

nodeConsumesFreshMiniKvEvidence remains false, which preserves the difference between a consumable artifact and actual runtime consumption.

routerActivationAllowed, writeRoutingAllowed, writeCommandsAllowed, adminCommandsAllowed, loadRestoreCompactAllowed, runtimeProbeAllowed, startsServices, startsJavaService, startsMiniKvService, executionAllowed, and activeShardPrototypeEnabled all remain false.

readOnly remains true.

## Tests and What They Protect

command_tests verifies the public SHARDJSON path: v218 release, frozen v217 source, v217 digest, v498 audit stage, stageSequence 18, Node v498 batch version, range end v218, range count 18, and no-router/no-write/no-execution fields.

shard_readiness_tests verifies the formatter path, sourceNodePlan v498, fixture parity, historicalFallback to v217, and exact fixture equality.

Full CTest ran 71 tests and passed. That confirms the audit did not change unrelated storage, WAL, TCP, restore, or runtime receipt behavior.

TCP smoke ran minikv_server on 127.0.0.1:6491 and read SHARDJSON through minikv_client. The parsed response was v218, digest fnv1a64:34d57ad0a803bc2c, Node v498, with executionAllowed=false, routerActivationAllowed=false, and writeRoutingAllowed=false. The server was stopped after validation.

## Archive and Cleanup

The v218 archive is e/218. It keeps screenshots for CMake configure/build, targeted CTest, full CTest, TCP smoke, and evidence summary, plus e/218/解释/说明.md. Temporary build output, raw logs, render HTML, screenshot server files, and Playwright scratch files are removed by cleanup.

## Downstream Value

v218 completes the v498 handshake package pair: v217 packages it and v218 audits it. A later Node consumer can now distinguish package creation from package audit while still seeing that mini-kv did not start services, route writes, or execute recovery/storage operations.