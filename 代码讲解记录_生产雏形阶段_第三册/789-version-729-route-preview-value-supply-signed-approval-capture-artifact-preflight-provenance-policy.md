# mini-kv v729 provenance policy fragment

This version advances the read-only signed approval capture artifact preflight chain. It is not a signed approval artifact draft, not a grant, not a value import path, not a runtime payload, and not a router or write-route activation.

## Role

- Command: $command
- Source plan: docs/plans3/v1086-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-preflight-closeout-roadmap.md
- Source fixture: ixtures/release/shard-readiness-v728.json
- Stage: $slug

## Implementation Notes

The artifact preflight implementation is split into core, fragments, seals, gates, and validation helpers. The core file owns the stage chain and final JSON assembly. The helper files own stable catalog rows so future maintenance can adjust fragment/seal/gate evidence without expanding shard_readiness.cpp or command.cpp.

## Boundary Reading

Control-plane consumers should read this as preflight-only evidence. eadOnly=true, executionAllowed=false, rtifactMaterializationAllowed=false, pprovalGrantAllowed=false, alueImportAllowed=false, writeRouteAllowed=false, and siblingMutationAllowed=false mean mini-kv is only publishing review evidence. It does not read credential values, raw endpoints, raw signatures, operator value payloads, or sibling project state.

## Tests and Evidence

Focused artifact preflight tests cover the command JSON, fragment catalog, seal catalog, gate catalog, validation summary, command catalog, line editor completion, and SHARDJSON embedding. Full CTest and CLI/TCP smoke prove the command remains read-only through local and socket execution surfaces.

One-line value: v729 moves the artifact preflight chain forward while keeping Node, Java, and mini-kv loosely coupled through versioned, read-only evidence.