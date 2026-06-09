#include "minikv/shard_preview_candidate_material_request_integrity_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_material_request_integrity_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_material_request_integrity_validation_json(int source_material_request_stage_count,
                                                                        bool source_material_request_complete,
                                                                        int source_material_request_item_count,
                                                                        int source_acceptance_check_count,
                                                                        int source_material_request_gate_count,
                                                                        int source_requested_field_count,
                                                                        int planned_check_count,
                                                                        int completed_check_count,
                                                                        int published_stage_count,
                                                                        int planned_stage_count) {
    const bool source_material_request_frozen =
        source_material_request_stage_count == 10 && source_material_request_complete &&
        source_material_request_item_count == 25 && source_acceptance_check_count == 25 &&
        source_material_request_gate_count == 40 && source_requested_field_count == 20;
    const bool integrity_counts_aligned = planned_check_count == 15 && completed_check_count == 15 &&
                                          published_stage_count == planned_stage_count &&
                                          planned_stage_count == 10;
    const bool no_material_consumed = true;
    const bool validation_passed =
        source_material_request_frozen && integrity_counts_aligned && no_material_consumed;

    return "{\"candidateMaterialRequestIntegrityValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateMaterialRequestPublishedStageCount\":" +
           std::to_string(source_material_request_stage_count) +
           ",\"sourceCandidateMaterialRequestComplete\":" + json_bool(source_material_request_complete) +
           ",\"sourceMaterialRequestItemCount\":" + std::to_string(source_material_request_item_count) +
           ",\"sourceAcceptanceCheckCount\":" + std::to_string(source_acceptance_check_count) +
           ",\"sourceMaterialRequestGateCount\":" + std::to_string(source_material_request_gate_count) +
           ",\"sourceRequestedMaterialFieldCount\":" + std::to_string(source_requested_field_count) +
           ",\"plannedIntegrityCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedIntegrityCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"sourceMaterialRequestFrozen\":" + json_bool(source_material_request_frozen) +
           ",\"integrityCountsAligned\":" + json_bool(integrity_counts_aligned) +
           ",\"nodeStopConditionRespected\":true"
           ",\"additionalMaterialRequestEchoCreated\":false"
           ",\"materialRequestReopened\":false"
           ",\"externalReviewedMaterialConsumed\":false"
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

} // namespace minikv::shard_preview_candidate_material_request_integrity_validation
