# mini-kv v1011-v1035 candidate profile section renderer split evidence

## Version Goal

v1011-v1035 adds a read-only candidate document profile section evidence handle for mini-kv:

```text
SHARDROUTECANDIDATEPROFILESECTIONJSON
```

The command freezes `fixtures/release/shard-readiness-v1010.json` as the source material submission precheck integrity fixture, records Node v1481 as a renderer ownership split, and publishes a 25-stage v1011-v1035 fixture chain. It is not a new candidate document intake path, not a material submission path, not a Node renderer execution, and not a fresh evidence dependency for Node.

The negative boundary is the point: mini-kv does not read candidate document bodies, does not consume reviewed material, does not import payloads, does not evaluate candidate material, does not install a router, does not write, does not touch WAL, does not start sibling services, and does not execute.

## Why This Version Exists

The latest Node plan is:

```text
D:\nodeproj\orderops-node\docs\plans3\v1481-controlled-read-only-shard-preview-candidate-document-profile-section-renderer-split-roadmap.md
```

That plan is a maintenance split. Node extracts five candidate document profile sections from a large live read-only window renderer into `controlledReadOnlyShardPreviewCandidateDocumentProfileSectionRenderer.ts`. The plan explicitly says Java and mini-kv do not need fresh evidence for Node v1457-v1481, and that they can continue their own read-only shard preview work in parallel.

mini-kv therefore does not pretend to consume a new Node artifact. It records the plan as read-only context and creates its own C++ evidence handle so later readers can see:

```text
Node v1481 profile section renderer ownership changed
v1010 mini-kv integrity evidence remains the source fixture
five candidate profile sections are known
route-facing output is declared stable
no fresh mini-kv evidence is required by Node
no runtime, router, write, WAL, or execution boundary is opened
```

## Module Split

The implementation is intentionally split into small modules:

```text
include/minikv/shard_preview_candidate_profile_section.hpp
include/minikv/shard_preview_candidate_profile_section_stages.hpp
include/minikv/shard_preview_candidate_profile_section_checks.hpp
include/minikv/shard_preview_candidate_profile_section_validation.hpp
src/shard_preview_candidate_profile_section.cpp
src/shard_preview_candidate_profile_section_stages.cpp
src/shard_preview_candidate_profile_section_checks.cpp
src/shard_preview_candidate_profile_section_validation.cpp
```

The core module formats the public JSON, status, digest marker, stage catalog, profile section checks, validation object, diagnostics, and boundary booleans.

The stages module owns the 25-version release chain from v1011 to v1035. It starts from the v1010 source freeze, then covers Node renderer-split context, five-section grouping, section heading catalog, request package, submission precheck, intake packet, material request, material submission precheck, route Markdown parity, no fresh evidence requirement, disabled upstream probes, type catalog compatibility, service startup, archive scan, runtime probe, router, write/WAL, execution, command catalog exposure, SHARDJSON embedding, validation split, CLI/TCP smoke, and closeout.

The checks module owns the 25 read-only checks. These checks are not document checks. They verify that the profile section evidence remains metadata-only, that Node renderer execution is not performed, that fresh mini-kv evidence is not required by Node v1481, and that router/write/WAL/execution remain disabled.

The validation module computes a real pass/fail value. It requires source integrity stage count 25, source chain complete, five candidate profile sections, five Node-migrated sections, no fresh mini-kv requirement, no route Markdown change, no profile material consumption, and 25 completed profile section checks. The validation test passes a bad migrated-section count to prove the validation can fail.

## Public Contract

The new contract is:

```text
shard-route-preview-candidate-document-profile-section-renderer-split.v1
```

The new `SHARDJSON` section is:

```text
shardPreviewCandidateProfileSection
```

Important source fields:

```text
sourceNodeProfileSectionReleaseVersion = Node v1481
sourceNodeCandidateProfileSectionRendererModule = controlledReadOnlyShardPreviewCandidateDocumentProfileSectionRenderer.ts
sourceCandidateMaterialSubmissionPrecheckIntegrityReleaseVersion = v1010
sourceCandidateMaterialSubmissionPrecheckIntegrityFixturePath = fixtures/release/shard-readiness-v1010.json
sourceCandidateMaterialSubmissionPrecheckIntegrityPublishedStageCount = 25
sourceCandidateMaterialSubmissionPrecheckIntegrityComplete = true
```

Important profile fields:

```text
candidateProfileSectionReleaseVersion = v1035
candidateProfileSectionCount = 5
nodeMigratedCandidateProfileSectionCount = 5
freshMiniKvEvidenceRequiredByNode = false
routeFacingMarkdownStable = true
nodeRendererExecuted = false
miniKvConsumesProfileMaterial = false
```

The five profile section names are carried as strings:

```text
candidate-document-request-package
candidate-document-submission-precheck
candidate-document-intake-packet
candidate-document-material-request
candidate-document-material-submission-precheck
```

## Command And Aggregation Wiring

`src/command.cpp` adds `SHARDROUTECANDIDATEPROFILESECTIONJSON` to the runtime evidence dispatch table. Extra arguments fail with the normal usage error.

`src/command_contracts.cpp` records the command as:

```text
category = read
mutates_store = false
touches_wal = false
stable = true
```

`src/line_editor.cpp` exposes it in completion, and `CommandProcessor::help_text()` lists it next to the other candidate document preview commands.

`src/shard_readiness.cpp` advances the rolling release to `v1035`, makes the new profile section closeout status the top-level status, adds its digest marker into the evidence digest source, and embeds the new `shardPreviewCandidateProfileSection` section after the v1010 material submission precheck integrity section.

## Tests

Focused tests cover the new split:

```text
candidate_profile_section_stages_tests
candidate_profile_section_checks_tests
candidate_profile_section_validation_tests
candidate_profile_section_tests
candidate_profile_section_fixture_tests
```

`command_tests` verifies direct command execution, usage errors, `COMMANDS`, `COMMANDSJSON`, `HELP`, and SHARDJSON visibility.

`line_editor_tests` verifies completion.

`shard_readiness_tests` verifies that rolling SHARDJSON now reports v1035 status while preserving the v1010 source integrity evidence.

Older SHARDJSON tests that were hard-coded to the previous latest status were updated to assert the v1035 top-level status. The v1010 module's own status test remains unchanged, so historical meaning is preserved.

## Fixture Evidence

The versioned fixtures are:

```text
fixtures/release/shard-readiness-v1011.json
...
fixtures/release/shard-readiness-v1035.json
fixtures/release/shard-readiness.json
```

They were generated from the real CLI `SHARDJSON` output by temporarily advancing the profile-section stage count and rolling release version, then restoring the source constants to v1035.

## Boundary Summary

The critical false fields remain false:

```text
sourceNodeRequiresFreshMiniKvEvidence
sourceNodeStartsMiniKvService
sourceNodeStartsJavaService
sourceNodeRouteMarkdownChanged
sourceNodeRuntimeBehaviorAdded
candidateProfileSectionRendererModuleImported
nodeRendererExecuted
nodeCatalogImported
miniKvConsumesProfileMaterial
reviewedRealCandidateDocumentRead
documentIntakeOpened
materialSubmissionOpened
materialPayloadImported
runtimePayloadImported
activeRouterInstalled
writeRoutingAllowed
touchesWal
executionAllowed
```

One sentence: v1011-v1035 gives mini-kv a versioned, test-backed profile-section evidence handle that matches Node v1481's renderer split context while keeping the C++ side strictly read-only and non-executable.
