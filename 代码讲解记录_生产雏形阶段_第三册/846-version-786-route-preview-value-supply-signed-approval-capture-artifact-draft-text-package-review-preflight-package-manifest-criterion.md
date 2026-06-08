# mini-kv v786 route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-package-manifest-criterion

v786 starts a new read-only draft text package review preflight chain. It freezes `fixtures/release/shard-readiness-v785.json` as the source proof that the draft-instruction-preflight chain closed cleanly, then publishes the first review criterion for the next handoff surface: package manifest review.

This version is not a draft text package parser, not signed approval capture, not a detached signature verifier, not an operator value importer, not an active shard router, and not runtime execution. It only tells downstream control-plane readers which package-level checks must be reviewed before a later approval chain can even be considered.

## What Changed

The new module family is intentionally split into small files instead of one large formatter. `shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.cpp` owns the top-level JSON object and digest. The companion `stages`, `slots`, `guards`, `profiles`, and `validation` files publish the stage catalog, criterion slots, rejection controls, review profiles, and validation summary as separate units.

`command.cpp`, `command_contracts.cpp`, and `line_editor.cpp` register `SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON` as a metadata-only command. `shard_readiness.cpp` embeds that JSON object exactly once in SHARDJSON, rolls `releaseVersion` to `v786`, and moves the rolling status to `route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-package-manifest-criterion-read-only`.

The current fixture is regenerated from the CLI output. The frozen source fixture, `fixtures/release/shard-readiness-v785.json`, remains the immutable predecessor for this chain.

## Boundary Fields

The fields with operational meaning are the denials. `draftTextPackageReviewPreflightOnly=true` means the evidence is a review checklist, not the package itself. `draftTextPackagePresent=false`, `draftTextPackageParsed=false`, and `draftTextPackageAccepted=false` close the door on accepting draft text as data. `submittedOperatorValueCount=0` and `importedEvidenceValueCount=0` keep operator values out of mini-kv.

`readyForDraftTextPackageReview=true` is deliberately narrow: a downstream reader may review the criteria. It does not imply `readyForSignedApprovalCapture`, and `approvalGrantEmitted=false` proves this command does not grant approval. The storage boundary remains unchanged through `writeRoutingAllowed=false`, `touchesWal=false`, `activeRouterInstalled=false`, and `executionAllowed=false`.

## Tests

The new tests cover stage continuity, criterion slot publication, guard/rejection fields, profile metadata, validation summaries, command dispatch, line-editor completion, and SHARDJSON embedding. Existing artifact preflight and draft authoring readiness tests were also adjusted so they assert the current rolling status while still proving the historical instruction preflight object remains present.

Focused CTest exercises the new module and neighboring compatibility surfaces. Full CTest proves the change does not regress WAL, TCP, restore, runtime receipt, fixture guard, or command catalog behavior. CLI and TCP smoke both read the new command as evidence only, with no package parsing, no write route, and no execution.

One-sentence value: v786 turns the closed instruction-preflight chain into a versioned source for draft text package review, while keeping mini-kv a read-only evidence publisher rather than an approval or execution system.
