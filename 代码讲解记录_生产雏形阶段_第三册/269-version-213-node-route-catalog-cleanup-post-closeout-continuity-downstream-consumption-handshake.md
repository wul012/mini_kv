# mini-kv v213 - Post-Closeout Continuity Downstream Consumption Handshake

## Goal and Role

v213 advances the mini-kv shard-readiness evidence from the v212 retained release-catalog audit into a downstream consumption handshake. The purpose is narrow: make the post-closeout continuity lane easier for Node-side consumers to recognize as a frozen, digest-pinned, read-only evidence stream.

The current Node plan entry read for this version is docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md. That plan says Java and mini-kv can proceed in parallel and that Node v498 does not require fresh sibling runtime evidence. So v213 does not try to satisfy a live Node call. It produces a stronger local mini-kv artifact that a later Node consumer can read without asking mini-kv to start services, open routers, write storage, or execute recovery commands.

This version is not a router, not a write-routing path, not a runtime approval, not a service lifecycle change, not a Java startup helper, and not a LOAD, RESTORE, or COMPACT execution path. It is a versioned evidence checkpoint.

## Frozen Source Baseline

Before changing the current SHARDJSON output, v213 freezes the v212 current fixture into fixtures/release/shard-readiness-v212.json. That file is the source baseline for the new version. The new live fixture then reports:

- releaseVersion: v213
- sourceFrozenReleaseVersion: v212
- sourceFrozenFixturePath: fixtures/release/shard-readiness-v212.json
- sourceFrozenDigest: fnv1a64:1649e22a22a59fd8
- continuityStage: post-closeout-continuity-downstream-consumption-handshake
- stageSequence: 13
- trackedPostCloseoutRangeEnd: v213
- trackedPostCloseoutReleaseCount: 13

The important part is that v213 does not rely on rolling current as history. The historical fallback section now points to v212 and says Node v491+ may consume the v212 post-closeout continuity release catalog retention audit. That keeps history stable even after current moves to v213.

## Implementation Shape

src/shard_readiness.cpp owns the top-level SHARDJSON envelope. v213 changes only the current release identity and digest seed inputs: releaseVersion becomes v213, the historicalFallback diagnostic key moves to v212-frozen-no-rolling-current, and the post-closeout digest component becomes v213-v212-continuity-downstream-consumption-handshake-no-execution. Those strings intentionally include both the new version and the frozen source version so the digest changes when either side of the evidence chain changes.

src/shard_readiness_history.cpp owns the diagnostic and fallback history. v213 updates nodeConsumer to describe v213 as the current evidence and appends fixtures/release/shard-readiness-v212.json to the fixture parity list. It also updates historicalFallback.previousConsumedReleaseVersion and previousConsumedFixturePath to v212. This is the part that proves the previous version is now a stored artifact, not an implied current output.

src/shard_readiness_route_catalog_post_closeout.cpp owns the post-closeout continuity object. v213 keeps the same module and JSON shape, but changes the stage constants to the downstream consumption handshake. The formatter still emits sourceNodePlan, source frozen fixture fields, closeout ranges, archive verifier counts, and all no-runtime flags in one object. This avoids adding a new broad file or a parallel schema for a small semantic step.

The frozen fixture fixtures/release/shard-readiness-v212.json is added beside older release fixtures. The rolling fixture fixtures/release/shard-readiness.json is regenerated from the v213 binary and is byte-for-byte compared by tests against minikv::shard_readiness::format_json().

README.md records v213 at the top of the current-version section with the new digest fnv1a64:81d869972aeed221 and the same boundary catalog version read-only-boundary-fields.v29.

## Field Semantics for Consumers

continuityMode remains node-route-catalog-cleanup-post-closeout-continuity-read-only. A consumer should interpret the object as evidence metadata, not a command surface.

sourceFrozenReleaseVersion, sourceFrozenFixturePath, and sourceFrozenDigest identify the immutable source baseline. For v213 these fields point to v212. If a consumer cannot find that fixture or the digest does not match, it should fail closed rather than silently use current.

continuityStage is post-closeout-continuity-downstream-consumption-handshake. This means mini-kv is advertising a stable handoff shape for downstream readers. It does not mean mini-kv is consuming Node live output or opening a fresh integration route.

stageSequence is 13 and trackedPostCloseoutReleaseCount is 13. These two fields intentionally match for this continuity lane. Tests assert both values so a future version cannot move the stage without also moving the counted range.

nodePlanStillLatestForMiniKv remains true because the local mini-kv work is still aligned with the active route-catalog cleanup consumer-readiness chain. It is a planning alignment flag, not proof of live four-project integration.

nodeConsumesFreshMiniKvEvidence is false. This is subtle but important: v213 prepares evidence for future consumption, but it does not claim Node has consumed fresh mini-kv runtime output during this version.

routerActivationAllowed, writeRoutingAllowed, writeCommandsAllowed, adminCommandsAllowed, loadRestoreCompactAllowed, executionAllowed, activeShardPrototypeEnabled, startsServices, startsJavaService, and startsMiniKvService all remain false. These fields are the hard boundary. The handshake is useful precisely because it is readable without granting operational authority.

readOnly remains true. Consumers may read and verify the object, but must not treat it as approval to mutate mini-kv state.

## Tests and What They Protect

command_tests checks the SHARDJSON command path. It verifies the v213 releaseVersion, frozen v212 fallback, source digest, continuity stage, stage sequence, range end, range count, and the no-router/no-write/no-execution flags. This proves the public CLI command exposes the new contract exactly.

shard_readiness_tests checks the formatter directly. It validates the same contract at the library layer and also checks fixture parity. The fixture parity assertion is deliberately strict: fixtures/release/shard-readiness.json must equal the formatter output byte-for-byte. During validation this caught a bad fixture write that introduced a file-level mismatch, which is exactly the kind of drift this test is supposed to prevent.

The historical fixture list now includes fixtures/release/shard-readiness-v212.json. That protects the v212 evidence from being lost while current advances.

Full CTest ran 71 tests. That is broader than the changed module because SHARDJSON shares the same build with WAL, store, TCP, restore, runtime receipt, and historical evidence tests. Passing the full suite gives confidence that the metadata-only v213 update did not alter storage or execution behavior.

The TCP smoke starts minikv_server on 127.0.0.1:6486, reads SHARDJSON through minikv_client, parses the JSON, and checks releaseVersion v213 plus executionAllowed=false, routerActivationAllowed=false, and writeRoutingAllowed=false. The server is stopped after the smoke. This proves the network read path can expose the same evidence while remaining read-only.

## Archive and Cleanup

The archive for this version is e/213. The final kept evidence is e/213/图片 with five screenshots and e/213/解释/说明.md with the command explanation. Temporary render HTML, logs, and cmake-build-v213 are removed during cleanup after the archive is created.

## Downstream Value

v213 gives Node or Java a clearer frozen handoff point: read v213 current, verify that its source baseline is v212, verify the digest and stage sequence, and then stop. No side channel, no runtime startup, and no command authority are needed. That keeps the cross-project integration path disciplined while still making the evidence chain more consumable.