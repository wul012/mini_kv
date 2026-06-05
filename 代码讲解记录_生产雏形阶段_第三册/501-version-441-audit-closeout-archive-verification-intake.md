# mini-kv v441 - Audit Closeout Archive Verification Intake

## Version Goal

v441 starts a new read-only archive verification layer over the v421-v440 audit closeout package. The new command is `SHARDROUTEVERIFYREPORTARCHIVECLOSEOUTVERIFYAUDITCLOSEOUTARCHIVEVERIFYJSON`. It is not a router, not a write route, not a WAL operation, not a filesystem archive crawler, not a restore/load/compact entry point, and not Java order authority.

The reason for making a new command instead of extending the v440 command forever is maintenance: v421-v440 is now a closed source package, and v441-v460 can verify that package as a separate evidence chain. That keeps the old closeout contract stable while giving downstream control planes a clear place to read the new archive verification status.

## Module Roles

- `shard_route_preview_audit_closeout_archive_verification` owns the v441-v460 stage catalog and formatter. It publishes the current stage, the frozen source fixture, the source closeout digest marker, the declared archive root hint, and the no-execution boundary fields.
- `shard_route_preview_stage_catalog` remains the shared stage serialization helper. v441 reuses it so the new chain does not copy another bespoke stage JSON formatter.
- `command.cpp`, `command_contracts.cpp`, and `line_editor.cpp` expose the new command as a read command. `EXPLAINJSON` classifies it as `metadata_read`; `CHECKJSON` keeps `execution_allowed=false`.
- `shard_readiness.cpp` embeds exactly one `shardRoutePreviewVerificationReportArchiveCloseoutVerificationAuditCloseoutArchiveVerification` section in `SHARDJSON`, updates the release to v441, and adds the new digest marker to the shard readiness digest input.
- `fixtures/release/shard-readiness-v440.json` freezes the previous current fixture before `fixtures/release/shard-readiness.json` rolls forward to v441.

## Boundary Interpretation

`readOnly=true` means the command is safe to consume as evidence. `declaredArchiveVerificationOnly=true` and `archiveRootHintUsedForHumanReviewOnly=true` mean `e/440/` is a retained evidence pointer, not a runtime directory walk. `filesystemReadPerformed=false`, `runtimeArchiveWalkAllowed=false`, `mutatesStore=false`, `touchesWal=false`, `activeRouterInstalled=false`, `writeRoutingAllowed=false`, and `executionAllowed=false` keep the same hard boundary: this version cannot activate shard routing, execute writes, start services, run restore/load/compact, or grant order authority.

## Tests And Smoke Evidence

The focused `shard_route_preview_audit_closeout_archive_verification_tests` checks the direct formatter, the single SHARDJSON section, command usage errors, store size staying zero, `EXPLAINJSON`, and `CHECKJSON`. `command_tests`, `shard_readiness_tests`, and `line_editor_tests` cover command catalog exposure, current fixture parity, help text, and completion behavior.

Full CTest passed with 82 tests. CLI smoke read the new command from `minikv_cli`, and TCP smoke used an owned `minikv_server` process on loopback, read the new command, sent `QUIT`, and stopped the process. No runtime write, WAL mutation, archive directory read, router installation, or service process remains as part of the evidence.

One sentence summary: v441 turns the closed v421-v440 audit closeout package into a separately verifiable read-only archive evidence chain without weakening mini-kv's no-router/no-write/no-execution boundary.
