# mini-kv v1086-v1110 signed approval artifact draft text package profile section renderer split

## Version Goal

v1086-v1110 adds a read-only evidence command for the signed approval artifact draft text package profile-section renderer split:

```text
SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON
```

The command freezes `fixtures/release/shard-readiness-v1085.json` as the current mini-kv source fixture, carries forward the older text-package source evidence from v810, v835, v860, and v880, and publishes a versioned v1086-v1110 fixture chain.

It is not a text package material importer, not a signed approval creator, not a Node renderer executor, not a Node type catalog import, not a material intake path, not a runtime probe, and not a router or write path.

The purpose is to let Node/Java/mini-kv readers see that Node v1531 is only a renderer ownership split. mini-kv records the shape of that split as evidence, but does not consume text package material, does not create approval artifacts, does not start sibling services, does not touch WAL, and does not execute anything.

## Why This Version Exists

The latest Node plan read for this round is:

```text
D:\nodeproj\orderops-node\docs\plans3\v1531-controlled-read-only-shard-preview-signed-approval-artifact-draft-text-package-profile-section-renderer-split-roadmap.md
```

That plan says Node v1507-v1531 is a renderer ownership and maintenance split for signed approval artifact draft text package profile sections. It does not ask mini-kv or Java for fresh evidence, does not add runtime behavior, and does not ask sibling projects to open approval, material, governance, deployment, or execution chains.

mini-kv therefore treats the plan as read-only context. The C++ side records that v1085 is the last current shard readiness fixture before the text package profile-section evidence starts, then proves that the new command is only a versioned metadata view over frozen source evidence.

## Module Split

The implementation is intentionally split into short files:

```text
include/minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks.hpp
include/minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation.hpp
src/shard_preview_signed_approval_artifact_draft_text_package_profile_section.cpp
src/shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages.cpp
src/shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks.cpp
src/shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation.cpp
```

The core module formats the public JSON. It aggregates Node v1531 context, source fixture lineage, source command completion, stage chain, profile-section names, check catalog, validation summary, diagnostics, and boundary fields.

The stages module owns the 25-version chain from v1086 to v1110. It starts with the v1085 source freeze, then records Node v1531 renderer ownership, nine text package profile sections, renderer module metadata, each carried Node marker, type catalog order lock, route Markdown parity, no-fresh-evidence status, false-flag locks, command exposure, SHARDJSON embedding, validation split, CLI/TCP smoke, and closeout.

The checks module owns the 25 read-only checks. Every check repeats the denial fields that matter: `nodeRendererExecuted:false`, `nodeCatalogImported:false`, `textPackageMaterialImported:false`, `signedApprovalCreated:false`, `runtimePayloadImported:false`, `secretValueRead:false`, `materialIntakeOpened:false`, `activeRouterInstalled:false`, `writeRoutingAllowed:false`, `touchesWal:false`, and `executionAllowed:false`.

The validation module computes the final pass/fail bit from real inputs. It requires v1085 profile-section integrity completion, v810 review preflight completion, v835 review closeout audit completion, v860 comparison closeout audit completion, v880 compared package intake audit completion, exactly nine text package profile sections, no fresh mini-kv evidence requirement, no route Markdown change, no mini-kv text package material consumption, 25 checks, and a complete 25-stage chain.

## Public Contract

The new contract is:

```text
shard-route-preview-signed-approval-artifact-draft-text-package-profile-section-renderer-split.v1
```

The new `SHARDJSON` section is:

```text
shardPreviewSignedApprovalArtifactDraftTextPackageProfileSection
```

Important source fields:

```text
sourceCurrentShardReadinessReleaseVersion = v1085
sourceCurrentShardReadinessFixturePath = fixtures/release/shard-readiness-v1085.json
sourceSignedApprovalDraftProfileSectionIntegrityReleaseVersion = v1085
sourceDraftTextPackageReviewPreflightReleaseVersion = v810
sourceDraftTextPackageReviewCloseoutAuditReleaseVersion = v835
sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion = v860
sourceComparedPackageEvidenceIntakeAuditReleaseVersion = v880
sourceNodeSignedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion = Node v1531
sourceNodeRequiresFreshMiniKvEvidence = false
sourceNodeRequiresFreshJavaEvidence = false
```

Important profile-section fields:

```text
signedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion = v1110
signedApprovalArtifactDraftTextPackageProfileSectionReleaseRangeStart = v1086
signedApprovalArtifactDraftTextPackageProfileSectionReleaseRangeEnd = v1110
publishedStageCount = 25
plannedStageCount = 25
signedApprovalArtifactDraftTextPackageProfileSectionCount = 9
nodeMigratedSignedApprovalArtifactDraftTextPackageProfileSectionCount = 9
signedApprovalArtifactDraftTextPackageProfileSectionValidationPassed = true
```

