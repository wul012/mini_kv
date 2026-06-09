#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_profile_section.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_checks.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_stages.hpp"
#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace candidate_profile = shard_preview_candidate_profile_section;
namespace checks = shard_preview_signed_approval_artifact_draft_profile_section_checks;
namespace instruction_preflight =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight;
namespace stages = shard_preview_signed_approval_artifact_draft_profile_section_stages;
namespace validation = shard_preview_signed_approval_artifact_draft_profile_section_validation;

constexpr std::string_view profile_section_command =
    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1506-controlled-read-only-shard-preview-signed-approval-artifact-draft-profile-section-renderer-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_candidate_profile_section_planned_stage_count = 25;
constexpr int source_instruction_preflight_planned_stage_count = 25;
constexpr int signed_approval_draft_profile_section_count = 5;
constexpr int node_migrated_signed_approval_draft_profile_section_count = 5;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool route_markdown_changed = false;
constexpr bool mini_kv_consumes_signed_approval_material = false;

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
    return stages::signed_approval_artifact_draft_profile_section_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_artifact_draft_profile_section_stages(),
        current_stage_count,
        stages::planned_signed_approval_artifact_draft_profile_section_stage_count(),
        stages::first_signed_approval_artifact_draft_profile_section_release_number());
}

int completed_profile_section_check_count() {
    return current_stage_count;
}

bool source_candidate_profile_section_complete() {
    return candidate_profile::published_stage_count() == source_candidate_profile_section_planned_stage_count;
}

bool source_instruction_preflight_complete() {
    return instruction_preflight::published_stage_count() == source_instruction_preflight_planned_stage_count;
}

} // namespace

std::string format_signed_approval_artifact_draft_profile_section_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_artifact_draft_profile_section_stages(),
        current_stage_count);
}

std::string signed_approval_artifact_draft_profile_section_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_signed_approval_artifact_draft_profile_section_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_candidate_profile_stage_count = candidate_profile::published_stage_count();
    const bool source_candidate_profile_complete = source_candidate_profile_section_complete();
    const int source_instruction_preflight_stage_count = instruction_preflight::published_stage_count();
    const bool source_instruction_complete = source_instruction_preflight_complete();
    const int planned_check_count = checks::planned_signed_approval_artifact_draft_profile_section_check_count();
    const int check_count = completed_profile_section_check_count();
    const bool chain_complete =
        current_stage_count == stages::planned_signed_approval_artifact_draft_profile_section_stage_count() &&
        source_candidate_profile_complete && source_instruction_complete;

    return "{\"contract\":\"shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(profile_section_command) +
           ",\"signedApprovalArtifactDraftProfileSectionMode\":\"controlled-read-only-signed-approval-artifact-draft-profile-section-renderer-split-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\""
           ",\"sourceNodeSignedApprovalArtifactDraftProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewSignedApprovalArtifactDraftProfileSectionRenderer.ts\""
           ",\"sourceNodeSignedApprovalArtifactDraftProfileSectionGroupCount\":" +
           std::to_string(signed_approval_draft_profile_section_count) +
           ",\"sourceNodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":" +
           std::to_string(node_migrated_signed_approval_draft_profile_section_count) +
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRouteMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeVersionMarkers\":" +
           json_string_array({"Node v1111", "Node v1136", "Node v1161", "Node v1186", "Node v1211"}) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1035\""
           ",\"sourceCurrentShardReadinessFixturePath\":\"fixtures/release/shard-readiness-v1035.json\""
           ",\"sourceCandidateProfileSectionCommand\":\"SHARDROUTECANDIDATEPROFILESECTIONJSON\""
           ",\"sourceCandidateProfileSectionReleaseVersion\":\"v1035\""
           ",\"sourceCandidateProfileSectionPublishedStageCount\":" +
           std::to_string(source_candidate_profile_stage_count) +
           ",\"sourceCandidateProfileSectionComplete\":" + json_bool(source_candidate_profile_complete) +
           ",\"sourceCandidateProfileSectionDigestMarker\":" +
           json_string(candidate_profile::candidate_profile_section_digest_marker()) +
           ",\"sourceCandidateProfileSectionStatus\":" +
           json_string(candidate_profile::candidate_profile_section_status()) +
           ",\"sourceDraftInstructionPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\""
           ",\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\""
           ",\"sourceDraftInstructionPreflightFixturePath\":\"fixtures/release/shard-readiness-v785.json\""
           ",\"sourceDraftInstructionPreflightPublishedStageCount\":" +
           std::to_string(source_instruction_preflight_stage_count) +
           ",\"sourceDraftInstructionPreflightComplete\":" + json_bool(source_instruction_complete) +
           ",\"sourceDraftInstructionPreflightDigestMarker\":" +
           json_string(instruction_preflight::signed_approval_capture_artifact_draft_instruction_preflight_digest_marker()) +
           ",\"signedApprovalArtifactDraftProfileSectionStage\":" + json_string(stage.stage) +
           ",\"signedApprovalArtifactDraftProfileSectionStageSequence\":" + std::to_string(stage.sequence) +
           ",\"signedApprovalArtifactDraftProfileSectionReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalArtifactDraftProfileSectionReleaseRangeStart\":\"v1036\""
           ",\"signedApprovalArtifactDraftProfileSectionReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_artifact_draft_profile_section_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedSignedApprovalArtifactDraftProfileSectionCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSignedApprovalArtifactDraftProfileSectionCheckCount\":" + std::to_string(check_count) +
           ",\"signedApprovalArtifactDraftProfileSectionRendererSplitDeclared\":true"
           ",\"signedApprovalArtifactDraftProfileSectionOnly\":true"
           ",\"signedApprovalArtifactDraftProfileSectionChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceCurrentFixtureFrozen\":true"
           ",\"sourceDraftInstructionPreflightFrozen\":true"
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
           ",\"signedApprovalArtifactDraftProfileSectionRendererModuleImported\":false"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvConsumesSignedApprovalMaterial\":" + json_bool(mini_kv_consumes_signed_approval_material) +
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
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"moduleSplit\":[\"signed_approval_artifact_draft_profile_section_core\",\"signed_approval_artifact_draft_profile_section_stages\",\"signed_approval_artifact_draft_profile_section_checks\",\"signed_approval_artifact_draft_profile_section_validation\"]"
           ",\"stageCatalog\":" + format_signed_approval_artifact_draft_profile_section_stage_catalog_json() +
           ",\"profileSectionChecks\":" +
           checks::format_signed_approval_artifact_draft_profile_section_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_signed_approval_artifact_draft_profile_section_validation_json(
               source_candidate_profile_stage_count,
               source_candidate_profile_complete,
               source_instruction_preflight_stage_count,
               source_instruction_complete,
               signed_approval_draft_profile_section_count,
               node_migrated_signed_approval_draft_profile_section_count,
               node_requires_fresh_minikv_evidence,
               route_markdown_changed,
               mini_kv_consumes_signed_approval_material,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_signed_approval_artifact_draft_profile_section_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1506 is treated as a renderer ownership split, not a fresh evidence dependency",
                              "v1035 remains the frozen current fixture before v1036-v1060 profile-section evidence",
                              "v785 instruction preflight remains the signed approval draft source marker",
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

std::string signed_approval_artifact_draft_profile_section_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_signed_approval_artifact_draft_profile_section_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_signed_approval_artifact_draft_profile_section
