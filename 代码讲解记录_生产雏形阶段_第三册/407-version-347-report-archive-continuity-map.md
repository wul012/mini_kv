# mini-kv v347 Route Preview Verification Report Archive Continuity Map

v347 advances the route preview verification report archive chain to route-preview-verification-report-archive-continuity-map. It is not an archive crawler, not a filesystem scanner, not a route executor, not an active shard router, not a write path, not a WAL writer, and not permission to start mini-kv or run restore/load/compact.

The latest Node plan remains v549. It says Java and mini-kv do not need to start and remain recommended parallel, so this version only strengthens mini-kv read-only evidence.

## Versioned Baseline

fixtures/release/shard-readiness-v346.json freezes the previous SHARDJSON output before v347 updates the current fixture. The post-closeout descriptor advances to feature-read-only-shard-route-preview-verification-report-archive-continuity-map with sourceFrozenReleaseVersion=v346 and the previous fixture digest fnv1a64:5a2f46c60bb2a2e6.

## Archive Stage

src/shard_route_preview_verification_report_archive.cpp keeps the twenty-stage archive catalog in one module and publishes only the first 7 stage(s). The current stage scope is: maps v320-v340 continuity from report rollout through closeout and archive.

SHARDROUTEVERIFYREPORTARCHIVEJSON remains a no-argument read command. It reports the current archive stage, source frozen fixture, publishedStageCount=7, plannedStageCount=20, and the control-plane interpretation that consumers may read evidence but must not execute runtime behavior.

## Boundaries

The archive evidence continues to set readOnly=true, mutatesStore=false, touchesWal=false, filesystemReadPerformed=false, runtimeArchiveWalkAllowed=false, activeRouterInstalled=false, writeRoutingAllowed=false, and executionAllowed=false.

## Tests And Smoke

The focused archive test, command tests, shard readiness tests, post-closeout continuity test, line editor completion test, full CTest, CLI smoke, and TCP smoke all run during closure. The TCP server used for smoke is stopped after verification.

One-line summary: v347 adds a versioned read-only archive stage for downstream consumption without opening archive scanning, router, write, WAL, service-start, or execution authority.