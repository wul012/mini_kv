# v1592-v1596 Production Shard Execution Owner Receipt Request Packet

## Goal

This version batch turns the Node v2087 production shard execution owner-receipt gap into local mini-kv evidence. The new evidence names the exact receipt slots that mini-kv, Java, and the cross-project cleanup layer will eventually need, while keeping mini-kv in a read-only proof role.

The release range is v1592 through v1596. The active command is:

```text
SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON
```

The contract is:

```text
shard-production-shard-execution-owner-receipt-request-packet.v1
```

## Role

The role of this module is to publish a request packet, not to satisfy the request. It is a control-plane checklist in JSON form. It records that Node v2087 wants owner receipts for abort, rollback, lifecycle ownership, cleanup no-start behavior, Java lifecycle and rollback ownership, and cross-project cleanup reconciliation.

The module also records the current absence of signed sibling receipts. That absence is deliberate evidence: it proves production shard execution is still blocked until future receipts arrive from the owning sides.

## What This Is Not

This is not a production shard executor.

This is not a signed mini-kv owner receipt.

This is not a signed Java owner receipt.

This is not a cross-project cleanup reconciliation receipt.

This does not bind managed audit storage, parse raw endpoints, read credential values, install an active shard router, route writes, touch WAL, start Node, start Java, start mini-kv services, or execute rollback, restore, load, compact, abort, or deployment behavior.

The repeated false fields are intentional. They make the packet consumable by Node and Java without allowing either side to infer that execution authority was opened.

## Entry Command

The CLI entry point is wired in `src/command.cpp`.

The command handler accepts only the bare command:

```text
SHARDPRODUCTIONSHARDEXECUTIONOWNERRECEIPTREQUESTPACKETJSON
```

Any extra token returns a usage error. That keeps the command read-only and prevents it from becoming a parameterized execution surface.

The command is registered in `src/command_catalog.cpp` with:

- category `read`
- `mutates_store=false`
- `touches_wal=false`
- stable command metadata
- `RuntimeEvidence` dispatch

That registration also lets `COMMANDS`, `COMMANDSJSON`, `EXPLAINJSON`, `CHECKJSON`, and line-editor completion see the command through the shared command catalog.

## Module Split

The implementation is split instead of growing another oversized evidence file.

`include/minikv/shard_preview_production_shard_execution_owner_receipt_request_packet.hpp` exposes the public formatter, digest marker, status, stage catalog formatter, and published stage count.

`src/shard_preview_production_shard_execution_owner_receipt_request_packet.cpp` builds the final JSON response. It owns the command name, Node plan pointer, source quality-gate fields, receipt request slot grouping, archive plan, diagnostics, and boundary booleans.

`src/shard_preview_production_shard_execution_owner_receipt_request_packet_stages.cpp` owns the five release stages:

- v1592 plan intake
- v1593 mini-kv slot catalog
- v1594 Java and cleanup slot catalog
- v1595 unsigned boundary validation
- v1596 clean CI closeout

`src/shard_preview_production_shard_execution_owner_receipt_request_packet_checks.cpp` owns the 12 checks. Those checks are the durable review list for future work.

`src/shard_preview_production_shard_execution_owner_receipt_request_packet_validation.cpp` owns the closure rule. It returns true only when the source quality gate is complete, Node is not waiting on mini-kv, no existing signed receipts are claimed, exactly nine request slots are present, per-version archives are required, no combined batch archive folder is allowed, authority boundaries are closed, and stage/check counts align.

## Evidence Structure

The response starts with contract and command identity, then records the upstream plan:

```text
docs/plans3/v2087-production-shard-execution-owner-receipt-request-packet-roadmap.md
```

The previous local source is frozen as v1591:

```text
SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON
fixtures/release/shard-readiness-v1591.json
```

That means v1592-v1596 does not reinterpret old walkthrough quality evidence. It uses v1591 as a stable source boundary, then adds a new owner receipt request layer on top.

The response then publishes the active stage, source frozen release, release range, stage chain, receipt slots, warnings, blockers, archive plan, stage catalog, checks, validation object, and final boundary fields.

## Receipt Request Slots

The packet groups nine request slots.

Mini-kv slots:

- `mini-kv.abort-owner-receipt`
- `mini-kv.rollback-owner-receipt`
- `mini-kv.lifecycle-owner-receipt`
- `mini-kv.cleanup-no-start-proof`

Java slots:

