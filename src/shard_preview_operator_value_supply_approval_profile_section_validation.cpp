#include "minikv/shard_preview_operator_value_supply_approval_profile_section_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_operator_value_supply_approval_profile_section_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_operator_value_supply_approval_profile_section_validation_json(
    int source_approval_template_stage_count,
    bool source_approval_template_complete,
    int source_signed_approval_template_stage_count,
    bool source_signed_approval_template_complete,
    int source_capture_preflight_stage_count,
    bool source_capture_preflight_complete,
    int source_artifact_preflight_stage_count,
    bool source_artifact_preflight_complete,
    int approval_profile_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_approval_material,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool sources_frozen = source_approval_template_stage_count == 25 && source_approval_template_complete &&
                                source_signed_approval_template_stage_count == 25 &&
                                source_signed_approval_template_complete &&
                                source_capture_preflight_stage_count == 25 &&
                                source_capture_preflight_complete &&
                                source_artifact_preflight_stage_count == 25 &&
                                source_artifact_preflight_complete;
    const bool renderer_split_scope_aligned =
        approval_profile_section_count == 5 && node_migrated_section_count == 5 &&
        !node_requires_fresh_minikv_evidence && !route_markdown_changed &&
        !mini_kv_consumes_approval_material;
    const bool counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool validation_passed = sources_frozen && renderer_split_scope_aligned && counts_aligned;

    return "{\"operatorValueSupplyApprovalProfileSectionValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceApprovalTemplatePublishedStageCount\":" +
           std::to_string(source_approval_template_stage_count) +
           ",\"sourceApprovalTemplateComplete\":" + json_bool(source_approval_template_complete) +
           ",\"sourceSignedApprovalTemplatePublishedStageCount\":" +
           std::to_string(source_signed_approval_template_stage_count) +
           ",\"sourceSignedApprovalTemplateComplete\":" + json_bool(source_signed_approval_template_complete) +
           ",\"sourceSignedApprovalCapturePreflightPublishedStageCount\":" +
           std::to_string(source_capture_preflight_stage_count) +
           ",\"sourceSignedApprovalCapturePreflightComplete\":" + json_bool(source_capture_preflight_complete) +
           ",\"sourceSignedApprovalCaptureArtifactPreflightPublishedStageCount\":" +
           std::to_string(source_artifact_preflight_stage_count) +
           ",\"sourceSignedApprovalCaptureArtifactPreflightComplete\":" +
           json_bool(source_artifact_preflight_complete) +
           ",\"sourcesFrozen\":" + json_bool(sources_frozen) +
           ",\"operatorValueSupplyApprovalProfileSectionCount\":" +
           std::to_string(approval_profile_section_count) +
           ",\"nodeMigratedOperatorValueSupplyApprovalProfileSectionCount\":" +
           std::to_string(node_migrated_section_count) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"routeMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"miniKvConsumesApprovalMaterial\":" + json_bool(mini_kv_consumes_approval_material) +
           ",\"rendererSplitScopeAligned\":" + json_bool(renderer_split_scope_aligned) +
           ",\"plannedOperatorValueSupplyApprovalProfileSectionCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedOperatorValueSupplyApprovalProfileSectionCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"approvalMaterialImported\":false"
           ",\"approvalMaterialAccepted\":false"
           ",\"signedApprovalCreated\":false"
           ",\"signedApprovalCaptured\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"runtimePayloadImported\":false"
           ",\"secretValueRead\":false"
           ",\"materialIntakeOpened\":false"
           ",\"governanceEchoCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_operator_value_supply_approval_profile_section_validation
