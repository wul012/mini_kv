#pragma once

#include "runtime_receipt_json_builder.hpp"

#include <vector>

namespace minikv::runtime_evidence_receipts::sandbox_receipt_detail {

inline constexpr runtime_receipt_json::BooleanField read_only_prefix_profile[] = {
    {"read_only", true},
    {"execution_allowed", false},
    {"dry_run_only", true},
    {"node_auto_start_allowed", false},
    {"java_auto_start_allowed", false},
    {"mini_kv_auto_start_allowed", false},
};

inline constexpr runtime_receipt_json::BooleanField closed_write_profile[] = {
    {"connection_execution_allowed", false},  {"write_commands_executed", false},
    {"admin_commands_executed", false},       {"runtime_write_observed", false},
    {"managed_audit_store", false},           {"managed_audit_storage_backend", false},
    {"sandbox_audit_storage_backend", false}, {"storage_write_allowed", false},
    {"managed_audit_write_executed", false},  {"production_record_written", false},
};

inline constexpr runtime_receipt_json::BooleanField closed_execution_profile[] = {
    {"schema_rehearsal_execution_allowed", false}, {"schema_migration_execution_allowed", false},
    {"restore_execution_allowed", false},          {"load_restore_compact_executed", false},
    {"setnxex_execution_allowed", false},          {"order_authoritative", false},
};

inline void append_read_only_prefix(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    runtime_receipt_json::append_boolean_fields(fields, read_only_prefix_profile);
}

inline void append_closed_write_fields(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    runtime_receipt_json::append_boolean_fields(fields, closed_write_profile);
}

inline void append_closed_execution_tail(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    runtime_receipt_json::append_boolean_fields(fields, closed_execution_profile);
}

} // namespace minikv::runtime_evidence_receipts::sandbox_receipt_detail
