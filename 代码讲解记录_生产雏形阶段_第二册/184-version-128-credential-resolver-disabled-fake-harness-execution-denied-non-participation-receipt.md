# mini-kv v128 credential resolver disabled fake harness execution-denied non-participation receipt

## Goal

v128 consumes the Node v290 disabled fake harness execution-denied route preflight and turns it into a mini-kv non-participation receipt for Node v291 verification.

This is not a fake harness runtime, not a credential resolver implementation, not a provider/client shell, not a credential reader, not a raw endpoint parser, not an HTTP/TCP connector, not an approval ledger writer, not a schema migration runner, and not a new LOAD/COMPACT/RESTORE/SETNXEX execution path. mini-kv remains a read-only evidence provider.

## Why This Version Exists

Node v289 verified that the disabled fake harness contract was aligned across Node, Java, and mini-kv. Node v290 then added a route preflight that explicitly denies execution: the route is registered and read-only, but every dangerous action remains blocked.

Before Node v291 can verify the next cross-project state, mini-kv needs to echo that denial from its side. The v128 receipt freezes the Node v290 `preflightDigest`, the source Node v289 verification digest, the 25/25 check result, the 8 simulated route attempts, the 8 denial reasons, and the fact that actual execution attempts stay at 0.

## Module Roles

`include/minikv/runtime_evidence_receipts.hpp` adds the public v128 receipt API:

```cpp
credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_digest(...)
format_credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_json(...)
```

`src/runtime_credential_resolver_execution_denied_receipts.cpp` owns the v128 receipt. It records:

- Node v290 profile version, route path, route method, format modes, and `preflightDigest=21ea6d7a6c641489048fc2540126e2f461909246fdd2129bcb0fc6d1ffbbf1cb`.
- Node v289 source profile and `verificationDigest=e5b596b65f68e753459be163f8a58d1f677098e64b3be01d4e83737ef8c4d94f`.
- Eight simulated denied attempts: approval gate, credential value, raw endpoint URL, provider/client instantiation, HTTP/TCP dial, ledger/schema write, fake harness runtime, and automatic upstream start.
- Summary counts: 25 checks, 25 passed checks, 8 simulated attempts, 8 denied attempts, 0 actual execution attempts, and 0 production blockers.
- The downstream hint that Node v291 may verify this receipt after the Java quality queue state is known.

`src/command_smoke_formatters.cpp` exposes the receipt through runtime `SMOKEJSON`, adds the receipt name to the diagnostics notes, and moves the Node v291/v128 sentence to the front of `diagnostics.node_consumption` while preserving the older Node v289/v127 chain.

`fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-non-participation-receipt.json` is the standalone v128 fixture. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` expose the same receipt to release and runtime verification.

## Boundary Fields

For the control plane, the important fields are:

- `read_only=true`: this is evidence only.
- `execution_allowed=false`: the receipt is not an execution window.
- `execution_denied_non_participation_receipt_only=true`: mini-kv is echoing denial, not implementing a runtime.
- `route_execution_denied=true` with `actual_execution_attempt_count=0`: Node v290 denial is preserved without a real attempt.
- `fake_harness_runtime_enabled=false`, `fake_harness_invocation_allowed=false`, and `execute_fake_harness_runtime_allowed=false`: no fake harness can run from mini-kv.
- `credential_value_read_allowed=false` and `credential_value_read=false`: credential values remain outside mini-kv.
- `raw_endpoint_url_parse_allowed=false` and `raw_endpoint_url_parsed=false`: raw endpoints are not expanded.
- `provider_client_instantiation_allowed=false`, `secret_provider_instantiated=false`, `resolver_client_instantiated=false`, `fake_secret_provider_instantiated=false`, and `fake_resolver_client_instantiated=false`: neither real nor fake providers/clients are created.
- `http_tcp_dial_allowed=false`, `external_request_sent=false`, and `connects_managed_audit=false`: no external connection is opened.
- `storage_write_allowed=false`, `approval_ledger_written=false`, `schema_migration_executed=false`, `load_restore_compact_executed=false`, and `setnxex_execution_allowed=false`: no storage, ledger, schema, restore, compact, or token execution path is introduced.
- `audit_authoritative=false` and `order_authoritative=false`: mini-kv does not become an authority for audit or order state.

## Tests And Smoke

`tests/credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime `SMOKEJSON`, release manifest exposure, digest alignment, Node v290 preflight fields, Node v289 source fields, summary counts, denial reasons, simulated attempts, and every closed execution boundary.

Existing aggregation tests were extended so the new receipt appears in the live runtime and release fixtures:

- `smokejson_command_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`

The v127 disabled fake harness test was also adjusted to assert that the historical Node v289/v127 diagnostic remains present without requiring it to stay as the first `node_consumption` sentence after v128 moved Node v291 to the front.

Targeted CTest passes 5/5, full CTest passes 55/55, and CLI smoke verifies `SMOKEJSON`, `CHECKJSON SMOKEJSON`, and `GET restore:real-read-token` without starting a background server or executing write/admin commands.

One-sentence summary: v128 gives Node v291 a concrete mini-kv execution-denied receipt for the Node v290 route preflight while keeping mini-kv read-only, non-executing, non-authoritative, and outside fake harness or credential resolver runtime behavior.
