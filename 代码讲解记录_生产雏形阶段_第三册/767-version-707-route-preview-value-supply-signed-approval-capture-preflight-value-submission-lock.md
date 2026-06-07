# v707 route-preview-value-supply-signed-approval-capture-preflight-value-submission-lock

## Target

v707 advances the signed approval capture preflight chain to stage 22: zero value submission acceptance import lock.

The command remains SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON. This version is not a signature capture endpoint, not an approval grant, not value submission, not evidence import, and not a router/write/execution path.

## Source Evidence

The stage freezes v706 as ixtures/release/shard-readiness-v706.json before rolling the current fixture to v707. The source signed approval template remains v685, and the capture preflight chain only consumes it as read-only versioned evidence.

## Implementation Notes

src/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.cpp advances current_capture_preflight_stage_count to 22. The stage catalog, inputs, policies, attestations, and validation helpers keep the published-stage flags coherent without introducing runtime state.

src/shard_readiness.cpp rolls eleaseVersion, status, and the evidence digest seed to v707. The old signed-template section remains embedded as source evidence, while the new capture-preflight section records the current stage.

## Boundary Meaning

Control-plane consumers should read this as a preflight evidence increment only. signedApprovalPresent=false, signatureCaptured=false, pprovalGrantEmitted=false, all operator value counts remain zero, raw material storage is blocked, sibling mutation is blocked, and execution remains disallowed.

## Verification

The version archive includes CMake configure/build, focused capture-preflight tests, full CTest, CLI smoke, and TCP smoke. TCP verifies the command catalog and EXPLAIN/CHECK read-only contract without requesting the oversized SHARDJSON payload.

One sentence: v707 strengthens the capture preflight checklist around zero value submission acceptance import lock while keeping mini-kv a read-only evidence provider rather than an approval or execution system.