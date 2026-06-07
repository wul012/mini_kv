#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace stage_catalog =
    shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_stages;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1086-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-"
    "preflight-closeout-roadmap.md";
constexpr int current_artifact_preflight_stage_count = 14;

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
    return stage_catalog::signed_approval_capture_artifact_preflight_stages()
        [static_cast<std::size_t>(current_artifact_preflight_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stage_catalog::signed_approval_capture_artifact_preflight_stages(),
        current_artifact_preflight_stage_count,
        stage_catalog::planned_signed_approval_capture_artifact_preflight_stage_count(),
        stage_catalog::first_signed_approval_capture_artifact_preflight_release_number());
}

} // namespace

std::string format_signed_approval_capture_artifact_preflight_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stage_catalog::signed_approval_capture_artifact_preflight_stages(),
        current_artifact_preflight_stage_count);
}

std::string format_signed_approval_capture_artifact_preflight_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const bool source_chain_complete =
        shard_route_preview_operator_value_supply_signed_approval_capture_preflight::published_stage_count() == 25;
    const int fragment_count =
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments::
            planned_signed_approval_capture_artifact_preflight_fragment_count();
    const int seal_count =
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals::
            planned_signed_approval_capture_artifact_preflight_seal_count();
    const int gate_count =
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates::
            planned_signed_approval_capture_artifact_preflight_gate_count();
    const bool catalogs_aligned = fragment_count == seal_count && seal_count == gate_count;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-preflight.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\""
           ",\"signedApprovalCaptureArtifactPreflightMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-capture-artifact-preflight\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceSignedApprovalCapturePreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\""
           ",\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\""
           ",\"sourceSignedApprovalCapturePreflightFixturePath\":\"fixtures/release/shard-readiness-v710.json\""
           ",\"sourceSignedApprovalCapturePreflightArchiveRootHint\":\"e/710/\""
           ",\"sourceSignedApprovalCapturePreflightPublishedStageCount\":" +
           std::to_string(
               shard_route_preview_operator_value_supply_signed_approval_capture_preflight::published_stage_count()) +
           ",\"sourceSignedApprovalCapturePreflightPlannedStageCount\":25"
           ",\"sourceSignedApprovalCapturePreflightChainComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceSignedApprovalCapturePreflightDigestMarker\":" +
           json_string(shard_route_preview_operator_value_supply_signed_approval_capture_preflight::
                           signed_approval_capture_preflight_digest_marker()) +
           ",\"artifactPreflightStage\":" + json_string(stage.stage) +
           ",\"artifactPreflightStageSequence\":" + std::to_string(stage.sequence) +
           ",\"artifactPreflightReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"signedApprovalCaptureArtifactPreflightReleaseRangeStart\":\"v711\""
           ",\"signedApprovalCaptureArtifactPreflightReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_artifact_preflight_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stage_catalog::planned_signed_approval_capture_artifact_preflight_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"signedApprovalCaptureArtifactPreflightFragmentCount\":" + std::to_string(fragment_count) +
           ",\"signedApprovalCaptureArtifactPreflightSealCount\":" + std::to_string(seal_count) +
           ",\"signedApprovalCaptureArtifactPreflightGateCount\":" + std::to_string(gate_count) +
           ",\"signedApprovalCaptureArtifactPreflightCatalogsAligned\":" + json_bool(catalogs_aligned) +
           ",\"operatorValueSupplyApprovalPacketState\":\"capture-artifact-preflight-required-no-artifact-present\""
           ",\"signedApprovalCaptureArtifactPreflightState\":\"artifact-preflight-only-no-artifact-materialized\""
           ",\"signedApprovalCaptureArtifactPreflightDeclared\":true"
           ",\"signedApprovalCaptureArtifactPreflightOnly\":true"
           ",\"captureArtifactPreflightOnly\":true"
           ",\"sourceCapturePreflightPresent\":true"
           ",\"signedApprovalArtifactDraftPresent\":false"
           ",\"signedApprovalArtifactMaterialized\":false"
           ",\"artifactDigestMaterialized\":false"
           ",\"signatureCaptured\":false"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantEmitted\":false"
           ",\"artifactPreflightRequestIdRequired\":true"
           ",\"artifactPreflightRequestIdPresent\":false"
           ",\"manualCaptureWindowOpen\":false"
           ",\"captureChannelOpen\":false"
           ",\"signatureMaterialAccepted\":false"
           ",\"rawSignatureMaterialRejected\":true"
           ",\"submittedOperatorValueCount\":0"
           ",\"acceptedOperatorValueCount\":0"
           ",\"importedEvidenceValueCount\":0"
           ",\"persistedOperatorValueCount\":0"
           ",\"signedApprovalCaptureArtifactPreflightCheckCount\":25"
           ",\"signedApprovalCaptureArtifactPreflightPassedCount\":" +
           std::to_string(current_artifact_preflight_stage_count) +
           ",\"readyForSignedApprovalCaptureArtifactPreflightReview\":true"
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
           ",\"signedApprovalCaptureArtifactPreflightFragmentHelperApplied\":true"
           ",\"signedApprovalCaptureArtifactPreflightSealHelperApplied\":true"
           ",\"signedApprovalCaptureArtifactPreflightGateHelperApplied\":true"
           ",\"signedApprovalCaptureArtifactPreflightValidationHelperApplied\":true"
           ",\"moduleSplit\":[\"artifact_preflight_core\",\"artifact_preflight_stages\",\"artifact_preflight_fragments\",\"artifact_preflight_seals\",\"artifact_preflight_gates\",\"artifact_preflight_validation\"]"
           ",\"stageCatalog\":" + format_signed_approval_capture_artifact_preflight_stage_catalog_json() +
           ",\"artifactFragments\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments::
               format_signed_approval_capture_artifact_preflight_fragments_json(
                   current_artifact_preflight_stage_count) +
           ",\"artifactSeals\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals::
               format_signed_approval_capture_artifact_preflight_seals_json(current_artifact_preflight_stage_count) +
           ",\"artifactGates\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates::
               format_signed_approval_capture_artifact_preflight_gates_json(current_artifact_preflight_stage_count) +
           ",\"validation\":" +
           shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation::
               format_signed_approval_capture_artifact_preflight_validation_json(fragment_count,
                                                                                 seal_count,
                                                                                 gate_count,
                                                                                 source_chain_complete,
                                                                                 current_artifact_preflight_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1086 artifact preflight closeout plan is referenced as plan evidence only",
                              "source capture preflight remains frozen at v710",
                              "artifact fragments are cataloged separately",
                              "artifact seals are cataloged separately",
                              "artifact gates are cataloged separately",
                              "no signed approval artifact draft is materialized",
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

std::string signed_approval_capture_artifact_preflight_digest_marker() {
    const int planned_stage_count =
        stage_catalog::planned_signed_approval_capture_artifact_preflight_stage_count();
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_artifact_preflight_stage_count,
                                                                   planned_stage_count);
}

int published_stage_count() {
    return current_artifact_preflight_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight
