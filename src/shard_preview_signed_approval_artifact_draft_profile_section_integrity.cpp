#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_signed_approval_artifact_draft_profile_section_integrity_checks;
namespace profile_section = shard_preview_signed_approval_artifact_draft_profile_section;
namespace stages = shard_preview_signed_approval_artifact_draft_profile_section_integrity_stages;
namespace validation = shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation;

constexpr std::string_view integrity_command =
    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1506-controlled-read-only-shard-preview-signed-approval-artifact-draft-profile-section-renderer-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_profile_section_planned_stage_count = 25;
constexpr int signed_approval_draft_profile_section_count = 5;
constexpr int node_migrated_signed_approval_draft_profile_section_count = 5;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool route_markdown_changed = false;
constexpr bool mini_kv_consumes_signed_approval_material = false;
constexpr bool signed_approval_created = false;
constexpr bool material_intake_opened = false;

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
    return stages::signed_approval_artifact_draft_profile_section_integrity_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_artifact_draft_profile_section_integrity_stages(),
        current_stage_count,
        stages::planned_signed_approval_artifact_draft_profile_section_integrity_stage_count(),
        stages::first_signed_approval_artifact_draft_profile_section_integrity_release_number());
}

bool source_profile_section_complete() {
    return profile_section::published_stage_count() == source_profile_section_planned_stage_count;
}

int completed_integrity_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_signed_approval_artifact_draft_profile_section_integrity_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_artifact_draft_profile_section_integrity_stages(),
        current_stage_count);
}

std::string signed_approval_artifact_draft_profile_section_integrity_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_signed_approval_artifact_draft_profile_section_integrity_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_profile_stage_count = profile_section::published_stage_count();
    const bool source_profile_complete = source_profile_section_complete();
    const int planned_check_count =
        checks::planned_signed_approval_artifact_draft_profile_section_integrity_check_count();
    const int check_count = completed_integrity_check_count();
    const bool chain_complete =
        current_stage_count ==
            stages::planned_signed_approval_artifact_draft_profile_section_integrity_stage_count() &&
        source_profile_complete;

    return "{\"contract\":\"shard-route-preview-signed-approval-artifact-draft-profile-section-integrity.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(integrity_command) +
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityMode\":\"controlled-read-only-signed-approval-artifact-draft-profile-section-integrity-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\""
           ",\"sourceNodeSignedApprovalArtifactDraftProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewSignedApprovalArtifactDraftProfileSectionRenderer.ts\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRouteMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeVersionMarkers\":" +
           json_string_array({"Node v1111", "Node v1136", "Node v1161", "Node v1186", "Node v1211"}) +
           ",\"sourceProfileSectionCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\""
           ",\"sourceProfileSectionContract\":\"shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1\""
           ",\"sourceProfileSectionReleaseVersion\":\"v1060\""
           ",\"sourceProfileSectionFixturePath\":\"fixtures/release/shard-readiness-v1060.json\""
           ",\"sourceProfileSectionPublishedStageCount\":" + std::to_string(source_profile_stage_count) +
           ",\"sourceProfileSectionComplete\":" + json_bool(source_profile_complete) +
           ",\"sourceProfileSectionStatus\":" +
           json_string(profile_section::signed_approval_artifact_draft_profile_section_status()) +
           ",\"sourceProfileSectionDigestMarker\":" +
           json_string(profile_section::signed_approval_artifact_draft_profile_section_digest_marker()) +
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityStage\":" + json_string(stage.stage) +
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityReleaseRangeStart\":\"v1061\""
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_artifact_draft_profile_section_integrity_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":" +
           std::to_string(check_count) +
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityDeclared\":true"
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityOnly\":true"
           ",\"signedApprovalArtifactDraftProfileSectionIntegrityChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceProfileSectionFixtureFrozen\":true"
           ",\"sourceProfileSectionDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeRendererSplitObservedAsReadOnlyPlan\":true"
           ",\"routeFacingMarkdownStable\":true"
           ",\"routeFacingOutputChanged\":false"
           ",\"nodeRuntimeBehaviorAdded\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"signedApprovalArtifactDraftProfileSectionNames\":" +
           json_string_array({"signed-approval-artifact-draft-preflight",
                              "signed-approval-artifact-draft-readiness",
                              "signed-approval-artifact-draft-review-package-preflight",
                              "signed-approval-artifact-draft-authoring-readiness",
                              "signed-approval-artifact-draft-instruction-preflight"}) +
           ",\"signedApprovalArtifactDraftProfileSectionCount\":" +
           std::to_string(signed_approval_draft_profile_section_count) +
           ",\"nodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":" +
           std::to_string(node_migrated_signed_approval_draft_profile_section_count) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvConsumesSignedApprovalMaterial\":" + json_bool(mini_kv_consumes_signed_approval_material) +
           ",\"draftArtifactCreated\":false"
           ",\"draftArtifactImported\":false"
           ",\"signedApprovalCreated\":" + json_bool(signed_approval_created) +
           ",\"signedApprovalEmitted\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"governanceEchoCreated\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"materialIntakeOpened\":" + json_bool(material_intake_opened) +
           ",\"materialPayloadImported\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"moduleSplit\":[\"signed_approval_artifact_draft_profile_section_integrity_core\",\"signed_approval_artifact_draft_profile_section_integrity_stages\",\"signed_approval_artifact_draft_profile_section_integrity_checks\",\"signed_approval_artifact_draft_profile_section_integrity_validation\"]"
           ",\"stageCatalog\":" +
           format_signed_approval_artifact_draft_profile_section_integrity_stage_catalog_json() +
           ",\"integrityChecks\":" +
           checks::format_signed_approval_artifact_draft_profile_section_integrity_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_signed_approval_artifact_draft_profile_section_integrity_validation_json(
               source_profile_stage_count,
               source_profile_complete,
               signed_approval_draft_profile_section_count,
               node_migrated_signed_approval_draft_profile_section_count,
               node_requires_fresh_minikv_evidence,
               route_markdown_changed,
               mini_kv_consumes_signed_approval_material,
               signed_approval_created,
               material_intake_opened,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_signed_approval_artifact_draft_profile_section_integrity_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1060 is the frozen signed approval draft profile section source fixture",
                              "integrity evidence rechecks section counts and false boundary fields",
                              "Node v1506 remains renderer ownership context only",
                              "mini-kv does not create signed approval artifacts or consume reviewed material",
                              "router write routing WAL touch service startup runtime probes and execution remain disabled"}) +
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

std::string signed_approval_artifact_draft_profile_section_integrity_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_signed_approval_artifact_draft_profile_section_integrity_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity
