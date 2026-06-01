# mini-kv v214 - Post-Closeout Continuity Stage Descriptor Split

## Goal and Role

v214 is a contract-preserving refactor plus a normal evidence version roll. The user asked to keep advancing versions while also doing necessary split and optimization work. This version answers that directly: it freezes the v213 downstream consumption handshake and moves the post-closeout stage metadata out of the JSON formatter into a small descriptor module.

The Node plan read for this version is docs/plans3/v498-post-java-mini-kv-route-catalog-cleanup-consumer-readiness-batch-closeout-archive-roadmap.md. The plan says Java and mini-kv can proceed in parallel and that Node v498 does not need fresh sibling evidence. That makes a mini-kv quality pass appropriate: v214 improves the internal ownership of the post-closeout continuity stage without pretending to run Node, Java, or any live integration.

This is not a new route, not a new evidence gate, not a service startup change, not a runtime probe, not a router activation, not a write/admin command path, and not a LOAD, RESTORE, or COMPACT path. It only keeps the evidence chain easier to maintain as more versions are added.

## Frozen Source Baseline

Before the current fixture moved, v213 was copied to fixtures/release/shard-readiness-v213.json. v214 then reports v213 as its frozen baseline:

- sourceFrozenReleaseVersion: v213
- sourceFrozenFixturePath: fixtures/release/shard-readiness-v213.json
- sourceFrozenDigest: fnv1a64:81d869972aeed221
- continuityStage: post-closeout-continuity-stage-descriptor-split
- stageSequence: 14
- stageReleaseVersion: v214
- trackedPostCloseoutRangeEnd: v214
- trackedPostCloseoutReleaseCount: 14

The fallback section now points at v213 and describes it as the downstream consumption handshake. This means consumers can still reconstruct why v214 exists even after current moves beyond it.

## Implementation Shape

include/minikv/shard_readiness_route_catalog_post_closeout_stage.hpp introduces PostCloseoutStageDescriptor. The type is intentionally small and plain: it carries string_view fields for the stage name, release version, scope, frozen source version/path/digest, and one baseline assertion, plus an integer stage sequence. It does not format JSON and does not know about runtime execution rules.

src/shard_readiness_route_catalog_post_closeout_stage.cpp owns the current descriptor instance. In v214 it declares the stage as post-closeout-continuity-stage-descriptor-split, sequence 14, release v214, and frozen source v213. Keeping these constants in a dedicated file keeps future version rolls from crowding the formatter with more stage bookkeeping.

src/shard_readiness_route_catalog_post_closeout.cpp still owns the JSON shape. The formatter now asks current_stage_descriptor() for the stage values and then emits the same fields as before. This is the key design point: descriptor ownership changed, but the SHARDJSON object remains stable. The formatter also adds the assertion stage descriptor split preserves the JSON contract, making the purpose visible to consumers.

CMakeLists.txt adds src/shard_readiness_route_catalog_post_closeout_stage.cpp to the minikv library. The configure/build archive proves the new file participates in the normal build rather than being an unused source artifact.

src/shard_readiness.cpp rolls the top-level releaseVersion to v214 and changes only digest seed components that must move when a release advances: v214 release seed, v213 historical fallback seed, v214 boundary catalog no-execution seed, v214 route split compatibility seed, and v214-v213 post-closeout continuity seed.

src/shard_readiness_history.cpp updates diagnostics, fixture parity, and historicalFallback. The historical fixture list now includes fixtures/release/shard-readiness-v213.json, and previousConsumedReleaseVersion is v213.

README.md records the new digest fnv1a64:05b61e656f9925b7 and explicitly describes v214 as a descriptor split with no runtime permission changes.

## Field Semantics for Consumers

continuityStage now says post-closeout-continuity-stage-descriptor-split. Consumers should read that as a maintenance stage in the evidence lane. It does not mean the consuming side gets a different schema.

stageSequence and trackedPostCloseoutReleaseCount are both 14. Their match proves that the stage roll and range roll happened together.

sourceFrozenReleaseVersion, sourceFrozenFixturePath, and sourceFrozenDigest point at v213. Consumers that want to verify v214 must check v213 as the baseline, not a rolling current fixture.

stageAssertions now includes stage descriptor split preserves the JSON contract. This phrase is meant for reviewers and downstream tools: the C++ ownership changed, but fields such as executionAllowed, routerActivationAllowed, and writeRoutingAllowed must not change meaning.

nodeConsumesFreshMiniKvEvidence remains false. v214 prepares and preserves evidence locally; it does not claim Node v498 or a later Node route actually consumed the new fixture in this same version.

routerActivationAllowed, writeRoutingAllowed, writeCommandsAllowed, adminCommandsAllowed, loadRestoreCompactAllowed, runtimeProbeAllowed, startsServices, startsJavaService, startsMiniKvService, executionAllowed, and activeShardPrototypeEnabled all remain false. This is the contract boundary that the refactor must preserve.

readOnly remains true. The descriptor split makes the read-only evidence easier to maintain; it does not make it executable.

## Tests and What They Protect

command_tests protects the public SHARDJSON command path. It checks releaseVersion v214, frozen v213 fixture path, source digest fnv1a64:81d869972aeed221, stageSequence 14, stageReleaseVersion v214, range end v214, range count 14, and the no-router/no-write/no-execution fields.

shard_readiness_tests protects the formatter-level contract. It checks the same fields directly and verifies fixture parity. The fixture parity check is especially important for this version because a refactor can accidentally leave a generated fixture behind even if the formatter changed. The test requires fixtures/release/shard-readiness.json to exactly equal minikv::shard_readiness::format_json().

The historical fixture list assertion now includes fixtures/release/shard-readiness-v213.json. That protects the frozen source from being skipped in the version chain.

Full CTest ran 71 tests and passed. This confirms the new descriptor source file and CMake registration did not disturb store, WAL, TCP, restore, runtime evidence receipts, or historical fixtures.

TCP smoke started minikv_server on 127.0.0.1:6487 and read SHARDJSON through minikv_client. The parsed response was v214, digest fnv1a64:05b61e656f9925b7, stage post-closeout-continuity-stage-descriptor-split, with executionAllowed=false, routerActivationAllowed=false, and writeRoutingAllowed=false. The server was stopped after validation.

## Archive and Cleanup

The v214 archive is e/214. It keeps five screenshots: CMake configure/build, targeted CTest, full CTest, TCP smoke, and evidence summary. e/214/解释/说明.md explains those screenshots. Temporary raw logs, render HTML, cmake-build-v214, and the transient screenshot server are removed by the cleanup gate.

## Downstream Value

v214 makes the post-closeout continuity lane easier to extend through more versions. The downstream contract remains one read-only SHARDJSON object, but the C++ code no longer concentrates stage descriptor constants inside the formatter. That is the small kind of refactor that lets the evidence chain keep moving without growing into a hard-to-maintain file.