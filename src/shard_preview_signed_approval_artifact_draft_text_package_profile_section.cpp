#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_signed_approval_artifact_draft_text_package_profile_section_checks;
namespace compared_intake =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit;
namespace comparison_audit =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit;
namespace profile_integrity = shard_preview_signed_approval_artifact_draft_profile_section_integrity;
namespace review_audit =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit;
namespace review_preflight =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight;
namespace stages = shard_preview_signed_approval_artifact_draft_text_package_profile_section_stages;
namespace validation = shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation;

constexpr std::string_view profile_section_command =
    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1531-controlled-read-only-shard-preview-signed-approval-artifact-draft-text-package-profile-section-renderer-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_profile_integrity_planned_stage_count = 25;
constexpr int source_review_preflight_planned_stage_count = 25;
constexpr int source_review_closeout_audit_planned_stage_count = 25;
constexpr int source_comparison_closeout_audit_planned_stage_count = 25;
constexpr int source_compared_package_intake_audit_planned_stage_count = 20;
constexpr int text_package_profile_section_count = 9;
constexpr int node_migrated_text_package_profile_section_count = 9;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool route_markdown_changed = false;
constexpr bool mini_kv_consumes_text_package_material = false;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

const StageRecord& current_stage() {
    return stages::signed_approval_artifact_draft_text_package_profile_section_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_artifact_draft_text_package_profile_section_stages(),
        current_stage_count,
        stages::planned_signed_approval_artifact_draft_text_package_profile_section_stage_count(),
        stages::first_signed_approval_artifact_draft_text_package_profile_section_release_number());
}

int completed_profile_section_check_count() {
    return current_stage_count;
}

bool source_profile_integrity_complete() {
    return profile_integrity::published_stage_count() == source_profile_integrity_planned_stage_count;
}

bool source_review_preflight_complete() {
    return review_preflight::published_stage_count() == source_review_preflight_planned_stage_count;
}

bool source_review_closeout_audit_complete() {
    return review_audit::published_stage_count() == source_review_closeout_audit_planned_stage_count;
}

bool source_comparison_closeout_audit_complete() {
    return comparison_audit::published_stage_count() == source_comparison_closeout_audit_planned_stage_count;
}

bool source_compared_package_intake_audit_complete() {
    return compared_intake::published_stage_count() == source_compared_package_intake_audit_planned_stage_count;
}

} // namespace

std::string format_signed_approval_artifact_draft_text_package_profile_section_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_artifact_draft_text_package_profile_section_stages(),
        current_stage_count);
}

