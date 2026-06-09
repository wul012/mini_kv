# mini-kv v931-v955 candidate intake packet handoff

## Goal

v931-v955 follows Node plan `docs/plans3/v1421-controlled-read-only-shard-preview-candidate-document-intake-packet-closeout-roadmap.md`. Node v1412-v1421 defines a candidate document intake packet after the v1411 submission precheck. mini-kv mirrors that direction with:

```text
SHARDROUTECANDIDATEINTAKEPACKETJSON
```

This is not real intake. It does not consume reviewed material, does not accept a real or synthetic document, does not import payloads, does not evaluate candidates, does not approve, does not sign, does not route writes, does not touch WAL, and does not execute.

## Source Chain

The source fixture is:

```text
fixtures/release/shard-readiness-v930.json
```

That means v955 is downstream of the v930 submission precheck, which is downstream of the v920 request-package integrity chain, which is downstream of the v895 request package. The new section keeps those relationships visible through:

- `sourceCandidateSubmissionPrecheckReleaseVersion = v930`
- `sourceCandidateSubmissionPrecheckFixturePath = fixtures/release/shard-readiness-v930.json`
- `sourceNodeIntakePacketReleaseVersion = Node v1421`
- `sourceNodeSubmissionPrecheckReleaseVersion = Node v1411`

Control-plane consumers should read this as a durable intake packet handle for future reviewed material, not as permission to import material now.

## Module Split

The implementation uses five short modules:

- `shard_preview_candidate_intake_packet.cpp` builds the public JSON, digest marker, status, diagnostics, and command payload.
- `shard_preview_candidate_intake_packet_stages.cpp` owns the v931-v955 stage catalog and fixture continuity.
- `shard_preview_candidate_intake_packet_slots.cpp` owns the ten intake slots.
- `shard_preview_candidate_intake_packet_guards.cpp` owns the ten intake guards.
- `shard_preview_candidate_intake_packet_validation.cpp` owns the final pass/fail summary.

This split keeps slot and guard data out of the core JSON builder and gives future reviewed-material work a clean extension point.

## Command Contract

`src/command.cpp` registers `SHARDROUTECANDIDATEINTAKEPACKETJSON` as a no-argument runtime evidence command. `src/command_contracts.cpp` reports:

```text
category=read
mutates_store=false
touches_wal=false
stable=true
```

`HELP`, `COMMANDS`, `COMMANDSJSON`, and line-editor completion all expose the command, so both CLI and TCP smoke can verify the short command without parsing the full `SHARDJSON` response first.

## SHARDJSON Integration

`src/shard_readiness.cpp` advances current `releaseVersion` to `v955`, and the top-level status now comes from:

```cpp
shard_preview_candidate_intake_packet::candidate_intake_packet_status()
```

The new `shardPreviewCandidateIntakePacket` section carries:

- Node v1421 intake slot and guard counts.
- twenty-five covered source submission checkpoints and validators.
- twenty carried candidate fields.
- thirty-five intake gates.
- ten mini-kv intake slots and ten intake guards.
- `candidateIntakePacketOnly=true`.
- `reviewedMaterialPresent=false`.
- `documentIntakeOpened=false`.
- `candidatePayloadImported=false`.
- `documentRouterInstalled=false`.
- `writeRoutingAllowed=false`.
- `touchesWal=false`.
- `executionAllowed=false`.

These fields keep the packet useful to future consumers without quietly becoming an execution path.

## Tests

New focused tests cover:

- v931-v955 stage continuity and source fixture references;
- ten slot records and no-material flags;
- ten guard records and no-side-effect flags;
- validation pass/fail behavior for the frozen v930 source;
- direct command JSON fields, status, digest marker, and published stage count;
- fixture continuity from source v930 to current v955.

Existing command, line editor, and SHARDJSON tests now check the new command and the rolling top-level status.

## Summary

v931-v955 gives Node, Java, and future mini-kv work a stable intake packet evidence handle while preserving the hard boundary: no reviewed material intake, no active router, no write path, no WAL touch, and no execution.
