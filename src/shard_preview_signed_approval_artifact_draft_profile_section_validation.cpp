#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_artifact_draft_profile_section_validation_json(
    int source_candidate_profile_section_stage_count,
    bool source_candidate_profile_section_complete,
    int source_instruction_preflight_stage_count,
    bool source_instruction_preflight_complete,
    int signed_approval_draft_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_signed_approval_material,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool sources_frozen = source_candidate_profile_section_stage_count == 25 &&
                                source_candidate_profile_section_complete &&
                                source_instruction_preflight_stage_count == 25 &&
                                source_instruction_preflight_complete;
    const bool renderer_split_scope_aligned =
        signed_approval_draft_section_count == 5 && node_migrated_section_count == 5 &&
        !node_requires_fresh_minikv_evidence && !route_markdown_changed &&
        !mini_kv_consumes_signed_approval_material;
    const bool counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool validation_passed = sources_frozen && renderer_split_scope_aligned && counts_aligned;

    return "{\"signedApprovalArtifactDraftProfileSectionValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateProfileSectionPublishedStageCount\":" +
           std::to_string(source_candidate_profile_section_stage_count) +
           ",\"sourceCandidateProfileSectionComplete\":" + json_bool(source_candidate_profile_section_complete) +
           ",\"sourceDraftInstructionPreflightPublishedStageCount\":" +
           std::to_string(source_instruction_preflight_stage_count) +
           ",\"sourceDraftInstructionPreflightComplete\":" + json_bool(source_instruction_preflight_complete) +
           ",\"sourcesFrozen\":" + json_bool(sources_frozen) +
           ",\"signedApprovalArtifactDraftProfileSectionCount\":" +
           std::to_string(signed_approval_draft_section_count) +
           ",\"nodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":" +
           std::to_string(node_migrated_section_count) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"routeMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"miniKvConsumesSignedApprovalMaterial\":" + json_bool(mini_kv_consumes_signed_approval_material) +
           ",\"rendererSplitScopeAligned\":" + json_bool(renderer_split_scope_aligned) +
           ",\"plannedSignedApprovalArtifactDraftProfileSectionCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSignedApprovalArtifactDraftProfileSectionCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"signedApprovalCreated\":false"
           ",\"draftArtifactCreated\":false"
           ",\"runtimePayloadImported\":false"
           ",\"materialIntakeOpened\":false"
           ",\"governanceEchoCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_validation
