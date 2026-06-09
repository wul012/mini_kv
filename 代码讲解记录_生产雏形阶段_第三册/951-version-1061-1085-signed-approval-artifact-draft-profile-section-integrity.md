# mini-kv v1061-v1085 signed approval artifact draft profile section integrity

## Version Goal

v1061-v1085 adds a read-only integrity evidence layer for the v1036-v1060 signed approval artifact draft profile section command:

```text
SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON
```

The command freezes `fixtures/release/shard-readiness-v1060.json` as the source profile-section fixture, publishes a 25-stage v1061-v1085 fixture chain, and proves that the v1060 profile-section evidence remains only evidence.

It is not a new signed approval flow, not a signed approval creator, not a material intake path, not a Node renderer execution path, not a Node catalog import, and not a runtime probe.

The point of this version is negative integrity: no fresh mini-kv or Java evidence is required by Node v1506, mini-kv does not execute the Node renderer, does not import Node catalogs, does not create draft artifacts, does not create or emit signed approvals, does not open material intake, does not import runtime payloads, does not install a router, does not write, does not touch WAL, and does not execute.

## Why This Version Exists

The latest Node plan read for this round is:

```text
D:\nodeproj\orderops-node\docs\plans3\v1506-controlled-read-only-shard-preview-signed-approval-artifact-draft-profile-section-renderer-split-roadmap.md
```

That plan says Node v1482-v1506 is a renderer ownership and maintenance split. It does not ask Java or mini-kv for fresh evidence, does not introduce runtime behavior, and does not ask sibling projects to open an approval/material/execution chain.

mini-kv therefore uses the plan as read-only context. The C++ side records that v1060 is already the signed approval draft profile-section source, then adds an integrity command whose job is to freeze and re-check that source. The command lets future Node/Java readers consume one small summary instead of re-inferring every false boundary from the older profile-section JSON.

## Module Split

The implementation is deliberately split to avoid one huge formatter:

```text
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation.hpp
src/shard_preview_signed_approval_artifact_draft_profile_section_integrity.cpp
src/shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages.cpp
src/shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks.cpp
src/shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation.cpp
```

The core module formats the public JSON: source Node plan, source v1060 profile-section command, stage chain summary, section names, migrated section count, boundary booleans, diagnostics, and validation object.

The stages module owns the 25-version release chain from v1061 to v1085. It starts with the v1060 source freeze, then covers digest pinning, command contract lock, five-section count, Node marker metadata, route Markdown stability, no fresh evidence, draft artifact false lock, signed approval false lock, material intake lock, runtime payload lock, renderer execution lock, catalog import lock, service startup lock, runtime probe lock, router lock, write/WAL lock, load/restore/compact lock, execution lock, command exposure, SHARDJSON embedding, validation split, current fixture rollout, CLI/TCP smoke, and closeout.

The checks module owns 25 read-only integrity checks. Each check repeats the same important denial fields so that a single malformed check cannot accidentally imply authority: `nodeRendererExecuted:false`, `nodeCatalogImported:false`, `freshMiniKvEvidenceRequiredByNode:false`, `draftArtifactCreated:false`, `signedApprovalCreated:false`, `materialIntakeOpened:false`, `runtimePayloadImported:false`, `writeRoutingAllowed:false`, `loadRestoreCompactAllowed:false`, `touchesWal:false`, and `executionAllowed:false`.

The validation module computes pass/fail from real values rather than just printing a happy-path sentence. It requires v1060 source profile-section completion, exactly five signed approval draft profile sections, exactly five Node-migrated sections, no fresh evidence requirement, no route Markdown change, no signed approval material consumption, 25 completed checks, and every execution/write/router boundary to remain false.

## Public Contract

The new contract is:

```text
shard-route-preview-signed-approval-artifact-draft-profile-section-integrity.v1
```

The new `SHARDJSON` section is:

```text
shardPreviewSignedApprovalArtifactDraftProfileSectionIntegrity
```

Important source fields:

```text
sourceProfileSectionCommand = SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON
sourceProfileSectionContract = shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1
sourceProfileSectionReleaseVersion = v1060
sourceProfileSectionFixturePath = fixtures/release/shard-readiness-v1060.json
sourceProfileSectionPublishedStageCount = 25
sourceProfileSectionComplete = true
sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion = Node v1506
sourceNodeRequiresFreshMiniKvEvidence = false
sourceNodeRequiresFreshJavaEvidence = false
```

Important integrity fields:

