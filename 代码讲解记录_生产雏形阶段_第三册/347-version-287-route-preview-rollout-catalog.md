# mini-kv v287 route-preview-rollout-catalog

## Version Goal

v287 freezes v286 as ixtures/release/shard-readiness-v286.json and advances the route preview rollout to $(@{Version=287; Count=9; Stage=route-preview-rollout-catalog; Scope=catalogs route preview rollout stages; Slug=route-preview-rollout-catalog}.Stage).

This is not an active shard router, not a write-routing implementation, not a multi-process runtime gate, and not an order/audit authority. SHARDROUTE and SHARDROUTEJSON remain read-only preview commands: they do not mutate the store, touch WAL, create shard directories, start services, run LOAD/RESTORE/COMPACT/SETNXEX, or permit execution.

## Code Path

src/shard_route_preview.cpp owns the rollout stage count and the preview command formatters. This version exposes 9 route-preview rollout stage(s), with the latest stage $(@{Version=287; Count=9; Stage=route-preview-rollout-catalog; Scope=catalogs route preview rollout stages; Slug=route-preview-rollout-catalog}.Stage).

src/shard_readiness.cpp rolls eleaseVersion to v287 and keeps publishing shardRoutePreview under SHARDJSON. The evidence digest includes the route preview rollout marker, so a stale stage cannot masquerade as current evidence.

src/shard_readiness_route_catalog_post_closeout_stage.cpp records the post-closeout descriptor: sequence 87, source fixture v286, and source digest fnv1a64:2193962195ba633a.

## Contract Reading

Control-plane readers should treat this version as versioned evidence only:

- eadOnly=true
- previewCommandAvailable=true
- previewJsonCommandAvailable=true
- ctiveRouterInstalled=false
- writeRoutingAllowed=false
- executionAllowed=false

The pinned sample slots remain compatibility evidence for old SHARDJSON samples; they are not a promise that mini-kv has activated shard placement.

## Tests And Smoke

Targeted CTest covered route preview, command contracts, shard readiness, post-closeout continuity, and line-editor completion. Full CTest passed across 73 tests. CLI and TCP smoke read SHARDROUTEJSON orderops:alpha, and the temporary server started for smoke was stopped.

One sentence summary: v287 strengthens the route preview evidence chain while keeping mini-kv a read-only evidence provider instead of a runtime router.