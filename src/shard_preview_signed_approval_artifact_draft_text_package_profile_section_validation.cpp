#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_artifact_draft_text_package_profile_section_validation_json(
    int source_profile_integrity_stage_count,
    bool source_profile_integrity_complete,
    int source_review_preflight_stage_count,
    bool source_review_preflight_complete,
    int source_review_closeout_audit_stage_count,
    bool source_review_closeout_audit_complete,
    int source_comparison_closeout_audit_stage_count,
    bool source_comparison_closeout_audit_complete,
    int source_compared_package_intake_audit_stage_count,
    bool source_compared_package_intake_audit_complete,
    int text_package_profile_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_text_package_material,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool sources_frozen = source_profile_integrity_stage_count == 25 && source_profile_integrity_complete &&
                                source_review_preflight_stage_count == 25 && source_review_preflight_complete &&
                                source_review_closeout_audit_stage_count == 25 &&
                                source_review_closeout_audit_complete &&
                                source_comparison_closeout_audit_stage_count == 25 &&
                                source_comparison_closeout_audit_complete &&
                                source_compared_package_intake_audit_stage_count == 20 &&
                                source_compared_package_intake_audit_complete;
    const bool renderer_split_scope_aligned =
        text_package_profile_section_count == 9 && node_migrated_section_count == 9 &&
        !node_requires_fresh_minikv_evidence && !route_markdown_changed &&
        !mini_kv_consumes_text_package_material;
    const bool counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool validation_passed = sources_frozen && renderer_split_scope_aligned && counts_aligned;

    return "{\"signedApprovalArtifactDraftTextPackageProfileSectionValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceProfileIntegrityPublishedStageCount\":" + std::to_string(source_profile_integrity_stage_count) +
           ",\"sourceProfileIntegrityComplete\":" + json_bool(source_profile_integrity_complete) +
           ",\"sourceDraftTextPackageReviewPreflightPublishedStageCount\":" +
           std::to_string(source_review_preflight_stage_count) +
           ",\"sourceDraftTextPackageReviewPreflightComplete\":" + json_bool(source_review_preflight_complete) +
           ",\"sourceDraftTextPackageReviewCloseoutAuditPublishedStageCount\":" +
           std::to_string(source_review_closeout_audit_stage_count) +
           ",\"sourceDraftTextPackageReviewCloseoutAuditComplete\":" +
           json_bool(source_review_closeout_audit_complete) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditPublishedStageCount\":" +
           std::to_string(source_comparison_closeout_audit_stage_count) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditComplete\":" +
           json_bool(source_comparison_closeout_audit_complete) +
           ",\"sourceComparedPackageEvidenceIntakeAuditPublishedStageCount\":" +
           std::to_string(source_compared_package_intake_audit_stage_count) +
           ",\"sourceComparedPackageEvidenceIntakeAuditComplete\":" +
           json_bool(source_compared_package_intake_audit_complete) +
           ",\"sourcesFrozen\":" + json_bool(sources_frozen) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionCount\":" +
           std::to_string(text_package_profile_section_count) +
           ",\"nodeMigratedSignedApprovalArtifactDraftTextPackageProfileSectionCount\":" +
           std::to_string(node_migrated_section_count) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"routeMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"miniKvConsumesTextPackageMaterial\":" + json_bool(mini_kv_consumes_text_package_material) +
           ",\"rendererSplitScopeAligned\":" + json_bool(renderer_split_scope_aligned) +
           ",\"plannedSignedApprovalArtifactDraftTextPackageProfileSectionCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSignedApprovalArtifactDraftTextPackageProfileSectionCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"textPackageMaterialImported\":false"
           ",\"textPackageMaterialAccepted\":false"
           ",\"signedApprovalCreated\":false"
           ",\"draftArtifactCreated\":false"
           ",\"runtimePayloadImported\":false"
           ",\"secretValueRead\":false"
           ",\"materialIntakeOpened\":false"
           ",\"governanceEchoCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation
