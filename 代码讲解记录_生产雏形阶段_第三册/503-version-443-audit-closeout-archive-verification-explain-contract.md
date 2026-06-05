# mini-kv v443 - Audit Closeout Archive Verification Explain Contract

## Version Goal

v443 advances the read-only archive verification chain for the closed v421-v440 audit closeout package to stage 3, `route-preview-verification-report-archive-closeout-verification-audit-closeout-archive-verification-explain-contract-read-only`. It is not a router, not a write path, not a WAL operation, not a filesystem archive crawler, not a restore/load/compact entry point, and not Java order authority.

The maintenance point is separation: v421-v440 remains the frozen source closeout package, while v441-v460 records how that package is verified for downstream control-plane consumption.

## Module Roles

- `shard_route_preview_audit_closeout_archive_verification` publishes the current stage, frozen source fixture, v440 source closeout digest marker, and no-execution boundary fields.
- `shard_route_preview_stage_catalog` keeps stage serialization shared instead of copying another formatter.
- `command.cpp`, `command_contracts.cpp`, and `line_editor.cpp` keep `SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON` visible as a read-only command and completion candidate.
- `shard_readiness.cpp` embeds one archive verification section in `SHARDJSON`, updates the release to v443, and includes the archive verification digest marker.
- `fixtures/release/shard-readiness-v442.json` freezes the previous current fixture before the v443 current fixture rolls forward.

## Boundary Interpretation

`readOnly=true` allows evidence consumption only. `declaredArchiveVerificationOnly=true`, `archiveRootHintUsedForHumanReviewOnly=true`, `filesystemReadPerformed=false`, and `runtimeArchiveWalkAllowed=false` mean archive paths are human review hints, not runtime I/O. `mutatesStore=false`, `touchesWal=false`, `activeRouterInstalled=false`, `writeRoutingAllowed=false`, and `executionAllowed=false` keep routing, writes, WAL mutation, restore/load/compact, service startup, and live reads blocked.

## Tests And Smoke Evidence

Focused tests check the archive verification formatter, single SHARDJSON section, command usage, store size staying zero, `EXPLAINJSON`, and `CHECKJSON`. `command_tests`, `shard_readiness_tests`, and `line_editor_tests` protect catalog exposure, fixture parity, HELP, and completion. Full CTest passed. CLI and TCP smoke both read `SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON`; TCP uses an owned loopback server and stops it after validation.

One sentence summary: v443 adds pins EXPLAINJSON metadata-read semantics for the archive verification command while preserving mini-kv's read-only/no-router/no-write/no-execution boundary.