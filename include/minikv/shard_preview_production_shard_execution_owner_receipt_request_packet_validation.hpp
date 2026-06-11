#pragma once

#include <string>

namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_validation {

std::string format_production_shard_execution_owner_receipt_request_packet_validation_json(
    int source_quality_gate_stage_count,
    bool source_quality_gate_complete,
    bool node_waits_for_mini_kv,
    bool node_requires_existing_signed_receipts,
    bool signed_mini_kv_receipt_present,
    bool signed_java_receipt_present,
    bool signed_cross_project_cleanup_receipt_present,
    bool owner_receipt_request_packet_only,
    int requested_receipt_slot_count,
    bool per_version_archive_required,
    bool combined_archive_folder_allowed,
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

} // namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_validation
