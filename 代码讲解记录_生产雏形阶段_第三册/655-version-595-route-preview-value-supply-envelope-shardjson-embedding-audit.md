# mini-kv v595 route-preview-value-supply-envelope-shardjson-embedding-audit

## Target

This version advances the mini-kv side of the Node v936 follow-up direction: a disabled-by-default operator value-supply envelope. It is not a value adapter, not an import path, not a router, not a write path, and not a runtime execution entry. The control plane can read the envelope to understand which value-supply policy slice is published, but it cannot use mini-kv to submit, normalize, persist, or execute operator values.

## Source Evidence

The envelope uses SHARDROUTEVALUEDRAFTJSON v585 as its frozen source. The source draft proves the operator value slots exist and that actual values are still absent. This version adds the next layer: fail-closed missing-value policy, redact-before-persist policy, and explicit source evidence provenance requirements. These are policy labels only; no runtime value is accepted.

## Files

src/shard_route_preview_operator_value_supply_envelope.cpp owns the public JSON artifact and the stage chain for v586-v610. src/shard_route_preview_operator_value_supply_envelope_slots.cpp owns per-slot policy mapping so the main artifact does not become a giant string table. src/shard_readiness.cpp embeds exactly one shardRoutePreviewOperatorValueSupplyEnvelope section and rolls eleaseVersion to v595. Command metadata is exposed through SHARDROUTEVALUESUPPLYJSON, COMMANDS, COMMANDSJSON, EXPLAINJSON, and CHECKJSON.

## Contract

The important fields are operatorValueEnvelopeState=disabled-by-default, missingValuePolicy=fail-closed, edactionPolicy=redact-before-persist, eadyForOperatorValueSupply=false, utomaticSiblingImportAllowed=false, untimePayloadAccepted=false, writeRoutingAllowed=false, and executionAllowed=false. A downstream reader should treat the envelope as a readiness and policy document, not as an input surface.

## Tests And Evidence

Focused tests cover the slot helper and the envelope artifact. Command and shard readiness tests prove the command catalog, metadata contract, SHARDJSON embedding, and fixture parity. Full CTest is run for this version, followed by CLI and TCP smoke against SHARDROUTEVALUESUPPLYJSON; the TCP process is stopped after capture.

## Maintenance

This version keeps the new evidence chain split at the helper boundary instead of growing the existing value-draft file. The stage list remains versioned, while slot details stay in a dedicated helper that can be audited or refactored independently.

One sentence: v595 gives Node and Java a readable disabled value-supply policy layer while mini-kv remains a read-only evidence producer with no authority to import, write, route, or execute.