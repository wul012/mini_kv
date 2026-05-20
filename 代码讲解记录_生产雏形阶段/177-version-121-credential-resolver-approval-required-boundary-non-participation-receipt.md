# 177. version 121 credential resolver approval-required boundary non-participation receipt

## Version Goal

v121 only adds a mini-kv read-only non-participation receipt for Node v274 `credential resolver disabled candidate upstream echo verification`. Its role is to let Node v275 verify that mini-kv understands the six approval-required boundaries, while mini-kv still does not implement a resolver, instantiate a resolver client or secret provider, read credential values, parse raw endpoint URLs, send external requests, write storage or approval ledger records, execute schema migration, or execute `LOAD` / `COMPACT` / `RESTORE` / `SETNXEX`.

This is not a new execution entry point, not a resolver stub, not a secret provider runtime, not a managed audit storage backend, not an approval ledger writer, and not an order authority path.

## Entry Points And File Roles

`src/runtime_credential_resolver_approval_boundary_receipts.cpp` is the new implementation file. It carries the stable Node v274 evidence shape: source profile version, upstream route path, `verification_state=credential-resolver-disabled-candidate-upstream-echo-verification-ready`, check count 25, candidate decision count 10, candidate-ready count 4, approval-required count 6, request/response/failure counts, candidate digest, and the previous mini-kv v120 receipt digest.

`include/minikv/runtime_evidence_receipts.hpp` exposes only two public functions: one digest function and one JSON formatter. That keeps the runtime formatter able to attach the new receipt without growing the older v120 disabled-candidate file again.

`src/command_response_formatters.cpp` adds `SMOKEJSON.credential_resolver_approval_required_boundary_non_participation_receipt` and moves the Node v275 consumption hint to the front of diagnostics. This remains read-only JSON aggregation; it does not execute commands.

## Boundary Fields

`read_only=true` and `execution_allowed=false` are the top-level control-plane signals: the receipt can be read by Node, but it never authorizes runtime action.

`approval_required_boundary_non_participation_receipt_only=true` and `approval_required_boundary_refinement_only=true` explain the narrow scope. v121 refines the interpretation of the six approval-required boundaries from Node v274; it does not turn any of them into mini-kv behavior.

The six boundary entries are explicit:

- `CREDENTIAL_HANDLE`: handle evidence only, no credential value read/store/render.
- `ENDPOINT_HANDLE`: handle evidence only, no raw endpoint parsing/rendering/connection.
- `OPERATOR_APPROVAL`: marker evidence only, no auto approval or upstream auto-start.
- `ROLLBACK_BOUNDARY`: marker evidence only, no rollback or production deployment.
- `SCHEMA_MIGRATION_POLICY`: policy evidence only, no SQL or schema migration.
- `AUDIT_LEDGER_WRITE_POLICY`: policy evidence only, no approval ledger, managed audit state, or storage write.

The hard false flags are the part downstream control planes should trust first: `real_resolver_implementation_allowed=false`, `resolver_client_instantiated=false`, `secret_provider_instantiated=false`, `credential_value_read_allowed=false`, `raw_endpoint_url_parse_allowed=false`, `external_request_allowed=false`, `approval_ledger_write_allowed=false`, `schema_migration_allowed=false`, `load_restore_compact_executed=false`, and `order_authoritative=false`.

## Fixtures And Manifest

`fixtures/release/credential-resolver-approval-required-boundary-non-participation-receipt.json` is the standalone receipt fixture. It includes the Node v274 source verification object, the six boundary details, summary counts, warnings, recommendations, digest fields, and pause diagnostics.

`fixtures/release/runtime-smoke-evidence.json` exposes the same receipt alongside the existing runtime smoke envelope, so Node can read the v121 evidence through the normal SMOKEJSON chain without losing older v102/v120 evidence anchors.

`fixtures/release/verification-manifest.json` registers the new target test, the new fixture path, and the same receipt object so release verification can compare the standalone fixture, runtime smoke fixture, and manifest view.

## Test Coverage

`tests/credential_resolver_approval_required_boundary_non_participation_receipt_tests.cpp` checks four layers:

- The standalone receipt fixture must contain the v121 release fields, v120 predecessor digest, Node v274 source shape, all six approval-required boundary details, summary counts, digest, and no-side-effect flags.
- The runtime smoke fixture must expose the same receipt and the Node v275 diagnostics hint.
- The verification manifest must register the test, fixture, digest, and receipt fields.
- A real `CommandProcessor` `SMOKEJSON` call must return the same receipt, and `GET restore:real-read-token` must still return `(nil)`.

The broader `runtime_smoke_evidence_tests` and `release_verification_manifest_tests` now also assert the v121 key, source state, six boundary codes, and Node v275 consumption hint.

## One-Sentence Summary

v121 gives Node v275 a concrete read-only mini-kv receipt for the six approval-required credential resolver boundaries while keeping mini-kv outside resolver execution, secrets, endpoints, schema migration, audit ledger writes, storage writes, restore, and order authority.
