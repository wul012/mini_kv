# mini-kv v761 draft instruction preflight request manifest slot

v761 starts the next evidence chain after v760 authoring readiness closeout. Its role is to expose a read-only draft instruction preflight surface for Node and Java planning: it says which instruction-review slots must exist before a later signed approval artifact draft can be discussed.

It is not a signed approval draft, not approval capture, not a signature payload, not an operator value import, not an active shard router, and not a storage execution path. The source is frozen at `fixtures/release/shard-readiness-v760.json`, so this command consumes completed authoring readiness as versioned evidence only.

## Module Split

`src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight.cpp` owns the public JSON command and digest marker. It references Node v1211 as plan evidence, points to the frozen v760 authoring readiness fixture, and publishes the first v761 stage.

`_stages.cpp` owns the v761-v785 stage chain. This keeps version sequencing, frozen source versions, fixture paths, and stage descriptions out of the core formatter.

`_slots.cpp`, `_guards.cpp`, and `_profiles.cpp` split the 25 instruction slot catalog into maintainable pieces. Slots describe what would need review, guards explain what remains blocked, and profiles describe how operators should read the preflight record.

`_validation.cpp` checks catalog alignment and source-chain completeness before the command reports validation success.

## Boundary Fields

`draftInstructionPreflightOnly=true` is the main control-plane signal. It means the command is a preflight manifest, not draft text. `draftInstructionTextGenerated=false`, `draftInstructionTextMaterialized=false`, and `materializedInstructionTextCount=0` prove that no instruction wording has been generated or stored.

`readyForDraftInstructionReview=true` is deliberately narrower than `readyForSignedApprovalArtifactDraft=false`. The first flag lets downstream reviewers inspect the slot manifest; the second flag keeps the stronger signed draft step closed.

The standard hard boundaries remain explicit: `readOnly=true`, `executionAllowed=false`, `writeRoutingAllowed=false`, `touchesWal=false`, zero operator-value counters, no signature payload, no approval grant, no sibling mutation, and no service startup.

## Tests

The new focused tests cover the command formatter, stage chain, slots, guards, profiles, and validation helper. Existing command and shard-readiness tests now verify command registration, line-editor completion, `SHARDJSON` embedding, the new status prefix, and the no-write/no-execution boundary fields.

One-sentence value: v761 gives the next Node/Java planning layer a versioned instruction preflight manifest while keeping mini-kv strictly read-only and non-authoritative.
