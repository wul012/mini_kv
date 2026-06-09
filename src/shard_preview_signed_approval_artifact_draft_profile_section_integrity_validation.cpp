#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_artifact_draft_profile_section_integrity_validation_json(
    int source_profile_section_stage_count,
    bool source_profile_section_complete,
    int signed_approval_draft_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_signed_approval_material,
    bool signed_approval_created,
    bool material_intake_opened,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_profile_section_frozen =
        source_profile_section_stage_count == 25 && source_profile_section_complete;
    const bool profile_scope_aligned = signed_approval_draft_section_count == 5 &&
                                       node_migrated_section_count == 5 &&
                                       !node_requires_fresh_minikv_evidence &&
                                       !route_markdown_changed &&
                                       !mini_kv_consumes_signed_approval_material &&
                                       !signed_approval_created &&
                                       !material_intake_opened;
    const bool counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool validation_passed = source_profile_section_frozen && profile_scope_aligned && counts_aligned;

    return "{\"signedApprovalArtifactDraftProfileSectionIntegrityValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceProfileSectionPublishedStageCount\":" +
           std::to_string(source_profile_section_stage_count) +
           ",\"sourceProfileSectionComplete\":" + json_bool(source_profile_section_complete) +
           ",\"sourceProfileSectionFrozen\":" + json_bool(source_profile_section_frozen) +
           ",\"signedApprovalArtifactDraftProfileSectionCount\":" +
           std::to_string(signed_approval_draft_section_count) +
           ",\"nodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":" +
           std::to_string(node_migrated_section_count) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"routeMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"miniKvConsumesSignedApprovalMaterial\":" + json_bool(mini_kv_consumes_signed_approval_material) +
           ",\"signedApprovalCreated\":" + json_bool(signed_approval_created) +
           ",\"materialIntakeOpened\":" + json_bool(material_intake_opened) +
           ",\"profileScopeAligned\":" + json_bool(profile_scope_aligned) +
           ",\"plannedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"draftArtifactCreated\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"runtimePayloadImported\":false"
           ",\"governanceEchoCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation
