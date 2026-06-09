# mini-kv v986-v1010 candidate material submission precheck integrity

## Version Goal

v986-v1010 adds an integrity layer for the v985 material submission precheck evidence. The public command is:

```text
SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON
```

This command freezes `fixtures/release/shard-readiness-v985.json` as the source precheck fixture, publishes a v986-v1010 integrity chain, and verifies that the precheck evidence stays internally consistent. It is not a new material submission echo and not a runtime intake path.

The important negative boundary is unchanged: mini-kv does not consume reviewed material, does not read real candidate document bodies, does not import payloads, does not evaluate candidate material, does not capture approval, does not sign, does not install a router, does not write, does not touch WAL, and does not execute.

## Why This Is Allowed After The Stop Condition

The Node v1456 plan says the material submission precheck chain must stop before real material submission. This version respects that stop condition. It does not add another submission precheck, and it does not pretend that reviewed material exists. Instead, it audits the already-frozen v985 mini-kv evidence so later readers can trust the release fixture chain and command catalog without opening any material path.

In other words, v985 is the source evidence. v986-v1010 is the integrity wrapper around that evidence.

## Module Split

The implementation is split into four small modules:

```text
include/minikv/shard_preview_candidate_material_submission_precheck_integrity.hpp
src/shard_preview_candidate_material_submission_precheck_integrity.cpp
src/shard_preview_candidate_material_submission_precheck_integrity_stages.cpp
src/shard_preview_candidate_material_submission_precheck_integrity_checks.cpp
src/shard_preview_candidate_material_submission_precheck_integrity_validation.cpp
```

The core module formats the public JSON, status, digest marker, stage catalog, integrity checks, validation object, diagnostics, and boundary booleans.

The stages module owns the 25-version release chain from v986 to v1010. It starts with a source freeze, then covers stop-condition pinning, fixture range, release continuity, source lineage, status parity, count parity, material absence, payload import, evaluation, approval, signature, runtime, router, write/WAL, sibling mutation, command catalog, CLI/TCP smoke, and closeout.

The checks module owns the 25 integrity checks. These checks are deliberately evidence checks, not document checks. They assert that source v985 remains frozen, counts remain aligned, and side-effect boundaries remain disabled.

The validation module computes a real pass/fail value. It requires source stage count 10, checkpoint count 10, validator count 10, request item count 25, acceptance check count 25, required field count 20, submission field count 20, gate count 41, and 25 completed integrity checks. Tests intentionally pass a bad validator count to prove validation can fail.

## Public Contract

The new contract is:

```text
shard-route-preview-candidate-material-submission-precheck-fixture-integrity.v1
```

The new `SHARDJSON` section is:

```text
shardPreviewCandidateMaterialSubmissionPrecheckIntegrity
```

Important source fields:

`sourceCandidateMaterialSubmissionPrecheckReleaseVersion` is `v985`.

`sourceCandidateMaterialSubmissionPrecheckFixturePath` is `fixtures/release/shard-readiness-v985.json`.

`sourceCandidateMaterialSubmissionPrecheckPublishedStageCount` is `10`.

`sourceMaterialSubmissionPrecheckCheckpointCount` and `sourceMaterialSubmissionPrecheckValidatorCount` are both `10`.

`sourceMaterialRequestItemCount` and `sourceAcceptanceCheckCount` are both `25`.

`sourceRequiredMaterialFieldCount` and `sourceSubmissionMaterialFieldCount` are both `20`.

`sourceMaterialSubmissionGateCount` is `41`.

The integrity layer itself reports `candidateMaterialSubmissionPrecheckIntegrityReleaseVersion=v1010`, `plannedIntegrityCheckCount=25`, `completedIntegrityCheckCount=25`, and `candidateMaterialSubmissionPrecheckIntegrityValidationPassed=true`.

The boundary fields are repeated at both top level and validation/check levels because they are the actual safety contract: `materialSubmissionOpened=false`, `externalReviewedMaterialConsumed=false`, `materialPayloadImported=false`, `documentIntakeOpened=false`, `documentRouterInstalled=false`, `writeRoutingAllowed=false`, `touchesWal=false`, and `executionAllowed=false`.

## Command And SHARDJSON Wiring

`src/command.cpp` registers `SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON`, so the command count moves from 65 to 66.

`src/command_contracts.cpp` marks the command as `read`, stable, non-mutating, and no-WAL.

`src/line_editor.cpp` adds completion support. The completion tests now distinguish:

```text
SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKJSON
SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON
```

`src/shard_readiness.cpp` moves the rolling release to v1010, adds the integrity digest input, sets the top-level status to the integrity closeout status, and embeds the new section after the v985 source precheck section.

## Fixtures

The new fixtures are:

```text
fixtures/release/shard-readiness-v986.json
...
fixtures/release/shard-readiness-v1010.json
fixtures/release/shard-readiness.json
```

They were generated from real `minikv_cli.exe SHARDJSON` output after rebuilding each stage. The temporary stage-count/release constants were restored to v1010 after fixture generation, then the final build and tests were run from the restored source.

The fixture tests prove the source v985 fixture does not contain the new integrity section, while rolling `shard-readiness.json` does contain it and still points back to v985 as source.

## Tests And Smoke

New tests:

```text
candidate_material_submission_precheck_integrity_stages_tests
candidate_material_submission_precheck_integrity_checks_tests
candidate_material_submission_precheck_integrity_validation_tests
candidate_material_submission_precheck_integrity_tests
candidate_material_submission_precheck_integrity_fixture_tests
```

Updated tests cover command count, `COMMANDS`, `COMMANDSJSON`, HELP, line-editor completion, SHARDJSON aggregation, and rolling status changes in older current-fixture consumers.

The closeout verification passed:

```text
focused CTest: 13/13
full CTest: 191/191
CLI smoke: SHARDROUTECANDIDATEMATERIALSUBMISSIONPRECHECKINTEGRITYJSON
TCP smoke: same command through minikv_server/minikv_client
```

## Summary

v986-v1010 turns v985 material submission precheck into a versioned integrity source that later Node/Java/mini-kv work can trust, while keeping the project on the intended side of the boundary: evidence integrity only, no material submission, no router, no write path, no WAL, and no execution.
