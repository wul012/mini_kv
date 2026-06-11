#pragma once

#include <string>

namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation {

std::string format_production_shard_execution_mini_kv_owner_receipt_bundle_validation_json(
    int source_request_packet_stage_count,
    bool source_request_packet_complete,
    bool source_request_packet_validation_passed,
    bool mini_kv_abort_owner_receipt_present,
    bool mini_kv_rollback_owner_receipt_present,
    bool mini_kv_lifecycle_owner_receipt_present,
    bool mini_kv_cleanup_no_start_proof_present,
    bool mini_kv_owner_receipt_bundle_only,
    int mini_kv_receipt_slot_count,
    bool java_owner_receipts_present,
    bool cross_project_cleanup_reconciliation_receipt_present,
    bool per_version_archive_required,
    bool archive_root_is_f,
    bool combined_archive_folder_allowed,
    bool abort_execution_performed,
    bool rollback_execution_performed,
    bool lifecycle_service_started,
    bool cleanup_execution_performed,
    bool production_shard_execution_enabled,
    bool managed_audit_store_bound,
    bool raw_endpoint_parsed,
    bool credential_value_read,
    bool starts_node_services,
    bool starts_java_service,
    bool starts_mini_kv_service,
    bool active_router_installed,
    bool write_routing_allowed,
    bool mutates_store,
    bool touches_wal,
    bool execution_allowed,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation
