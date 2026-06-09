#include "minikv/shard_preview_candidate_profile_section_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_profile_section_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_profile_section_validation_json(int source_integrity_stage_count,
                                                             bool source_integrity_complete,
                                                             int candidate_section_count,
                                                             int node_migrated_section_count,
                                                             bool node_requires_fresh_minikv_evidence,
                                                             bool route_markdown_changed,
                                                             bool mini_kv_consumes_profile_material,
                                                             int planned_check_count,
                                                             int completed_check_count,
                                                             int published_stage_count,
                                                             int planned_stage_count) {
    const bool source_integrity_frozen = source_integrity_stage_count == 25 && source_integrity_complete;
    const bool renderer_split_scope_aligned = candidate_section_count == 5 && node_migrated_section_count == 5 &&
                                              !node_requires_fresh_minikv_evidence && !route_markdown_changed &&
                                              !mini_kv_consumes_profile_material;
    const bool profile_section_counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                                published_stage_count == planned_stage_count &&
                                                planned_stage_count == 25;
    const bool validation_passed =
        source_integrity_frozen && renderer_split_scope_aligned && profile_section_counts_aligned;

    return "{\"candidateProfileSectionValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityPublishedStageCount\":" +
           std::to_string(source_integrity_stage_count) +
           ",\"sourceCandidateMaterialSubmissionPrecheckIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"sourceIntegrityFrozen\":" + json_bool(source_integrity_frozen) +
           ",\"candidateProfileSectionCount\":" + std::to_string(candidate_section_count) +
           ",\"nodeMigratedCandidateProfileSectionCount\":" + std::to_string(node_migrated_section_count) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"routeMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"miniKvConsumesProfileMaterial\":" + json_bool(mini_kv_consumes_profile_material) +
           ",\"rendererSplitScopeAligned\":" + json_bool(renderer_split_scope_aligned) +
           ",\"plannedProfileSectionCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedProfileSectionCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"profileSectionCountsAligned\":" + json_bool(profile_section_counts_aligned) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"candidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"materialSubmissionOpened\":false"
           ",\"materialPayloadImported\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"documentRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_candidate_profile_section_validation
