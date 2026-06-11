#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_code_walkthrough_quality_gate_non_participation_validation_json(
    int source_runtime_gate_stage_count,
    bool source_runtime_gate_complete,
    bool node_requires_fresh_minikv_evidence,
    bool node_requires_fresh_java_evidence,
    bool mini_kv_reads_node_walkthrough_files,
    bool mini_kv_scans_node_repository,
    bool mini_kv_executes_node_quality_route,
    bool mini_kv_starts_node_services,
    bool mini_kv_starts_java_service,
    bool mini_kv_starts_mini_kv_service,
    bool mini_kv_mutates_node_docs,
    bool mini_kv_mutates_java_docs,
    bool placeholder_walkthrough_allowed,
    bool bulk_historical_relocation_allowed,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_runtime_gate_stage_count == 20 && source_runtime_gate_complete;
    const bool node_plan_observed_only = !node_requires_fresh_minikv_evidence && !node_requires_fresh_java_evidence;
    const bool mini_kv_boundary_closed =
        !mini_kv_reads_node_walkthrough_files && !mini_kv_scans_node_repository &&
        !mini_kv_executes_node_quality_route && !mini_kv_starts_node_services && !mini_kv_starts_java_service &&
        !mini_kv_starts_mini_kv_service && !mini_kv_mutates_node_docs && !mini_kv_mutates_java_docs;
    const bool quality_floor_closed = !placeholder_walkthrough_allowed && !bulk_historical_relocation_allowed;
    const bool counts_aligned = planned_check_count == 12 && completed_check_count == 12 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 6;
    const bool validation_passed =
        source_locked && node_plan_observed_only && mini_kv_boundary_closed && quality_floor_closed && counts_aligned;

    return "{\"codeWalkthroughQualityGateNonParticipationValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationPublishedStageCount\":" +
           std::to_string(source_runtime_gate_stage_count) +
           ",\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationComplete\":" +
           json_bool(source_runtime_gate_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"nodeRequiresFreshJavaEvidence\":" + json_bool(node_requires_fresh_java_evidence) +
           ",\"nodePlanObservedOnly\":" + json_bool(node_plan_observed_only) +
           ",\"miniKvReadsNodeWalkthroughFiles\":" + json_bool(mini_kv_reads_node_walkthrough_files) +
           ",\"miniKvScansNodeRepository\":" + json_bool(mini_kv_scans_node_repository) +
           ",\"miniKvExecutesNodeQualityRoute\":" + json_bool(mini_kv_executes_node_quality_route) +
           ",\"miniKvStartsNodeServices\":" + json_bool(mini_kv_starts_node_services) +
           ",\"miniKvStartsJavaService\":" + json_bool(mini_kv_starts_java_service) +
           ",\"miniKvStartsMiniKvService\":" + json_bool(mini_kv_starts_mini_kv_service) +
           ",\"miniKvMutatesNodeDocs\":" + json_bool(mini_kv_mutates_node_docs) +
           ",\"miniKvMutatesJavaDocs\":" + json_bool(mini_kv_mutates_java_docs) +
           ",\"miniKvBoundaryClosed\":" + json_bool(mini_kv_boundary_closed) +
           ",\"placeholderWalkthroughAllowed\":" + json_bool(placeholder_walkthrough_allowed) +
           ",\"bulkHistoricalRelocationAllowed\":" + json_bool(bulk_historical_relocation_allowed) +
           ",\"qualityFloorClosed\":" + json_bool(quality_floor_closed) +
           ",\"plannedCodeWalkthroughQualityGateNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedCodeWalkthroughQualityGateNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"walkthroughQualityGateOnly\":true"
           ",\"nodeWalkthroughFilesReadByMiniKv\":false"
           ",\"nodeQualityRouteExecutedByMiniKv\":false"
           ",\"unsafeProductionClaimAllowed\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_validation
