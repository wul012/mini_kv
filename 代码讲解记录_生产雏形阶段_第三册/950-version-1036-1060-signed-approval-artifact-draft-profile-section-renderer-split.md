# mini-kv v1036-v1060 signed approval artifact draft profile section renderer split evidence

## Version Goal

v1036-v1060 adds a read-only signed approval artifact draft profile section evidence handle for mini-kv:

```text
SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON
```

The command freezes `fixtures/release/shard-readiness-v1035.json` as the source current shard readiness fixture, carries `fixtures/release/shard-readiness-v785.json` as the signed approval artifact draft instruction preflight lineage, records Node v1506 as a renderer ownership split, and publishes a 25-stage v1036-v1060 fixture chain.

It is not a signed approval creator, not a draft artifact authoring entry, not a material intake path, not a Node renderer execution path, and not a fresh evidence dependency for Node.

The negative boundary is the main value: mini-kv does not create signed approvals, does not import signed approval material, does not parse detached signature payloads, does not open governance echo, does not install a router, does not write, does not touch WAL, does not start sibling services, and does not execute.

## Why This Version Exists

The latest Node plan read for this round is:

```text
D:\nodeproj\orderops-node\docs\plans3\v1506-controlled-read-only-shard-preview-signed-approval-artifact-draft-profile-section-renderer-split-roadmap.md
```

That plan is a maintenance split. Node extracts signed approval artifact draft profile section rendering into `controlledReadOnlyShardPreviewSignedApprovalArtifactDraftProfileSectionRenderer.ts`. The plan states that Java and mini-kv do not need fresh evidence for that Node maintenance range, and that sibling projects may keep advancing read-only evidence and contract hardening in parallel.

mini-kv therefore treats Node v1506 as read-only planning context. It does not import a Node catalog or run a Node renderer. It records:

```text
Node v1506 profile section renderer ownership changed
v1035 remains the source current shard readiness fixture
v785 remains the signed approval draft instruction preflight source marker
five signed approval draft profile sections are known
route-facing output is declared stable
no fresh mini-kv evidence is required by Node
no signed approval, router, write, WAL, or execution boundary is opened
```

## Module Split

The implementation is split into small modules:

```text
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section_stages.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section_checks.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section_validation.hpp
src/shard_preview_signed_approval_artifact_draft_profile_section.cpp
src/shard_preview_signed_approval_artifact_draft_profile_section_stages.cpp
src/shard_preview_signed_approval_artifact_draft_profile_section_checks.cpp
src/shard_preview_signed_approval_artifact_draft_profile_section_validation.cpp
```

The core module formats the public JSON, status, digest marker, source lineage, section names, stage catalog, checks, validation object, diagnostics, and boundary booleans.

The stages module owns the 25-version release chain from v1036 to v1060. It starts from the v1035 source freeze, then covers Node renderer-split context, five-section grouping, heading catalog, draft preflight, draft readiness, review package preflight, authoring readiness, instruction preflight, Node version markers, route-facing stability, route Markdown parity, no fresh evidence requirement, false flag lock, type catalog compatibility, service startup boundary, runtime probe boundary, router boundary, write/WAL boundary, execution boundary, command exposure, SHARDJSON embedding, validation split, CLI/TCP smoke, and closeout.

The checks module owns 25 read-only checks. These are not signed approval checks in the sense of validating a real approval. They verify that the evidence is metadata-only, that Node renderer execution is not performed, that no fresh mini-kv evidence is required by Node v1506, and that router/write/WAL/execution remain disabled.

The validation module computes a real pass/fail value. It requires the v1035 source current fixture chain to be complete, the v785 instruction preflight source chain to be complete, exactly five signed approval draft profile sections, exactly five Node-migrated sections, no fresh mini-kv evidence requirement, no route Markdown change, no signed approval material consumption, and 25 completed checks. Its dedicated test passes a bad section count to prove the validation can fail.

## Public Contract

The new contract is:

```text
shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1
```

The new `SHARDJSON` section is:

```text
shardPreviewSignedApprovalArtifactDraftProfileSection
```

Important source fields:

