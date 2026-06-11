#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_precheck_upstream_receipt_verification_split_non_participation_validation_json(
    int source_blocked_execution_rehearsal_split_stage_count,
    bool source_blocked_execution_rehearsal_split_complete,
    bool node_requires_fresh_minikv_evidence,
    bool node_requires_fresh_java_evidence,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    bool mini_kv_reads_endpoints,
    bool mini_kv_reads_credentials,
    bool mini_kv_parses_historical_receipts,
    bool mini_kv_scans_java_evidence,
    bool mini_kv_executes_downstream_checks,
    bool mini_kv_mutates_large_file_inventory,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_blocked_execution_rehearsal_split_stage_count == 20 &&
                               source_blocked_execution_rehearsal_split_complete;
    const bool node_plan_observed_only = !node_requires_fresh_minikv_evidence && !node_requires_fresh_java_evidence;
    const bool mini_kv_boundary_closed =
        !mini_kv_imports_node_modules && !mini_kv_executes_node_checks && !mini_kv_starts_services &&
        !mini_kv_reads_endpoints && !mini_kv_reads_credentials && !mini_kv_parses_historical_receipts &&
        !mini_kv_scans_java_evidence && !mini_kv_executes_downstream_checks &&
        !mini_kv_mutates_large_file_inventory;
    const bool counts_aligned = planned_check_count == 20 && completed_check_count == 20 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 20;
    const bool validation_passed =
        source_locked && node_plan_observed_only && mini_kv_boundary_closed && counts_aligned;

    return "{\"precheckUpstreamReceiptVerificationSplitNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationPublishedStageCount\":" +
           std::to_string(source_blocked_execution_rehearsal_split_stage_count) +
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationComplete\":" +
           json_bool(source_blocked_execution_rehearsal_split_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"nodeRequiresFreshJavaEvidence\":" + json_bool(node_requires_fresh_java_evidence) +
           ",\"nodePlanObservedOnly\":" + json_bool(node_plan_observed_only) +
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvParsesHistoricalReceipts\":" + json_bool(mini_kv_parses_historical_receipts) +
           ",\"miniKvScansJavaEvidence\":" + json_bool(mini_kv_scans_java_evidence) +
           ",\"miniKvExecutesDownstreamChecks\":" + json_bool(mini_kv_executes_downstream_checks) +
           ",\"miniKvMutatesLargeFileInventory\":" + json_bool(mini_kv_mutates_large_file_inventory) +
           ",\"miniKvBoundaryClosed\":" + json_bool(mini_kv_boundary_closed) +
           ",\"plannedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"sourceBlockedExecutionRehearsalSplitNonParticipationFixtureFrozen\":true"
           ",\"nodePrecheckUpstreamReceiptVerificationPublicLoaderImportedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationTypesImportedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationConstantsImportedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationReferencesParsedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationCheckPolicyExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationMessagePolicyExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationRendererExecutedByMiniKv\":false"
           ",\"nodePrecheckUpstreamReceiptVerificationEntrypointExecutedByMiniKv\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation_validation
