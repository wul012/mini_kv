# mini-kv v217 - Node v498 Consumer Readiness Handshake Package

## Goal and Role

v217 moves the current post-closeout continuity lane from the earlier Node v490-aligned checkpoint to the current Node v498 consumer-readiness batch closeout archive plan. The goal is to package mini-kv evidence so a later Node consumer can verify the current state against the v498 plan without starting mini-kv as an integration participant.

The Node plan read for this version is docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md. It says Node v498 archives the v497 batch closeout route output, that Java and mini-kv may proceed in parallel, and that no fresh sibling runtime evidence is required. v217 follows that boundary: it aligns the evidence metadata with v498, but does not claim live Node consumption.

This version is not a route, not an evidence gate, not a service startup, not a runtime probe, not a router activation, not a write/admin path, and not a LOAD, RESTORE, or COMPACT execution path. It is a read-only handshake package.

## Frozen Source Baseline

v217 freezes the v216 current fixture as fixtures/release/shard-readiness-v216.json. The current SHARDJSON then reports:

- releaseVersion: v217
- sourceNodePlan: docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md
- nodeBatchCloseoutVersion: Node v498
- sourceFrozenReleaseVersion: v216
- sourceFrozenFixturePath: fixtures/release/shard-readiness-v216.json
- sourceFrozenDigest: fnv1a64:e4fc4c802f59ad80
- continuityStage: post-closeout-continuity-node-v498-consumer-readiness-handshake-package
- stageSequence: 17
- trackedPostCloseoutRangeEnd: v217
- trackedPostCloseoutReleaseCount: 17

The freeze rule remains unchanged: v217 never treats rolling current as history. It builds from a stored v216 fixture.

## Implementation Shape

src/shard_readiness_route_catalog_post_closeout_stage.cpp carries the v217 descriptor. The descriptor names the v498 handshake package stage, points to the v216 frozen fixture, pins the v216 digest, and gives the stage a sequence of 17.

src/shard_readiness_route_catalog_post_closeout.cpp updates the current sourceNodePlan to the v498 plan and nodeBatchCloseoutVersion to Node v498. This is the main semantic move in the version. The formatter still emits the same object shape and all no-runtime flags.

src/shard_readiness.cpp rolls the top-level release and digest seed strings to v217. The digest seed now includes v217-v216-continuity-node-v498-consumer-readiness-handshake-package-no-execution, so the digest reflects both the new current version and the frozen source version.

src/shard_readiness_history.cpp updates the current nodeConsumer message to Node v499+ may consume v217 as route catalog cleanup consumer readiness handshake evidence only. That wording is deliberately cautious: v499+ may consume it later, but v217 does not assert that Node has consumed it during this mini-kv version.

README.md records the new digest fnv1a64:c335f2b6ab5014b7 and explicitly says the version adds no runtime authority.

## Field Semantics for Consumers

sourceNodePlan now points to the Node v498 plan. A downstream reader should understand this as plan alignment, not proof of live route execution.

nodeBatchCloseoutVersion is Node v498. This field is separately asserted so the plan path and batch version cannot drift apart silently.

continuityStage is post-closeout-continuity-node-v498-consumer-readiness-handshake-package. It means mini-kv has packaged its read-only evidence for a later consumer-readiness check.

sourceFrozenReleaseVersion, sourceFrozenFixturePath, and sourceFrozenDigest point to v216. A consumer that wants to validate v217 should verify v216 first.

nodeConsumesFreshMiniKvEvidence remains false. This protects the distinction between evidence readiness and actual cross-project runtime consumption.

routerActivationAllowed, writeRoutingAllowed, writeCommandsAllowed, adminCommandsAllowed, loadRestoreCompactAllowed, runtimeProbeAllowed, startsServices, startsJavaService, startsMiniKvService, executionAllowed, and activeShardPrototypeEnabled all remain false. The package is useful because it can be read without opening these capabilities.

readOnly remains true.

## Tests and What They Protect

command_tests checks the public command path. It now verifies releaseVersion v217, frozen v216 source, v216 digest, the v498 handshake stage, stageSequence 17, stageReleaseVersion v217, nodeBatchCloseoutVersion Node v498, post-closeout range end v217, range count 17, and the no-router/no-write/no-execution fields.

shard_readiness_tests checks the formatter and fixture parity. It verifies sourceNodePlan v498, the Node v498 batch version, the historical fallback to v216, and exact fixture equality.

Full CTest ran 71 tests and passed. This confirms the v498 plan alignment did not alter storage, WAL, TCP, restore, or runtime receipt behavior.

TCP smoke ran minikv_server on 127.0.0.1:6490 and read SHARDJSON through minikv_client. The parsed response was v217, digest fnv1a64:c335f2b6ab5014b7, Node v498, with executionAllowed=false, routerActivationAllowed=false, and writeRoutingAllowed=false. The server was stopped after validation.

## Archive and Cleanup

The v217 archive is e/217. It keeps screenshots for CMake configure/build, targeted CTest, full CTest, TCP smoke, and evidence summary, plus e/217/解释/说明.md. Temporary build output, raw logs, render HTML, screenshot server files, and Playwright scratch files are removed by the cleanup gate.

## Downstream Value

v217 gives the next Node-side consumer a cleaner mini-kv evidence package: read v217, verify sourceNodePlan v498, verify Node v498, verify frozen v216 digest, verify no execution authority, and stop. That is the right level of integration pressure for this stage.