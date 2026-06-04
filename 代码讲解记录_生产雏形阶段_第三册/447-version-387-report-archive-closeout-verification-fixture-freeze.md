# mini-kv v387 - Archive Closeout Verification Fixture Freeze

## Goal and Boundary

v387 freezes ixtures/release/shard-readiness-v386.json before rolling the current SHARDJSON fixture forward. The version advances the archive closeout verification chain to $(@{Version=387; Sequence=7; Stage=route-preview-verification-report-archive-closeout-verification-fixture-freeze; Short=fixture-freeze; Title=Archive Closeout Verification Fixture Freeze; Scope=freezes the previous shard readiness fixture before rolling current verification evidence forward}.Stage), using the completed v361-v380 SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON closeout chain as immutable source evidence.

This version is not a new execution entry, not a route writer, not a Java order authority path, and not a runtime archive scanner. It does not start mini-kv as part of evidence generation, does not run restore/load/compact, does not touch WAL, and does not install an active shard router. The only new command remains a read-only evidence command: SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYJSON.

## Source Closeout Chain

The verification module treats v380 as the closed source package. The fields sourceArchiveCloseoutReleaseVersion, sourceArchiveCloseoutFixturePath, sourceArchiveCloseoutPublishedStageCount, sourceArchiveCloseoutPlannedStageCount, sourceArchiveCloseoutChainComplete, and sourceArchiveCloseoutDigestMarker tell downstream control planes how to read the upstream closeout evidence: the v361-v380 archive closeout chain is complete, pinned by digest marker, and consumed as data only.

The rolling fields erificationStage, erificationStageSequence, erificationReleaseVersion, publishedStageCount, plannedStageCount, sourceFrozenReleaseVersion, and sourceFrozenFixturePath describe this version's position in the new v381-v400 verification chain. sourceFrozenFixturePath is intentionally the previous release fixture, so consumers can compare current output with a versioned source instead of relying on mutable shard-readiness.json.

## Code Shape

The main implementation lives in src/shard_route_preview_verification_report_archive_closeout_verification.cpp, with the public surface in include/minikv/shard_route_preview_verification_report_archive_closeout_verification.hpp. Keeping the rollout catalog, formatter, digest marker, and stage count in this focused module avoids adding another large block to src/shard_readiness.cpp.

src/command.cpp only wires the command into the runtime evidence dispatch table and help output. src/command_contracts.cpp registers the command as ead, mutates_store=false, 	ouches_wal=false, and stable, which is what COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON consume. src/shard_readiness.cpp embeds the verification JSON as shardRoutePreviewVerificationReportArchiveCloseoutVerification and folds erification_digest_marker() into the overall evidence digest.

## Field Interpretation

eadOnly=true means the response is evidence metadata. mutatesStore=false and 	ouchesWal=false mean the command cannot alter the key-value store or persistence log. ilesystemReadPerformed=false and untimeArchiveWalkAllowed=false mean the version does not inspect archive directories at runtime; fixture paths are declared evidence pointers. ctiveRouterInstalled=false, outerActivationAllowed=false, and writeRoutingAllowed=false keep shard routing hypothetical. loadRestoreCompactAllowed=false, startsMiniKvService=false, untimeProbeAllowed=false, liveReadAllowed=false, and executionAllowed=false keep operational execution closed.

## Tests and Evidence

	ests/shard_route_preview_verification_report_archive_closeout_verification_tests.cpp validates the new command response, its source closeout fields, rolling stage fields, and all no-router/no-write/no-execution boundaries. Existing command and shard readiness tests assert catalog exposure and embedded SHARDJSON evidence. Line editor tests assert completion remains deterministic now that the SHARDROUTEVERIFYREPORTARCHIVECLOSEOUT... prefix has two possible commands.

The version archive under e/387/ captures configure/build, targeted tests, full CTest, CLI smoke, TCP smoke, and git diff --check. The TCP smoke starts an owned loopback server only for validation and stops it before the version is committed.

## One-Line Value

v387 makes the completed archive closeout chain easier for Node and Java to consume as versioned, read-only evidence while keeping mini-kv firmly outside routing, writing, service startup, and order authority.