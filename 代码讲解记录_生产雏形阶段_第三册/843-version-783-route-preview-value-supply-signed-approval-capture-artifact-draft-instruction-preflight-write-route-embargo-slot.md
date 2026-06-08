# mini-kv v783 route-preview-value-supply-signed-approval-capture-artifact-draft-instruction-preflight-write-route-embargo-slot

v783 advances the draft instruction preflight chain to sequence 23: $(System.Collections.Hashtable.slug). It freezes ixtures/release/shard-readiness-v782.json as the source for this step and keeps the rolling SHARDJSON evidence on the same disabled instruction-preflight surface.

This version is not a signed approval draft, not approval capture, not a detached signature payload, not an operator value import, not active routing, and not runtime execution. It only adds another reviewed metadata slot to the instruction preflight chain.

## What Changed

The core formatter updates current_instruction_preflight_stage_count to 23, so the stage catalog, instruction slots, guards, and profiles now publish the first 23 entries. shard_readiness.cpp updates the rolling release and status to $(System.Collections.Hashtable.stage) and keeps the instruction preflight object embedded exactly once in SHARDJSON.

The frozen source fixture for this version is ixtures/release/shard-readiness-v782.json; the current fixture is regenerated from the CLI output and carries eleaseVersion=v783.

## Boundary Fields

The important fields remain stable: draftInstructionPreflightOnly=true, draftInstructionTextGenerated=false, draftInstructionTextMaterialized=false, materializedInstructionTextCount=0, pprovalGrantEmitted=false, writeRoutingAllowed=false, 	ouchesWal=false, and executionAllowed=false.

eadyForDraftInstructionReview=true means downstream readers can inspect the slot manifest. eadyForSignedApprovalArtifactDraft=false keeps the stronger signed draft step closed.

## Tests

Focused tests cover command registration, line-editor completion, SHARDJSON embedding, stage continuity, slots, guards, profiles, validation, and old authoring/artifact preflight compatibility. Full CTest then confirms no WAL, TCP, restore, runtime receipt, or fixture guard regression.

One-sentence value: v783 thickens the instruction preflight evidence chain while preserving the same no-write/no-execution mini-kv boundary.