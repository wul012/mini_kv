#include "minikv/shard_preview_candidate_material_request.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_intake_packet.hpp"
#include "minikv/shard_preview_candidate_material_request_checks.hpp"
#include "minikv/shard_preview_candidate_material_request_items.hpp"
#include "minikv/shard_preview_candidate_material_request_stages.hpp"
#include "minikv/shard_preview_candidate_material_request_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_material_request {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_candidate_material_request_checks;
namespace items = shard_preview_candidate_material_request_items;
namespace source_intake = shard_preview_candidate_intake_packet;
namespace stages = shard_preview_candidate_material_request_stages;
namespace validation = shard_preview_candidate_material_request_validation;

constexpr std::string_view material_request_command = "SHARDROUTECANDIDATEMATERIALREQUESTJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1446-controlled-read-only-shard-preview-candidate-document-material-request-closeout-roadmap.md";
constexpr int current_candidate_material_request_stage_count = 10;
constexpr int source_candidate_intake_packet_planned_stage_count = 25;
constexpr int source_candidate_intake_packet_release_number = 955;
constexpr int node_material_request_item_count = 25;
constexpr int node_material_acceptance_check_count = 25;
constexpr int node_intake_packet_slot_count = 10;
constexpr int node_intake_packet_guard_count = 10;
constexpr int requested_material_field_count = 20;
constexpr int material_request_gate_count = 40;

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
    return stages::candidate_material_request_stages()
        [static_cast<std::size_t>(current_candidate_material_request_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::candidate_material_request_stages(),
        current_candidate_material_request_stage_count,
        stages::planned_candidate_material_request_stage_count(),
        stages::first_candidate_material_request_release_number());
}

bool source_intake_packet_complete() {
    return source_intake::published_stage_count() == source_candidate_intake_packet_planned_stage_count;
}

} // namespace

std::string format_candidate_material_request_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::candidate_material_request_stages(),
        current_candidate_material_request_stage_count);
}

