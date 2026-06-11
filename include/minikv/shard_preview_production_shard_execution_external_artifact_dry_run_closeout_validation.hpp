#pragma once

#include <string>

namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation {

std::string format_production_shard_execution_external_artifact_dry_run_closeout_validation_json(
    int source_mini_kv_bundle_stage_count,
    bool source_mini_kv_bundle_complete,
    bool source_mini_kv_bundle_validation_passed,
    bool dry_run_batch_closed,
    bool node_only_growth_stopped,
    bool real_external_artifact_present,
    bool external_artifact_parsed,
    bool external_artifact_accepted,
    bool external_artifact_stored,
    bool dry_run_artifact_executed,
    bool real_signed_approval_present,
    bool managed_audit_owner_binding_present,
    bool real_java_receipt_present,
    bool real_mini_kv_signed_receipt_present,
    bool real_cleanup_receipt_present,
    bool missing_real_artifact_blocker_listed,
    bool per_version_archive_required,
    bool archive_root_is_f,
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

} // namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation
