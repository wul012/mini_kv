#include "minikv/shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

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
    int planned_stage_count) {
    const bool source_locked = source_mini_kv_bundle_stage_count == 5 && source_mini_kv_bundle_complete &&
                               source_mini_kv_bundle_validation_passed;
    const bool dry_run_closeout_locked = dry_run_batch_closed && node_only_growth_stopped &&
                                         missing_real_artifact_blocker_listed;
    const bool real_external_artifact_absent =
        !real_external_artifact_present && !external_artifact_parsed && !external_artifact_accepted &&
        !external_artifact_stored && !dry_run_artifact_executed;
    const bool real_receipts_missing =
        !real_signed_approval_present && !managed_audit_owner_binding_present && !real_java_receipt_present &&
        !real_mini_kv_signed_receipt_present && !real_cleanup_receipt_present;
    const bool archive_split_closed =
        per_version_archive_required && archive_root_is_f && !combined_archive_folder_allowed;
    const bool authority_boundary_closed =
        !production_shard_execution_enabled && !managed_audit_store_bound && !raw_endpoint_parsed &&
        !credential_value_read && !starts_node_services && !starts_java_service && !starts_mini_kv_service &&
        !active_router_installed && !write_routing_allowed && !mutates_store && !touches_wal &&
        !execution_allowed;
    const bool counts_aligned = planned_check_count == 12 && completed_check_count == 12 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 5;
    const bool validation_passed = source_locked && dry_run_closeout_locked && real_external_artifact_absent &&
                                   real_receipts_missing && archive_split_closed && authority_boundary_closed &&
                                   counts_aligned;

    return "{\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceMiniKvOwnerReceiptBundlePublishedStageCount\":" +
           std::to_string(source_mini_kv_bundle_stage_count) +
           ",\"sourceMiniKvOwnerReceiptBundleComplete\":" + json_bool(source_mini_kv_bundle_complete) +
           ",\"sourceMiniKvOwnerReceiptBundleValidationPassed\":" +
           json_bool(source_mini_kv_bundle_validation_passed) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"dryRunBatchClosed\":" + json_bool(dry_run_batch_closed) +
           ",\"nodeOnlyGrowthStopped\":" + json_bool(node_only_growth_stopped) +
           ",\"missingRealArtifactBlockerListed\":" + json_bool(missing_real_artifact_blocker_listed) +
           ",\"dryRunCloseoutLocked\":" + json_bool(dry_run_closeout_locked) +
           ",\"realExternalArtifactPresent\":" + json_bool(real_external_artifact_present) +
           ",\"externalArtifactParsed\":" + json_bool(external_artifact_parsed) +
           ",\"externalArtifactAccepted\":" + json_bool(external_artifact_accepted) +
           ",\"externalArtifactStored\":" + json_bool(external_artifact_stored) +
           ",\"dryRunArtifactExecuted\":" + json_bool(dry_run_artifact_executed) +
           ",\"realExternalArtifactAbsent\":" + json_bool(real_external_artifact_absent) +
           ",\"realSignedApprovalPresent\":" + json_bool(real_signed_approval_present) +
           ",\"managedAuditOwnerBindingPresent\":" + json_bool(managed_audit_owner_binding_present) +
           ",\"realJavaReceiptPresent\":" + json_bool(real_java_receipt_present) +
           ",\"realMiniKvSignedReceiptPresent\":" + json_bool(real_mini_kv_signed_receipt_present) +
           ",\"realCleanupReceiptPresent\":" + json_bool(real_cleanup_receipt_present) +
           ",\"realReceiptsMissing\":" + json_bool(real_receipts_missing) +
           ",\"perVersionArchiveRequired\":" + json_bool(per_version_archive_required) +
           ",\"archiveRootIsF\":" + json_bool(archive_root_is_f) +
           ",\"combinedArchiveFolderAllowed\":" + json_bool(combined_archive_folder_allowed) +
           ",\"archiveSplitClosed\":" + json_bool(archive_split_closed) +
           ",\"productionShardExecutionEnabled\":" + json_bool(production_shard_execution_enabled) +
           ",\"managedAuditStoreBound\":" + json_bool(managed_audit_store_bound) +
           ",\"rawEndpointParsed\":" + json_bool(raw_endpoint_parsed) +
           ",\"credentialValueRead\":" + json_bool(credential_value_read) +
           ",\"startsNodeServices\":" + json_bool(starts_node_services) +
           ",\"startsJavaService\":" + json_bool(starts_java_service) +
           ",\"startsMiniKvService\":" + json_bool(starts_mini_kv_service) +
           ",\"activeRouterInstalled\":" + json_bool(active_router_installed) +
           ",\"writeRoutingAllowed\":" + json_bool(write_routing_allowed) +
           ",\"mutatesStore\":" + json_bool(mutates_store) +
           ",\"touchesWal\":" + json_bool(touches_wal) +
           ",\"executionAllowed\":" + json_bool(execution_allowed) +
           ",\"authorityBoundaryClosed\":" + json_bool(authority_boundary_closed) +
           ",\"plannedProductionShardExecutionExternalArtifactDryRunCloseoutCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionShardExecutionExternalArtifactDryRunCloseoutCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"readOnly\":true}";
}

} // namespace minikv::shard_preview_production_shard_execution_external_artifact_dry_run_closeout_validation
