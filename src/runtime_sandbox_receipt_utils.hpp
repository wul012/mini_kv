#pragma once

#include "runtime_receipt_json_builder.hpp"

#include <vector>

namespace minikv::runtime_evidence_receipts::sandbox_receipt_detail {

inline void append_read_only_prefix(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    using runtime_receipt_json::json_bool;

    fields.push_back({"read_only", json_bool(true)});
    fields.push_back({"execution_allowed", json_bool(false)});
    fields.push_back({"dry_run_only", json_bool(true)});
    fields.push_back({"node_auto_start_allowed", json_bool(false)});
    fields.push_back({"java_auto_start_allowed", json_bool(false)});
    fields.push_back({"mini_kv_auto_start_allowed", json_bool(false)});
}

inline void append_closed_write_fields(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    using runtime_receipt_json::json_bool;

    fields.push_back({"connection_execution_allowed", json_bool(false)});
    fields.push_back({"write_commands_executed", json_bool(false)});
    fields.push_back({"admin_commands_executed", json_bool(false)});
    fields.push_back({"runtime_write_observed", json_bool(false)});
    fields.push_back({"managed_audit_store", json_bool(false)});
    fields.push_back({"managed_audit_storage_backend", json_bool(false)});
    fields.push_back({"sandbox_audit_storage_backend", json_bool(false)});
    fields.push_back({"storage_write_allowed", json_bool(false)});
    fields.push_back({"managed_audit_write_executed", json_bool(false)});
    fields.push_back({"production_record_written", json_bool(false)});
}

inline void append_closed_execution_tail(std::vector<runtime_receipt_json::OrderedJsonField>& fields) {
    using runtime_receipt_json::json_bool;

    fields.push_back({"schema_rehearsal_execution_allowed", json_bool(false)});
    fields.push_back({"schema_migration_execution_allowed", json_bool(false)});
    fields.push_back({"restore_execution_allowed", json_bool(false)});
    fields.push_back({"load_restore_compact_executed", json_bool(false)});
    fields.push_back({"setnxex_execution_allowed", json_bool(false)});
    fields.push_back({"order_authoritative", json_bool(false)});
}

} // namespace minikv::runtime_evidence_receipts::sandbox_receipt_detail