```text
signedApprovalArtifactDraftProfileSectionIntegrityReleaseVersion = v1085
signedApprovalArtifactDraftProfileSectionIntegrityReleaseRangeStart = v1061
signedApprovalArtifactDraftProfileSectionIntegrityReleaseRangeEnd = v1085
publishedStageCount = 25
plannedStageCount = 25
signedApprovalArtifactDraftProfileSectionCount = 5
nodeMigratedSignedApprovalArtifactDraftProfileSectionCount = 5
nodeRendererExecuted = false
nodeCatalogImported = false
draftArtifactCreated = false
signedApprovalCreated = false
materialIntakeOpened = false
runtimePayloadImported = false
writeRoutingAllowed = false
touchesWal = false
executionAllowed = false
```

The five profile section names remain:

```text
signed-approval-artifact-draft-preflight
signed-approval-artifact-draft-readiness
signed-approval-artifact-draft-review-package-preflight
signed-approval-artifact-draft-authoring-readiness
signed-approval-artifact-draft-instruction-preflight
```

## Command And Aggregation Wiring

`src/command.cpp` adds `SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON` to the runtime evidence dispatch table. Extra arguments fail with the normal usage error.

`src/command_contracts.cpp` records the command as:

```text
category = read
mutates_store = false
touches_wal = false
stable = true
```

`src/line_editor.cpp` exposes the command in completion. The long common prefix with the v1060 command is now intentionally ambiguous until the user types the `I` in `INTEGRITYJSON`, so `line_editor_tests` asserts the common-prefix behavior and then the full completion behavior.

`src/shard_readiness.cpp` advances the rolling release to `v1085`, makes the integrity closeout status the top-level status, adds the integrity digest marker into the evidence digest source, and embeds the new `shardPreviewSignedApprovalArtifactDraftProfileSectionIntegrity` section after the v1060 profile-section evidence.

## Tests

Focused tests cover the split:

```text
signed_approval_artifact_draft_profile_section_integrity_stages_tests
signed_approval_artifact_draft_profile_section_integrity_checks_tests
signed_approval_artifact_draft_profile_section_integrity_validation_tests
signed_approval_artifact_draft_profile_section_integrity_tests
signed_approval_artifact_draft_profile_section_integrity_fixture_tests
```

`command_tests` verifies direct command execution, usage errors, `COMMANDS`, `COMMANDSJSON`, `HELP`, read-only command contracts, and SHARDJSON visibility.

`line_editor_tests` verifies completion without breaking the older v1060 command. `shard_readiness_tests` verifies that rolling SHARDJSON now reports v1085 status while preserving the v1060 source profile-section section.

Older SHARDJSON tests that asserted the previous latest status were updated to assert the v1085 top-level status. The v1060 profile-section module's own status test remains unchanged, so historical meaning is preserved.

## Fixture Evidence

The versioned fixtures are:

```text
fixtures/release/shard-readiness-v1061.json
...
fixtures/release/shard-readiness-v1085.json
fixtures/release/shard-readiness.json
```

They were generated from the real CLI `SHARDJSON` output by temporarily advancing the integrity stage count and rolling release version, then restoring the source constants to v1085.

## Verification

Final verification used the CLion CMake, CTest, Ninja, and MinGW runtime paths.

The focused suite passed 9/9:

```text
command_tests
shard_readiness_tests
signed_approval_artifact_draft_profile_section_fixture_tests
signed_approval_artifact_draft_profile_section_integrity_stages_tests
signed_approval_artifact_draft_profile_section_integrity_checks_tests
signed_approval_artifact_draft_profile_section_integrity_validation_tests
signed_approval_artifact_draft_profile_section_integrity_tests
signed_approval_artifact_draft_profile_section_integrity_fixture_tests
line_editor_tests
```

The full suite passed 206/206.

Real CLI smoke returned v1085 from:

```text
SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON
```

Real TCP smoke sent the same read-only command through `minikv_server` and `minikv_client`. It observed `writeRoutingAllowed:false` and `executionAllowed:false`, and the temporary WAL statistics stayed at 0 records and 0 bytes.

## Boundary Summary

The critical false fields remain false:

```text
sourceNodeRequiresFreshMiniKvEvidence
sourceNodeRequiresFreshJavaEvidence
sourceNodeStartsMiniKvService
sourceNodeStartsJavaService
nodeRendererExecuted
nodeCatalogImported
miniKvConsumesSignedApprovalMaterial
draftArtifactCreated
draftArtifactImported
signedApprovalCreated
signedApprovalEmitted
approvalGrantCaptured
governanceEchoCreated
reviewedMaterialPresent
externalReviewedMaterialConsumed
materialIntakeOpened
materialPayloadImported
runtimePayloadImported
runtimePayloadAccepted
activeRouterInstalled
writeRoutingAllowed
loadRestoreCompactAllowed
touchesWal
executionAllowed
```

One sentence: v1061-v1085 turns the v1060 signed approval draft profile-section evidence into a versioned integrity checkpoint that future consumers can trust without mini-kv importing Node renderers, creating approval material, opening material intake, routing writes, touching WAL, or executing.
