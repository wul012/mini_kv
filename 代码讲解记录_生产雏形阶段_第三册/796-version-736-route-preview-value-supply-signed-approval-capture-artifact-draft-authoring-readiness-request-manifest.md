# mini-kv v736 draft authoring readiness request manifest

v736 starts a new read-only evidence chain after the v711-v735 signed approval capture artifact preflight closeout. Its job is to say whether mini-kv has enough versioned evidence to let a human or sibling control plane prepare for a future signed approval artifact draft. It is not the draft itself, not signed text, not detached signature material, not an approval grant, not value import, not a runtime payload, and not a router or write-route activation.

## Role

- Command: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON
- Source plan: docs/plans3/v1186-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-closeout-roadmap.md
- Source fixture: fixtures/release/shard-readiness-v735.json
- Stage: route-preview-value-supply-signed-approval-capture-artifact-draft-authoring-readiness-request-manifest

## Module Responsibilities

The core implementation lives in src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness.cpp. It assembles the final JSON, embeds the current stage-chain report, references the frozen v735 artifact preflight digest, and publishes the hard boundary fields that consumers should treat as authoritative for this evidence surface.

The stage catalog lives in src/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages.cpp. It defines the full v736-v760 route, including each release version, stage slug, previous frozen fixture, and short maintenance scope. Keeping this table out of the core prevents the main JSON formatter from becoming a large unmaintainable file as the chain grows.

The requirements, blockers, and profiles helpers split the authoring-readiness vocabulary into three independent catalogs. Requirements say what a future draft package would need to cite; blockers say why mini-kv still refuses to produce or accept draft content; profiles describe how a control plane can classify each readiness step without importing values or signatures.

The validation helper compares the catalog counts, source artifact preflight completion, and published stage count. It gives consumers a compact summary without doing filesystem reads, archive walks, service starts, or sibling project inspection.

## Boundary Reading

Control-plane consumers should read authoringReadinessOnly=true and signedApprovalCaptureArtifactDraftAuthoringReadinessOnly=true as the main mode switch. The evidence declares readiness metadata only. signedApprovalArtifactDraftPresent=false, signedApprovalArtifactDraftMaterialized=false, signedApprovalArtifactDraftTextGenerated=false, approvalStatementTextGenerated=false, detachedSignaturePayloadPresent=false, signatureCaptured=false, approvalGrantPresent=false, and approvalGrantEmitted=false mean no approval artifact has been authored, accepted, signed, or granted.

The value boundary stays closed through submittedOperatorValueCount=0, acceptedOperatorValueCount=0, importedEvidenceValueCount=0, persistedOperatorValueCount=0, runtimePayloadAccepted=false, automaticSiblingImportAllowed=false, secretValueStored=false, credentialValueStored=false, rawEndpointStored=false, rawSignatureMaterialStored=false, and draftTextStored=false. The runtime boundary stays closed through activeRouterInstalled=false, routerActivationAllowed=false, writeRoutingAllowed=false, writeCommandsAllowed=false, mutatesStore=false, touchesWal=false, startsServices=false, liveReadAllowed=false, filesystemReadPerformed=false, siblingMutationAllowed=false, readOnly=true, and executionAllowed=false.

## Tests and Evidence

The new linked tests cover the core command JSON, the 25-stage catalog, requirements, blockers, profiles, and validation summary. Existing command, SHARDJSON, and line-editor tests were extended from 52 to 53 commands so COMMANDS, COMMANDSJSON, HELP, EXPLAINJSON, CHECKJSON, completion, and embedded shard readiness evidence all see the new command consistently.

The v736 fixture sequence freezes fixtures/release/shard-readiness-v735.json before rolling fixtures/release/shard-readiness.json to v736. Focused CTest proves the new command and adjacent artifact-preflight inheritance remain stable. Full CTest and CLI/TCP smoke prove the same read-only/no-router/no-write/no-WAL/no-execution behavior through the local executable and socket surfaces.

One-line value: v736 opens the signed approval artifact draft authoring readiness chain while keeping mini-kv as a versioned read-only evidence publisher rather than an approval, signature, value, routing, or runtime execution authority.
