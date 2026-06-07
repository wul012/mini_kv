#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;

constexpr std::string_view source_node_plan =
    "docs/plans3/"
    "v1086-controlled-read-only-shard-preview-operator-evidence-value-supply-signed-approval-capture-artifact-"
    "preflight-closeout-roadmap.md";
constexpr int first_artifact_preflight_release_number = 711;
constexpr int current_artifact_preflight_stage_count = 2;
constexpr int planned_artifact_preflight_stage_count = 25;

constexpr std::array<StageRecord, planned_artifact_preflight_stage_count> artifact_preflight_stages = {{
    {1, "v711", "route-preview-value-supply-signed-approval-capture-artifact-preflight-request-id", "v710",
     "fixtures/release/shard-readiness-v710.json",
     "starts artifact preflight request id review without minting an artifact"},
    {2, "v712", "route-preview-value-supply-signed-approval-capture-artifact-preflight-capture-digest", "v711",
     "fixtures/release/shard-readiness-v711.json",
     "binds artifact preflight to the frozen v710 capture preflight digest"},
    {3, "v713", "route-preview-value-supply-signed-approval-capture-artifact-preflight-template-digest", "v712",
     "fixtures/release/shard-readiness-v712.json",
     "binds artifact preflight to the frozen signed approval template digest"},
    {4, "v714", "route-preview-value-supply-signed-approval-capture-artifact-preflight-review-digest", "v713",
     "fixtures/release/shard-readiness-v713.json",
     "keeps approval packet review digest as a required absent fragment"},
    {5, "v715", "route-preview-value-supply-signed-approval-capture-artifact-preflight-operator-identity", "v714",
     "fixtures/release/shard-readiness-v714.json",
     "records operator identity as a required artifact fragment without storing the value"},
    {6, "v716", "route-preview-value-supply-signed-approval-capture-artifact-preflight-operator-role", "v715",
     "fixtures/release/shard-readiness-v715.json",
     "records operator role as a required artifact fragment without storing the value"},
    {7, "v717", "route-preview-value-supply-signed-approval-capture-artifact-preflight-window-id", "v716",
     "fixtures/release/shard-readiness-v716.json",
     "keeps capture window id as review metadata without opening a window"},
    {8, "v718", "route-preview-value-supply-signed-approval-capture-artifact-preflight-channel-policy", "v717",
     "fixtures/release/shard-readiness-v717.json",
     "records channel policy while rejecting raw channel and endpoint values"},
    {9, "v719", "route-preview-value-supply-signed-approval-capture-artifact-preflight-signature-algorithm", "v718",
     "fixtures/release/shard-readiness-v718.json",
     "records signature algorithm metadata without parsing signature material"},
    {10, "v720", "route-preview-value-supply-signed-approval-capture-artifact-preflight-detached-signature",
     "v719", "fixtures/release/shard-readiness-v719.json",
     "keeps detached signature as a placeholder and does not capture bytes"},
    {11, "v721", "route-preview-value-supply-signed-approval-capture-artifact-preflight-signature-redaction",
     "v720", "fixtures/release/shard-readiness-v720.json",
     "locks raw signature material behind redaction policy"},
    {12, "v722", "route-preview-value-supply-signed-approval-capture-artifact-preflight-statement-digest",
     "v721", "fixtures/release/shard-readiness-v721.json",
     "keeps approval statement digest as a placeholder and does not fabricate text"},
    {13, "v723", "route-preview-value-supply-signed-approval-capture-artifact-preflight-source-version", "v722",
     "fixtures/release/shard-readiness-v722.json",
     "pins source evidence version without consuming rolling current evidence"},
    {14, "v724", "route-preview-value-supply-signed-approval-capture-artifact-preflight-source-file", "v723",
     "fixtures/release/shard-readiness-v723.json",
     "records source file id without runtime file lookup"},
    {15, "v725", "route-preview-value-supply-signed-approval-capture-artifact-preflight-source-snippet", "v724",
     "fixtures/release/shard-readiness-v724.json",
     "records source snippet id without runtime snippet lookup"},
    {16, "v726", "route-preview-value-supply-signed-approval-capture-artifact-preflight-redacted-value-digest",
     "v725", "fixtures/release/shard-readiness-v725.json",
     "requires redacted value digest while rejecting raw value material"},
    {17, "v727", "route-preview-value-supply-signed-approval-capture-artifact-preflight-value-shape", "v726",
     "fixtures/release/shard-readiness-v726.json",
     "records value shape while keeping payload absent"},
    {18, "v728", "route-preview-value-supply-signed-approval-capture-artifact-preflight-redaction-policy", "v727",
     "fixtures/release/shard-readiness-v727.json",
     "requires redaction policy and stores no unredacted evidence"},
    {19, "v729", "route-preview-value-supply-signed-approval-capture-artifact-preflight-provenance-policy", "v728",
     "fixtures/release/shard-readiness-v728.json",
     "requires provenance policy and performs no evidence import"},
    {20, "v730", "route-preview-value-supply-signed-approval-capture-artifact-preflight-no-raw-secret", "v729",
     "fixtures/release/shard-readiness-v729.json",
     "locks raw secret credential endpoint and signature material out of the artifact"},
    {21, "v731", "route-preview-value-supply-signed-approval-capture-artifact-preflight-no-grant", "v730",
     "fixtures/release/shard-readiness-v730.json",
     "proves artifact preflight emits no approval grant"},
    {22, "v732", "route-preview-value-supply-signed-approval-capture-artifact-preflight-zero-value-import", "v731",
     "fixtures/release/shard-readiness-v731.json",
     "proves artifact preflight imports zero operator values"},
    {23, "v733", "route-preview-value-supply-signed-approval-capture-artifact-preflight-no-write-route", "v732",
     "fixtures/release/shard-readiness-v732.json",
     "proves artifact preflight installs no active write route"},
    {24, "v734", "route-preview-value-supply-signed-approval-capture-artifact-preflight-sibling-non-mutation",
     "v733", "fixtures/release/shard-readiness-v733.json",
     "proves artifact preflight mutates no sibling project or evidence"},
    {25, "v735", "route-preview-value-supply-signed-approval-capture-artifact-preflight-closeout", "v734",
     "fixtures/release/shard-readiness-v734.json",
     "closes artifact preflight as read-only evidence before any signed artifact draft"},
}};

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
    return artifact_preflight_stages.at(static_cast<std::size_t>(current_artifact_preflight_stage_count - 1));
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        artifact_preflight_stages,
        current_artifact_preflight_stage_count,
        planned_artifact_preflight_stage_count,
        first_artifact_preflight_release_number);
}

} // namespace

std::string format_signed_approval_capture_artifact_preflight_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        artifact_preflight_stages,
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
           ",\"plannedStageCount\":" + std::to_string(planned_artifact_preflight_stage_count) +
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
           ",\"moduleSplit\":[\"artifact_preflight_core\",\"artifact_preflight_fragments\",\"artifact_preflight_seals\",\"artifact_preflight_gates\",\"artifact_preflight_validation\"]"
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
    return shard_route_preview_stage_catalog::format_digest_marker(current_stage(),
                                                                   current_artifact_preflight_stage_count,
                                                                   planned_artifact_preflight_stage_count);
}

int published_stage_count() {
    return current_artifact_preflight_stage_count;
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight
