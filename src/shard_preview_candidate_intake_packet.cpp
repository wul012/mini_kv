#include "minikv/shard_preview_candidate_intake_packet.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_candidate_intake_packet_guards.hpp"
#include "minikv/shard_preview_candidate_intake_packet_slots.hpp"
#include "minikv/shard_preview_candidate_intake_packet_stages.hpp"
#include "minikv/shard_preview_candidate_intake_packet_validation.hpp"
#include "minikv/shard_preview_candidate_submission_precheck.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_candidate_intake_packet {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace guards = shard_preview_candidate_intake_packet_guards;
namespace slots = shard_preview_candidate_intake_packet_slots;
namespace source_precheck = shard_preview_candidate_submission_precheck;
namespace stages = shard_preview_candidate_intake_packet_stages;
namespace validation = shard_preview_candidate_intake_packet_validation;

constexpr std::string_view intake_command = "SHARDROUTECANDIDATEINTAKEPACKETJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1421-controlled-read-only-shard-preview-candidate-document-intake-packet-closeout-roadmap.md";
constexpr int current_candidate_intake_packet_stage_count = 25;
constexpr int source_candidate_submission_precheck_planned_stage_count = 10;
constexpr int source_candidate_submission_precheck_release_number = 930;
constexpr int node_intake_packet_slot_count = 10;
constexpr int node_intake_packet_guard_count = 10;
constexpr int source_submission_precheck_checkpoint_count = 25;
constexpr int source_submission_precheck_validator_count = 25;
constexpr int carried_candidate_field_count = 20;
constexpr int intake_packet_gate_count = 35;

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
    return stages::candidate_intake_packet_stages()
        [static_cast<std::size_t>(current_candidate_intake_packet_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::candidate_intake_packet_stages(),
        current_candidate_intake_packet_stage_count,
        stages::planned_candidate_intake_packet_stage_count(),
        stages::first_candidate_intake_packet_release_number());
}

bool source_precheck_complete() {
    return source_precheck::published_stage_count() == source_candidate_submission_precheck_planned_stage_count;
}

} // namespace

std::string format_candidate_intake_packet_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::candidate_intake_packet_stages(),
        current_candidate_intake_packet_stage_count);
}