std::string candidate_material_request_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_material_request_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_intake_stage_count = source_intake::published_stage_count();
    const bool source_intake_chain_complete = source_intake_packet_complete();
    const int planned_stage_count = stages::planned_candidate_material_request_stage_count();
    const int planned_item_count = items::planned_candidate_material_request_item_count();
    const int planned_check_count = checks::planned_candidate_material_acceptance_check_count();
    const bool material_request_chain_complete =
        current_candidate_material_request_stage_count == planned_stage_count && source_intake_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-document-material-request.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(material_request_command) +
           ",\"candidateMaterialRequestMode\":\"controlled-read-only-material-request-closeout\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\""
           ",\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\""
           ",\"sourceNodeMaterialRequestItemCount\":" + std::to_string(node_material_request_item_count) +
           ",\"sourceNodeAcceptanceCheckCount\":" + std::to_string(node_material_acceptance_check_count) +
           ",\"sourceNodeIntakeSlotCount\":" + std::to_string(node_intake_packet_slot_count) +
           ",\"sourceNodeIntakeGuardCount\":" + std::to_string(node_intake_packet_guard_count) +
           ",\"sourceNodeRequestedMaterialFieldCount\":" + std::to_string(requested_material_field_count) +
           ",\"sourceNodeMaterialRequestGateCount\":" + std::to_string(material_request_gate_count) +
           ",\"sourceCandidateIntakePacketCommand\":\"SHARDROUTECANDIDATEINTAKEPACKETJSON\""
           ",\"sourceCandidateIntakePacketReleaseVersion\":\"v" +
           std::to_string(source_candidate_intake_packet_release_number) + "\""
           ",\"sourceCandidateIntakePacketFixturePath\":\"fixtures/release/shard-readiness-v955.json\""
           ",\"sourceCandidateIntakePacketPublishedStageCount\":" + std::to_string(source_intake_stage_count) +
           ",\"sourceCandidateIntakePacketPlannedStageCount\":" +
           std::to_string(source_candidate_intake_packet_planned_stage_count) +
           ",\"sourceCandidateIntakePacketComplete\":" + json_bool(source_intake_chain_complete) +
           ",\"sourceCandidateIntakePacketDigestMarker\":" +
           json_string(source_intake::candidate_intake_packet_digest_marker()) +
           ",\"sourceCandidateIntakePacketStatus\":" +
           json_string(source_intake::candidate_intake_packet_status()) +
           ",\"requestedMaterialFieldCount\":" + std::to_string(requested_material_field_count) +
           ",\"materialRequestGateCount\":" + std::to_string(material_request_gate_count) +
           ",\"candidateMaterialRequestStage\":" + json_string(stage.stage) +
           ",\"candidateMaterialRequestStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateMaterialRequestReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateMaterialRequestReleaseRangeStart\":\"v956\""
           ",\"candidateMaterialRequestReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_candidate_material_request_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedMaterialRequestItemCount\":" + std::to_string(planned_item_count) +
           ",\"completedMaterialRequestItemCount\":" + std::to_string(planned_item_count) +
           ",\"plannedAcceptanceCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedAcceptanceCheckCount\":" + std::to_string(planned_check_count) +
           ",\"candidateMaterialRequestPackageDeclared\":true"
           ",\"candidateMaterialRequestOnly\":true"
           ",\"candidateMaterialRequestHandoffReady\":true"
           ",\"candidateMaterialRequestChainComplete\":" + json_bool(material_request_chain_complete) +
           ",\"sourceCandidateIntakePacketFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"externalReviewedMaterialRequested\":true"
           ",\"reviewedRealCandidateDocumentRequired\":true"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"materialRequestSatisfied\":false"
           ",\"realMaterialAccepted\":false"
           ",\"syntheticMaterialAccepted\":false"
           ",\"unreviewedMaterialAccepted\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidateDocumentCreated\":false"
           ",\"candidateDocumentImported\":false"
           ",\"candidateDocumentStaged\":false"
           ",\"candidateDocumentMaterialized\":false"
           ",\"candidateSubmissionAccepted\":false"
           ",\"candidateSubmissionImported\":false"
           ",\"candidatePayloadImported\":false"
           ",\"stagedCandidatePayloadImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"candidateDocumentAccepted\":false"
           ",\"candidateDocumentRejected\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"detachedSignaturePayloadParsed\":false"
           ",\"detachedSignaturePayloadVerified\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"additionalMaterialRequestEchoCreated\":false"
           ",\"moduleSplit\":[\"candidate_material_request_core\",\"candidate_material_request_stages\",\"candidate_material_request_items\",\"candidate_material_request_checks\",\"candidate_material_request_validation\"]"
           ",\"stageCatalog\":" + format_candidate_material_request_stage_catalog_json() +
           ",\"materialRequestItems\":" +
           items::format_candidate_material_request_items_json(planned_item_count) +
           ",\"materialAcceptanceChecks\":" +
           checks::format_candidate_material_acceptance_checks_json(planned_check_count) +
           ",\"validation\":" +
           validation::format_candidate_material_request_validation_json(
               source_intake_stage_count,
               source_intake_chain_complete,
               planned_item_count,
               planned_item_count,
               planned_check_count,
               planned_check_count,
               current_candidate_material_request_stage_count,
               planned_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1446 material request package is consumed as read-only planning context",
                              "v955 candidate intake packet evidence is frozen before v956-v965 material request closeout",
                              "mini-kv records request items and acceptance checks but does not read reviewed material",
                              "material request asks for external human-reviewed evidence without importing payloads",
                              "document intake router write routing WAL touch approval signature runtime import and execution remain disabled"}) +
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
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string candidate_material_request_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_candidate_material_request_stage_count,
        stages::planned_candidate_material_request_stage_count());
}

int published_stage_count() {
    return current_candidate_material_request_stage_count;
}

} // namespace minikv::shard_preview_candidate_material_request
