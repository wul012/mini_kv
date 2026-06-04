# mini-kv v340 Route Preview Verification Report Closeout Summary

v340 turns the v320-v339 route preview verification report rollout into a read-only closeout summary. It is not a route executor, not an active shard router, not a write path, not a WAL writer, not a service-start approval, and not a restore/load/compact permission.

The latest Node plan remains v549. That plan says Java and mini-kv do not need to start and remain recommended parallel, so this version only strengthens mini-kv evidence.

## Versioned Baseline

fixtures/release/shard-readiness-v339.json freezes the previous current SHARDJSON output. The post-closeout descriptor advances to feature-read-only-shard-route-preview-verification-report-closeout-summary with sourceFrozenReleaseVersion=v339.

## Closeout Command

include/minikv/shard_route_preview_verification_report_closeout.hpp declares the new evidence surface. src/shard_route_preview_verification_report_closeout.cpp formats SHARDROUTEVERIFYREPORTCLOSEOUTJSON as a no-argument read command.

The JSON records the closed release range v320-v339, closedVersionCount=20, reportStageCount=20, latest report stage closeout-audit, and the planned v341 archive manifest plus v342 archive verification follow-up.

## Command Surface

src/command.cpp dispatches SHARDROUTEVERIFYREPORTCLOSEOUTJSON without accepting arguments. src/command_contracts.cpp marks it category=read, mutates_store=false, touches_wal=false. src/line_editor.cpp adds completion.

## SHARDJSON

src/shard_readiness.cpp embeds shardRoutePreviewVerificationReportCloseout and adds its digest marker. src/shard_readiness_history.cpp rolls nodeConsumer to v340 while historicalFallback points at frozen v339 instead of rolling current.

## Tests And Smoke

tests/shard_route_preview_verification_report_closeout_tests.cpp checks the direct formatter, command execution, EXPLAINJSON, CHECKJSON, and store size. command, shard readiness, post-closeout, and line editor tests verify the new section stays read-only and visible.

One-line summary: v340 gives Node/Java/mini-kv a stable closeout summary for the v320-v339 report rollout without opening router, write, WAL, service, or execution behavior.
