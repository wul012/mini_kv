#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_stages.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace authoring =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness;
namespace guards =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards;
namespace profiles =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles;
namespace slots =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots;
namespace stages =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_stages;
namespace validation =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_validation;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1211-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-"
    "draft-instruction-preflight-closeout-roadmap.md";
constexpr int current_instruction_preflight_stage_count = 10;

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
    return stages::signed_approval_capture_artifact_draft_instruction_preflight_stages()
        [static_cast<std::size_t>(current_instruction_preflight_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::signed_approval_capture_artifact_draft_instruction_preflight_stages(),
        current_instruction_preflight_stage_count,
        stages::planned_signed_approval_capture_artifact_draft_instruction_preflight_stage_count(),
        stages::first_signed_approval_capture_artifact_draft_instruction_preflight_release_number());
}

} // namespace

std::string format_signed_approval_capture_artifact_draft_instruction_preflight_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::signed_approval_capture_artifact_draft_instruction_preflight_stages(),
        current_instruction_preflight_stage_count);
}

std::string format_signed_approval_capture_artifact_draft_instruction_preflight_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete = authoring::published_stage_count() == 25;
    const int slot_count =
        slots::planned_signed_approval_capture_artifact_draft_instruction_preflight_slot_count();
    const int guard_count =
        guards::planned_signed_approval_capture_artifact_draft_instruction_preflight_guard_count();
    const int profile_count =
        profiles::planned_signed_approval_capture_artifact_draft_instruction_preflight_profile_count();
    const bool catalogs_aligned = slot_count == guard_count && guard_count == profile_count;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-instruction-preflight.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\""
           ",\"signedApprovalCaptureArtifactDraftInstructionPreflightMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-artifact-draft-instruction-preflight\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceDraftAuthoringReadinessCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\""
           ",\"sourceDraftAuthoringReadinessReleaseVersion\":\"v760\""
           ",\"sourceDraftAuthoringReadinessFixturePath\":\"fixtures/release/shard-readiness-v760.json\""
           ",\"sourceDraftAuthoringReadinessArchiveRootHint\":\"e/760/\""
           ",\"sourceDraftAuthoringReadinessPublishedStageCount\":" +
           std::to_string(authoring::published_stage_count()) +
           ",\"sourceDraftAuthoringReadinessPlannedStageCount\":25"
           ",\"sourceDraftAuthoringReadinessChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceDraftAuthoringReadinessDigestMarker\":" +
           json_string(authoring::signed_approval_capture_artifact_draft_authoring_readiness_digest_marker()) +
           ",\"instructionPreflightStage\":" + json_string(stage.stage) +
           ",\"instructionPreflightStageSequence\":" + std::to_string(stage.sequence) +
           ",\"instructionPreflightReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalCaptureArtifactDraftInstructionPreflightReleaseRangeStart\":\"v761\""
           ",\"signedApprovalCaptureArtifactDraftInstructionPreflightReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_instruction_preflight_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_signed_approval_capture_artifact_draft_instruction_preflight_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"instructionSlotCount\":" + std::to_string(slot_count) +
           ",\"instructionGuardCount\":" + std::to_string(guard_count) +
           ",\"instructionProfileCount\":" + std::to_string(profile_count) +
           ",\"instructionCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"draftInstructionSlotPreparedCount\":" + std::to_string(current_instruction_preflight_stage_count) +
           ",\"materializedInstructionTextCount\":0"
           ",\"signedApprovalCaptureArtifactDraftInstructionPreflightDeclared\":true"
           ",\"signedApprovalCaptureArtifactDraftInstructionPreflightOnly\":true"
           ",\"draftInstructionPreflightOnly\":true"
           ",\"sourceDraftAuthoringReadinessPresent\":true"
           ",\"sourceDraftAuthoringReadinessClosed\":true"
           ",\"signedApprovalArtifactDraftPresent\":false"
           ",\"signedApprovalArtifactDraftMaterialized\":false"
           ",\"signedApprovalArtifactDraftTextGenerated\":false"
           ",\"signedApprovalArtifactDraftInstructionsGenerated\":false"
           ",\"draftInstructionTextGenerated\":false"
           ",\"draftInstructionTextMaterialized\":false"
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
           ",\"readyForDraftInstructionReview\":true"
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
           ",\"instructionPreflightSlotHelperApplied\":true"
           ",\"instructionPreflightGuardHelperApplied\":true"
           ",\"instructionPreflightProfileHelperApplied\":true"
           ",\"instructionPreflightValidationHelperApplied\":true"
           ",\"moduleSplit\":[\"draft_instruction_preflight_core\",\"draft_instruction_preflight_stages\",\"draft_instruction_preflight_slots\",\"draft_instruction_preflight_guards\",\"draft_instruction_preflight_profiles\",\"draft_instruction_preflight_validation\"]"
           ",\"stageCatalog\":" + format_signed_approval_capture_artifact_draft_instruction_preflight_stage_catalog_json() +
           ",\"instructionSlots\":" +
           slots::format_signed_approval_capture_artifact_draft_instruction_preflight_slots_json(
               current_instruction_preflight_stage_count) +
           ",\"instructionGuards\":" +
           guards::format_signed_approval_capture_artifact_draft_instruction_preflight_guards_json(
               current_instruction_preflight_stage_count) +
           ",\"instructionProfiles\":" +
           profiles::format_signed_approval_capture_artifact_draft_instruction_preflight_profiles_json(
               current_instruction_preflight_stage_count) +
           ",\"validation\":" +
           validation::format_signed_approval_capture_artifact_draft_instruction_preflight_validation_json(
               slot_count,
               guard_count,
               profile_count,
               source_chain_complete,
               current_instruction_preflight_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1211 draft instruction preflight plan is referenced as plan evidence only",
                              "source draft authoring readiness remains frozen at v760",
                              "instruction slots are cataloged without materialized instruction text",
                              "instruction guards block draft text signatures values write routes and execution",
                              "instruction profiles are separated for maintenance",
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

std::string signed_approval_capture_artifact_draft_instruction_preflight_digest_marker() {
    const int planned_stage_count =
        stages::planned_signed_approval_capture_artifact_draft_instruction_preflight_stage_count();
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_instruction_preflight_stage_count,
                                                                   planned_stage_count);
}

int published_stage_count() {
    return current_instruction_preflight_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight