# mini-kv v921-v930 candidate submission precheck handoff

## Goal

v921-v930 follows Node plan `docs/plans3/v1411-controlled-read-only-shard-preview-candidate-document-submission-precheck-closeout-roadmap.md`. Node v1387-v1411 moves from a request package to a candidate document submission precheck, but still stops before accepting a real document. mini-kv mirrors that boundary by publishing:

```text
SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON
```

This is not document intake. It does not read a real candidate document, does not accept a synthetic document, does not import payloads, does not evaluate candidates, does not approve, does not sign, does not route writes, does not touch WAL, and does not execute anything.

## Source Chain

The batch first freezes the previous current fixture:

```text
fixtures/release/shard-readiness-v920.json
```

That fixture remains the source integrity evidence for the new handoff. The new JSON points back to both important ancestors:

- `sourceCandidateDocumentRequestPackageReleaseVersion = v895`
- `sourceCandidateRequestPackageIntegrityReleaseVersion = v920`
- `sourceCandidateRequestPackageIntegrityFixturePath = fixtures/release/shard-readiness-v920.json`

Control-plane consumers should read this as: mini-kv can describe how a future reviewed real candidate document should be prechecked, but the actual document still has not entered the system.

## Module Split

The implementation uses four short modules:

- `shard_preview_candidate_submission_precheck.cpp` builds the public JSON, digest marker, command payload, diagnostics, and current status.
- `shard_preview_candidate_submission_precheck_checkpoints.cpp` owns the v921-v930 checkpoint catalog and stage-chain continuity.
- `shard_preview_candidate_submission_precheck_validators.cpp` owns the validator catalog and per-validator no-side-effect flags.
- `shard_preview_candidate_submission_precheck_validation.cpp` owns the final pass/fail summary for source integrity, checkpoint counts, validator counts, and disabled side effects.

The split is intentional. It keeps the JSON builder from becoming another oversized catalog file and leaves future document-intake work with clear places to extend only after reviewed real material exists.

## Command Contract

`src/command.cpp` registers `SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON` as a runtime evidence command with no extra arguments. `src/command_contracts.cpp` classifies it as:

```text
category=read
mutates_store=false
touches_wal=false
stable=true
```

`HELP`, `COMMANDS`, `COMMANDSJSON`, and line-editor completion all expose the command. That gives CLI and TCP consumers a short, independently smoke-testable handle instead of requiring them to parse the full `SHARDJSON` payload first.

## SHARDJSON Integration

`src/shard_readiness.cpp` now advances current `releaseVersion` to `v930`, and the top-level status comes from:

```cpp
shard_preview_candidate_submission_precheck::candidate_submission_precheck_status()
```

The new `shardPreviewCandidateSubmissionPrecheck` section carries:

- Node v1411 submission checkpoint and validator counts as read-only context.
- v895 request-package lineage through the v920 integrity fixture.
- 10 mini-kv checkpoints and 10 validators for v921-v930.
- `candidateSubmissionPrecheckOnly=true`.
- `documentIntakeOpened=false`.
- `reviewedRealCandidateDocumentRead=false`.
- `candidatePayloadImported=false`.
- `documentRouterInstalled=false`.
- `writeRoutingAllowed=false`.
- `touchesWal=false`.
- `executionAllowed=false`.

Those fields are the important boundary. They say the precheck handoff is durable evidence, not an execution path.

## Tests

The focused tests cover the new layer in five places:

- checkpoint catalog continuity from v921 to v930;
- validator catalog count, partial/full JSON formatting, and disabled side-effect fields;
- validation pass/fail behavior when the v920 source integrity chain is complete or incomplete;
- direct command JSON fields and digest/status helpers;
- fixture continuity from frozen v920 to current v930.

Existing command, line editor, and `SHARDJSON` tests were updated so the new command appears in every user-facing command surface and the top-level current readiness status points to v930.

## Summary

v921-v930 gives future Node/Java/mini-kv integration a stable submission-precheck evidence handle while preserving the same hard boundary: no real document intake, no active router, no write path, no WAL touch, and no execution.
