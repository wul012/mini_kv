# mini-kv v320 Route Preview Verification Report Command Contract

v320 freezes v319 route preview verification evidence and adds `SHARDROUTEVERIFYREPORTJSON key`. The new command packages the existing route preview verification result into a versioned report that Node or Java can read later as evidence. It is not an active shard router, write-routing path, WAL writer, restore/load/compact executor, service-start hook, credential reader, or deployment approval.

The version follows the latest Node v549 plan as read-only context: mini-kv may continue strengthening evidence in parallel, while Java and mini-kv do not need to start for Node's current route archive verifier work.

## Versioned Baseline

`fixtures/release/shard-readiness-v319.json` freezes the previous current SHARDJSON output. The post-closeout descriptor now records `sourceFrozenReleaseVersion=v319`, `sourceFrozenFixturePath=fixtures/release/shard-readiness-v319.json`, and continuity stage `feature-read-only-shard-route-preview-verification-report-command-contract`.

This keeps the new report line honest: v320 is built from a named v319 baseline, not from a rolling current fixture.

## Report Module

`include/minikv/shard_route_preview_verification_report.hpp` exposes only three functions: `format_report_json`, `format_rollout_json`, and `rollout_digest_marker`.

`src/shard_route_preview_verification_report.cpp` owns the v320-v339 report stage catalog. For v320 it publishes stage sequence 1, release `v320`, source `v319`, and command `SHARDROUTEVERIFYREPORTJSON`. The module calls `shard_route_preview_verification::verify_key`, so it reuses the established nine checks instead of duplicating route math.

The report JSON carries `reportedCommand=SHARDROUTEVERIFYJSON`, `previewCommand=SHARDROUTEJSON`, `verificationPassed`, `checkCount`, `passedCount`, and the boundary fields `readOnly`, `mutatesStore`, `touchesWal`, `executesRoute`, `activeRouterInstalled`, `writeRoutingAllowed`, and `executionAllowed`.

## Command Surface

`src/command.cpp` adds a thin runtime-evidence branch for `SHARDROUTEVERIFYREPORTJSON key`. It validates exactly one key argument and delegates formatting to the new report module.

`src/command_contracts.cpp` marks the command as category `read`, `mutates_store=false`, and `touches_wal=false`. `EXPLAINJSON SHARDROUTEVERIFYREPORTJSON key` reports `metadata_read`; `CHECKJSON SHARDROUTEVERIFYREPORTJSON key` remains `read_only=true` and `execution_allowed=false`.

`src/line_editor.cpp` and `tests/line_editor_tests.cpp` add completion support. The shared prefix with `SHARDROUTEVERIFYJSON` now completes to `SHARDROUTEVERIFY` until the user types `J` or `R`.

## SHARDJSON Integration

`src/shard_readiness.cpp` updates the rolling release to `v320`, includes the report rollout section as `shardRoutePreviewVerificationReport`, and folds the report digest marker into the top-level evidence digest.

`src/shard_readiness_history.cpp` updates the Node consumer sentence to v320 report command-contract evidence and moves historical fallback to the frozen v319 fixture. The old v319 verifier section remains present, so consumers can still distinguish the field-level verifier from the packaged report.

## Tests And Smoke

`tests/shard_route_preview_verification_report_tests.cpp` is the focused guard for the new module and command. It checks the report JSON, rollout JSON, usage errors, store size, `EXPLAINJSON`, and `CHECKJSON`.

Existing command, SHARDJSON, post-closeout, verifier, route preview, and line editor tests were updated only where their public contracts changed. Focused CTest passed 7/7, and full CTest passed 75/75.

CLI smoke ran `SHARDROUTEVERIFYREPORTJSON orderops:alpha`, `SHARDROUTEVERIFYJSON orderops:alpha`, and `CHECKJSON SHARDROUTEVERIFYREPORTJSON orderops:alpha`. TCP smoke ran the same report/check commands through a temporary local server on port 7320, then stopped the server.

One-line summary: v320 turns route preview verification into a versioned read-only report package that future Node/Java consumers can inspect without opening router, write, WAL, service, or execution authority.
