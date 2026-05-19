# v114 credential resolver non-participation receipt

## Goal and boundary

This version gives Node v261 a read-only mini-kv echo for the Node v260 sandbox endpoint credential resolver decision record.

It is not a credential resolver, not a secret provider adapter, not a connection implementation, and not a managed audit storage backend. It does not start mini-kv or Java, does not read, load, store, or include credential values, does not parse raw endpoint URLs, does not send external requests, does not write approval ledger or managed audit state, and does not execute schema migration, `LOAD`, `COMPACT`, `RESTORE`, or `SETNXEX`.

## Version position

Node v260 records a policy-only human decision envelope before any credential resolver rehearsal exists. mini-kv v114 mirrors that envelope as evidence: endpoint handle, credential handle, resolver policy handle, approval marker, operator identity requirement, approval correlation requirement, redaction policy, fallback rotation plan, required decision fields, and explicit no-go conditions.

The version intentionally stays inside the existing runtime sandbox receipt module. `src/runtime_sandbox_receipts.cpp` remains below the current large-file threshold after this addition, so a new split would add more structure than the change needs.

## Code modules

`src/runtime_sandbox_receipts.cpp` now owns `credential_resolver_non_participation_receipt_digest` and `format_credential_resolver_non_participation_receipt_json`. The formatter echoes the Node v260 source shape, records source counts, lists required decision fields and no-go conditions, and hard-codes the no credential/no resolver/no external request boundaries.

`include/minikv/runtime_evidence_receipts.hpp` exposes the new digest and formatter through the same public receipt interface used by v111-v113.

`src/command_response_formatters.cpp` adds `credential_resolver_non_participation_receipt` to `SMOKEJSON`, includes it in the notes array, and adds the Node v261 diagnostics hint while preserving the older v108/v112/v113/v111/v107/v102 consumption hints.

## Contract fields

The v114 receipt echoes these upstream facts from Node v260:

- `source_required_decision_field_count=8`, `source_explicit_no_go_condition_count=9`, and `source_check_count=20`.
- `endpoint_handle`, `credential_handle`, `resolver_policy_handle`, and `approval_marker` are names or markers only.
- `operator_identity_required=true` and `approval_correlation_required=true` are requirements, not identity inference by mini-kv.
- `resolver_mode=policy-record-only-no-value-read` and `resolver_candidate_implementation=not-implemented` keep the resolver unimplemented.
- `source_credential_value_read=false`, `source_credential_value_loaded=false`, `source_credential_value_included=false`, `source_raw_endpoint_url_parsed=false`, `source_external_request_sent=false`, and `source_automatic_upstream_start=false`.

Control-plane readers should treat `read_only=true` and `execution_allowed=false` as evidence-only. `credential_resolver_implemented=false`, `credential_resolver_invoked=false`, `secret_provider_instantiated=false`, `mini_kv_auto_start_allowed=false`, `storage_write_allowed=false`, `managed_audit_storage_backend=false`, `restore_execution_allowed=false`, `load_restore_compact_executed=false`, `setnxex_execution_allowed=false`, and `order_authoritative=false` are the actual mini-kv boundary proof.

## Fixtures and tests

`fixtures/release/credential-resolver-non-participation-receipt.json` is the standalone v114 receipt. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` embed the same receipt so Node can consume either the standalone fixture or the `SMOKEJSON` aggregate.

`tests/credential_resolver_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime smoke fixture, release manifest, live `CommandProcessor` `SMOKEJSON`, and `GET restore:real-read-token`. It verifies the Node v260 source shape, decision record fields, no-go conditions, and hard no-start/no-write/no-credential/no-resolver/no-execution boundaries.

Existing runtime smoke, release manifest, and command tests were extended only for the new key and diagnostics hint. They keep the historical v102 runtime evidence and v107-v113 receipt contracts in place.

## Summary

v114 lets the cross-project chain acknowledge a credential resolver decision without letting mini-kv become the resolver, the credential reader, the endpoint parser, the transport, or the audit store.
