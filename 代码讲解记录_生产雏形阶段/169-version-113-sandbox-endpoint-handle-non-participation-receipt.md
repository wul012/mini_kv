# v113 sandbox endpoint handle non-participation receipt

## Goal and boundary

This version gives Node v259 one more read-only upstream echo point: mini-kv now exposes `sandbox_endpoint_handle_non_participation_receipt` for the Node v258 sandbox endpoint handle preflight review.

It is not a connection implementation, not a credential resolver, not a managed audit storage backend, and not a new execution path. It does not start mini-kv or Java, does not parse raw endpoint URLs, does not read credential values, does not send external requests, does not write approval ledger or managed audit state, and does not execute `LOAD`, `COMPACT`, `RESTORE`, or `SETNXEX`.

## Version position

Node v258 reviews only endpoint and credential handles plus the surrounding network allowlist, TLS policy, redaction policy, and closed operator window. mini-kv v113 mirrors that shape as evidence so the later Node v259 upstream echo can verify that mini-kv stayed outside the sandbox connection.

The version also keeps the refactoring rhythm under control: `src/runtime_evidence_receipts.cpp` had grown past 1100 lines, so the newer sandbox receipt implementations were moved into `src/runtime_sandbox_receipts.cpp`. The public header remains `include/minikv/runtime_evidence_receipts.hpp`.

## Code modules

`src/runtime_sandbox_receipts.cpp` owns the v111, v112, and v113 sandbox-style receipt formatters and digest functions. For v113 it defines the Node v258 source fields, review counts, handle names, policy handles, boundary booleans, and `sandbox_endpoint_handle_non_participation_receipt_digest`.

`src/runtime_evidence_receipts.cpp` keeps the core runtime evidence chain: live-read session, failure taxonomy, operator-window proof, CI/artifact retention hints, binary provenance, retention check, retention replay marker, and the earlier manual sandbox command/precheck receipts.

`src/command_response_formatters.cpp` adds the new receipt to `SMOKEJSON` and to the diagnostics notes. The diagnostics string now says Node v259 may verify the mini-kv v113 receipt after Node v258 endpoint handle preflight review, while the existing Node v246/v257/v254/v244/v239 hints remain intact.

## Contract fields

The v113 receipt echoes these upstream facts from Node v258:

- `source_required_review_item_count=7`, `source_completed_review_item_count=7`, `source_forbidden_operation_count=7`, and `source_check_count=19`.
- `endpoint_handle` and `credential_handle` are handle names only.
- `network_allowlist_review`, `tls_policy_review`, `redaction_policy`, and `operator_window` are reviewed without exposing raw host, CIDR, certificate material, private key, raw endpoint URL, or credential value.
- `source_ready_for_managed_audit_sandbox_adapter_connection=false`, `source_external_request_sent=false`, `source_raw_endpoint_url_parsed=false`, `source_credential_value_read=false`, and `source_automatic_upstream_start=false`.

Control-plane readers should interpret `read_only=true` and `execution_allowed=false` as evidence-only. `mini_kv_auto_start_allowed=false`, `storage_write_allowed=false`, `managed_audit_storage_backend=false`, `restore_execution_allowed=false`, `load_restore_compact_executed=false`, `setnxex_execution_allowed=false`, and `order_authoritative=false` are hard boundaries, not runtime toggles.

## Fixtures and tests

`fixtures/release/sandbox-endpoint-handle-non-participation-receipt.json` is the standalone v113 receipt. `fixtures/release/runtime-smoke-evidence.json` and `fixtures/release/verification-manifest.json` embed the same receipt so Node can consume either the standalone fixture or the `SMOKEJSON` aggregate.

`tests/sandbox_endpoint_handle_non_participation_receipt_tests.cpp` checks the standalone fixture, runtime smoke fixture, release manifest, live `CommandProcessor` `SMOKEJSON`, and `GET restore:real-read-token`. The test rejects accidental credential value or raw endpoint URL fields and verifies the no-start, no-write, no-execution, and non-authoritative flags.

Existing runtime smoke, release manifest, and command tests were extended only for the new key and diagnostics hint. They keep the historical v102 runtime evidence and v107-v112 receipt contracts in place.

## Summary

v113 gives the Node/Java/mini-kv integration chain a clean endpoint-handle preflight echo without letting mini-kv become the endpoint, the credential reader, the transport, or the audit store.
