# mini-kv v130 credential resolver disabled runtime shell non-participation receipt

## Goal

v130 does not implement a disabled runtime shell. It consumes the Node v295 disabled runtime shell design review and the Node v294 pre-plan intake, then emits a mini-kv non-participation receipt for Node v296.

This is not a resolver runtime, not a shell invocation surface, not provider/client wiring, not a credential reader, not a raw endpoint parser, not an HTTP/TCP connector, not an approval ledger or schema writer, and not a LOAD/COMPACT/RESTORE/SETNXEX path. mini-kv remains a read-only evidence provider.

## Why This Version Exists

Node v295 concluded that Node should not jump straight into runtime shell implementation. The next safe step is parallel upstream echo: Java v132 echoes the handoff, and mini-kv v130 confirms non-participation. That gives Node v296 a small, explicit receipt to consume before any later implementation-candidate gate.

The value is restraint. v130 keeps mini-kv out of runtime behavior while recording exactly which disabled-runtime-shell boundary it refuses to cross.

## Module Roles

`include/minikv/runtime_evidence_receipts.hpp` adds the public v130 receipt API:

```cpp
credential_resolver_disabled_runtime_shell_non_participation_receipt_digest(...)
format_credential_resolver_disabled_runtime_shell_non_participation_receipt_json(...)
```

`src/runtime_credential_resolver_disabled_runtime_shell_receipts.cpp` owns the v130 receipt. It records:

- Node v295 disabled runtime shell design review.
- Node v294 disabled runtime shell pre-plan intake.
- Node v296 as the next consumer.
- The review digest `3bbe96497638d826ab644c7503ab5309c0cc4c4fccdd39a0e82a9b6123ca36c9`.
- The receipt digest `fnv1a64:80181b2752099581`.
- The closed boundary that keeps implementation, invocation, provider/client instantiation, credential reads, raw endpoint parsing, network, writes, migrations, restore/load/compact/SETNXEX, auto-start, and authority claims out of mini-kv.

`src/command_smoke_formatters.cpp` exposes the receipt through runtime `SMOKEJSON`, prepends the Node v296 consumption sentence, and keeps the v129/v128 history visible.

`fixtures/release/credential-resolver-disabled-runtime-shell-non-participation-receipt.json` is the standalone v130 fixture. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` expose the same receipt to runtime and release verification.

## Boundary Fields

For the control plane, the important fields are:

- `read_only=true`: this is evidence only.
- `execution_allowed=false`: the receipt is not an execution window.
- `disabled_runtime_shell_non_participation_receipt_only=true`: the receipt only records non-participation.
- `source_node_v295_reference.design_review_state=disabled-runtime-shell-design-review-ready`: Node v295 is the source design review.
- `source_node_v294_reference.pre_plan_intake_state=disabled-runtime-shell-pre-plan-intake-ready`: Node v294 is the source boundary intake.
- `ready_for_node_v296_disabled_runtime_shell_upstream_echo_verification=true`: the receipt is ready for Node v296 to verify.
- `ready_for_node_v296_runtime_shell_implementation=false`: it does not authorize implementation.
- `runtime_shell_implemented=false`, `runtime_shell_enabled=false`, and `runtime_shell_invocation_allowed=false`: no shell exists or can run.
- `credential_value_read_allowed=false` and `raw_endpoint_url_parse_allowed=false`: credential values and raw endpoints stay out.
- `resolver_client_instantiated=false`, `secret_provider_instantiated=false`, and fake provider/client flags are false: no client or provider shell is created.
- `external_request_sent=false` and `connects_managed_audit=false`: no external request or managed audit connection is opened.
- `storage_write_allowed=false`, `approval_ledger_written=false`, `schema_migration_executed=false`, `load_restore_compact_executed=false`, and `setnxex_execution_allowed=false`: no write, schema, restore, compact, load, or token path is introduced.
- `audit_authoritative=false` and `order_authoritative=false`: mini-kv does not become an authority.

## Tests And Smoke

`tests/credential_resolver_disabled_runtime_shell_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime `SMOKEJSON`, release manifest exposure, digest alignment, Node v295 and Node v294 source shapes, summary counts, and closed execution boundaries.

Existing aggregation tests were extended so the new receipt appears in live runtime and release evidence:

- `smokejson_command_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`
- `credential_resolver_disabled_fake_harness_execution_denied_receipt_verification_retention_check_tests`

Targeted CTest passed 5/5, full CTest passed 57/57, and CLI smoke stayed read-only. `SMOKEJSON` exposes the v130 receipt, `CHECKJSON SMOKEJSON` remains a metadata read, and `GET restore:real-read-token` returns `(nil)`.

One-sentence summary: v130 gives Node v296 a clean mini-kv non-participation anchor for disabled runtime shell review while keeping mini-kv completely outside runtime shell execution and authority.
