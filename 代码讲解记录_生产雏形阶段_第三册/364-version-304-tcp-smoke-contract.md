# mini-kv v304 Route Preview Verification Tcp Smoke Contract

v304 freezes v303 route preview verification evidence and advances current SHARDJSON to route-preview-verification-tcp-smoke-contract. It is not a route executor, active shard router, write-routing path, WAL writer, or service-start approval.

The version remains aligned with the latest Node v549 plan: mini-kv may strengthen read-only evidence in parallel, but it does not start Node or Java, open runtime connections, deploy, roll back, read credentials, parse raw endpoints, or become order/audit authority.

## Versioned Baseline

fixtures/release/shard-readiness-v303.json captures the previous current SHARDJSON output. The post-closeout descriptor records sourceFrozenReleaseVersion=v303 and continuity stage feature-read-only-shard-route-preview-verification-tcp-smoke-contract.

## Verifier Rollout

src/shard_route_preview_verification.cpp moves current_verification_stage_count to 6. The shardRoutePreviewVerification section keeps verifiedCommand=SHARDROUTEJSON, commands=[SHARDROUTEVERIFYJSON], rolloutStage=route-preview-verification-tcp-smoke-contract, rolloutStageSequence=6, rolloutReleaseVersion=v304, and publishedStageCount=6.

The nine checks remain slot_within_range, single_shard, preview_only, pinned_sample_consistent, hash_present, read_only, no_active_router, no_write_routing, and no_execution. They explain why the preview fields are trustworthy; they do not execute routing.

## Command And History Surface

SHARDROUTEVERIFYJSON key remains a read command with mutates_store=false, touches_wal=false, and executesRoute=false. COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON keep the same boundary story.

src/shard_readiness_history.cpp rolls nodeConsumer to v304 tcp-smoke-contract evidence and points historical fallback at the frozen v303 fixture.

## Tests And Smoke

Focused tests cover command catalog, current SHARDJSON, post-closeout continuity, verifier JSON, line editor candidates, and old route preview compatibility. Full CTest protects the broader store/WAL/TCP/restore/runtime receipt surface.

CLI and TCP smoke both execute SHARDROUTEVERIFYJSON orderops:alpha and confirm passed=true plus executionAllowed=false. TCP server lifetime is limited to the smoke and is stopped immediately afterward.

One-line summary: v304 makes route preview verification more versioned, explainable, and replayable while preserving no router, no write, and no execution boundaries.
