# mini-kv v279 Read-Only Shard Route Preview Command

## Version Goal

v279 freezes v278 as `fixtures/release/shard-readiness-v278.json` and adds the first real route-preview command surface: `SHARDROUTE key` and `SHARDROUTEJSON key`. The goal is to let a control plane inspect the preview slot and `shard-0` assignment for a key without turning the preview into an active router.

This is not a write-routing implementation, not a multi-shard storage engine, not a runtime gate, not a service startup path, and not an order/audit authority. The command computes metadata only. It does not create shard directories, touch WAL, mutate the store, execute LOAD/RESTORE/COMPACT/SETNXEX, or make Node/Java depend on a live mini-kv process.

## Module Responsibilities

`include/minikv/shard_route_preview.hpp` defines the small `RoutePreview` shape and the public formatters. Keeping this separate avoids growing `shard_readiness.cpp` with command-specific logic.

`src/shard_route_preview.cpp` owns the preview calculation and JSON/text renderers. The current route mode is explicitly `preview-only`, and the only shard id is `shard-0`. The three historical `SHARDJSON` sample keys keep their pinned slots so v279 does not silently drift older evidence:

- `orderops:alpha -> slot 5`
- `audit:receipt:001 -> slot 9`
- `mini-kv:health -> slot 14`

Other keys receive a deterministic FNV-1a preview hash and `hash % 16` slot. That hash is diagnostic evidence, not a router token.

## Command Contract

`src/command.cpp` registers `SHARDROUTE` and `SHARDROUTEJSON` in the runtime evidence command family. Both require exactly one key argument and return usage errors for missing or extra tokens.

`src/command_contracts.cpp` adds both commands to `COMMANDS` / `COMMANDSJSON` with `category=read`, `mutates_store=false`, `touches_wal=false`, and `stable=true`. `EXPLAINJSON SHARDROUTEJSON orderops:alpha` reports a metadata read with the key captured, and `CHECKJSON` wraps the same no-execution contract.

`src/line_editor.cpp` adds both commands to interactive completion, so the bundled client does not lag behind the executable command surface.

## SHARDJSON Evidence

`src/shard_readiness.cpp` now publishes `shardRoutePreview` from the new module. The rollout section says the current stage is `route-preview-command-contract`, sequence 1, release v279, sourced from frozen v278 evidence.

The section also repeats the important boundary fields:

- `readOnly=true`
- `mutatesStore=false`
- `touchesWal=false`
- `activeRouterInstalled=false`
- `writeRoutingAllowed=false`
- `executionAllowed=false`

`src/shard_readiness_route_catalog_post_closeout_stage.cpp` advances the post-closeout descriptor to `feature-read-only-shard-route-preview-command-contract`, sequence 79.

## Tests And Smoke

`tests/shard_route_preview_tests.cpp` is the focused test for the new feature. It verifies pinned slots, hash-backed custom keys, JSON fields, rollout fields, command usage errors, store size staying zero, and `EXPLAINJSON` / `CHECKJSON` read-only semantics.

Existing `command_tests`, `shard_readiness_tests`, `shard_readiness_post_closeout_continuity_tests`, and `line_editor_tests` were extended so the command catalog, fixture parity, SHARDJSON rollout evidence, and completion surface all agree.

Validation for v279:

- targeted CTest: 5/5 passed
- full CTest: 73/73 passed
- CLI smoke: `SHARDROUTEJSON orderops:alpha` and `SHARDROUTE audit:receipt:001` passed
- TCP smoke: temporary server served `SHARDROUTEJSON orderops:alpha`, then the started process was stopped

One sentence summary: v279 gives Node/Java a concrete read-only key route preview surface while keeping mini-kv in evidence-provider mode, not active-routing or execution mode.
