#include "minikv/shard_preview_production_shard_execution_owner_receipt_request_packet_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

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
    int planned_stage_count) {
    const bool source_locked = source_quality_gate_stage_count == 6 && source_quality_gate_complete;
    const bool node_plan_allows_parallel =
        !node_waits_for_mini_kv && !node_requires_existing_signed_receipts;
    const bool signed_receipts_absent =
        !signed_mini_kv_receipt_present && !signed_java_receipt_present &&
        !signed_cross_project_cleanup_receipt_present;
    const bool request_scope_closed =
        owner_receipt_request_packet_only && requested_receipt_slot_count == 9 && signed_receipts_absent;
    const bool archive_split_closed = per_version_archive_required && !combined_archive_folder_allowed;
    const bool authority_boundary_closed =
        !production_shard_execution_enabled && !managed_audit_store_bound && !raw_endpoint_parsed &&
        !credential_value_read && !starts_node_services && !starts_java_service && !starts_mini_kv_service &&
        !active_router_installed && !write_routing_allowed && !mutates_store && !touches_wal && !execution_allowed;
    const bool counts_aligned = planned_check_count == 12 && completed_check_count == 12 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 5;
    const bool validation_passed = source_locked && node_plan_allows_parallel && request_scope_closed &&
                                   archive_split_closed && authority_boundary_closed && counts_aligned;

    return "{\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceCodeWalkthroughQualityGateNonParticipationPublishedStageCount\":" +
           std::to_string(source_quality_gate_stage_count) +
           ",\"sourceCodeWalkthroughQualityGateNonParticipationComplete\":" +
           json_bool(source_quality_gate_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeWaitsForMiniKv\":" + json_bool(node_waits_for_mini_kv) +
           ",\"nodeRequiresExistingSignedReceipts\":" + json_bool(node_requires_existing_signed_receipts) +
           ",\"nodePlanAllowsParallel\":" + json_bool(node_plan_allows_parallel) +
           ",\"signedMiniKvReceiptPresent\":" + json_bool(signed_mini_kv_receipt_present) +
           ",\"signedJavaReceiptPresent\":" + json_bool(signed_java_receipt_present) +
           ",\"signedCrossProjectCleanupReceiptPresent\":" +
           json_bool(signed_cross_project_cleanup_receipt_present) +
           ",\"signedReceiptsAbsent\":" + json_bool(signed_receipts_absent) +
           ",\"ownerReceiptRequestPacketOnly\":" + json_bool(owner_receipt_request_packet_only) +
           ",\"requestedReceiptSlotCount\":" + std::to_string(requested_receipt_slot_count) +
           ",\"requestScopeClosed\":" + json_bool(request_scope_closed) +
           ",\"perVersionArchiveRequired\":" + json_bool(per_version_archive_required) +
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
           ",\"plannedProductionShardExecutionOwnerReceiptRequestPacketCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionShardExecutionOwnerReceiptRequestPacketCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"readOnly\":true}";
}

} // namespace minikv::shard_preview_production_shard_execution_owner_receipt_request_packet_validation
