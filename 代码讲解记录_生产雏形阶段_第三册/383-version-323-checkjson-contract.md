# mini-kv v323 Route Preview Verification Report Checkjson Contract

v323 freezes v322 route preview verification report evidence and advances current SHARDJSON to route-preview-verification-report-checkjson-contract. It is not a route executor, active shard router, write-routing path, WAL writer, service-start approval, restore/load/compact path, credential reader, or deployment control.

The version remains aligned with the latest Node v549 plan: mini-kv may strengthen read-only evidence in parallel, while Node's current route archive verifier does not require starting Java or mini-kv.

## Versioned Baseline

fixtures/release/shard-readiness-v322.json captures the previous current SHARDJSON output. The post-closeout descriptor records sourceFrozenReleaseVersion=v322, digest fnv1a64:2193962195ba633a, and continuity stage feature-read-only-shard-route-preview-verification-report-checkjson-contract.

## Report Rollout

src/shard_route_preview_verification_report.cpp moves current_report_stage_count to 4. The shardRoutePreviewVerificationReport section keeps reportedCommand=SHARDROUTEVERIFYJSON, previewCommand=SHARDROUTEJSON, rolloutStage=route-preview-verification-report-checkjson-contract, rolloutStageSequence=4, rolloutReleaseVersion=v323, and publishedStageCount=4.

The report still reuses the nine verification checks from SHARDROUTEVERIFYJSON: slot range, single shard, preview-only route mode, pinned sample consistency, hash presence, read-only, no active router, no write routing, and no execution.

## Command And History Surface

SHARDROUTEVERIFYREPORTJSON key remains a read command with mutates_store=false, touches_wal=false, and executesRoute=false. EXPLAINJSON and CHECKJSON keep reporting metadata-only behavior.

src/shard_readiness_history.cpp rolls nodeConsumer to v323 report checkjson-contract evidence and points historical fallback at the frozen v322 fixture.

## Tests And Smoke

Focused tests cover the report module, command catalog, current SHARDJSON, post-closeout continuity, verifier compatibility, route preview compatibility, and line editor completion. Full CTest protects the broader store/WAL/TCP/restore/runtime receipt surface.

CLI and TCP smoke both execute SHARDROUTEVERIFYREPORTJSON orderops:alpha and CHECKJSON SHARDROUTEVERIFYREPORTJSON orderops:alpha; TCP starts a temporary server only for the smoke and stops it immediately afterward.

One-line summary: v323 advances the report rollout one named, versioned step while preserving no router, no write, no WAL, and no execution boundaries.