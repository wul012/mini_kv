# mini-kv v802 route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-value-shape-criterion

v802 advances the draft text package review preflight chain to sequence 17. It freezes `fixtures/release/shard-readiness-v801.json` as the source for this step and keeps the rolling SHARDJSON evidence on the disabled draft text package review surface.

This version is not a draft text package parser, not signed approval capture, not detached signature verification, not operator value import, not active routing, and not runtime execution. It only adds another review criterion or rejection control to the read-only package review preflight chain.

## What Changed

The core formatter updates `current_draft_text_package_review_preflight_stage_count` to 17. The stage catalog, review criteria, rejection controls, and review profiles now publish the first 17 entries, while `shard_readiness.cpp` rolls `releaseVersion` and status to `route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-value-shape-criterion-read-only`.

The frozen source fixture for this version is `fixtures/release/shard-readiness-v801.json`. The rolling fixture is regenerated from `SHARDJSON` and carries `releaseVersion=v802`.

## Boundary Fields

The control-plane meaning stays narrow. `draftTextPackageReviewPreflightOnly=true` says this is a checklist, not package content. `draftTextPackagePresent=false`, `draftTextPackageParsed=false`, and `draftTextPackageAccepted=false` prove mini-kv does not accept draft text packages. `approvalGrantEmitted=false` and `readyForSignedApprovalCapture=false` keep approval capture closed.

The storage and runtime boundaries remain explicit: `submittedOperatorValueCount=0`, `importedEvidenceValueCount=0`, `writeRoutingAllowed=false`, `touchesWal=false`, `activeRouterInstalled=false`, `siblingMutationAllowed=false`, and `executionAllowed=false`.

## Tests

Focused CTest covers the text package review preflight module, command dispatch, line-editor completion, SHARDJSON embedding, instruction-preflight adjacency, and artifact/authoring compatibility. Full CTest verifies the change does not regress WAL, TCP, restore, runtime receipt, fixture guard, or command catalog behavior. CLI and TCP smoke both read the new command as metadata only.

One-sentence value: v802 adds a versioned review-preflight checkpoint for downstream package review while keeping mini-kv strictly read-only and non-authoritative.