std::string candidate_intake_packet_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_candidate_intake_packet_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_precheck_stage_count = source_precheck::published_stage_count();
    const bool source_precheck_chain_complete = source_precheck_complete();
    const int planned_stage_count = stages::planned_candidate_intake_packet_stage_count();
    const int planned_slot_count = slots::planned_candidate_intake_packet_slot_count();
    const int planned_guard_count = guards::planned_candidate_intake_packet_guard_count();
    const bool intake_chain_complete =
        current_candidate_intake_packet_stage_count == planned_stage_count && source_precheck_chain_complete;

    return "{\"contract\":\"shard-route-preview-candidate-document-intake-packet.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(intake_command) +
           ",\"candidateIntakePacketMode\":\"controlled-read-only-intake-packet-handoff\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeIntakePacketReleaseVersion\":\"Node v1421\""
           ",\"sourceNodeSubmissionPrecheckReleaseVersion\":\"Node v1411\""
           ",\"sourceNodeIntakeSlotCount\":" + std::to_string(node_intake_packet_slot_count) +
           ",\"sourceNodeIntakeGuardCount\":" + std::to_string(node_intake_packet_guard_count) +
           ",\"sourceNodeCoveredSubmissionCheckpointCount\":" +
           std::to_string(source_submission_precheck_checkpoint_count) +
           ",\"sourceNodeCoveredSubmissionValidatorCount\":" +
           std::to_string(source_submission_precheck_validator_count) +
           ",\"sourceNodeIntakeGateCount\":" + std::to_string(intake_packet_gate_count) +
           ",\"sourceCandidateSubmissionPrecheckCommand\":\"SHARDROUTECANDIDATESUBMISSIONPRECHECKJSON\""
           ",\"sourceCandidateSubmissionPrecheckReleaseVersion\":\"v" +
           std::to_string(source_candidate_submission_precheck_release_number) + "\""
           ",\"sourceCandidateSubmissionPrecheckFixturePath\":\"fixtures/release/shard-readiness-v930.json\""
           ",\"sourceCandidateSubmissionPrecheckPublishedStageCount\":" +
           std::to_string(source_precheck_stage_count) +
           ",\"sourceCandidateSubmissionPrecheckPlannedStageCount\":" +
           std::to_string(source_candidate_submission_precheck_planned_stage_count) +
           ",\"sourceCandidateSubmissionPrecheckComplete\":" + json_bool(source_precheck_chain_complete) +
           ",\"sourceCandidateSubmissionPrecheckDigestMarker\":" +
           json_string(source_precheck::candidate_submission_precheck_digest_marker()) +
           ",\"sourceCandidateSubmissionPrecheckStatus\":" +
           json_string(source_precheck::candidate_submission_precheck_status()) +
           ",\"carriedCandidateFieldCount\":" + std::to_string(carried_candidate_field_count) +
           ",\"intakePacketGateCount\":" + std::to_string(intake_packet_gate_count) +
           ",\"candidateIntakePacketStage\":" + json_string(stage.stage) +
           ",\"candidateIntakePacketStageSequence\":" + std::to_string(stage.sequence) +
           ",\"candidateIntakePacketReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"candidateIntakePacketReleaseRangeStart\":\"v931\""
           ",\"candidateIntakePacketReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_candidate_intake_packet_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedIntakeSlotCount\":" + std::to_string(planned_slot_count) +
           ",\"completedIntakeSlotCount\":" + std::to_string(planned_slot_count) +
           ",\"plannedIntakeGuardCount\":" + std::to_string(planned_guard_count) +
           ",\"completedIntakeGuardCount\":" + std::to_string(planned_guard_count) +
           ",\"candidateIntakePacketDeclared\":true"
           ",\"candidateIntakePacketOnly\":true"
           ",\"candidateIntakePacketHandoffReady\":true"
           ",\"candidateIntakePacketChainComplete\":" + json_bool(intake_chain_complete) +
           ",\"sourceCandidateSubmissionPrecheckFrozen\":true"
           ",\"versionedSourceFixtureFrozen\":true"
           ",\"reviewedMaterialContractReady\":true"
           ",\"reviewedRealCandidateDocumentRequired\":true"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidateDocumentCreated\":false"
           ",\"candidateDocumentImported\":false"
           ",\"candidateDocumentStaged\":false"
           ",\"candidateDocumentMaterialized\":false"
           ",\"syntheticCandidateDocumentAccepted\":false"
           ",\"unreviewedCandidateDocumentAccepted\":false"
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
           ",\"additionalIntakeEchoCreated\":false"
           ",\"moduleSplit\":[\"candidate_intake_packet_core\",\"candidate_intake_packet_stages\",\"candidate_intake_packet_slots\",\"candidate_intake_packet_guards\",\"candidate_intake_packet_validation\"]"
           ",\"stageCatalog\":" + format_candidate_intake_packet_stage_catalog_json() +
           ",\"intakeSlots\":" + slots::format_candidate_intake_packet_slots_json(planned_slot_count) +
           ",\"intakeGuards\":" + guards::format_candidate_intake_packet_guards_json(planned_guard_count) +
           ",\"validation\":" +
           validation::format_candidate_intake_packet_validation_json(
               source_precheck_stage_count,
               source_precheck_chain_complete,
               planned_slot_count,
               planned_slot_count,
               planned_guard_count,
               planned_guard_count,
               current_candidate_intake_packet_stage_count,
               planned_stage_count) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1421 intake packet scope is consumed as read-only planning context",
                              "v930 submission precheck evidence is frozen before v931-v955 intake packet handoff",
                              "mini-kv records intake slots and guards but does not read reviewed material",
                              "candidate document intake remains closed until external reviewed material is supplied",
                              "document router write routing WAL touch payload import approval signature runtime import and execution remain disabled"}) +
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

std::string candidate_intake_packet_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_candidate_intake_packet_stage_count,
        stages::planned_candidate_intake_packet_stage_count());
}

int published_stage_count() {
    return current_candidate_intake_packet_stage_count;
}

} // namespace minikv::shard_preview_candidate_intake_packet
