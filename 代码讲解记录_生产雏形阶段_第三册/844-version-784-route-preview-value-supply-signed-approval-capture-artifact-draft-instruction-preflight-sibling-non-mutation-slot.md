# mini-kv v784 route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-sibling-non-mutation-slot

v784 advances the draft instruction preflight chain to sequence 24. It freezes `fixtures/release/shard-readiness-v783.json` as the source for this step and keeps the rolling SHARDJSON evidence on the disabled instruction-preflight surface.

This version is not a signed approval draft, not approval capture, not a detached signature payload, not an operator value import, not active routing, and not runtime execution. It only adds another reviewed metadata slot to the instruction preflight chain.

## What Changed

The core formatter updates `current_instruction_preflight_stage_count` to 24, so the stage catalog, instruction slots, guards, and profiles publish the first 24 entries. `shard_readiness.cpp` updates the rolling release and status to `route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-sibling-non-mutation-slot-read-only` and keeps the instruction preflight object embedded exactly once in SHARDJSON.

The frozen source fixture for this version is `fixtures/release/shard-readiness-v783.json`. The current fixture is regenerated from CLI output and carries `releaseVersion=v784`.

## Boundary Fields

The important fields remain stable: `draftInstructionPreflightOnly=true`, `draftInstructionTextGenerated=false`, `draftInstructionTextMaterialized=false`, `materializedInstructionTextCount=0`, `approvalGrantEmitted=false`, `writeRoutingAllowed=false`, `touchesWal=false`, and `executionAllowed=false`.

`readyForDraftInstructionReview=true` means downstream readers can inspect the slot manifest. `readyForSignedApprovalArtifactDraft=false` keeps the stronger signed draft step closed.

## Tests

Focused tests cover command registration, line-editor completion, SHARDJSON embedding, stage continuity, slots, guards, profiles, validation, and old authoring/artifact preflight compatibility. Full CTest confirms no WAL, TCP, restore, runtime receipt, or fixture guard regression.

One-sentence value: v784 thickens the instruction preflight evidence chain while preserving the same no-write/no-execution mini-kv boundary.
