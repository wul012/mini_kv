# mini-kv v497 route preview archive maintenance verification

## Goal

v497 embeds verifier evidence into SHARDJSON. The version stays in the C++ mini-kv evidence plane and does not create a router, run a
route, route writes, start services, read archive files at runtime, or execute restore/load/compact.

## Implementation

- src/shard_route_preview_archive_maintenance_verification.cpp publishes the current verifier stage, source
  maintenance ledger reference, stage catalog, stage-chain report, control-plane interpretation, and boundary fields.
- src/shard_route_preview_stage_chain.cpp owns reusable release/fixture/sequence continuity checks so the verifier
  does not duplicate stage-chain logic inside tests or a giant command file.
- src/shard_readiness.cpp embeds the verifier once in SHARDJSON and includes its digest marker in the top-level
  evidence digest.
- src/command.cpp, src/command_contracts.cpp, and src/line_editor.cpp expose the read-only command,
  CHECKJSON/EXPLAINJSON metadata, help text, and client completion.

## Tests

The focused verifier test checks the exact v497 stage, the frozen source fixture, source v485 maintenance chain
completion, reusable stage-chain helper output, command usage errors, command response parity, store size preservation,
and CHECKJSON/EXPLAINJSON no-execution metadata. The broader command and shard readiness tests keep only the stable
embedding and boundary assertions, so maintenance versions can advance without spreading version-string churn through
large test files.

## Value

This version makes the v461-v485 maintenance ledger consumable as verified, versioned, read-only evidence while keeping
mini-kv independent from Node/Java runtime authority.