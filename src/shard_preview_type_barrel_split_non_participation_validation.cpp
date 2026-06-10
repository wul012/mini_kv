#include "minikv/shard_preview_type_barrel_split_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_type_barrel_split_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_type_barrel_split_non_participation_validation_json(
    int source_catalog_split_stage_count,
    bool source_catalog_split_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_type_barrel_split,
    bool mini_kv_imports_node_types,
    bool mini_kv_consumes_profile_boundaries,
    bool mini_kv_reads_evidence_endpoint,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_catalog_split_stage_count == 30 && source_catalog_split_complete;
    const bool type_split_closed = !node_requires_fresh_minikv_evidence && !mini_kv_starts_type_barrel_split &&
                                   !mini_kv_imports_node_types && !mini_kv_consumes_profile_boundaries &&
                                   !mini_kv_reads_evidence_endpoint;
    const bool counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool validation_passed = source_locked && type_split_closed && counts_aligned;

    return "{\"typeBarrelSplitNonParticipationValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCatalogEntryGroupSplitPublishedStageCount\":" +
           std::to_string(source_catalog_split_stage_count) +
           ",\"sourceCatalogEntryGroupSplitComplete\":" + json_bool(source_catalog_split_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvStartsTypeBarrelSplit\":" + json_bool(mini_kv_starts_type_barrel_split) +
           ",\"miniKvImportsNodeTypes\":" + json_bool(mini_kv_imports_node_types) +
           ",\"miniKvConsumesProfileBoundaries\":" + json_bool(mini_kv_consumes_profile_boundaries) +
           ",\"miniKvReadsEvidenceEndpoint\":" + json_bool(mini_kv_reads_evidence_endpoint) +
           ",\"typeSplitClosed\":" + json_bool(type_split_closed) +
           ",\"plannedTypeBarrelSplitNonParticipationCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedTypeBarrelSplitNonParticipationCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
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
           ",\"miniKvTypeBarrelSplitStarted\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"evidenceEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"activeRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_type_barrel_split_non_participation_validation