std::string signed_approval_artifact_draft_text_package_profile_section_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_signed_approval_artifact_draft_text_package_profile_section_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_profile_integrity_stage_count = profile_integrity::published_stage_count();
    const bool profile_integrity_complete = source_profile_integrity_complete();
    const int review_preflight_stage_count = review_preflight::published_stage_count();
    const bool review_preflight_complete = source_review_preflight_complete();
    const int review_audit_stage_count = review_audit::published_stage_count();
    const bool review_audit_complete = source_review_closeout_audit_complete();
    const int comparison_audit_stage_count = comparison_audit::published_stage_count();
    const bool comparison_audit_complete = source_comparison_closeout_audit_complete();
    const int compared_intake_stage_count = compared_intake::published_stage_count();
    const bool compared_intake_complete = source_compared_package_intake_audit_complete();
    const int planned_check_count =
        checks::planned_signed_approval_artifact_draft_text_package_profile_section_check_count();
    const int check_count = completed_profile_section_check_count();
    const bool chain_complete =
        current_stage_count ==
            stages::planned_signed_approval_artifact_draft_text_package_profile_section_stage_count() &&
        profile_integrity_complete && review_preflight_complete && review_audit_complete &&
        comparison_audit_complete && compared_intake_complete;

    return "{\"contract\":\"shard-route-preview-signed-approval-artifact-draft-text-package-profile-section-renderer-split.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(profile_section_command) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionMode\":\"controlled-read-only-signed-approval-artifact-draft-text-package-profile-section-renderer-split-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeSignedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"Node v1531\""
           ",\"sourceNodeSignedApprovalArtifactDraftTextPackageProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewSignedApprovalArtifactDraftTextPackageProfileSectionRenderer.ts\""
           ",\"sourceNodeSignedApprovalArtifactDraftTextPackageSubmissionRendererModule\":\"controlledReadOnlyShardPreviewSignedApprovalArtifactDraftTextPackageSubmissionProfileSectionRenderer.ts\""
           ",\"sourceNodeSignedApprovalArtifactDraftTextPackageComparedEvidenceRendererModule\":\"controlledReadOnlyShardPreviewSignedApprovalArtifactDraftTextPackageComparedEvidenceProfileSectionRenderer.ts\""
           ",\"sourceNodeSignedApprovalArtifactDraftTextPackageProfileSectionGroupCount\":" +
           std::to_string(text_package_profile_section_count) +
           ",\"sourceNodeMigratedSignedApprovalArtifactDraftTextPackageProfileSectionCount\":" +
           std::to_string(node_migrated_text_package_profile_section_count) +
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRouteMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeVersionMarkers\":" +
           json_string_array({"Node v1236",
                              "Node v1261",
                              "Node v1286",
                              "Node v1311",
                              "Node v1321",
                              "Node v1331",
                              "Node v1351",
                              "Node v1361",
                              "Node v1371"}) +
           ",\"sourceNodeTypeCatalogOrderRange\":\"211-214\""
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1085\""
           ",\"sourceCurrentShardReadinessFixturePath\":\"fixtures/release/shard-readiness-v1085.json\""
           ",\"sourceSignedApprovalDraftProfileSectionIntegrityCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON\""
           ",\"sourceSignedApprovalDraftProfileSectionIntegrityReleaseVersion\":\"v1085\""
           ",\"sourceSignedApprovalDraftProfileSectionIntegrityPublishedStageCount\":" +
           std::to_string(source_profile_integrity_stage_count) +
           ",\"sourceSignedApprovalDraftProfileSectionIntegrityComplete\":" +
           json_bool(profile_integrity_complete) +
           ",\"sourceSignedApprovalDraftProfileSectionIntegrityDigestMarker\":" +
           json_string(profile_integrity::signed_approval_artifact_draft_profile_section_integrity_digest_marker()) +
           ",\"sourceSignedApprovalDraftProfileSectionIntegrityStatus\":" +
           json_string(profile_integrity::signed_approval_artifact_draft_profile_section_integrity_status()) +
           ",\"sourceDraftTextPackageReviewPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\""
           ",\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\""
           ",\"sourceDraftTextPackageReviewPreflightFixturePath\":\"fixtures/release/shard-readiness-v810.json\""
           ",\"sourceDraftTextPackageReviewPreflightPublishedStageCount\":" +
           std::to_string(review_preflight_stage_count) +
           ",\"sourceDraftTextPackageReviewPreflightComplete\":" + json_bool(review_preflight_complete) +
           ",\"sourceDraftTextPackageReviewPreflightDigestMarker\":" +
           json_string(review_preflight::signed_approval_capture_artifact_draft_text_package_review_preflight_digest_marker()) +
           ",\"sourceDraftTextPackageReviewCloseoutAuditCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\""
           ",\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\""
           ",\"sourceDraftTextPackageReviewCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v835.json\""
           ",\"sourceDraftTextPackageReviewCloseoutAuditPublishedStageCount\":" +
           std::to_string(review_audit_stage_count) +
           ",\"sourceDraftTextPackageReviewCloseoutAuditComplete\":" + json_bool(review_audit_complete) +
           ",\"sourceDraftTextPackageReviewCloseoutAuditDigestMarker\":" +
           json_string(review_audit::signed_approval_capture_artifact_draft_text_package_review_closeout_audit_digest_marker()) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON\""
           ",\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\""
           ",\"sourceDraftTextPackageComparisonCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v860.json\""
           ",\"sourceDraftTextPackageComparisonCloseoutAuditPublishedStageCount\":" +
           std::to_string(comparison_audit_stage_count) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditComplete\":" + json_bool(comparison_audit_complete) +
           ",\"sourceDraftTextPackageComparisonCloseoutAuditDigestMarker\":" +
           json_string(comparison_audit::signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_digest_marker()) +
           ",\"sourceComparedPackageEvidenceIntakeAuditCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPAREDPACKAGEEVIDENCEINTAKEAUDITJSON\""
           ",\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\""
           ",\"sourceComparedPackageEvidenceIntakeAuditFixturePath\":\"fixtures/release/shard-readiness-v880.json\""
           ",\"sourceComparedPackageEvidenceIntakeAuditPublishedStageCount\":" +
           std::to_string(compared_intake_stage_count) +
           ",\"sourceComparedPackageEvidenceIntakeAuditComplete\":" + json_bool(compared_intake_complete) +
           ",\"sourceComparedPackageEvidenceIntakeAuditDigestMarker\":" +
           json_string(compared_intake::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_digest_marker()) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionStage\":" + json_string(stage.stage) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionStageSequence\":" + std::to_string(stage.sequence) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseRangeStart\":\"v1086\""
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_artifact_draft_text_package_profile_section_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedSignedApprovalArtifactDraftTextPackageProfileSectionCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSignedApprovalArtifactDraftTextPackageProfileSectionCheckCount\":" +
           std::to_string(check_count) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionRendererSplitDeclared\":true"
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionOnly\":true"
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceCurrentFixtureFrozen\":true"
           ",\"sourceTextPackageEvidenceFrozen\":true"
           ",\"nodeRendererSplitObservedAsReadOnlyPlan\":true"
           ",\"routeFacingMarkdownStable\":true"
           ",\"routeFacingOutputChanged\":false"
           ",\"nodeRuntimeBehaviorAdded\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionNames\":" +
           json_string_array({"text-package-intake",
                              "text-package-review-preflight",
                              "text-package-submission-preflight",
                              "text-package-comparison-preflight",
                              "text-package-comparison-acceptance-precheck",
                              "compared-package-evidence-intake",
                              "compared-evidence-evaluation-preflight",
                              "compared-evidence-candidate",
                              "compared-evidence-candidate-intake"}) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionCount\":" +
           std::to_string(text_package_profile_section_count) +
           ",\"nodeMigratedSignedApprovalArtifactDraftTextPackageProfileSectionCount\":" +
           std::to_string(node_migrated_text_package_profile_section_count) +
           ",\"signedApprovalArtifactDraftTextPackageProfileSectionRendererModuleImported\":false"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvConsumesTextPackageMaterial\":" + json_bool(mini_kv_consumes_text_package_material) +
           ",\"textPackageMaterialImported\":false"
           ",\"textPackageMaterialAccepted\":false"
           ",\"comparedPackageEvidenceAccepted\":false"
           ",\"draftArtifactCreated\":false"
           ",\"draftArtifactImported\":false"
           ",\"signedApprovalCreated\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"governanceEchoCreated\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"materialIntakeOpened\":false"
           ",\"materialPayloadImported\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"secretValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"moduleSplit\":[\"signed_approval_artifact_draft_text_package_profile_section_core\",\"signed_approval_artifact_draft_text_package_profile_section_stages\",\"signed_approval_artifact_draft_text_package_profile_section_checks\",\"signed_approval_artifact_draft_text_package_profile_section_validation\"]"
           ",\"stageCatalog\":" +
           format_signed_approval_artifact_draft_text_package_profile_section_stage_catalog_json() +
           ",\"profileSectionChecks\":" +
           checks::format_signed_approval_artifact_draft_text_package_profile_section_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_signed_approval_artifact_draft_text_package_profile_section_validation_json(
               source_profile_integrity_stage_count,
               profile_integrity_complete,
               review_preflight_stage_count,
               review_preflight_complete,
               review_audit_stage_count,
               review_audit_complete,
               comparison_audit_stage_count,
               comparison_audit_complete,
               compared_intake_stage_count,
               compared_intake_complete,
               text_package_profile_section_count,
               node_migrated_text_package_profile_section_count,
               node_requires_fresh_minikv_evidence,
               route_markdown_changed,
               mini_kv_consumes_text_package_material,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_signed_approval_artifact_draft_text_package_profile_section_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1531 is treated as a text package renderer ownership split, not a fresh evidence dependency",
                              "v1085 remains the frozen current fixture before v1086-v1110 text package profile-section evidence",
                              "v810 v835 v860 and v880 remain the frozen text package source lineage",
                              "mini-kv does not create signed approval artifacts or consume text package material",
                              "router write routing WAL touch service startup runtime probes secret reads and execution remain disabled"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsSiblingServices\":false"
           ",\"liveReadAllowed\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string signed_approval_artifact_draft_text_package_profile_section_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_signed_approval_artifact_draft_text_package_profile_section_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section
