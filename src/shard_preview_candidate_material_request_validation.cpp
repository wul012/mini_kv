#include "minikv/shard_preview_candidate_material_request_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_material_request_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_material_request_validation_json(int source_intake_packet_stage_count,
                                                              bool source_intake_packet_complete,
                                                              int planned_item_count,
                                                              int completed_item_count,
                                                              int planned_check_count,
                                                              int completed_check_count,
                                                              int published_stage_count,
                                                              int planned_stage_count) {
    const bool source_intake_packet_frozen = source_intake_packet_stage_count == 25 && source_intake_packet_complete;
    const bool item_counts_aligned = planned_item_count == 25 && completed_item_count == 25;
    const bool check_counts_aligned = planned_check_count == 25 && completed_check_count == 25;
    const bool stage_counts_aligned = published_stage_count == planned_stage_count && planned_stage_count == 10;
    const bool no_material_consumed = true;
    const bool validation_passed =
        source_intake_packet_frozen && item_counts_aligned && check_counts_aligned && stage_counts_aligned &&
        no_material_consumed;

    return "{\"candidateMaterialRequestValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateIntakePacketPublishedStageCount\":" + std::to_string(source_intake_packet_stage_count) +
           ",\"sourceCandidateIntakePacketComplete\":" + json_bool(source_intake_packet_complete) +
           ",\"plannedMaterialRequestItemCount\":" + std::to_string(planned_item_count) +
           ",\"completedMaterialRequestItemCount\":" + std::to_string(completed_item_count) +
           ",\"plannedAcceptanceCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedAcceptanceCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"sourceIntakePacketFrozen\":" + json_bool(source_intake_packet_frozen) +
           ",\"itemCountsAligned\":" + json_bool(item_counts_aligned) +
           ",\"checkCountsAligned\":" + json_bool(check_counts_aligned) +
           ",\"stageCountsAligned\":" + json_bool(stage_counts_aligned) +
           ",\"additionalMaterialRequestEchoCreated\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidatePayloadImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"candidateDocumentAccepted\":false"
           ",\"candidateDocumentRejected\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"runtimePayloadImported\":false"
           ",\"documentRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_candidate_material_request_validation
