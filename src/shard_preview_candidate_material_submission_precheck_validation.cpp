#include "minikv/shard_preview_candidate_material_submission_precheck_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_material_submission_precheck_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_material_submission_precheck_validation_json(int source_integrity_stage_count,
                                                                         bool source_integrity_complete,
                                                                         int source_material_request_item_count,
                                                                         int source_acceptance_check_count,
                                                                         int source_requested_field_count,
                                                                         int source_submission_field_count,
                                                                         int source_gate_count,
                                                                         int planned_checkpoint_count,
                                                                         int completed_checkpoint_count,
                                                                         int planned_validator_count,
                                                                         int completed_validator_count,
                                                                         int published_stage_count,
                                                                         int planned_stage_count) {
    const bool source_integrity_frozen = source_integrity_stage_count == 10 && source_integrity_complete;
    const bool source_counts_aligned = source_material_request_item_count == 25 &&
                                       source_acceptance_check_count == 25 &&
                                       source_requested_field_count == 20 &&
                                       source_submission_field_count == 20 &&
                                       source_gate_count == 41;
    const bool checkpoint_counts_aligned = planned_checkpoint_count == 10 &&
                                           completed_checkpoint_count == published_stage_count &&
                                           published_stage_count == planned_stage_count;
    const bool validator_counts_aligned = planned_validator_count == 10 &&
                                          completed_validator_count == published_stage_count;
    const bool no_side_effects = true;
    const bool validation_passed = source_integrity_frozen && source_counts_aligned &&
                                   checkpoint_counts_aligned && validator_counts_aligned && no_side_effects;

    return "{\"candidateMaterialSubmissionPrecheckValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateMaterialRequestIntegrityPublishedStageCount\":" +
           std::to_string(source_integrity_stage_count) +
           ",\"sourceCandidateMaterialRequestIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"sourceCountsAligned\":" + json_bool(source_counts_aligned) +
           ",\"plannedMaterialSubmissionPrecheckCheckpointCount\":" + std::to_string(planned_checkpoint_count) +
           ",\"completedMaterialSubmissionPrecheckCheckpointCount\":" + std::to_string(completed_checkpoint_count) +
           ",\"plannedMaterialSubmissionPrecheckValidatorCount\":" + std::to_string(planned_validator_count) +
           ",\"completedMaterialSubmissionPrecheckValidatorCount\":" + std::to_string(completed_validator_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"sourceIntegrityFrozen\":" + json_bool(source_integrity_frozen) +
           ",\"checkpointCountsAligned\":" + json_bool(checkpoint_counts_aligned) +
           ",\"validatorCountsAligned\":" + json_bool(validator_counts_aligned) +
           ",\"additionalMaterialRequestEchoCreated\":false"
           ",\"additionalMaterialSubmissionPrecheckEchoCreated\":false"
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"materialPayloadImported\":false"
           ",\"candidatePayloadImported\":false"
           ",\"candidateDocumentEvaluated\":false"
           ",\"candidateDocumentAccepted\":false"
           ",\"approvalGrantCaptured\":false"
           ",\"signedApprovalEmitted\":false"
           ",\"runtimePayloadImported\":false"
           ",\"documentRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_candidate_material_submission_precheck_validation
