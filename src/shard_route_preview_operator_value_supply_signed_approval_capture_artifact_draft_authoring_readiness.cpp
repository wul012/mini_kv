#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace blockers =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers;
namespace preflight =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight;
namespace profiles =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles;
namespace requirements =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_requirements;
namespace stages =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_stages;
namespace validation =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_validation;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1186-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-"
    "draft-authoring-readiness-closeout-roadmap.md";
constexpr int current_authoring_readiness_stage_count = 14;

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
    return stages::signed_approval_capture_artifact_draft_authoring_readiness_stages()
        [static_cast<std::size_t>(current_authoring_readiness_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_capture_artifact_draft_authoring_readiness_stages(),
        current_authoring_readiness_stage_count,
        stages::planned_signed_approval_capture_artifact_draft_authoring_readiness_stage_count(),
        stages::first_signed_approval_capture_artifact_draft_authoring_readiness_release_number());
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_authoring_readiness_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_capture_artifact_draft_authoring_readiness_stages(),
        current_authoring_readiness_stage_count);
}

std::string format_signed_approval_capture_artifact_draft_authoring_readiness_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete = preflight::published_stage_count() == 25;
    const int requirement_count =
        requirements::planned_signed_approval_capture_artifact_draft_authoring_readiness_requirement_count();
    const int blocker_count =
        blockers::planned_signed_approval_capture_artifact_draft_authoring_readiness_blocker_count();
    const int profile_count =
        profiles::planned_signed_approval_capture_artifact_draft_authoring_readiness_profile_count();
    const bool catalogs_aligned =
        requirement_count == blocker_count && blocker_count == profile_count;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-authoring-readiness.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\""
           ",\"signedApprovalCaptureArtifactDraftAuthoringReadinessMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-artifact-draft-authoring-readiness\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceArtifactPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\""
           ",\"sourceArtifactPreflightReleaseVersion\":\"v735\""
           ",\"sourceArtifactPreflightFixturePath\":\"fixtures/release/shard-readiness-v735.json\""
           ",\"sourceArtifactPreflightArchiveRootHint\":\"e/735/\""
           ",\"sourceArtifactPreflightPublishedStageCount\":" +
           std::to_string(preflight::published_stage_count()) +
           ",\"sourceArtifactPreflightPlannedStageCount\":25"
           ",\"sourceArtifactPreflightChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceArtifactPreflightDigestMarker\":" +
           json_string(preflight::signed_approval_capture_artifact_preflight_digest_marker()) +
           ",\"authoringReadinessStage\":" + json_string(stage.stage) +
           ",\"authoringReadinessStageSequence\":" + std::to_string(stage.sequence) +
           ",\"authoringReadinessReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalCaptureArtifactDraftAuthoringReadinessReleaseRangeStart\":\"v736\""
           ",\"signedApprovalCaptureArtifactDraftAuthoringReadinessReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_authoring_readiness_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_capture_artifact_draft_authoring_readiness_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"authoringRequirementCount\":" + std::to_string(requirement_count) +
           ",\"authoringBlockerCount\":" + std::to_string(blocker_count) +
           ",\"authoringProfileCount\":" + std::to_string(profile_count) +
           ",\"authoringCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"signedApprovalCaptureArtifactDraftAuthoringReadinessDeclared\":true"
           ",\"signedApprovalCaptureArtifactDraftAuthoringReadinessOnly\":true"
           ",\"authoringReadinessOnly\":true"
           ",\"sourceArtifactPreflightPresent\":true"
           ",\"sourceArtifactPreflightClosed\":true"
           ",\"signedApprovalArtifactDraftPresent\":false"
           ",\"signedApprovalArtifactDraftMaterialized\":false"
           ",\"signedApprovalArtifactDraftTextGenerated\":false"
           ",\"signedApprovalArtifactDraftInstructionsGenerated\":false"
           ",\"approvalStatementTextGenerated\":false"
           ",\"detachedSignaturePayloadPresent\":false"
           ",\"signatureCaptured\":false"
           ",\"approvalCapturePerformed\":false"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantEmitted\":false"
           ",\"submittedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"persistedOperatorValueCount\":0"
           ",\"draftAuthoringRequirementCheckCount\":25"
           ",\"draftAuthoringRequirementPassedCount\":" +
           std::to_string(current_authoring_readiness_stage_count) +
           ",\"readyForDraftAuthoringReview\":true"
           ",\"readyForSignedApprovalArtifactDraft\":false"
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
           ",\"signedApprovalCaptureArtifactDraftAuthoringRequirementHelperApplied\":true"
           ",\"signedApprovalCaptureArtifactDraftAuthoringBlockerHelperApplied\":true"
           ",\"signedApprovalCaptureArtifactDraftAuthoringProfileHelperApplied\":true"
           ",\"signedApprovalCaptureArtifactDraftAuthoringValidationHelperApplied\":true"
           ",\"moduleSplit\":[\"draft_authoring_readiness_core\",\"draft_authoring_readiness_stages\",\"draft_authoring_readiness_requirements\",\"draft_authoring_readiness_blockers\",\"draft_authoring_readiness_profiles\",\"draft_authoring_readiness_validation\"]"
           ",\"stageCatalog\":" + format_signed_approval_capture_artifact_draft_authoring_readiness_stage_catalog_json() +
           ",\"authoringRequirements\":" +
           requirements::format_signed_approval_capture_artifact_draft_authoring_readiness_requirements_json(
               current_authoring_readiness_stage_count) +
           ",\"authoringBlockers\":" +
           blockers::format_signed_approval_capture_artifact_draft_authoring_readiness_blockers_json(
               current_authoring_readiness_stage_count) +
           ",\"authoringProfiles\":" +
           profiles::format_signed_approval_capture_artifact_draft_authoring_readiness_profiles_json(
               current_authoring_readiness_stage_count) +
           ",\"validation\":" +
           validation::format_signed_approval_capture_artifact_draft_authoring_readiness_validation_json(
               requirement_count,
               blocker_count,
               profile_count,
               source_chain_complete,
               current_authoring_readiness_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1186 authoring readiness closeout plan is referenced as plan evidence only",
                              "source artifact preflight remains frozen at v735",
                              "draft authoring requirements are cataloged separately",
                              "draft authoring blockers are cataloged separately",
                              "draft authoring profiles are cataloged separately",
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

std::string signed_approval_capture_artifact_draft_authoring_readiness_digest_marker() {
    const int planned_stage_count =
        stages::planned_signed_approval_capture_artifact_draft_authoring_readiness_stage_count();
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_authoring_readiness_stage_count,
                                                                   planned_stage_count);
}

int published_stage_count() {
    return current_authoring_readiness_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness
