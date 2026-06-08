# mini-kv v840 draft text package comparison closeout audit

## Goal

v840 publishes comparison closeout audit stage 5 of 25: $stage. The version freezes v839 as `fixtures/release/shard-readiness-v839.json` before rolling current evidence to v840.

The latest Node plan read for this version was `D:\nodeproj\orderops-node\docs\plans3\v1321-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-acceptance-precheck-closeout-roadmap.md`. That plan is compatible with this mini-kv step because it remains read-only downstream context and still forbids draft text parsing, detached signature payload parsing, approval grants, runtime payload import, sibling service starts, and Java/mini-kv mutation.

## Boundary

This version is not a package parser, acceptance gate, approval grant emitter, write router, WAL writer, restore/load/compact executor, or sibling-service launcher. The control plane should interpret `readOnly=true`, `writeRoutingAllowed=false`, `touchesWal=false`, and `executionAllowed=false` as hard boundary evidence.

## Code

The current stage count is declared in `src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit.cpp`. The 25-stage catalog remains in `src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_stages.cpp`; checks, blockers, profiles, and validation stay split into their own modules so the core formatter does not become a giant maintenance sink.

`src/shard_readiness.cpp` updates `releaseVersion`, status, digest seed, and the embedded comparison closeout audit section. `fixtures/release/shard-readiness.json` is regenerated from the real CLI after build.

## Tests and Evidence

Focused tests cover command dispatch, SHARDJSON embedding, line editor completion, and the comparison audit stages/checks/blockers/profiles/validation. Full CTest verifies the wider mini-kv surface. CLI and TCP smokes prove the new stage is readable through real binaries while preserving no-parse, no-accept, no-grant, no-write, and no-execution boundaries.

One-line value: v840 extends the comparison closeout audit chain without turning mini-kv into an acceptance authority or runtime executor.