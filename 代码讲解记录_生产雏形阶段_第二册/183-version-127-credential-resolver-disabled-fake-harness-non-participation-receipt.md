# mini-kv v127 credential resolver disabled fake harness non-participation receipt

## Goal

v127 consumes the Node v288 disabled fake harness contract and turns it into a mini-kv non-participation receipt for Node v289 verification.

This is not a fake harness runtime, not a credential resolver implementation, not a new execution entry, not a secret provider, not a raw endpoint parser, not a managed audit connector, and not approval to write storage, approval ledger, schema, audit, or order state. mini-kv remains a read-only evidence provider.

## Why This Version Exists

Node v287 finished the test-only fake harness precheck, and Node v288 then defined the disabled fake harness contract. The contract is intentionally closed: it names handles, artifacts, assertions, and prohibited actions, but it still does not provide a runnable fake harness.

Before Node v289 verifies the cross-project alignment, mini-kv needs to provide its side of the evidence chain. The v127 receipt echoes the Node v288 `contractDigest`, the Node v287 precheck digest, the disabled runtime toggle, the required/prohibited counts, and the boundary flags proving mini-kv does not participate in fake harness execution.

## Module Roles

`include/minikv/runtime_evidence_receipts.hpp` adds two public functions:

```cpp
credential_resolver_disabled_fake_harness_non_participation_receipt_digest(...)
format_credential_resolver_disabled_fake_harness_non_participation_receipt_json(...)
```

`src/runtime_credential_resolver_disabled_fake_harness_receipts.cpp` owns the v127 receipt. It records:

- Node v288 profile version, route, contract state, and `contractDigest=2ebb03732323ee1f05715ec8f29843670f9131c9d212f144728fc327b4ceefb0`.
- Node v287 source profile and `precheckDigest=9cfcbdf067028c52a4465b0a21ffbaaa713270690a11638b7068e65510d391a5`.
- The disabled fake harness contract mode, runtime toggle, and invocation state.
- Six required inputs, five allowed outputs, six prohibited inputs, nine required artifacts, ten contract assertions, and fifteen prohibited actions.
- The upstream echo requirement that Java v122 and mini-kv v127 must exist before Node v289 verifies the contract.

`src/command_smoke_formatters.cpp` exposes the receipt through runtime `SMOKEJSON` and moves Node v289/v127 to the front of `diagnostics.node_consumption`, while preserving the older Node v284/v126 and earlier receipt chain.

`fixtures/release/credential-resolver-disabled-fake-harness-non-participation-receipt.json` is the standalone release fixture. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` expose the same receipt for release verification and downstream consumers.

## Boundary Fields

For the control plane, the important fields are:

- `read_only=true`: this is evidence only.
- `execution_allowed=false`: Node cannot treat the receipt as an execution window.
- `disabled_fake_harness_non_participation_receipt_only=true`: the receipt is not a fake harness implementation.
- `fake_harness_runtime_enabled=false`, `fake_harness_invocation_allowed=false`, and `execute_fake_harness_runtime_allowed=false`: no fake harness can run from mini-kv.
- `credential_value_read=false`, `credential_value_stored=false`, and `credential_value_rendered=false`: credential values remain outside mini-kv.
- `raw_endpoint_url_parsed=false` and `raw_endpoint_url_rendered=false`: endpoint handles are not expanded into raw URLs.
- `secret_provider_instantiated=false`, `resolver_client_instantiated=false`, `fake_secret_provider_instantiated=false`, and `fake_resolver_client_instantiated=false`: neither real nor fake provider clients are created.
- `external_request_sent=false` and `connects_managed_audit=false`: no managed audit or external network connection is opened.
- `approval_ledger_written=false`, `schema_migration_executed=false`, `load_restore_compact_executed=false`, and `setnxex_execution_allowed=false`: no write, schema, restore, compact, or token operation is introduced.
- `audit_authoritative=false` and `order_authoritative=false`: mini-kv does not become an authority for audit or order state.

## Tests And Smoke

`tests/credential_resolver_disabled_fake_harness_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime `SMOKEJSON`, release manifest exposure, digest alignment, Node v288/Node v287 source fields, count fields, required arrays, and the closed fake harness, credential, endpoint, provider, network, write, restore, and authority boundaries.

The existing aggregation tests now accept v127 as the newest receipt while keeping the previous v126 chain intact:

- `smokejson_command_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`

Targeted CTest passes 4/4, full CTest passes 54/54, and CLI smoke verifies `SMOKEJSON`, `CHECKJSON SMOKEJSON`, and `GET restore:real-read-token` without starting a background server or executing write/admin commands.

One-sentence summary: v127 gives Node v289 a concrete mini-kv non-participation receipt for the Node v288 disabled fake harness contract while keeping mini-kv read-only, non-executing, non-authoritative, and outside credential resolver runtime behavior.
