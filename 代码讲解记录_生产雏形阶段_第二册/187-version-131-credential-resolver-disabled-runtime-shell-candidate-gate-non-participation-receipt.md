# mini-kv v131 credential resolver disabled runtime shell candidate gate non-participation receipt

## Goal

v131 does not implement a disabled runtime shell. It consumes the Node v297 disabled runtime shell implementation candidate gate and the Node v296 upstream echo, then emits a mini-kv non-participation receipt for Node v298.

This is not a runtime implementation, not a shell invocation surface, not provider/client wiring, not a credential reader, not a raw endpoint parser, not an HTTP/TCP connector, not an approval ledger or schema writer, and not a LOAD/COMPACT/RESTORE/SETNXEX path. mini-kv remains a read-only evidence provider.

## Why This Version Exists

Node v297 reviewed the disabled runtime shell candidate gate and kept it blocked. The plan asks Java v134 and mini-kv v131 to echo the same blocked candidate-gate facts before Node v298 verifies the upstream chain.

The value is a small, explicit stop sign. v131 lets Node v298 see that mini-kv understands the five candidate gates and still refuses to participate in implementation, invocation, credential handling, network access, writes, schema work, restore/load/compact/SETNXEX, or authority.

## Module Roles

`include/minikv/runtime_evidence_receipts.hpp` adds the public v131 receipt API:

```cpp
credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_digest(...)
format_credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_json(...)
```

`src/runtime_credential_resolver_disabled_runtime_shell_candidate_gate_receipts.cpp` owns the v131 receipt. It records:

- Node v297 disabled runtime shell implementation candidate gate.
- Node v296 disabled runtime shell upstream echo verification.
- Node v298 as the next consumer.
- The Node v297 gate digest `651383bcd175bdaff2691c026135a1cebbcf30de91be7709cbc7843866684e22`.
- The mini-kv receipt digest `fnv1a64:0557867ad4f6ed7e`.
- The five blocked candidate gates: `DEDICATED_DISABLED_BY_DEFAULT_FLAG`, `OPERATOR_APPROVAL`, `ABORT_SEMANTICS`, `NO_NETWORK_TESTS`, and `HISTORICAL_FALLBACK_EVIDENCE`.
- The closed boundary that keeps runtime implementation, invocation, provider/client instantiation, credential reads, raw endpoint parsing, network, writes, migrations, restore/load/compact/SETNXEX, auto-start, and authority claims out of mini-kv.

`src/command_smoke_formatters.cpp` exposes the receipt through runtime `SMOKEJSON`, prepends the Node v298 consumption sentence, and keeps the v130/v129/v128 history visible.

`fixtures/release/credential-resolver-disabled-runtime-shell-candidate-gate-non-participation-receipt.json` is the standalone v131 fixture. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` expose the same receipt to runtime and release verification.

## Boundary Fields

For the control plane, the important fields are:

- `read_only=true`: this is evidence only.
- `execution_allowed=false`: the receipt is not an execution window.
- `runtime_shell_candidate_gate_non_participation_receipt_only=true`: the receipt only records candidate-gate non-participation.
- `implementation_candidate_gate_only=true`: the Node v297 source is a blocked candidate gate, not an implementation grant.
- `source_node_v297_reference.gate_decision=blocked-request-parallel-java-v134-and-mini-kv-v131-before-implementation`: the upstream gate remains blocked.
- `required_gate_count=5`, `documented_gate_count=5`, and `review_evidence_satisfied_count=5`: the five candidate gates are visible for Node v298.
- `runtime_prerequisite_satisfied_count=0` and `implementation_allowed_gate_count=0`: no gate authorizes runtime behavior.
- `ready_for_node_v298_runtime_shell_candidate_gate_upstream_echo_verification=true`: the receipt is ready for Node v298 verification.
- `ready_for_node_v298_runtime_shell_implementation=false`: it does not authorize implementation.
- `runtime_shell_implemented=false`, `runtime_shell_enabled=false`, and `runtime_shell_invocation_allowed=false`: no shell exists or can run.
- `credential_value_read_allowed=false` and `raw_endpoint_url_parse_allowed=false`: credential values and raw endpoints stay out.
- `resolver_client_instantiated=false`, `secret_provider_instantiated=false`, and provider/client flags are false: no client or provider shell is created.
- `external_request_sent=false`, `http_tcp_dial_allowed=false`, and `connects_managed_audit=false`: no external request or managed audit connection is opened.
- `storage_write_allowed=false`, `approval_ledger_written=false`, `schema_migration_executed=false`, `load_restore_compact_executed=false`, and `setnxex_execution_allowed=false`: no write, schema, restore, compact, load, or token path is introduced.
- `audit_authoritative=false` and `order_authoritative=false`: mini-kv does not become an authority.

## Tests And Smoke

`tests/credential_resolver_disabled_runtime_shell_candidate_gate_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime `SMOKEJSON`, release manifest exposure, digest alignment, Node v297 and Node v296 source shapes, the five candidate gates, summary counts, and closed execution boundaries.

Existing aggregation tests were extended so the new receipt appears in live runtime and release evidence while v130 remains visible as historical context:

- `smokejson_command_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`
- `credential_resolver_disabled_runtime_shell_non_participation_receipt_tests`
- `credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_tests`

The expected verification sequence is targeted CTest, full CTest, and CLI smoke. `SMOKEJSON` exposes the v131 receipt, `CHECKJSON SMOKEJSON` remains a metadata read, and `GET restore:real-read-token` must return `(nil)`.

One-sentence summary: v131 gives Node v298 a clean mini-kv candidate-gate non-participation anchor for the disabled runtime shell chain while keeping mini-kv completely outside runtime shell execution and authority.
