# mini-kv v976-v985 candidate material submission precheck

## Version Goal

v976-v985 turns the Node v1456 material submission precheck plan into a mini-kv read-only evidence surface. The new command is:

```text
SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON
```

Its job is to freeze the v975 material request integrity evidence, carry the Node v1456 checkpoint and validator shape, and publish a stable handoff object that future Node/Java work can inspect without asking mini-kv to read, import, evaluate, or execute any candidate material.

This version is deliberately not a material submission endpoint. It does not create another material request echo, does not consume externally reviewed material, does not open document intake, does not install a router, does not write to the store, does not touch WAL, and does not execute restore/load/compact/runtime paths. It is a control-plane evidence handle.

## Source And Boundary

The source plan is:

```text
D:\nodeproj\orderops-node\docs\plans3\v1456-controlled-read-only-shard-preview-candidate-document-material-submission-precheck-roadmap.md
```

That plan says Node has produced a material submission precheck roadmap after the v1446 material request package. mini-kv consumes only the stable shape: ten checkpoints, ten validators, twenty-five request items, twenty-five acceptance checks, twenty required material fields, twenty submission material fields, and forty-one gates.

The upstream mini-kv source is v975:

```text
fixtures/release/shard-readiness-v975.json
SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON
```

v976 starts from that frozen fixture. v985 closes the new chain. The important distinction is that v975 remains the material request integrity source; v985 is only the submission precheck overlay built on top of that source.

## Module Split

The implementation intentionally avoids another large monolithic file. It is split into four narrow modules:

```text
include/minikv/shard_preview_candidate_material_submission_precheck.hpp
src/shard_preview_candidate_material_submission_precheck.cpp
src/shard_preview_candidate_material_submission_precheck_checkpoints.cpp
src/shard_preview_candidate_material_submission_precheck_validators.cpp
src/shard_preview_candidate_material_submission_precheck_validation.cpp
```

The core module formats the public JSON, status, digest marker, and published stage count. It owns the command contract string, source plan pointer, source release numbers, and aggregate boundary booleans. It also embeds the checkpoint catalog, validator catalog, and validation object into one command response.

The checkpoint module owns the v976-v985 release chain. Each record says which release is being published, which prior fixture it freezes, and what that version proves. For example, v980 carries the material field tables, v982 carries signature attestation metadata without signature parsing, and v984 freezes runtime import and router/write/WAL/execution boundaries.

The validator module owns the ten validator codes. These are not runtime validators over submitted documents; they are evidence validators over the precheck contract. They prove that source integrity is frozen, Node v1456 is pinned, count fields still align, real material remains absent, payload intake is closed, router/write/WAL paths are blocked, and the closeout stops before execution.

The validation module is a small pure formatter with boolean logic. It can return `candidateMaterialSubmissionPrecheckValidationPassed=false` when counts or source integrity are wrong, which gives the tests a real failure surface instead of only checking static string fragments.

## Public Contract

The new section appears in `SHARDJSON` as:

```text
shardPreviewCandidateMaterialSubmissionPrecheck
```

The contract name is:

```text
shard-route-preview-candidate-document-material-submission-precheck.v1
```

Key fields are meant for a control-plane reader:

`sourceCandidateMaterialRequestIntegrityReleaseVersion` is `v975`, so readers know the submission precheck did not replace the material request integrity source.

`candidateMaterialSubmissionPrecheckReleaseVersion` is `v985`, so the current rolling fixture and the new command agree about the latest precheck closeout.

`plannedMaterialSubmissionPrecheckCheckpointCount` and `completedMaterialSubmissionPrecheckCheckpointCount` are both `10`. The validator count follows the same pattern. This proves the chain is complete, not partial.

`materialSubmissionGateCount` is `41`, copied as a planning expectation from Node v1456. mini-kv does not run those gates over submitted material.

`readOnly=true`, `mutatesStore=false`, `touchesWal=false`, and `executionAllowed=false` are the operational boundary. They are also echoed through command catalog tests and smoke checks.

The material-specific negative fields are the heart of the version: `externalReviewedMaterialConsumed=false`, `reviewedMaterialPresent=false`, `materialSubmissionOpened=false`, `materialPayloadImported=false`, `candidatePayloadImported=false`, `candidateDocumentEvaluated=false`, `approvalGrantCaptured=false`, `signedApprovalEmitted=false`, and `runtimePayloadImported=false`.

## Command Integration

`src/command.cpp` registers `SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON` and routes it to the new formatter. The command count moves from 64 to 65. `src/command_contracts.cpp` marks the command as read-only, stable, no store mutation, and no WAL touch. `src/line_editor.cpp` adds completion support while preserving older material request and material request integrity completions.

`src/shard_readiness.cpp` updates the rolling release to v985, includes the new section in `SHARDJSON`, adds the new digest input, and moves the top-level readiness status to:

```text
route-preview-candidate-material-submission-precheck-closeout-archive-checkpoint-read-only
```

That top status is a summary marker only. It does not imply a live submission flow has started.

## Fixtures

The release fixture range is:

```text
fixtures/release/shard-readiness-v976.json
...
fixtures/release/shard-readiness-v985.json
fixtures/release/shard-readiness.json
```

The fixtures were generated from the real `minikv_cli.exe SHARDJSON` output for each stage. v985 also updates the rolling `shard-readiness.json`. The fixture tests assert that v975 does not already contain the new precheck section, while the rolling fixture does contain the v985 section and still points back to v975 as source integrity.

## Tests

The focused tests for this version are:

```text
candidate_material_submission_precheck_checkpoints_tests
candidate_material_submission_precheck_validators_tests
candidate_material_submission_precheck_validation_tests
candidate_material_submission_precheck_tests
candidate_material_submission_precheck_fixture_tests
```

The checkpoint test verifies v976-v985 sequence continuity, source fixture lineage, and final closeout status. The validator test verifies all ten validator codes and their no-material/no-router/no-write/no-execution booleans. The validation test proves the happy path passes and a broken source integrity count fails. The command-level test verifies the public JSON fields, release labels, count alignment, source v975 link, and disabled side-effect fields. The fixture test protects the historical boundary between frozen v975 source and rolling v985 current evidence.

The existing `command_tests`, `line_editor_tests`, and `shard_readiness_tests` were updated because the new command participates in the command catalog, completion tree, and top-level `SHARDJSON`.

## Smoke And Archive

The archive for v985 is:

```text
e/985/图片/
e/985/解释/说明.md
```

The closeout reran CMake configure, CMake build, focused CTest, full CTest, CLI smoke, and TCP smoke. The CLI smoke proves the new command is listed and returns the v985 contract. The TCP smoke proves the same evidence can be returned through the server/client path while the temporary server is stopped afterward.

## Summary

v976-v985 gives Node and Java a versioned material submission precheck evidence handle while keeping mini-kv on the right side of the boundary: versioned evidence plus read-only validation, no material intake, no router, no write path, no WAL, and no execution.