```text
sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion = Node v1506
sourceNodeSignedApprovalArtifactDraftProfileSectionRendererModule = controlledReadOnlyShardPreviewSignedApprovalArtifactDraftProfileSectionRenderer.ts
sourceCurrentShardReadinessReleaseVersion = v1035
sourceCurrentShardReadinessFixturePath = fixtures/release/shard-readiness-v1035.json
sourceCandidateProfileSectionCommand = SHARDROUTECANDIDATEPROFILESECTIONJSON
sourceCandidateProfileSectionReleaseVersion = v1035
sourceDraftInstructionPreflightCommand = SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON
sourceDraftInstructionPreflightReleaseVersion = v785
```

Important profile fields:

```text
signedApprovalArtifactDraftProfileSectionReleaseVersion = v1060
signedApprovalArtifactDraftProfileSectionCount = 5
nodeMigratedSignedApprovalArtifactDraftProfileSectionCount = 5
freshMiniKvEvidenceRequiredByNode = false
routeFacingMarkdownStable = true
nodeRendererExecuted = false
miniKvConsumesSignedApprovalMaterial = false
```

The five profile section names are carried as strings:

```text
signed-approval-artifact-draft-preflight
signed-approval-artifact-draft-readiness
signed-approval-artifact-draft-review-package-preflight
signed-approval-artifact-draft-authoring-readiness
signed-approval-artifact-draft-instruction-preflight
```

## Command And Aggregation Wiring

`src/command.cpp` adds `SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON` to the runtime evidence dispatch table. Extra arguments fail with the normal usage error.

`src/command_contracts.cpp` records the command as:

```text
category = read
mutates_store = false
touches_wal = false
stable = true
```

`src/line_editor.cpp` exposes it in completion, and `CommandProcessor::help_text()` lists it next to the other route preview value supply signed approval draft commands.

`src/shard_readiness.cpp` advances the rolling release to `v1060`, makes the new profile section closeout status the top-level status, adds its digest marker into the evidence digest source, and embeds the new `shardPreviewSignedApprovalArtifactDraftProfileSection` section after the candidate profile section evidence.

## Tests

Focused tests cover the new split:

```text
signed_approval_artifact_draft_profile_section_stages_tests
signed_approval_artifact_draft_profile_section_checks_tests
signed_approval_artifact_draft_profile_section_validation_tests
signed_approval_artifact_draft_profile_section_tests
signed_approval_artifact_draft_profile_section_fixture_tests
```

`command_tests` verifies direct command execution, usage errors, `COMMANDS`, `COMMANDSJSON`, `HELP`, and SHARDJSON visibility.

`line_editor_tests` verifies completion.

`shard_readiness_tests` verifies that rolling SHARDJSON now reports v1060 status while preserving the v1035 current source and v785 instruction preflight lineage.

Older SHARDJSON tests that asserted the previous latest status were updated to assert the v1060 top-level status. The v1035 candidate profile module's own status test remains unchanged, so historical meaning is preserved.

## Fixture Evidence

The versioned fixtures are:

```text
fixtures/release/shard-readiness-v1036.json
...
fixtures/release/shard-readiness-v1060.json
fixtures/release/shard-readiness.json
```

They were generated from the real CLI `SHARDJSON` output by temporarily advancing the signed approval draft profile section stage count and rolling release version, then restoring the source constants to v1060.

## Boundary Summary

The critical false fields remain false:

```text
sourceNodeRequiresFreshMiniKvEvidence
sourceNodeStartsMiniKvService
sourceNodeStartsJavaService
sourceNodeRouteMarkdownChanged
sourceNodeRuntimeBehaviorAdded
signedApprovalArtifactDraftProfileSectionRendererModuleImported
nodeRendererExecuted
nodeCatalogImported
miniKvConsumesSignedApprovalMaterial
draftArtifactCreated
draftArtifactImported
signedApprovalCreated
signedApprovalEmitted
approvalGrantCaptured
governanceEchoCreated
materialIntakeOpened
materialPayloadImported
runtimePayloadImported
activeRouterInstalled
writeRoutingAllowed
touchesWal
executionAllowed
```

One sentence: v1036-v1060 gives mini-kv a versioned, test-backed signed approval draft profile-section evidence handle that matches Node v1506's renderer split context while keeping the C++ side strictly read-only and non-executable.