The nine profile section names are:

```text
text-package-intake
text-package-review-preflight
text-package-submission-preflight
text-package-comparison-preflight
text-package-comparison-acceptance-precheck
compared-package-evidence-intake
compared-evidence-evaluation-preflight
compared-evidence-candidate
compared-evidence-candidate-intake
```

The boundary fields remain false:

```text
nodeRendererExecuted
nodeCatalogImported
textPackageMaterialImported
textPackageMaterialAccepted
comparedPackageEvidenceAccepted
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
secretValueRead
rawEndpointParsed
detachedSignaturePayloadParsed
detachedSignaturePayloadVerified
activeRouterInstalled
documentRouterInstalled
routerActivationAllowed
writeRoutingAllowed
writeCommandsAllowed
mutatesStore
adminCommandsAllowed
loadRestoreCompactAllowed
touchesWal
startsServices
startsMiniKvService
startsSiblingServices
liveReadAllowed
runtimeProbeAllowed
filesystemReadPerformed
runtimeArchiveWalkAllowed
siblingMutationAllowed
executionAllowed
```

## Command And Aggregation Wiring

`src/command.cpp` adds `SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON` to the runtime evidence dispatch table. The handler rejects extra arguments with the normal usage error.

`src/command_contracts.cpp` records the command as a stable read command:

```text
category = read
mutates_store = false
touches_wal = false
stable = true
```

`src/line_editor.cpp` exposes the command in completion. The command is intentionally long because it names the full evidence boundary; the completion test uses a text-package-specific prefix so it does not blur into the older signed approval draft profile-section commands.

`src/shard_readiness.cpp` advances the rolling release to `v1110`, updates the top-level status to `route-preview-signed-approval-artifact-draft-text-package-profile-section-closeout-summary-read-only`, adds the new digest marker into the evidence digest source, and embeds `shardPreviewSignedApprovalArtifactDraftTextPackageProfileSection` after the v1085 integrity section.

## Tests

Focused tests cover the split:

```text
signed_approval_artifact_draft_text_package_profile_section_stages_tests
signed_approval_artifact_draft_text_package_profile_section_checks_tests
signed_approval_artifact_draft_text_package_profile_section_validation_tests
signed_approval_artifact_draft_text_package_profile_section_tests
signed_approval_artifact_draft_text_package_profile_section_fixture_tests
```

`command_tests` verifies direct command execution, usage errors, `COMMANDS`, `COMMANDSJSON`, `HELP`, read-only command contracts, and SHARDJSON visibility.

`line_editor_tests` verifies completion for the new command. `shard_readiness_tests` verifies that rolling SHARDJSON now reports v1110 status and embeds the new section once. The v1085 integrity fixture test was kept alive and updated only to understand that the current fixture is now v1110 while the v1085 integrity evidence remains preserved.

The new fixture test reads versioned fixtures through `test_support`, so CTest can run from the build directory without path guessing. That small cleanup avoids a fragile test helper while keeping the fixture assertions focused on release versions and false boundary fields.

## Fixture Evidence

The versioned fixtures are:

```text
fixtures/release/shard-readiness-v1086.json
...
fixtures/release/shard-readiness-v1110.json
fixtures/release/shard-readiness.json
```

They were generated from real CLI `SHARDJSON` output by temporarily advancing the profile-section stage count and rolling release version, then restoring the source constants to v1110 and rebuilding.

## Verification

Final verification used the CLion CMake, CTest, Ninja, and MinGW runtime paths.

The focused suite passed 9/9:

```text
command_tests
shard_readiness_tests
signed_approval_artifact_draft_profile_section_integrity_fixture_tests
signed_approval_artifact_draft_text_package_profile_section_stages_tests
signed_approval_artifact_draft_text_package_profile_section_checks_tests
signed_approval_artifact_draft_text_package_profile_section_validation_tests
signed_approval_artifact_draft_text_package_profile_section_tests
signed_approval_artifact_draft_text_package_profile_section_fixture_tests
line_editor_tests
```

The full suite passed 211/211.

Real CLI smoke returned v1110 from:

```text
SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON
```

Real TCP smoke sent the same read-only command through `minikv_server` and `minikv_client`. It observed `writeRoutingAllowed:false`, `secretValueRead:false`, and `executionAllowed:false`; the temporary server was stopped after the smoke.

## Boundary Summary

The most important engineering choice in v1086-v1110 is that the C++ side did not chase Node's renderer split by implementing renderer behavior. It created a frozen, queryable, versioned evidence section that says exactly what mini-kv knows and exactly what it refuses to do.

One sentence: v1086-v1110 turns Node v1531 signed approval artifact draft text package profile-section renderer ownership into mini-kv read-only evidence without importing Node renderers, consuming text package material, creating signed approvals, opening routers, writing WAL, or executing runtime behavior.
