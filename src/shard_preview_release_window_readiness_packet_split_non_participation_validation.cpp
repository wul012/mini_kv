#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_release_window_readiness_packet_split_non_participation_validation_json(
    int source_implementation_plan_stage_count,
    bool source_implementation_plan_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    bool mini_kv_reads_endpoints,
    bool mini_kv_reads_credentials,
    bool mini_kv_executes_release_window_packet,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_implementation_plan_stage_count == 3 && source_implementation_plan_complete;
    const bool node_plan_observed_only = !node_requires_fresh_minikv_evidence;
    const bool mini_kv_boundary_closed = !mini_kv_imports_node_modules && !mini_kv_executes_node_checks &&
                                         !mini_kv_starts_services && !mini_kv_reads_endpoints &&
                                         !mini_kv_reads_credentials && !mini_kv_executes_release_window_packet;
    const bool counts_aligned = planned_check_count == 15 && completed_check_count == 15 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 15;
    const bool validation_passed =
        source_locked && node_plan_observed_only && mini_kv_boundary_closed && counts_aligned;

    return "{\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceImplementationPlanUpstreamEchoCloseoutPublishedStageCount\":" +
           std::to_string(source_implementation_plan_stage_count) +
           ",\"sourceImplementationPlanUpstreamEchoCloseoutComplete\":" +
           json_bool(source_implementation_plan_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"nodePlanObservedOnly\":" + json_bool(node_plan_observed_only) +
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvExecutesReleaseWindowPacket\":" + json_bool(mini_kv_executes_release_window_packet) +
           ",\"miniKvBoundaryClosed\":" + json_bool(mini_kv_boundary_closed) +
           ",\"plannedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true"
           ",\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false"
           ",\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_validation
