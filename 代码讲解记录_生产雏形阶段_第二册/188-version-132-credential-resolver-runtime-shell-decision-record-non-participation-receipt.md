# mini-kv v132 credential resolver runtime shell decision record non-participation receipt

## Goal

v132 does not implement a disabled runtime shell. It consumes the Node v299 runtime shell candidate gate decision record, which itself consumed the Node v298 upstream echo, and emits a mini-kv non-participation receipt for Node v300.

This is not a runtime implementation, not a shell invocation surface, not provider/client wiring, not a credential reader, not a raw endpoint parser, not an HTTP/TCP connector, not an approval ledger or schema writer, and not a LOAD/COMPACT/RESTORE/SETNXEX path. mini-kv stays a read-only evidence provider.

## Why This Version Exists

Node v299 records the already verified runtime shell candidate gate decision as `blocked`. The plan asks Java v135 and mini-kv v132 to echo that decision record before Node v300 verifies the two downstream echoes.

The value is a small boundary receipt after a decision record. v131 said mini-kv does not participate in the candidate gate; v132 says the later blocked decision record still does not authorize implementation, invocation, credential handling, network access, writes, schema work, restore/load/compact/SETNXEX, or authority.

## Module Roles

`include/minikv/runtime_evidence_receipts.hpp` adds the public v132 receipt API:

```cpp
credential_resolver_runtime_shell_decision_record_non_participation_receipt_digest(...)
format_credential_resolver_runtime_shell_decision_record_non_participation_receipt_json(...)
```

`src/runtime_credential_resolver_runtime_shell_decision_record_receipts.cpp` owns the v132 receipt. It records:

- Node v299 runtime shell candidate gate decision record.
- Node v298 runtime shell candidate gate upstream echo verification.
- Node v300 as the next consumer.
- The Node v299 decision digest `4f6f73fa2806a9ba74174d7bbab17b43459bd1d790237276d95a3937c646e9c0`.
- The mini-kv receipt digest `fnv1a64:7c144f01161c2f81`.
- Four required evidence items: Node v298 upstream echo, Java v134 echo, mini-kv v131 receipt, and runtime shell still blocked.
- Six no-go conditions for runtime shell implementation, credential values, raw endpoints, managed audit connection, ledger/schema writes, and auto-start.

`src/command_smoke_formatters.cpp` exposes the receipt through runtime `SMOKEJSON`, prepends the Node v300 consumption sentence, and keeps the v131/v130/v129 history visible.

`fixtures/release/credential-resolver-runtime-shell-decision-record-non-participation-receipt.json` is the standalone v132 fixture. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` expose the same receipt to runtime and release verification.

## Boundary Fields

For the control plane, the important fields are:

- `read_only=true`: this is evidence only.
- `execution_allowed=false`: the receipt is not an execution window.
- `runtime_shell_decision_record_non_participation_receipt_only=true`: the receipt only records decision-record non-participation.
- `runtime_shell_decision=blocked`: Node v299 did not approve runtime implementation.
- `source_node_v299_reference.decision_digest=4f6f73fa2806a9ba74174d7bbab17b43459bd1d790237276d95a3937c646e9c0`: the upstream decision record is pinned.
- `required_evidence_count=4` and `missing_required_evidence_count=0`: v299 had the minimum upstream evidence it needed for a blocked decision record.
- `no_go_condition_count=6`: the stop conditions are explicit.
- `ready_for_node_v300_runtime_shell_decision_record_upstream_echo_verification=true`: the receipt is ready for Node v300 verification.
- `ready_for_node_v300_runtime_shell_implementation=false`: it does not authorize implementation.
- `runtime_shell_implemented=false`, `runtime_shell_enabled=false`, and `runtime_shell_invocation_allowed=false`: no shell exists or can run.
- `credential_value_read_allowed=false` and `raw_endpoint_url_parse_allowed=false`: credential values and raw endpoints stay out.
- `resolver_client_instantiated=false` and `secret_provider_instantiated=false`: no client or provider is created.
- `external_request_sent=false`, `http_tcp_dial_allowed=false`, and `connects_managed_audit=false`: no external request or managed audit connection is opened.
- `storage_write_allowed=false`, `approval_ledger_written=false`, `schema_migration_executed=false`, `load_restore_compact_executed=false`, and `setnxex_execution_allowed=false`: no write, schema, restore, compact, load, or token path is introduced.
- `audit_authoritative=false` and `order_authoritative=false`: mini-kv does not become an authority.

## Tests And Smoke

`tests/credential_resolver_runtime_shell_decision_record_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime `SMOKEJSON`, release manifest exposure, digest alignment, Node v299 and Node v298 source shapes, required evidence, no-go conditions, summary counts, and closed execution boundaries.

Existing aggregation tests were extended so the new receipt appears in live runtime and release evidence while v131 remains visible as historical context:

- `smokejson_command_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`
- `credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_tests`
- `credential_resolver_disabled_runtime_shell_non_participation_receipt_tests`
- `credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_tests`

The expected verification sequence is targeted CTest, full CTest, and CLI smoke. `SMOKEJSON` exposes the v132 receipt, `CHECKJSON SMOKEJSON` remains a metadata read, and `GET restore:real-read-token` must return `(nil)`.

One-sentence summary: v132 gives Node v300 a clean mini-kv echo of the Node v299 blocked runtime shell decision record while keeping mini-kv completely outside runtime shell execution and authority.
