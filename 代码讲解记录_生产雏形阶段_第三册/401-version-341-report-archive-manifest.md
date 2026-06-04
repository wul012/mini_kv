# mini-kv v341 Route Preview Verification Report Archive Manifest

v341 starts the post-closeout archive chain for the route preview verification report rollout. It is not an archive crawler, not a filesystem scanner, not a route executor, not an active shard router, not a write path, not a WAL writer, and not permission to start mini-kv or run restore/load/compact.

The latest Node plan remains v549. That plan says Java and mini-kv do not need to start and remain recommended parallel, so this version only publishes read-only mini-kv evidence that Node or Java can consume later.

## Versioned Baseline

fixtures/release/shard-readiness-v340.json freezes the v340 closeout summary before v341 writes the new current SHARDJSON fixture. The post-closeout descriptor advances to feature-read-only-shard-route-preview-verification-report-archive-manifest with sourceFrozenReleaseVersion=v340 and the v340 evidence digest.

## Archive Module

include/minikv/shard_route_preview_verification_report_archive.hpp declares the archive evidence surface. src/shard_route_preview_verification_report_archive.cpp owns the archive stage catalog instead of extending the closeout formatter.

The module exposes SHARDROUTEVERIFYREPORTARCHIVEJSON as a no-argument read command. Its JSON records the source closeout command, report command, verification command, v320-v339 report range, v340 closeout source, the current archive stage, plannedStageCount=20, and control-plane interpretation notes.

## Boundary Fields

The archive manifest sets readOnly=true, mutatesStore=false, touchesWal=false, filesystemReadPerformed=false, runtimeArchiveWalkAllowed=false, activeRouterInstalled=false, writeRoutingAllowed=false, and executionAllowed=false.

The important control-plane meaning is simple: consumers may read the manifest, but must not treat it as authority to walk local archives, install a shard router, route writes, start services, or execute restore/load/compact.

## Command Surface

src/command.cpp dispatches SHARDROUTEVERIFYREPORTARCHIVEJSON through RuntimeEvidence and rejects extra arguments. src/command_contracts.cpp marks it category=read, mutates_store=false, touches_wal=false. src/line_editor.cpp adds completion.

## SHARDJSON

src/shard_readiness.cpp embeds shardRoutePreviewVerificationReportArchive after the closeout section and adds its digest marker. src/shard_readiness_history.cpp rolls nodeConsumer to v341 while historicalFallback points at frozen v340 rather than rolling current.

## Tests And Smoke

tests/shard_route_preview_verification_report_archive_tests.cpp checks the direct formatter, command execution, EXPLAINJSON, CHECKJSON, and store size. command, shard readiness, post-closeout, and line editor tests verify that the new archive evidence is visible, versioned, and still read-only.

One-line summary: v341 turns the v340 closeout into a versioned archive manifest that downstream systems can consume without opening router, write, WAL, archive-walk, service-start, or execution behavior.
