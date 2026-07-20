#pragma once

#include "runtime_receipt_json_builder.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts::credential_resolver_detail {

using runtime_evidence::DigestPart;

inline std::string field_string(std::string_view value) { return runtime_evidence::json_string(value); }

inline std::string receipt_digest(std::string_view prefix, const std::vector<DigestPart>& parts) {
    return runtime_evidence::digest(prefix, parts);
}

inline constexpr runtime_receipt_json::BooleanField common_credential_boundary_profile[] = {
    {"read_only", true},
    {"execution_allowed", false},
    {"dry_run_only", true},
    {"credential_resolver_implemented", false},
    {"credential_resolver_invoked", false},
    {"secret_provider_instantiated", false},
    {"node_auto_start_allowed", false},
    {"java_auto_start_allowed", false},
    {"mini_kv_auto_start_allowed", false},
    {"external_audit_service_auto_start_allowed", false},
    {"connection_execution_allowed", false},
    {"write_commands_executed", false},
    {"admin_commands_executed", false},
    {"runtime_write_observed", false},
    {"managed_audit_store", false},
    {"managed_audit_storage_backend", false},
    {"sandbox_audit_storage_backend", false},
    {"storage_write_allowed", false},
    {"managed_audit_write_executed", false},
    {"production_record_written", false},
    {"approval_ledger_write_allowed", false},
    {"approval_ledger_write_executed", false},
    {"sandbox_managed_audit_state_write_allowed", false},
    {"credential_value_required", false},
    {"credential_value_read_allowed", false},
    {"credential_value_loaded", false},
    {"credential_value_stored", false},
    {"credential_value_included", false},
    {"raw_endpoint_url_parsed", false},
    {"raw_endpoint_url_included", false},
    {"external_request_sent", false},
    {"schema_rehearsal_execution_allowed", false},
    {"schema_migration_execution_allowed", false},
    {"restore_execution_allowed", false},
    {"load_restore_compact_executed", false},
    {"setnxex_execution_allowed", false},
    {"order_authoritative", false},
};

inline constexpr runtime_receipt_json::BooleanField closed_runtime_shell_profile[] = {
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
    {"real_resolver_implementation_allowed", false},
    {"credential_resolver_implemented", false},
    {"credential_resolver_invoked", false},
    {"resolver_client_instantiated", false},
    {"secret_provider_instantiated", false},
    {"fake_secret_provider_instantiated", false},
    {"fake_resolver_client_instantiated", false},
    {"provider_client_instantiation_allowed", false},
    {"credential_value_accepted", false},
    {"credential_value_read_allowed", false},
    {"credential_value_read", false},
    {"credential_value_provided", false},
    {"credential_value_loaded", false},
    {"credential_value_stored", false},
    {"credential_value_included", false},
    {"credential_value_rendered", false},
    {"raw_endpoint_url_accepted", false},
    {"raw_endpoint_url_parse_allowed", false},
    {"raw_endpoint_url_render_allowed", false},
    {"raw_endpoint_url_parsed", false},
    {"raw_endpoint_url_rendered", false},
    {"raw_endpoint_url_provided", false},
    {"raw_endpoint_url_included", false},
    {"external_request_payload_accepted", false},
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

inline void append_common_credential_boundary_fields(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    runtime_receipt_json::append_boolean_fields(fields, common_credential_boundary_profile);
}

inline void append_closed_runtime_shell_boundary_fields(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    runtime_receipt_json::append_boolean_fields(fields, closed_runtime_shell_profile);
}

inline std::string format_no_go_conditions_json() {
    return "[\"CREDENTIAL_VALUE_REQUIRED\",\"RAW_ENDPOINT_URL_REQUIRED\","
           "\"REAL_CONNECTION_REQUIRED\",\"EXTERNAL_REQUEST_REQUIRED\","
           "\"SCHEMA_MIGRATION_REQUIRED\",\"UPSTREAM_WRITE_REQUIRED\","
           "\"AUTO_START_REQUIRED\",\"MINI_KV_BACKEND_REQUIRED\",\"PRODUCTION_WINDOW_REQUIRED\"]";
}

} // namespace minikv::runtime_evidence_receipts::credential_resolver_detail
