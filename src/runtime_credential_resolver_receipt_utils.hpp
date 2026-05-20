#pragma once

#include "minikv/runtime_evidence.hpp"

#include <string>
#include <string_view>
#include <vector>

namespace minikv::runtime_evidence_receipts::credential_resolver_detail {

using runtime_evidence::DigestPart;

inline std::string field_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

inline std::string receipt_digest(std::string_view prefix, const std::vector<DigestPart>& parts) {
    return runtime_evidence::digest(prefix, parts);
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
