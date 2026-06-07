# mini-kv v657 route-preview-value-supply-approval-template-no-write-audit

## Version Goal

v657 advances SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON to route-preview-value-supply-approval-template-no-write-audit. It is a controlled, disabled, read-only approval packet template for future value-supply review. It is not a signed approval capture path, not an operator value importer, not a router, not a write path, not a runtime payload receiver, and not an execution gate.

## Source Evidence

The source is the frozen v635 SHARDROUTEVALUESUPPLYPRECHECKJSON package at fixtures/release/shard-readiness-v635.json. This version also freezes v656 before rolling fixtures/release/shard-readiness.json, so consumers can compare stable historical evidence with the current approval-template view.

## Module Responsibilities

src/shard_route_preview_operator_value_supply_approval_template.cpp owns the release stage, source precheck contract, digest marker, and public JSON response. src/shard_route_preview_operator_value_supply_approval_template_fields.cpp keeps the draft approval fields separate from the main artifact. src/shard_route_preview_operator_value_supply_approval_template_slots.cpp maps each approval-template slot back to the v611-v635 precheck slot chain. This split keeps the value-supply approval surface maintainable instead of creating one oversized evidence file.

The aggregate src/shard_readiness.cpp embeds exactly one shardRoutePreviewOperatorValueSupplyApprovalTemplate section and includes the template digest in the shard readiness digest source. Command handling exposes the artifact through SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON, with command catalog, help, EXPLAINJSON, CHECKJSON, and line-editor completion all remaining metadata-only surfaces.

## Boundary Fields

Control-plane readers should interpret approvalTemplateOnly=true and operatorApprovalPacketTemplateDeclared=true as field-shape evidence only. approvalPacketPresent=false, approvalGrantPresent=false, operatorIdentityPresent=false, approvalTimestampPresent=false, approvalCorrelationIdPresent=false, redactedValueDigestSupplied=false, typedValueEnvelopeSupplied=false, and reviewRecordPresent=false mean no operator value can be approved or accepted. missingValueRejected=true and malformedValueRejected=true document fail-closed policy, while automaticSiblingImportAllowed=false, runtimePayloadAccepted=false, writeRoutingEnabled=false, walTouched=false, and executionAllowed=false keep mini-kv out of live routing and execution.

## Tests And Smoke

The approval-template focused tests protect field catalog, slot mapping, command routing, SHARDJSON embedding, and read-only boundaries. command_tests, shard_readiness_tests, line_editor_tests, and the upstream envelope/precheck tests protect adjacent surfaces. The version also runs full CTest plus CLI and TCP smoke for the approval-template command, proving the response is available through real command surfaces without mutating the store or touching WAL.

## Maintenance Value

v657 gives Node and Java a clearer approval-packet draft shape while preserving the mini-kv boundary: versioned evidence, no real approval, no value ingest, no sibling import, no router, no write path, and no execution authority.