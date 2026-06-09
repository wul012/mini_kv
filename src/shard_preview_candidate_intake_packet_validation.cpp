#include "minikv/shard_preview_candidate_intake_packet_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_intake_packet_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_intake_packet_validation_json(int source_precheck_stage_count,
                                                           bool source_precheck_complete,
                                                           int planned_slot_count,
                                                           int completed_slot_count,
                                                           int planned_guard_count,
                                                           int completed_guard_count,
                                                           int published_stage_count,
                                                           int planned_stage_count) {
    const bool source_precheck_frozen = source_precheck_stage_count == 10 && source_precheck_complete;
    const bool slot_counts_aligned = planned_slot_count == 10 && completed_slot_count == 10;
    const bool guard_counts_aligned = planned_guard_count == 10 && completed_guard_count == 10;
    const bool stage_counts_aligned = published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool no_side_effects = true;
    const bool validation_passed =
        source_precheck_frozen && slot_counts_aligned && guard_counts_aligned && stage_counts_aligned &&
        no_side_effects;

    return "{\"candidateIntakePacketValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateSubmissionPrecheckPublishedStageCount\":" + std::to_string(source_precheck_stage_count) +
           ",\"sourceCandidateSubmissionPrecheckComplete\":" + json_bool(source_precheck_complete) +
           ",\"plannedIntakeSlotCount\":" + std::to_string(planned_slot_count) +
           ",\"completedIntakeSlotCount\":" + std::to_string(completed_slot_count) +
           ",\"plannedIntakeGuardCount\":" + std::to_string(planned_guard_count) +
           ",\"completedIntakeGuardCount\":" + std::to_string(completed_guard_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"sourcePrecheckFrozen\":" + json_bool(source_precheck_frozen) +
           ",\"slotCountsAligned\":" + json_bool(slot_counts_aligned) +
           ",\"guardCountsAligned\":" + json_bool(guard_counts_aligned) +
           ",\"stageCountsAligned\":" + json_bool(stage_counts_aligned) +
           ",\"additionalIntakeEchoCreated\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidatePayloadImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"runtimePayloadImported\":false"
           ",\"documentRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_candidate_intake_packet_validation
