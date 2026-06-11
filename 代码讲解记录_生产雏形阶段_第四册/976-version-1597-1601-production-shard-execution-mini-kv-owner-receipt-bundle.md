# v1597-v1601 Production Shard Execution Mini-kv Owner Receipt Bundle

## Goal

This version batch takes the request packet introduced in v1592-v1596 and answers only the mini-kv-owned part of it. The new evidence freezes the v1596 request packet, publishes a mini-kv owner receipt bundle, and proves that four local receipt slots are complete without opening production shard execution.

The release range is v1597 through v1601. The active command is:

```text
SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON
```

The contract is:

```text
shard-production-shard-execution-mini-kv-owner-receipt-bundle.v1
```

## Role

The role of this module is narrower than the previous request packet. v1596 described all requested owners. v1601 says mini-kv can satisfy its own four receipt responsibilities:

- `mini-kv.abort-owner-receipt`
- `mini-kv.rollback-owner-receipt`
- `mini-kv.lifecycle-owner-receipt`
- `mini-kv.cleanup-no-start-proof`

That is the important distinction. The bundle is a local owner receipt for mini-kv evidence only. It is not a sibling receipt collector, not a Java receipt proxy, and not a cross-project reconciliation layer.

## What This Is Not

This is not a production shard execution enablement.

This is not a Java owner receipt.

This is not a cross-project cleanup reconciliation receipt.

This does not execute abort, rollback, lifecycle transition, cleanup, restore, load, compact, deployment, or rollback behavior.

This does not start Node, Java, or mini-kv services.

This does not bind managed audit storage, parse raw endpoints, read credential values, install an active shard router, route writes, mutate the store, or touch WAL.

The false fields are part of the contract. They let Node and Java consume the mini-kv receipt without confusing it with runtime authority.

## Entry Command

The CLI entry point is wired in `src/command.cpp`.

The handler accepts only the bare command:

```text
SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON
```

Extra tokens return a usage error. That keeps the command from becoming a parameterized action surface.

The command is registered in `src/command_catalog.cpp` as a read command with stable metadata, `mutates_store=false`, `touches_wal=false`, and `RuntimeEvidence` dispatch. Through the shared catalog, the same command also appears in `COMMANDS`, `COMMANDSJSON`, `EXPLAINJSON`, `CHECKJSON`, and line-editor completion.

## Module Split

The implementation is intentionally split into four small modules.

`include/minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.hpp` exposes the public formatter, digest marker, stage catalog formatter, and published stage count.

`src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle.cpp` builds the final JSON response. It owns the command name, Node plan pointer, source request packet link, receipt groups, archive plan, diagnostics, and top-level boundary booleans.

`src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_stages.cpp` owns the five-stage version chain:

- v1597 plan intake and source freeze
- v1598 abort and rollback owner receipt proof
- v1599 lifecycle no-start owner receipt proof
- v1600 cleanup no-start proof
- v1601 clean CI closeout

`src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_checks.cpp` owns the 12 review checks. These checks are the durable checklist that reviewers can compare against the previous request packet.

`src/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation.cpp` owns the closure rule. It returns true only when the source request packet is complete, all four mini-kv receipt slots are present, Java and cross-project receipts remain absent, per-version archive rules use the `f` root, no runtime receipt execution occurred, and all authority boundaries remain closed.

## Evidence Structure

The response begins by linking back to the source:

- `sourceOwnerReceiptRequestPacketCommand`
- `sourceOwnerReceiptRequestPacketContract`
- `sourceOwnerReceiptRequestPacketReleaseVersion`
- `sourceOwnerReceiptRequestPacketFixturePath`
- `sourceOwnerReceiptRequestPacketComplete`
- `sourceOwnerReceiptRequestPacketValidationPassed`

Those fields say that v1601 is not inventing a new upstream request. It is answering the v1596 packet.

The `miniKvOwnerReceipts` block is the main value of this batch. It marks the four mini-kv receipt slots present and complete, while keeping:

- `abortExecutionPerformed=false`
- `rollbackExecutionPerformed=false`
- `lifecycleServiceStarted=false`
- `cleanupExecutionPerformed=false`

