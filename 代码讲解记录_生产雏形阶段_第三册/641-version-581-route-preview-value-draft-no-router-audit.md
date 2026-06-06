# mini-kv v581 route-preview-value-draft-no-router-audit

## Goal

v581 advances the read-only SHARDROUTEVALUEDRAFTJSON operator evidence value draft chain: audits that value drafts install no active shard router. It consumes the frozen v560 SHARDROUTEIMPORTPREFLIGHTJSON package as source evidence, not a live Node, Java, or mini-kv runtime.

This is not a new execution entry, not a value importer, not write routing, not an active shard router, and not a shortcut to restore/load/compact/service execution. Control-plane consumers should read it as versioned placeholder evidence only: the actual operator value is still absent.

## Code

src/shard_route_preview_operator_value_draft.cpp owns the top-level evidence: current stage, source v560 fixture, stage chain, blocked capabilities, control-plane interpretation, and boundary fields such as readyForOperatorEvidenceValueDraft=true and readyForEvidenceImport=false.

src/shard_route_preview_operator_value_draft_slots.cpp owns the 25 draft slot templates. Each slot remains actualValueState=not-supplied, operatorValueSupplied=false, operatorValueAccepted=false, and operatorValueImported=false, so the slot catalog cannot be confused with accepted data.

src/command.cpp, src/command_contracts.cpp, and src/line_editor.cpp expose the command as read-only metadata. The command never enters the write path and never touches the WAL gate.

## Tests

The slot helper test protects the 25-slot template catalog. The value draft command test protects command output, SHARDJSON embedding, COMMANDS/COMMANDSJSON catalog entries, and EXPLAIN/CHECK metadata contracts. Import preflight tests prove the old source section remains present. Full CTest covers the older WAL, TCP, restore, and credential resolver boundaries.

## Value

v581 moves the operator evidence value draft one version forward while keeping real values, import, writes, routing, and execution outside mini-kv.
