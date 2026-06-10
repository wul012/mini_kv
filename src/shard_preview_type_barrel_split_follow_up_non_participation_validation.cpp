#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_type_barrel_split_follow_up_non_participation_validation_json(
    int source_type_barrel_split_stage_count,
    bool source_type_barrel_split_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_follow_up,
    bool mini_kv_imports_node_types,
    bool mini_kv_reads_profile_endpoint,
    bool mini_kv_executes_node_checks,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_type_barrel_split_stage_count == 25 && source_type_barrel_split_complete;
    const bool follow_up_closed = !node_requires_fresh_minikv_evidence && !mini_kv_starts_follow_up &&
                                  !mini_kv_imports_node_types && !mini_kv_reads_profile_endpoint &&
                                  !mini_kv_executes_node_checks;
    const bool counts_aligned = planned_check_count == 20 && completed_check_count == 20 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 20;
    const bool validation_passed = source_locked && follow_up_closed && counts_aligned;

    return "{\"typeBarrelSplitFollowUpNonParticipationValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceTypeBarrelSplitNonParticipationPublishedStageCount\":" +
           std::to_string(source_type_barrel_split_stage_count) +
           ",\"sourceTypeBarrelSplitNonParticipationComplete\":" + json_bool(source_type_barrel_split_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvStartsTypeBarrelFollowUp\":" + json_bool(mini_kv_starts_follow_up) +
           ",\"miniKvImportsNodeTypes\":" + json_bool(mini_kv_imports_node_types) +
           ",\"miniKvReadsProfileEndpoint\":" + json_bool(mini_kv_reads_profile_endpoint) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"followUpClosed\":" + json_bool(follow_up_closed) +
           ",\"plannedTypeBarrelSplitFollowUpNonParticipationCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedTypeBarrelSplitFollowUpNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true"
           ",\"nodeStableBarrelImportedByMiniKv\":false"
           ",\"nodeProfileTypesImportedByMiniKv\":false"
           ",\"nodeAggregateReexportsImportedByMiniKv\":false"
           ",\"nodeNamedProfileBoundaryConsumedByMiniKv\":false"
           ",\"nodeProfileBoundaryEndpointReadByMiniKv\":false"
           ",\"nodeRouteRendererTestsExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"miniKvTypeBarrelFollowUpStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"activeRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_validation
