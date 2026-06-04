# mini-kv v361 Route Preview Verification Report Archive Closeout Command Contract

v361 starts a new archive-closeout chain after v360 completed the twenty-version route preview verification report archive. It is not a new route executor, not a filesystem archive walker, not an active shard router, not a write path, not a WAL writer, and not permission to start mini-kv or run restore/load/compact.

The latest Node plan remains v549. It says Java and mini-kv do not need to start and remain recommended parallel, so this version only strengthens mini-kv read-only evidence.

## Versioned Baseline

fixtures/release/shard-readiness-v360.json freezes the previous SHARDJSON output before v361 updates the current fixture. The post-closeout descriptor advances to feature-read-only-shard-route-preview-verification-report-archive-closeout-command-contract with sourceFrozenReleaseVersion=v360 and the previous fixture digest fnv1a64:ce4589e8c5ffd352.

## New Module And Command

src/shard_route_preview_verification_report_archive_closeout.cpp owns the new archive closeout catalog instead of extending the older archive module again. This keeps v341-v360 archive evidence stable and gives v361-v380 a focused place to evolve.

SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTJSON is a no-argument read command. It reports sourceArchiveCommand=SHARDROUTEVERIFYREPORTARCHIVEJSON, sourceArchiveReleaseVersion=v360, sourceArchivePublishedStageCount=20, sourceArchiveChainComplete=true, closeoutStage=route-preview-verification-report-archive-closeout-command-contract, publishedStageCount=1, plannedStageCount=20, and archiveCloseoutChainComplete=false.

## Command Surfaces

src/command.cpp routes the new command through RuntimeEvidence. src/command_contracts.cpp marks it read, mutates_store=false, touches_wal=false, and stable=true. src/line_editor.cpp adds CLI completion so the command is discoverable without changing parser behavior.

In SHARDJSON, src/shard_readiness.cpp now publishes shardRoutePreviewVerificationReportArchiveCloseout and includes its digest marker. src/shard_readiness_history.cpp rolls the Node consumer and historical fallback text from v360 to v361.

## Boundaries

The new evidence continues to set readOnly=true, mutatesStore=false, touchesWal=false, filesystemReadPerformed=false, runtimeArchiveWalkAllowed=false, activeRouterInstalled=false, writeRoutingAllowed=false, loadRestoreCompactAllowed=false, startsMiniKvService=false, and executionAllowed=false.

## Tests And Smoke

tests/shard_route_preview_verification_report_archive_closeout_tests.cpp covers the standalone formatter, command usage, EXPLAINJSON, CHECKJSON, and store non-mutation. command_tests, shard_readiness_tests, shard_readiness_post_closeout_continuity_tests, and line_editor_tests cover the integrated surfaces. Closure also runs full CTest, CLI smoke, TCP smoke, and git whitespace checks.

One-line summary: v361 opens a clean, read-only archive-closeout evidence lane for downstream consumers while preserving the completed archive chain and keeping router, write, WAL, service-start, and execution authority closed.
