#include "minikv/shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

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
    int planned_stage_count) {
    const bool source_locked = source_request_packet_stage_count == 5 && source_request_packet_complete &&
                               source_request_packet_validation_passed;
    const bool mini_kv_receipts_complete =
        mini_kv_abort_owner_receipt_present && mini_kv_rollback_owner_receipt_present &&
        mini_kv_lifecycle_owner_receipt_present && mini_kv_cleanup_no_start_proof_present &&
        mini_kv_owner_receipt_bundle_only && mini_kv_receipt_slot_count == 4;
    const bool external_receipts_absent =
        !java_owner_receipts_present && !cross_project_cleanup_reconciliation_receipt_present;
    const bool archive_split_closed =
        per_version_archive_required && archive_root_is_f && !combined_archive_folder_allowed;
    const bool no_runtime_receipt_execution =
        !abort_execution_performed && !rollback_execution_performed && !lifecycle_service_started &&
        !cleanup_execution_performed;
    const bool authority_boundary_closed =
        no_runtime_receipt_execution && !production_shard_execution_enabled && !managed_audit_store_bound &&
        !raw_endpoint_parsed && !credential_value_read && !starts_node_services && !starts_java_service &&
        !starts_mini_kv_service && !active_router_installed && !write_routing_allowed && !mutates_store &&
        !touches_wal && !execution_allowed;
    const bool counts_aligned = planned_check_count == 12 && completed_check_count == 12 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 5;
    const bool validation_passed = source_locked && mini_kv_receipts_complete && external_receipts_absent &&
                                   archive_split_closed && authority_boundary_closed && counts_aligned;

    return "{\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceRequestPacketPublishedStageCount\":" + std::to_string(source_request_packet_stage_count) +
           ",\"sourceRequestPacketComplete\":" + json_bool(source_request_packet_complete) +
           ",\"sourceRequestPacketValidationPassed\":" + json_bool(source_request_packet_validation_passed) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"miniKvAbortOwnerReceiptPresent\":" + json_bool(mini_kv_abort_owner_receipt_present) +
           ",\"miniKvRollbackOwnerReceiptPresent\":" + json_bool(mini_kv_rollback_owner_receipt_present) +
           ",\"miniKvLifecycleOwnerReceiptPresent\":" + json_bool(mini_kv_lifecycle_owner_receipt_present) +
           ",\"miniKvCleanupNoStartProofPresent\":" +
           json_bool(mini_kv_cleanup_no_start_proof_present) +
           ",\"miniKvOwnerReceiptBundleOnly\":" + json_bool(mini_kv_owner_receipt_bundle_only) +
           ",\"miniKvReceiptSlotCount\":" + std::to_string(mini_kv_receipt_slot_count) +
           ",\"miniKvReceiptsComplete\":" + json_bool(mini_kv_receipts_complete) +
           ",\"javaOwnerReceiptsPresent\":" + json_bool(java_owner_receipts_present) +
           ",\"crossProjectCleanupReconciliationReceiptPresent\":" +
           json_bool(cross_project_cleanup_reconciliation_receipt_present) +
           ",\"externalReceiptsAbsent\":" + json_bool(external_receipts_absent) +
           ",\"perVersionArchiveRequired\":" + json_bool(per_version_archive_required) +
           ",\"archiveRootIsF\":" + json_bool(archive_root_is_f) +
           ",\"combinedArchiveFolderAllowed\":" + json_bool(combined_archive_folder_allowed) +
           ",\"archiveSplitClosed\":" + json_bool(archive_split_closed) +
           ",\"abortExecutionPerformed\":" + json_bool(abort_execution_performed) +
           ",\"rollbackExecutionPerformed\":" + json_bool(rollback_execution_performed) +
           ",\"lifecycleServiceStarted\":" + json_bool(lifecycle_service_started) +
           ",\"cleanupExecutionPerformed\":" + json_bool(cleanup_execution_performed) +
           ",\"noRuntimeReceiptExecution\":" + json_bool(no_runtime_receipt_execution) +
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
           ",\"plannedProductionShardExecutionMiniKvOwnerReceiptBundleCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionShardExecutionMiniKvOwnerReceiptBundleCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"readOnly\":true}";
}

} // namespace minikv::shard_preview_production_shard_execution_mini_kv_owner_receipt_bundle_validation