- `java.abort-rollback-owner-receipt`
- `java.lifecycle-owner-receipt`
- `java.cleanup-no-start-echo-receipt`

Cross-project slots:

- `cross-project.cleanup-reconciliation-receipt`
- `cross-project.owner-receipt-handoff-index`

The important contract detail is that every slot is a request. None of them is marked as a completed signed receipt. The response keeps:

```text
signedMiniKvReceiptPresent=false
signedJavaReceiptPresent=false
signedCrossProjectCleanupReceiptPresent=false
ownerReceiptRequestPacketOnly=true
```

This lets downstream control planes prepare the next work without accidentally treating this packet as approval.

## Boundary Fields

`readOnly=true` means the command only renders evidence.

`executionAllowed=false` means no runtime action is authorized.

`orderAuthoritative=false` remains inherited through `SHARDJSON`; mini-kv still does not become Java's order authority.

`mutatesStore=false` and `touchesWal=false` prove this command does not write storage data and does not append WAL records.

`activeRouterInstalled=false` and `writeRoutingAllowed=false` prove the receipt request packet did not turn into an active sharding router.

`productionShardExecutionEnabled=false` is the main production safety boundary.

`managedAuditStoreBound=false`, `rawEndpointParsed=false`, and `credentialValueRead=false` prove the module did not attach to managed audit storage, inspect endpoints, or touch secrets.

`startsNodeServices=false`, `startsJavaService=false`, and `startsMiniKvService=false` prove the evidence can be reviewed without starting sibling services.

`perVersionArchiveRequired=true` and `combinedArchiveFolderAllowed=false` encode the user-facing archive rule for this batch: screenshots and explanations go under `e/1592/`, `e/1593/`, `e/1594/`, `e/1595/`, and `e/1596/`, not one crowded folder.

## SHARDJSON Integration

`src/shard_readiness.cpp` rolls current `releaseVersion` to v1596 and changes top-level status to:

```text
production-shard-execution-owner-receipt-request-packet-clean-ci-closeout-read-only
```

It also adds the new evidence block under:

```text
shardPreviewProductionShardExecutionOwnerReceiptRequestPacket
```

The digest list gains a marker for the new packet so the current readiness digest reflects this version batch. The command catalog quality block now spans v1546-v1596 and reports 91 commands, because the new read-only command is part of the single-source command catalog.

## Fixtures

The batch publishes:

```text
fixtures/release/shard-readiness-v1592.json
fixtures/release/shard-readiness-v1593.json
fixtures/release/shard-readiness-v1594.json
fixtures/release/shard-readiness-v1595.json
fixtures/release/shard-readiness-v1596.json
fixtures/release/shard-readiness.json
```

The historical fixtures record the progressive stage chain. The current fixture records the completed v1596 state. This keeps old release evidence readable without claiming earlier stages had already completed the entire closeout.

## Tests

`tests/production_shard_execution_owner_receipt_request_packet_stages_tests.cpp` protects the v1592-v1596 chain, contiguous release numbers, source frozen fixture path sequence, and final stage identity.

`tests/production_shard_execution_owner_receipt_request_packet_checks_tests.cpp` protects the 12 check records, partial rendering, out-of-range behavior, and the no signed receipt/no execution/no write/no WAL fields emitted by every check.

`tests/production_shard_execution_owner_receipt_request_packet_validation_tests.cpp` protects the validation truth table. It verifies the passing case and then flips source count, source completeness, Node waiting, existing-receipt claims, signed receipt presence, request-only mode, slot count, archive split rules, production authority fields, service startup fields, router/write/WAL/execution fields, and count alignment.

`tests/production_shard_execution_owner_receipt_request_packet_tests.cpp` protects the full JSON command contract and direct CLI handler, including the usage error when extra input is supplied.

`tests/production_shard_execution_owner_receipt_request_packet_fixture_tests.cpp` protects all five versioned fixtures plus the current rolling fixture. It proves each version contains the packet, keeps request-only owner slots, keeps signed receipts absent, keeps per-version archive policy, and keeps router/write/WAL/execution false.

Existing tests were also updated where the current release, command count, command catalog quality range, line-editor completion, and `SHARDJSON` status changed.

## Integration Value

This batch gives Node, Java, and mini-kv a precise loose-coupling handoff point: everyone can see which production shard execution owner receipts are still missing, while mini-kv remains a read-only evidence publisher with no production execution authority.
