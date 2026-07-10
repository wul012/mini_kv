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

inline void append_closed_runtime_shell_boundary_fields(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    using runtime_receipt_json::json_bool;

    fields.push_back({"ready_for_disabled_runtime_shell_implementation", json_bool(false)});
    fields.push_back({"ready_for_disabled_runtime_shell_invocation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_resolver_implementation", json_bool(false)});
    fields.push_back({"ready_for_managed_audit_sandbox_adapter_connection", json_bool(false)});
    fields.push_back({"ready_for_production_audit", json_bool(false)});
    fields.push_back({"ready_for_production_window", json_bool(false)});
    fields.push_back({"ready_for_production_operations", json_bool(false)});
    fields.push_back({"runtime_shell_implemented", json_bool(false)});
    fields.push_back({"runtime_shell_enabled", json_bool(false)});
    fields.push_back({"runtime_shell_invocation_allowed", json_bool(false)});
    fields.push_back({"runtime_shell_implementation_allowed", json_bool(false)});
    fields.push_back({"real_resolver_implementation_allowed", json_bool(false)});
    fields.push_back({"credential_resolver_implemented", json_bool(false)});
    fields.push_back({"credential_resolver_invoked", json_bool(false)});
    fields.push_back({"resolver_client_instantiated", json_bool(false)});
    fields.push_back({"secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_secret_provider_instantiated", json_bool(false)});
    fields.push_back({"fake_resolver_client_instantiated", json_bool(false)});
    fields.push_back({"provider_client_instantiation_allowed", json_bool(false)});
    fields.push_back({"credential_value_accepted", json_bool(false)});
    fields.push_back({"credential_value_read_allowed", json_bool(false)});
    fields.push_back({"credential_value_read", json_bool(false)});
    fields.push_back({"credential_value_provided", json_bool(false)});
    fields.push_back({"credential_value_loaded", json_bool(false)});
    fields.push_back({"credential_value_stored", json_bool(false)});
    fields.push_back({"credential_value_included", json_bool(false)});
    fields.push_back({"credential_value_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_accepted", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parse_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_render_allowed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_parsed", json_bool(false)});
    fields.push_back({"raw_endpoint_url_rendered", json_bool(false)});
    fields.push_back({"raw_endpoint_url_provided", json_bool(false)});
    fields.push_back({"raw_endpoint_url_included", json_bool(false)});
    fields.push_back({"external_request_payload_accepted", json_bool(false)});
    fields.push_back({"external_request_allowed", json_bool(false)});
    fields.push_back({"external_request_sent", json_bool(false)});
    fields.push_back({"http_tcp_dial_allowed", json_bool(false)});
    fields.push_back({"connects_managed_audit", json_bool(false)});
    fields.push_back({"reads_managed_audit_credential", json_bool(false)});
    fields.push_back({"stores_managed_audit_credential", json_bool(false)});
    fields.push_back({"managed_audit_store", json_bool(false)});
    fields.push_back({"managed_audit_storage_backend", json_bool(false)});
    fields.push_back({"sandbox_audit_storage_backend", json_bool(false)});
    fields.push_back({"storage_write_allowed", json_bool(false)});
    fields.push_back({"write_commands_executed", json_bool(false)});
    fields.push_back({"admin_commands_executed", json_bool(false)});
    fields.push_back({"runtime_write_observed", json_bool(false)});
    fields.push_back({"approval_ledger_write_allowed", json_bool(false)});
    fields.push_back({"approval_ledger_written", json_bool(false)});
    fields.push_back({"approval_ledger_write_executed", json_bool(false)});
    fields.push_back({"managed_audit_write_executed", json_bool(false)});
    fields.push_back({"production_record_written", json_bool(false)});
    fields.push_back({"schema_migration_allowed", json_bool(false)});
    fields.push_back({"schema_migration_executed", json_bool(false)});
    fields.push_back({"schema_rehearsal_execution_allowed", json_bool(false)});
    fields.push_back({"schema_migration_execution_allowed", json_bool(false)});
    fields.push_back({"restore_execution_allowed", json_bool(false)});
    fields.push_back({"load_restore_compact_executed", json_bool(false)});
    fields.push_back({"setnxex_execution_allowed", json_bool(false)});
    fields.push_back({"node_auto_start_allowed", json_bool(false)});
    fields.push_back({"java_auto_start_allowed", json_bool(false)});
    fields.push_back({"mini_kv_auto_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start_allowed", json_bool(false)});
    fields.push_back({"automatic_upstream_start", json_bool(false)});
    fields.push_back({"audit_authoritative", json_bool(false)});
    fields.push_back({"order_authoritative", json_bool(false)});
}

inline std::string format_common_credential_boundary_json() {
    return "\"read_only\":true,\"execution_allowed\":false,\"dry_run_only\":true,"
           "\"credential_resolver_implemented\":false,\"credential_resolver_invoked\":false,"
           "\"secret_provider_instantiated\":false,"
           "\"node_auto_start_allowed\":false,\"java_auto_start_allowed\":false,"
           "\"mini_kv_auto_start_allowed\":false,\"external_audit_service_auto_start_allowed\":false,"
           "\"connection_execution_allowed\":false,\"write_commands_executed\":false,"
           "\"admin_commands_executed\":false,\"runtime_write_observed\":false,"
           "\"managed_audit_store\":false,\"managed_audit_storage_backend\":false,"
           "\"sandbox_audit_storage_backend\":false,\"storage_write_allowed\":false,"
           "\"managed_audit_write_executed\":false,\"production_record_written\":false,"
           "\"approval_ledger_write_allowed\":false,\"approval_ledger_write_executed\":false,"
           "\"sandbox_managed_audit_state_write_allowed\":false,"
           "\"credential_value_required\":false,\"credential_value_read_allowed\":false,"
           "\"credential_value_loaded\":false,\"credential_value_stored\":false,"
           "\"credential_value_included\":false,\"raw_endpoint_url_parsed\":false,"
           "\"raw_endpoint_url_included\":false,\"external_request_sent\":false,"
           "\"schema_rehearsal_execution_allowed\":false,\"schema_migration_execution_allowed\":false,"
           "\"restore_execution_allowed\":false,\"load_restore_compact_executed\":false,"
           "\"setnxex_execution_allowed\":false,\"order_authoritative\":false";
}

inline std::string format_no_go_conditions_json() {
    return "[\"CREDENTIAL_VALUE_REQUIRED\",\"RAW_ENDPOINT_URL_REQUIRED\","
           "\"REAL_CONNECTION_REQUIRED\",\"EXTERNAL_REQUEST_REQUIRED\","
           "\"SCHEMA_MIGRATION_REQUIRED\",\"UPSTREAM_WRITE_REQUIRED\","
           "\"AUTO_START_REQUIRED\",\"MINI_KV_BACKEND_REQUIRED\",\"PRODUCTION_WINDOW_REQUIRED\"]";
}

} // namespace minikv::runtime_evidence_receipts::credential_resolver_detail
