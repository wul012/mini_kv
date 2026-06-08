#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_stages.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace instruction =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight;
namespace controls =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards;
namespace profiles =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles;
namespace criteria =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_slots;
namespace stages =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_stages;
namespace validation =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1261-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-"
    "draft-text-package-review-preflight-closeout-roadmap.md";
constexpr int current_draft_text_package_review_preflight_stage_count = 2;

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
    return stages::signed_approval_capture_artifact_draft_text_package_review_preflight_stages()
        [static_cast<std::size_t>(current_draft_text_package_review_preflight_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_capture_artifact_draft_text_package_review_preflight_stages(),
        current_draft_text_package_review_preflight_stage_count,
        stages::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_stage_count(),
        stages::first_signed_approval_capture_artifact_draft_text_package_review_preflight_release_number());
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_text_package_review_preflight_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_capture_artifact_draft_text_package_review_preflight_stages(),
        current_draft_text_package_review_preflight_stage_count);
}

std::string format_signed_approval_capture_artifact_draft_text_package_review_preflight_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete = instruction::published_stage_count() == 25;
    const int criterion_count =
        criteria::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_slot_count();
    const int rejection_control_count =
        controls::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_guard_count();
    const int profile_count =
        profiles::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_profile_count();
    const bool catalogs_aligned =
        criterion_count == rejection_control_count && rejection_control_count == profile_count;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\""
           ",\"signedApprovalCaptureArtifactDraftTextPackageReviewPreflightMode\":\"controlled-read-only-disabled-draft-text-package-review-preflight\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceDraftInstructionPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\""
           ",\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\""
           ",\"sourceDraftInstructionPreflightFixturePath\":\"fixtures/release/shard-readiness-v785.json\""
           ",\"sourceDraftInstructionPreflightArchiveRootHint\":\"e/785/\""
           ",\"sourceDraftInstructionPreflightPublishedStageCount\":" +
           std::to_string(instruction::published_stage_count()) +
           ",\"sourceDraftInstructionPreflightPlannedStageCount\":25"
           ",\"sourceDraftInstructionPreflightChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceDraftInstructionPreflightDigestMarker\":" +
           json_string(instruction::signed_approval_capture_artifact_draft_instruction_preflight_digest_marker()) +
           ",\"draftTextPackageReviewPreflightStage\":" + json_string(stage.stage) +
           ",\"draftTextPackageReviewPreflightStageSequence\":" + std::to_string(stage.sequence) +
           ",\"draftTextPackageReviewPreflightReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalCaptureArtifactDraftTextPackageReviewPreflightReleaseRangeStart\":\"v786\""
           ",\"signedApprovalCaptureArtifactDraftTextPackageReviewPreflightReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_draft_text_package_review_preflight_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"reviewCriterionCount\":" + std::to_string(criterion_count) +
           ",\"rejectionControlCount\":" + std::to_string(rejection_control_count) +
           ",\"reviewProfileCount\":" + std::to_string(profile_count) +
           ",\"reviewCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"preparedReviewCriterionCount\":" +
           std::to_string(current_draft_text_package_review_preflight_stage_count) +
           ",\"parsedDraftTextPackageCount\":0"
           ",\"acceptedDraftTextPackageCount\":0"
           ",\"draftTextPackageReviewPreflightDeclared\":true"
           ",\"draftTextPackageReviewPreflightOnly\":true"
           ",\"sourceDraftInstructionPreflightPresent\":true"
           ",\"sourceDraftInstructionPreflightClosed\":true"
           ",\"draftTextPackagePresent\":false"
           ",\"draftTextPackageParsed\":false"
           ",\"draftTextPackageAccepted\":false"
           ",\"draftTextPackageMaterialized\":false"
           ",\"signedApprovalArtifactDraftPresent\":false"
           ",\"signedApprovalArtifactDraftTextGenerated\":false"
           ",\"detachedSignaturePayloadPresent\":false"
           ",\"signatureCaptured\":false"
           ",\"approvalCapturePerformed\":false"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantEmitted\":false"
           ",\"submittedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"persistedOperatorValueCount\":0"
           ",\"readyForDraftTextPackageReview\":true"
           ",\"readyForSignedApprovalCapture\":false"
           ",\"operatorValueSubmitted\":false"
           ",\"operatorValueAccepted\":false"
           ",\"automaticSiblingImportAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"secretValueStored\":false"
           ",\"credentialValueStored\":false"
           ",\"rawEndpointStored\":false"
           ",\"rawSignatureMaterialStored\":false"
           ",\"draftTextStored\":false"
           ",\"draftTextPackageReviewCriterionHelperApplied\":true"
           ",\"draftTextPackageReviewRejectionControlHelperApplied\":true"
           ",\"draftTextPackageReviewProfileHelperApplied\":true"
           ",\"draftTextPackageReviewValidationHelperApplied\":true"
           ",\"moduleSplit\":[\"draft_text_package_review_preflight_core\",\"draft_text_package_review_preflight_stages\",\"draft_text_package_review_preflight_criteria\",\"draft_text_package_review_preflight_rejection_controls\",\"draft_text_package_review_preflight_profiles\",\"draft_text_package_review_preflight_validation\"]"
           ",\"stageCatalog\":" + format_signed_approval_capture_artifact_draft_text_package_review_preflight_stage_catalog_json() +
           ",\"reviewCriteria\":" +
           criteria::format_signed_approval_capture_artifact_draft_text_package_review_preflight_slots_json(
               current_draft_text_package_review_preflight_stage_count) +
           ",\"rejectionControls\":" +
           controls::format_signed_approval_capture_artifact_draft_text_package_review_preflight_guards_json(
               current_draft_text_package_review_preflight_stage_count) +
           ",\"reviewProfiles\":" +
           profiles::format_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles_json(
               current_draft_text_package_review_preflight_stage_count) +
           ",\"validation\":" +
           validation::format_signed_approval_capture_artifact_draft_text_package_review_preflight_validation_json(
               criterion_count,
               rejection_control_count,
               profile_count,
               source_chain_complete,
               current_draft_text_package_review_preflight_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1261 review preflight plan is referenced as plan evidence only",
                              "source draft instruction preflight remains frozen at v785",
                              "review criteria are cataloged without parsing a draft text package",
                              "rejection controls block unreviewable package material, signatures, values, write routes, and execution",
                              "review profiles are separated for maintenance",
                              "no signed approval artifact draft text is generated",
                              "no approval grant is emitted",
                              "no value import or runtime payload is accepted",
                              "no write route, router activation, WAL touch, or sibling mutation occurs"}) +
           ",\"activeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"liveReadAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string signed_approval_capture_artifact_draft_text_package_review_preflight_digest_marker() {
    const int planned_stage_count =
        stages::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_stage_count();
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_draft_text_package_review_preflight_stage_count,
                                                                   planned_stage_count);
}

int published_stage_count() {
    return current_draft_text_package_review_preflight_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight
