#include "minikv/shard_preview_candidate_material_request_integrity.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_material_request.hpp"
#include "minikv/shard_preview_candidate_material_request_integrity_checks.hpp"
#include "minikv/shard_preview_candidate_material_request_integrity_stages.hpp"
#include "minikv/shard_preview_candidate_material_request_integrity_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_material_request_integrity {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_candidate_material_request_integrity_checks;
namespace source_material_request = shard_preview_candidate_material_request;
namespace stages = shard_preview_candidate_material_request_integrity_stages;
namespace validation = shard_preview_candidate_material_request_integrity_validation;

constexpr std::string_view integrity_command = "SHARDROUTECANDIDATEMATERIALREQUESTINTEGRITYJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1446-controlled-read-only-shard-preview-candidate-document-material-request-closeout-roadmap.md";
constexpr int current_candidate_material_request_integrity_stage_count = 10;
constexpr int source_candidate_material_request_planned_stage_count = 10;
constexpr int source_material_request_item_count = 25;
constexpr int source_acceptance_check_count = 25;
constexpr int source_intake_packet_slot_count = 10;
constexpr int source_intake_packet_guard_count = 10;
constexpr int source_requested_material_field_count = 20;
constexpr int source_material_request_gate_count = 40;
constexpr int completed_integrity_check_count_value = 15;

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
    return stages::candidate_material_request_integrity_stages()
        [static_cast<std::size_t>(current_candidate_material_request_integrity_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::candidate_material_request_integrity_stages(),
        current_candidate_material_request_integrity_stage_count,
        stages::planned_candidate_material_request_integrity_stage_count(),
        stages::first_candidate_material_request_integrity_release_number());
}

bool source_material_request_complete() {
    return source_material_request::published_stage_count() == source_candidate_material_request_planned_stage_count;
}

int completed_integrity_check_count() {
    return completed_integrity_check_count_value;
}

} // namespace

std::string format_candidate_material_request_integrity_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::candidate_material_request_integrity_stages(),
        current_candidate_material_request_integrity_stage_count);
}

std::string candidate_material_request_integrity_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_material_request_integrity_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_material_request::published_stage_count();
    const bool source_chain_complete = source_material_request_complete();
    const int planned_stage_count = stages::planned_candidate_material_request_integrity_stage_count();
    const int planned_check_count = checks::planned_candidate_material_request_integrity_check_count();
    const int check_count = completed_integrity_check_count();
    const bool integrity_chain_complete =
        current_candidate_material_request_integrity_stage_count == planned_stage_count && source_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-material-request-fixture-integrity.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(integrity_command) +
           ",\"candidateMaterialRequestIntegrityMode\":\"controlled-read-only-material-request-fixture-integrity\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeMaterialRequestReleaseVersion\":\"Node v1446\""
           ",\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\""
           ",\"sourceCandidateMaterialRequestCommand\":\"SHARDROUTECANDIDATEMATERIALREQUESTJSON\""
           ",\"sourceCandidateMaterialRequestReleaseVersion\":\"v965\""
           ",\"sourceCandidateMaterialRequestFixturePath\":\"fixtures/release/shard-readiness-v965.json\""
           ",\"sourceCandidateMaterialRequestArchiveRootHint\":\"e/965/\""
           ",\"sourceCandidateMaterialRequestPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceCandidateMaterialRequestPlannedStageCount\":" +
           std::to_string(source_candidate_material_request_planned_stage_count) +
           ",\"sourceCandidateMaterialRequestComplete\":" + json_bool(source_chain_complete) +
           ",\"sourceCandidateMaterialRequestDigestMarker\":" +
           json_string(source_material_request::candidate_material_request_digest_marker()) +
           ",\"sourceCandidateMaterialRequestStatus\":" +
           json_string(source_material_request::candidate_material_request_status()) +
           ",\"sourceMaterialRequestItemCount\":" + std::to_string(source_material_request_item_count) +
           ",\"sourceAcceptanceCheckCount\":" + std::to_string(source_acceptance_check_count) +
           ",\"sourceIntakePacketSlotCount\":" + std::to_string(source_intake_packet_slot_count) +
           ",\"sourceIntakePacketGuardCount\":" + std::to_string(source_intake_packet_guard_count) +
           ",\"sourceRequestedMaterialFieldCount\":" + std::to_string(source_requested_material_field_count) +
           ",\"sourceMaterialRequestGateCount\":" + std::to_string(source_material_request_gate_count) +
           ",\"candidateMaterialRequestIntegrityStage\":" + json_string(stage.stage) +
           ",\"candidateMaterialRequestIntegrityStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateMaterialRequestIntegrityReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateMaterialRequestIntegrityReleaseRangeStart\":\"v966\""
           ",\"candidateMaterialRequestIntegrityReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" +
           std::to_string(current_candidate_material_request_integrity_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedIntegrityCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedIntegrityCheckCount\":" + std::to_string(check_count) +
           ",\"candidateMaterialRequestIntegrityDeclared\":true"
           ",\"candidateMaterialRequestIntegrityOnly\":true"
           ",\"candidateMaterialRequestIntegrityChainComplete\":" + json_bool(integrity_chain_complete) +
           ",\"sourceCandidateMaterialRequestFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"currentFixtureCarriesMaterialRequestIntegritySection\":true"
           ",\"nodeStopConditionRespected\":true"
           ",\"sourceMaterialRequestReadyForFutureReviewedMaterial\":true"
           ",\"additionalMaterialRequestEchoCreated\":false"
           ",\"newMaterialRequestCreated\":false"
           ",\"materialRequestReopened\":false"
           ",\"externalReviewedMaterialRequested\":true"
           ",\"externalReviewedMaterialConsumed\":false"
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
           ",\"moduleSplit\":[\"candidate_material_request_integrity_core\",\"candidate_material_request_integrity_stages\",\"candidate_material_request_integrity_checks\",\"candidate_material_request_integrity_validation\"]"
           ",\"stageCatalog\":" + format_candidate_material_request_integrity_stage_catalog_json() +
           ",\"integrityChecks\":" +
           checks::format_candidate_material_request_integrity_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_candidate_material_request_integrity_validation_json(
               source_stage_count,
               source_chain_complete,
               source_material_request_item_count,
               source_acceptance_check_count,
               source_material_request_gate_count,
               source_requested_material_field_count,
               planned_check_count,
               check_count,
               current_candidate_material_request_integrity_stage_count,
               planned_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1446 stop condition is honored by freezing v965 material request evidence instead of creating another request echo",
                              "v965 is versioned as fixtures/release/shard-readiness-v965.json before v966-v975 integrity hardening",
                              "mini-kv checks lineage counts command catalog fixture encoding archive references and no side effect boundaries only",
                              "reviewed real material remains absent unread unimported unaccepted and unevaluated",
                              "document intake router write routing WAL touch runtime payload import approval signature and execution remain disabled"}) +
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

std::string candidate_material_request_integrity_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_candidate_material_request_integrity_stage_count,
        stages::planned_candidate_material_request_integrity_stage_count());
}

int published_stage_count() {
    return current_candidate_material_request_integrity_stage_count;
}

} // namespace minikv::shard_preview_candidate_material_request_integrity
