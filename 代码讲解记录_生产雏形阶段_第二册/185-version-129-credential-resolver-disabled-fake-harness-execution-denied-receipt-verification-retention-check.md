# mini-kv v129 credential resolver disabled fake harness execution-denied receipt verification retention check

## Goal

v129 does not add a new execution path. It consumes the Node v292 blocked fake-harness readiness decision, the Node v291 execution-denied upstream echo, and the mini-kv v128 execution-denied receipt, then proves that the v128 receipt stays available, digest-stable, and visible through the runtime and release manifests for Node v293 review.

This is still not a fake harness runtime, not a credential resolver implementation, not a provider/client shell, not a credential reader, not a raw endpoint parser, not an HTTP/TCP connector, not an approval ledger writer, and not a LOAD/COMPACT/RESTORE/SETNXEX path. mini-kv remains a read-only evidence provider.

## Why This Version Exists

The earlier v128 receipt already froze the Node v290 execution-denied route preflight. v129 is the follow-on retention check: it asks whether the v128 receipt can survive as historical evidence while Node v292 says the fake-harness readiness decision is still blocked and Node v293 is the next verification consumer.

The value here is not in doing more. It is in keeping the evidence chain stable while explicitly proving that the receipt is retention-only and still outside runtime execution.

## Module Roles

`include/minikv/runtime_evidence_receipts.hpp` adds the public v129 receipt API:

```cpp
credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_digest(...)
format_credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_json(...)
```

`src/runtime_credential_resolver_execution_denied_retention_receipts.cpp` owns the v129 retention check. It records:

- Node v292 blocked readiness decision record.
- Node v291 execution-denied upstream echo.
- mini-kv v128 execution-denied receipt as the retained source receipt.
- Node v293 as the next consumer.
- The retention-check digest `fnv1a64:9d433e000a555c11`.
- The read-only boundary that keeps fake harness execution, credential reads, raw endpoint parsing, external requests, storage writes, approval-ledger writes, schema migration, restore/load/compact/SETNXEX, and authority claims out of mini-kv.

`src/command_smoke_formatters.cpp` exposes the receipt through runtime `SMOKEJSON`, extends `diagnostics.node_consumption` with the new Node v293 sentence, and keeps the older Node v291/v128 history visible.

`fixtures/release/credential-resolver-disabled-fake-harness-execution-denied-receipt-verification-retention-check.json` is the standalone v129 fixture. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` expose the same receipt to runtime and release verification.

## Boundary Fields

For the control plane, the important fields are:

- `read_only=true`: this is evidence only.
- `execution_allowed=false`: the receipt is not an execution window.
- `receipt_verification_retention_check_only=true` and `retention_check_only=true`: this is retention verification, not runtime behavior.
- `source_node_v292_reference.readiness_decision=blocked`: Node v292 remains blocked.
- `source_node_v291_reference.verification_state=blocked`: Node v291 remains blocked upstream echo evidence.
- `source_mini_kv_v128_receipt`: the v128 receipt is preserved as the retained historical source.
- `verifies_runtime_smoke_exposes_v128_receipt=true` and `verifies_release_manifest_exposes_v128_receipt=true`: both runtime and release fixtures still expose the v128 anchor.
- `fake_harness_runtime_enabled=false`, `fake_harness_invocation_allowed=false`, and `execute_fake_harness_runtime_allowed=false`: no fake harness can run from mini-kv.
- `credential_value_read_allowed=false` and `credential_value_read=false`: credential values stay out of mini-kv.
- `raw_endpoint_url_parse_allowed=false` and `raw_endpoint_url_parsed=false`: raw endpoints are not expanded.
- `http_tcp_dial_allowed=false`, `external_request_sent=false`, and `connects_managed_audit=false`: no external connection is opened.
- `storage_write_allowed=false`, `approval_ledger_written=false`, `schema_migration_executed=false`, `load_restore_compact_executed=false`, and `setnxex_execution_allowed=false`: no storage, ledger, schema, restore, compact, or token path is introduced.
- `audit_authoritative=false` and `order_authoritative=false`: mini-kv does not become an authority for audit or order state.

## Tests And Smoke

`tests/credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_tests.cpp` checks the standalone fixture, runtime `SMOKEJSON`, release manifest exposure, digest alignment, Node v292 and Node v291 history, v128 retention shape, and the closed execution boundaries.

Existing aggregation tests were extended so the new retention-check receipt appears in the live runtime and release fixtures:

- `smokejson_command_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`
- `credential_resolver_disabled_fake_harness_execution_denied_non_participation_receipt_tests`

Targeted CTest passed after the retention fixture and manifest were repaired, and full CTest passed 56/56. The CLI smoke stayed read-only and returned the expected `SMOKEJSON` chain with the new v129 receipt name and digest.

One-sentence summary: v129 keeps the v128 execution-denied receipt alive as historical evidence for Node v293 while staying read-only, non-executing, and outside fake harness or credential resolver runtime behavior.
