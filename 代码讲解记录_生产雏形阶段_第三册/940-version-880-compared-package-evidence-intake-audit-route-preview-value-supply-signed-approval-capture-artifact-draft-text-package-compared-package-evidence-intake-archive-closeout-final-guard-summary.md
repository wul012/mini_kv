# mini-kv v880 compared package evidence intake audit

## Goal

v880 publishes compared package evidence intake audit stage 20 of 20: $stage. The version freezes v879 as `fixtures/release/shard-readiness-v879.json` before rolling current evidence to v880.

The latest Node plan read for this version was `D:\nodeproj\orderops-node\docs\plans3\v1371-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-draft-text-package-compared-evidence-candidate-intake-closeout-roadmap.md`. That plan is compatible with this mini-kv step because it treats Java and mini-kv as read-only downstream context and forbids creating compared package evidence, parsing package material, accepting synthetic evidence, capturing approval grants, importing runtime payloads, starting sibling services, or mutating Java/mini-kv state.

## Boundary

This version is not a compared package parser, payload intake endpoint, approval grant capture, write router, WAL writer, restore/load/compact executor, or sibling-service launcher. The control plane should interpret `readOnly=true`, `writeRoutingAllowed=false`, `touchesWal=false`, and `executionAllowed=false` as hard boundary evidence.

## Code

The current stage count is declared in `src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.cpp`. The 20-stage catalog stays in `src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_stages.cpp`; evidence slots, intake guards, and validation are split into their own modules so the formatter remains reviewable.

`src/shard_readiness.cpp` updates `releaseVersion`, status, digest seed, and the embedded compared package evidence intake audit section. `fixtures/release/shard-readiness.json` is regenerated from the real CLI after build.

## Tests and Evidence

Focused tests cover command dispatch, SHARDJSON embedding, line editor completion, stage catalog, slots, guards, validation, and core JSON. Full CTest verifies the wider mini-kv surface. CLI and TCP smokes prove the new stage is readable through real binaries while preserving no-payload, no-accept, no-grant, no-write, and no-execution boundaries.

One-line value: v880 turns the future compared-package intake surface into versioned read-only evidence without making mini-kv an acceptance authority.