The `externalOwnerReceipts` block deliberately stays incomplete. `javaOwnerReceiptsPresent=false` and `crossProjectCleanupReconciliationReceiptPresent=false` mean production shard execution is still blocked outside mini-kv.

The `remainingReceiptBlockers` block names what is still missing. That prevents downstream consumers from interpreting mini-kv completion as whole-system completion.

The `archivePlan` block records the new screenshot archive root:

```text
f/1597/
f/1598/
f/1599/
f/1600/
f/1601/
```

The older roots remain historical. This batch starts `f` so screenshots and explanations are no longer squeezed into the previous active root.

## Boundary Fields

The important boundary fields are intentionally repetitive. A consumer should be able to inspect either the top-level fields, the validation block, or `CHECKJSON` and reach the same conclusion.

`readOnly=true` means the command is evidence only.

`executionAllowed=false` means the command cannot run receipt actions.

`mutatesStore=false` and `touchesWal=false` mean no storage state is changed and WAL is not touched.

`activeRouterInstalled=false` and `writeRoutingAllowed=false` mean no shard router or write routing path was opened.

`productionShardExecutionEnabled=false` means this bundle is not a production execution switch.

`managedAuditStoreBound=false`, `rawEndpointParsed=false`, and `credentialValueRead=false` mean no production connection material is consumed.

`startsNodeServices=false`, `startsJavaService=false`, and `startsMiniKvService=false` mean the receipt bundle is not allowed to start sibling or local runtime services.

## SHARDJSON Integration

`src/shard_readiness.cpp` rolls the active release to v1601 and embeds the new bundle under:

```text
shardPreviewProductionShardExecutionMiniKvOwnerReceiptBundle
```

The command catalog quality block now records 92 commands and extends the release range through v1601. The digest marker also includes:

```text
shardPreviewProductionShardExecutionMiniKvOwnerReceiptBundle
```

That makes the current `SHARDJSON` fixture and the standalone command tell the same story.

## Fixtures

The current fixture is:

```text
fixtures/release/shard-readiness.json
```

The versioned fixtures are:

```text
fixtures/release/shard-readiness-v1597.json
fixtures/release/shard-readiness-v1598.json
fixtures/release/shard-readiness-v1599.json
fixtures/release/shard-readiness-v1600.json
fixtures/release/shard-readiness-v1601.json
```

The intermediate fixtures preserve the staged evidence chain. The v1601 fixture is the clean closeout fixture with validation passing.

## Tests

`tests/production_shard_execution_mini_kv_owner_receipt_bundle_stages_tests.cpp` protects the five-stage release chain and prevents accidental stage-count drift.

`tests/production_shard_execution_mini_kv_owner_receipt_bundle_checks_tests.cpp` protects the 12-check catalog, including source freeze, mini-kv receipt presence, no execution, sibling receipt absence, authority closure, and `f` archive requirements.

`tests/production_shard_execution_mini_kv_owner_receipt_bundle_validation_tests.cpp` protects the closure rule. It proves validation is not just a hard-coded success string; it depends on source completion, receipt completeness, archive shape, no execution, and closed authority boundaries.

`tests/production_shard_execution_mini_kv_owner_receipt_bundle_tests.cpp` protects the standalone command payload and boundary fields.

`tests/production_shard_execution_mini_kv_owner_receipt_bundle_fixture_tests.cpp` protects the generated v1597-v1601 fixtures and the current release fixture.

Existing command, catalog, line-editor, and shard readiness tests were updated so the new command is visible everywhere the shared command catalog should expose it.

## Real Smoke

The CLI smoke covers:

```text
SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON
CHECKJSON SHARDPRODUCTIONSHARDEXECUTIONMINIKVOWNERRECEIPTBUNDLEJSON
SHARDJSON
COMMANDSJSON
```

The smoke verifies that the bundle validates, mini-kv receipts are complete, Java and cross-project receipts remain absent, archive root is `f`, `CHECKJSON` stays read-only, execution remains denied, WAL is untouched, `SHARDJSON` is v1601, and the command catalog has 92 commands.

## Integration Value

This batch gives Node and Java a precise mini-kv owner receipt bundle to consume later, while still keeping whole-system production shard execution blocked until Java and cross-project owners publish their own receipts.
