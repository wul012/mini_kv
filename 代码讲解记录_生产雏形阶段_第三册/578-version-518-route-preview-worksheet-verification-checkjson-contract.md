# mini-kv v518 checkjson contract

## Goal

v518 pins CHECKJSON no-execution semantics for the worksheet verifier command. This stays in the mini-kv C++ evidence plane: it is not a worksheet importer, not an execution entry point, not an active shard router, not a write-routing change, and not Java order authority. The source plan is Node v861, but mini-kv only records a read-only boundary echo from frozen v517 evidence.

## Implementation

- src/shard_route_preview_worksheet_verification.cpp publishes the current worksheet verification stage, source v510 maintenance verifier package, stage catalog, shared stage-chain report, worksheet readiness flags, and no-go boundary fields.
- src/shard_readiness.cpp embeds shardRoutePreviewWorksheetVerification once in SHARDJSON and adds the worksheet verifier digest marker to the top-level evidence digest.
- src/command.cpp, src/command_contracts.cpp, and src/line_editor.cpp keep the real command, command catalog, CHECKJSON/EXPLAINJSON metadata, help text, and client completion aligned.
- fixtures/release/shard-readiness-v517.json freezes the previous rolling fixture before v518 updates fixtures/release/shard-readiness.json.

## Tests

The focused worksheet verifier test checks the source verifier contract, v486-v510 source range, operator worksheet slot count, readyForOperatorEntryWorksheet=true, readyForManualEvidenceEntry=false, and the no manual import/no runtime payload/no synthetic evidence boundaries. The broader command, shard readiness, archive verifier, and line-editor tests prove command exposure, SHARDJSON embedding, fixture parity, and metadata-only CHECKJSON/EXPLAINJSON behavior. Full CTest then verifies the rest of WAL, TCP, snapshot, recovery, runtime evidence, and historical fixture contracts were not disturbed.

## Smoke And Archive

The archive under e/518/ keeps build, focused CTest, full CTest, CLI worksheet verifier, CLI SHARDJSON, and owned TCP smoke screenshots. The TCP smoke starts only the local mini-kv server needed for this check and stops it in the same version step. The command screenshots were rendered from captured logs because tool_search did not expose an additional Playwright screenshot tool for batch command archive generation in this run.

## Value

v518 makes the Node v861 worksheet direction easier to review downstream while preserving the important boundary: mini-kv can publish versioned evidence, but it cannot accept worksheet values, install routing, write data, or execute runtime operations.