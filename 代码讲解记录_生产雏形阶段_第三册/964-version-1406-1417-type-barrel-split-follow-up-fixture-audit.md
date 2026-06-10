# mini-kv v1406-v1417 Type Barrel Split Follow-up Fixture Audit

## Goal

v1406-v1417 turns the v1405 type barrel split follow-up evidence into a frozen source for a second, narrower fixture audit chain. The batch records that the v1405 follow-up can be consumed as versioned evidence, that current `SHARDJSON` can roll forward to v1417, and that every new check is still read-only metadata.

The batch is not a runtime router, not a Node integration executor, not a fixture file reader inside the runtime command, and not a write path. It does not import Node TypeScript modules, run Node tests, read profile or evidence endpoints, parse raw endpoints, read credentials, start sibling services, route writes, touch WAL, run load/restore/compact, or grant execution authority.

The latest Node plan read for this batch was `docs/plans3/v1866-test-only-shell-upstream-echo-module-split-roadmap.md`. That plan is Node-owned maintenance: it splits a test-only shell upstream echo service and explicitly says mini-kv is already ahead and does not need fresh evidence or sibling startup. mini-kv therefore advances independently by strengthening its own versioned fixture evidence.

## Runtime Section

The public runtime entry point is:

```text
SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON
```

The formatter lives in `src/shard_preview_type_barrel_split_follow_up_fixture_audit.cpp` and publishes the `shard-route-preview-type-barrel-split-follow-up-fixture-audit.v1` contract. It sources only in-memory facts from the v1405 module:

- `sourceFollowUpCommand=SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON`
- `sourceFollowUpReleaseVersion=v1405`
- `sourceFollowUpFixturePath=fixtures/release/shard-readiness-v1405.json`
- `sourceFollowUpPublishedStageCount=20`
- `sourceFollowUpStatus`
- `sourceFollowUpDigestMarker`

The important boundary is that the runtime command does not open `fixtures/release/shard-readiness-v1405.json`. The file path is evidence metadata, while fixture file reading belongs to tests and release verification. This keeps the command deterministic and avoids turning mini-kv into a runtime archive scanner.

## Split Modules

The batch keeps the new surface small and split by responsibility:

- `include/minikv/shard_preview_type_barrel_split_follow_up_fixture_audit.hpp` exposes the public formatter, status, digest marker, and published stage count.
- `src/shard_preview_type_barrel_split_follow_up_fixture_audit.cpp` builds the top-level JSON contract and embeds stage/check/validation output.
- `src/shard_preview_type_barrel_split_follow_up_fixture_audit_stages.cpp` owns the v1406-v1417 release sequence and source frozen fixture pointers.
- `src/shard_preview_type_barrel_split_follow_up_fixture_audit_checks.cpp` owns the 12 audit checks and their no-import/no-execution/no-write flags.
- `src/shard_preview_type_barrel_split_follow_up_fixture_audit_validation.cpp` decides whether the source, closure flags, and counts align.

This split follows the v1405 pattern instead of adding more logic to `command.cpp` or `shard_readiness.cpp`. Those shared files only register the command, connect the section, and roll the root status.

## Contract Fields

The control plane should read this section as fixture evidence only:

- `readOnly=true` means the command is a metadata read.
- `executionAllowed=false` means the evidence cannot trigger Node tests, route execution, restore/load/compact, or any production action.
- `sourceFollowUpNonParticipationFixtureFrozen=true` pins v1405 as the source release.
- `currentFixtureParityAudited=true` records that `fixtures/release/shard-readiness.json` and `shard-readiness-v1417.json` are regenerated from the real CLI output.
- `versionedFixtureChainOnly=true` says v1406-v1417 are archive evidence files, not active runtime state.
- `miniKvRuntimeReadsSourceFixturePayload=false` separates runtime formatter behavior from fixture tests.
- `miniKvImportsNodeModules=false` and `miniKvExecutesNodeChecks=false` keep the Node v1847-v1866 module split outside mini-kv.
- `activeRouterInstalled=false`, `writeRoutingAllowed=false`, `touchesWal=false`, and `loadRestoreCompactAllowed=false` keep the storage boundary closed.

The root `SHARDJSON` status now comes from `type_barrel_split_follow_up_fixture_audit_status()`, so current consumers see `route-preview-type-barrel-split-follow-up-fixture-audit-clean-workspace-ci-closeout-read-only` while the older v1405 follow-up section remains present as source evidence.

## Tests

The new tests are intentionally split to match the modules:

- `type_barrel_split_follow_up_fixture_audit_stages_tests.cpp` proves v1406-v1417 stage ordering, release versions, and source fixture paths are contiguous.
- `type_barrel_split_follow_up_fixture_audit_checks_tests.cpp` proves all 12 audit checks publish the no-Node-import, no-source-payload-runtime-read, no-router, no-write, no-WAL, and no-execution fields; it also checks out-of-range count handling.
- `type_barrel_split_follow_up_fixture_audit_validation_tests.cpp` exercises pass and fail cases for source lock, closure flags, and count alignment.
- `type_barrel_split_follow_up_fixture_audit_tests.cpp` verifies the standalone command JSON contract, source v1405 lock, Node v1866 context, v1417 closeout, and boundary flags.
- `type_barrel_split_follow_up_fixture_audit_fixture_tests.cpp` reads the generated release fixtures and proves v1406-v1417 all contain the audit section and source v1405 lineage.

Existing `command_tests`, `line_editor_tests`, and `shard_readiness_tests` were updated so command catalog count, autocomplete behavior, command JSON, and root `SHARDJSON` current status all understand the new command. Existing v1405 tests still protect the old follow-up section as historical source evidence.

## Fixture And Smoke Evidence

`fixtures/release/shard-readiness.json` and `fixtures/release/shard-readiness-v1417.json` are generated from the real Debug CLI `SHARDJSON` output. The intermediate v1406-v1416 fixtures are derived structurally from the v1417 JSON by lowering release metadata, published stage count, completed check count, stage catalog, and check catalog to the matching version.

Real smoke coverage should prove three surfaces:

- CLI can read `SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON` without mutating the store.
- TCP can read the same standalone command and close cleanly.
- `COMMANDS`, `COMMANDSJSON`, `EXPLAINJSON`, and `CHECKJSON` classify it as read-only metadata with `mutates_store=false` and `touches_wal=false`.

Those smoke checks do not start Node or Java. The only background process is the mini-kv server used for the TCP smoke, and it is stopped before closeout.

## Maintenance Value

The value of v1406-v1417 is not another operational switch. It is a stronger evidence chain: v1405 is now frozen as a versioned fixture source, current `SHARDJSON` can roll forward without losing historical meaning, and the command/test/docs layout remains split enough for later maintenance. This keeps mini-kv useful to Node and Java as read-only evidence while preserving the no-router, no-write, no-WAL, and no-execution boundary.
