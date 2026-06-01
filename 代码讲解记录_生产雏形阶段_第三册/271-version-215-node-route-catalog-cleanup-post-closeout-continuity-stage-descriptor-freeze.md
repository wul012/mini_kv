# mini-kv v215 - Post-Closeout Continuity Stage Descriptor Freeze

## Goal and Role

v215 freezes the v214 stage descriptor split and turns it into the next versioned source baseline. v214 was the quality pass that extracted post-closeout stage metadata into a descriptor module. v215 proves that the extracted descriptor is not just an in-flight refactor: it is now part of the historical evidence chain through fixtures/release/shard-readiness-v214.json.

The Node plan read for this version remains docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md. It allows Java and mini-kv to proceed in parallel and does not require fresh sibling runtime evidence. v215 therefore stays local to mini-kv and keeps the evidence as a read-only artifact.

This version is not a route, not a runtime approval, not a service startup, not a runtime probe, not a router activation, not a write/admin path, and not a LOAD, RESTORE, or COMPACT path. It freezes a descriptor-oriented evidence shape.

## Frozen Source Baseline

v215 begins by copying the v214 current fixture to fixtures/release/shard-readiness-v214.json. The new current SHARDJSON then points at that frozen fixture:

- releaseVersion: v215
- sourceFrozenReleaseVersion: v214
- sourceFrozenFixturePath: fixtures/release/shard-readiness-v214.json
- sourceFrozenDigest: fnv1a64:05b61e656f9925b7
- continuityStage: post-closeout-continuity-stage-descriptor-freeze
- stageSequence: 15
- trackedPostCloseoutRangeEnd: v215
- trackedPostCloseoutReleaseCount: 15

This preserves the exact rule used throughout the continuity lane: current moves forward only after the previous current has been stored as a versioned fixture.

## Implementation Shape

src/shard_readiness_route_catalog_post_closeout_stage.cpp is the main implementation file for this version. The PostCloseoutStageDescriptor instance now describes v215, sequence 15, and source fixture v214. This keeps the version-specific stage metadata in one small place.

src/shard_readiness_route_catalog_post_closeout.cpp keeps the JSON formatter stable. It still reads current_stage_descriptor(), emits the same public fields, and keeps all no-runtime flags. The only generic text change is the stage assertion stage descriptor lifecycle preserves the JSON contract, which is deliberately broader than v214's split-specific wording so future descriptor lifecycle stages can reuse the same formatter safely.

src/shard_readiness.cpp rolls releaseVersion to v215 and updates the digest seed components that identify the frozen baseline and stage. These digest seed strings are not user-facing schema, but they are important because evidenceDigest must change when the frozen source, stage sequence, or stage meaning changes.

src/shard_readiness_history.cpp moves nodeConsumer to v215, appends fixtures/release/shard-readiness-v214.json to fixtureParity, and updates historicalFallback to point at v214. This is what prevents the system from using rolling current as a historical baseline.

README.md records the new digest fnv1a64:b745fa9e79265743 and states explicitly that no route, evidence gate, service startup, runtime probe, router, write/admin command, or execution path was added.

## Field Semantics for Consumers

continuityStage is post-closeout-continuity-stage-descriptor-freeze. A consumer should read this as a maintenance checkpoint proving the descriptor split has become frozen evidence.

sourceFrozenReleaseVersion and sourceFrozenFixturePath point to v214. Consumers should verify v214 first when validating v215. If the fixture is missing or the digest does not match fnv1a64:05b61e656f9925b7, the safe behavior is to fail closed.

stageSequence and trackedPostCloseoutReleaseCount are both 15. This tells downstream readers that the post-closeout range has advanced in lockstep with the stage descriptor.

nodeConsumesFreshMiniKvEvidence remains false. v215 prepares evidence for future consumers but does not claim that Node v498 or another Node route consumed it live.

routerActivationAllowed, writeRoutingAllowed, writeCommandsAllowed, adminCommandsAllowed, loadRestoreCompactAllowed, runtimeProbeAllowed, startsServices, startsJavaService, startsMiniKvService, executionAllowed, and activeShardPrototypeEnabled all remain false. These are the hard safety fields.

readOnly remains true. The descriptor freeze is safe because it is only readable metadata.

## Tests and What They Protect

command_tests covers the public SHARDJSON command. It checks v215 release identity, frozen v214 fixture path, v214 digest, stage name, stageSequence 15, tracked range end v215, tracked release count 15, and the no-router/no-write/no-execution fields.

shard_readiness_tests covers the formatter and fixture parity. It verifies that fixtures/release/shard-readiness.json exactly matches minikv::shard_readiness::format_json(). For a freeze version, that exact equality is the main guard against accidental drift between source and archived evidence.

The fixture parity list now includes fixtures/release/shard-readiness-v214.json. That assertion keeps v214 available as a historical source for v215 and later versions.

Full CTest ran 71 tests and passed. This protects unrelated store, WAL, TCP, restore, runtime receipt, and historical fixture behavior from accidental changes.

TCP smoke ran minikv_server on 127.0.0.1:6488 and read SHARDJSON through minikv_client. The parsed response was v215 with digest fnv1a64:b745fa9e79265743 and the no-execution flags still false. The server was stopped after validation.

## Archive and Cleanup

The v215 archive lives under e/215. It keeps screenshots for CMake configure/build, targeted CTest, full CTest, TCP smoke, and evidence summary, plus e/215/解释/说明.md. Temporary build output, raw logs, render HTML, and the screenshot server are removed by the cleanup gate.

## Downstream Value

v215 gives downstream consumers a stable point after the v214 refactor. They can now read v215, verify that it freezes v214, and trust that the descriptor split is part of the versioned evidence chain rather than an unarchived implementation detail. That keeps the project moving quickly without letting small refactors become invisible debt.