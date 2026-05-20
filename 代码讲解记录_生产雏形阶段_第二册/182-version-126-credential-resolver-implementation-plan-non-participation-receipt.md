# mini-kv v126 credential resolver implementation plan non-participation receipt

## Goal

v126 consumes the Node v283 credential resolver implementation plan draft and turns it into a mini-kv non-participation receipt for Node v284 verification.

This is not a resolver implementation, not a new execution entry, not a secret provider, not a raw endpoint parser, not a managed audit storage backend, and not approval to write audit/order state. mini-kv still only emits read-only runtime evidence.

## Why This Version Exists

Node v283 finished the implementation plan draft after Node v282 verified Java v116 and mini-kv v122 readiness evidence. The next cross-project step is Java v121 plus mini-kv v126 in parallel, followed by Node v284 verification.

mini-kv's job is narrow: identify the Node v283 `planDigest` and `reviewDigest`, echo the seven interface boundaries and the four mini-kv v126 requirements, and prove that mini-kv remains outside resolver execution, secret handling, endpoint parsing, managed-audit connection, storage backend, ledger write, schema migration, restore/load/compact, and `SETNXEX`.

## Module Roles

`include/minikv/runtime_evidence_receipts.hpp` adds two public functions:

```cpp
credential_resolver_implementation_plan_non_participation_receipt_digest(...)
format_credential_resolver_implementation_plan_non_participation_receipt_json(...)
```

`src/runtime_credential_resolver_implementation_plan_receipts.cpp` owns the v126 receipt. It records:

- Node v283 profile version and route.
- `planDigest=152d4261b3c020159da2aebc2a5ef484dcb8f5381f90bf5f29cc21deb9f80edb`.
- `reviewDigest=f96ffbad4574e400216b0c6615f4c37fe979f9ede9797a039a5e55888d097a55`.
- Node v282 source verification digest.
- Seven boundary codes, twenty-one required artifacts, and twenty-one prohibited actions.
- Four Java v121 echo requirements and four mini-kv v126 receipt requirements.

`src/command_smoke_formatters.cpp` adds the receipt to runtime `SMOKEJSON` and puts Node v284 at the front of `diagnostics.node_consumption`, while keeping the older v282/v275/v274/v272/v269 chain intact.

`fixtures/release/credential-resolver-implementation-plan-non-participation-receipt.json` is the standalone release receipt. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` expose the same receipt for downstream consumers and release checks.

## Boundary Fields

For the control plane, the important fields are:

- `read_only=true`: the receipt is evidence, not permission to execute.
- `execution_allowed=false`: Node cannot treat this as an execution window.
- `credential_resolver_implemented=false` and `credential_resolver_invoked=false`: mini-kv has no resolver path.
- `resolver_client_instantiated=false` and `secret_provider_instantiated=false`: no runtime client or provider is created.
- `credential_value_read_allowed=false`, `credential_value_read=false`, and `credential_value_stored=false`: credential values stay outside mini-kv.
- `raw_endpoint_url_parse_allowed=false`, `raw_endpoint_url_render_allowed=false`, and `raw_endpoint_url_parsed=false`: endpoint handles are evidence only.
- `connects_managed_audit=false`, `managed_audit_storage_backend=false`, `storage_write_allowed=false`, and `approval_ledger_write_allowed=false`: mini-kv does not become an audit backend or ledger writer.
- `schema_migration_executed=false`, `load_restore_compact_executed=false`, and `setnxex_execution_allowed=false`: no storage/admin execution is introduced.
- `audit_authoritative=false` and `order_authoritative=false`: mini-kv is not the authority for audit or order state.

## Tests And Smoke

`tests/credential_resolver_implementation_plan_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime `SMOKEJSON`, release manifest exposure, digest alignment, Node v283/Node v282 source fields, all seven boundary codes, and the no-secret/no-endpoint/no-write/no-restore boundaries.

The existing aggregation tests were updated so they now accept Node v284/v126 as the newest `node_consumption` head while still protecting the historical v282 and earlier evidence chain:

- `smokejson_command_receipt_tests`
- `runtime_smoke_evidence_tests`
- `release_verification_manifest_tests`

Full CTest passes with 53 registered tests, and CLI smoke verifies `SMOKEJSON`, `CHECKJSON SMOKEJSON`, and `GET restore:real-read-token` without starting a background service or executing write/admin commands.

One-sentence summary: v126 gives Node v284 a precise mini-kv receipt for the Node v283 implementation plan draft while keeping mini-kv strictly read-only, non-authoritative, and outside resolver execution.
