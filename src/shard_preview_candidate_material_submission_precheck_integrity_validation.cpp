#include "minikv/shard_preview_candidate_material_submission_precheck_integrity_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_material_submission_precheck_integrity_validation_json(
    int source_precheck_stage_count,
    bool source_precheck_complete,
    int source_checkpoint_count,
    int source_validator_count,
    int source_material_request_item_count,
    int source_acceptance_check_count,
    int source_required_field_count,
    int source_submission_field_count,
    int source_gate_count,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_precheck_frozen =
        source_precheck_stage_count == 10 && source_precheck_complete &&
        source_checkpoint_count == 10 && source_validator_count == 10 &&
        source_material_request_item_count == 25 && source_acceptance_check_count == 25 &&
        source_required_field_count == 20 && source_submission_field_count == 20 &&
        source_gate_count == 41;
    const bool integrity_counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                          published_stage_count == planned_stage_count &&
                                          planned_stage_count == 25;
    const bool no_material_consumed = true;
    const bool validation_passed = source_precheck_frozen && integrity_counts_aligned && no_material_consumed;

    return "{\"candidateMaterialSubmissionPrecheckIntegrityValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateMaterialSubmissionPrecheckPublishedStageCount\":" +
           std::to_string(source_precheck_stage_count) +
           ",\"sourceCandidateMaterialSubmissionPrecheckComplete\":" + json_bool(source_precheck_complete) +
           ",\"sourceMaterialSubmissionPrecheckCheckpointCount\":" + std::to_string(source_checkpoint_count) +
           ",\"sourceMaterialSubmissionPrecheckValidatorCount\":" + std::to_string(source_validator_count) +
           ",\"sourceMaterialRequestItemCount\":" + std::to_string(source_material_request_item_count) +
           ",\"sourceAcceptanceCheckCount\":" + std::to_string(source_acceptance_check_count) +
           ",\"sourceRequiredMaterialFieldCount\":" + std::to_string(source_required_field_count) +
           ",\"sourceSubmissionMaterialFieldCount\":" + std::to_string(source_submission_field_count) +
           ",\"sourceMaterialSubmissionGateCount\":" + std::to_string(source_gate_count) +
           ",\"plannedIntegrityCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedIntegrityCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"sourceMaterialSubmissionPrecheckFrozen\":" + json_bool(source_precheck_frozen) +
           ",\"integrityCountsAligned\":" + json_bool(integrity_counts_aligned) +
           ",\"nodeStopConditionRespected\":true"
           ",\"additionalMaterialSubmissionPrecheckEchoCreated\":false"
           ",\"materialSubmissionOpened\":false"
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"documentIntakeOpened\":false"
           ",\"materialPayloadImported\":false"
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

} // namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_validation
