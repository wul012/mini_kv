# mini-kv v623 route-preview-value-supply-precheck-provenance-policy

## Version Goal

v623 advances SHARDROUTEVALUESUPPLYPRECHECKJSON to route-preview-value-supply-precheck-provenance-policy. It is a disabled, read-only operator value-supply approval precheck. It is not a router, not a write path, not a runtime payload receiver, not an approval packet, and not an execution gate.

## Source Evidence

The source remains the frozen v610 SHARDROUTEVALUESUPPLYJSON envelope at fixtures/release/shard-readiness-v610.json. The current version also freezes v622 before rolling fixtures/release/shard-readiness.json, so downstream consumers can compare a stable previous release with the new current evidence.

## Module Responsibilities

src/shard_route_preview_operator_value_supply_precheck.cpp owns the release stage, source envelope contract, digest marker, and public JSON response. src/shard_route_preview_operator_value_supply_precheck_slots.cpp maps each approval-precheck slot back to the source envelope slot. src/shard_route_preview_operator_value_supply_precheck_requirements.cpp keeps withheld approval requirements separate from the main artifact, which prevents this layer from becoming a large mixed-purpose file.

The aggregate src/shard_readiness.cpp embeds exactly one shardRoutePreviewOperatorValueSupplyPrecheck section and includes the precheck digest in the shard readiness digest source. Command handling exposes the artifact through SHARDROUTEVALUESUPPLYPRECHECKJSON, with command catalog, help, EXPLAINJSON, CHECKJSON, and line-editor completion updated as metadata-only surfaces.

## Boundary Fields

Control-plane readers should interpret approvalPacketRequired=true with approvalPacketPresent=false as a hard stop, not as partial approval. operatorIdentityPresent=false, approvalTimestampPresent=false, and perValueProvenanceSupplied=false mean no operator value can be accepted. missingValueRejected=true and malformedValueRejected=true document rejection policy, while automaticSiblingImportAllowed=false, runtimePayloadAccepted=false, writeRoutingAllowed=false, and executionAllowed=false keep mini-kv out of live routing and execution.

## Tests And Smoke

The focused precheck, slot, and requirement tests protect the new contract fields. command_tests, shard_readiness_tests, and line_editor_tests protect command registration, SHARDJSON embedding, catalog metadata, and completion behavior. The version also runs full CTest plus CLI and TCP smoke for the precheck command, proving the response is available through real command surfaces without mutating the store or touching WAL.

## Maintenance Value

v623 keeps the value-supply approval boundary explicit while splitting growing requirement and slot metadata into maintainable helpers. That gives Node and Java a clearer read-only evidence surface without asking mini-kv to become an order authority or runtime executor.