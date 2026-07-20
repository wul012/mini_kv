#pragma once

#include "runtime_receipt_json_builder.hpp"

#include <vector>

namespace minikv::runtime_evidence_receipts::disabled_runtime_shell_detail {

inline constexpr runtime_receipt_json::BooleanField runtime_shell_readiness_profile[] = {
    {"ready_for_disabled_runtime_shell_implementation", false},
    {"ready_for_disabled_runtime_shell_invocation", false},
    {"ready_for_managed_audit_resolver_implementation", false},
    {"ready_for_managed_audit_sandbox_adapter_connection", false},
    {"ready_for_production_audit", false},
    {"ready_for_production_window", false},
    {"ready_for_production_operations", false},
    {"runtime_shell_implemented", false},
    {"runtime_shell_enabled", false},
    {"runtime_shell_invocation_allowed", false},
    {"runtime_shell_implementation_allowed", false},
};

inline constexpr runtime_receipt_json::BooleanField non_participation_tail_profile[] = {
    {"disabled_runtime_shell_participates", false},
    {"test_only_fake_harness_allowed", false},
    {"test_only_fake_harness_execution_allowed", false},
    {"fake_harness_runtime_enabled", false},
    {"fake_harness_runtime_invoked", false},
    {"real_resolver_implementation_allowed", false},
    {"credential_resolver_implemented", false},
    {"credential_resolver_invoked", false},
    {"resolver_client_instantiated", false},
    {"secret_provider_instantiated", false},
    {"fake_secret_provider_instantiated", false},
    {"fake_resolver_client_instantiated", false},
    {"provider_client_instantiation_allowed", false},
    {"credential_value_read_allowed", false},
    {"credential_value_read", false},
    {"credential_value_provided", false},
    {"credential_value_loaded", false},
    {"credential_value_stored", false},
    {"credential_value_included", false},
    {"credential_value_rendered", false},
    {"raw_endpoint_url_parse_allowed", false},
    {"raw_endpoint_url_render_allowed", false},
    {"raw_endpoint_url_parsed", false},
    {"raw_endpoint_url_rendered", false},
    {"raw_endpoint_url_provided", false},
    {"raw_endpoint_url_included", false},
    {"external_request_allowed", false},
    {"external_request_sent", false},
    {"http_tcp_dial_allowed", false},
    {"connects_managed_audit", false},
    {"reads_managed_audit_credential", false},
    {"stores_managed_audit_credential", false},
    {"managed_audit_store", false},
    {"managed_audit_storage_backend", false},
    {"sandbox_audit_storage_backend", false},
    {"storage_write_allowed", false},
    {"write_commands_executed", false},
    {"admin_commands_executed", false},
    {"runtime_write_observed", false},
    {"approval_gate_required", true},
    {"approval_gate_satisfied", false},
    {"approval_ledger_write_allowed", false},
    {"approval_ledger_written", false},
    {"approval_ledger_write_executed", false},
    {"managed_audit_write_executed", false},
    {"production_record_written", false},
    {"schema_migration_allowed", false},
    {"schema_migration_executed", false},
    {"schema_rehearsal_execution_allowed", false},
    {"schema_migration_execution_allowed", false},
    {"restore_execution_allowed", false},
    {"load_restore_compact_executed", false},
    {"setnxex_execution_allowed", false},
    {"node_auto_start_allowed", false},
    {"java_auto_start_allowed", false},
    {"mini_kv_auto_start_allowed", false},
    {"automatic_upstream_start_allowed", false},
    {"automatic_upstream_start", false},
    {"audit_authoritative", false},
    {"order_authoritative", false},
};

inline void append_runtime_shell_readiness_fields(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    runtime_receipt_json::append_boolean_fields(fields, runtime_shell_readiness_profile);
}

// The candidate-gate formatter inserts three decision fields between these two shared blocks.
inline void append_non_participation_tail_fields(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    runtime_receipt_json::append_boolean_fields(fields, non_participation_tail_profile);
}

} // namespace minikv::runtime_evidence_receipts::disabled_runtime_shell_detail
