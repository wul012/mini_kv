#include "minikv/shard_preview_candidate_submission_precheck_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_submission_precheck_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_submission_precheck_validation_json(int source_integrity_stage_count,
                                                                 bool source_integrity_complete,
                                                                 int planned_checkpoint_count,
                                                                 int completed_checkpoint_count,
                                                                 int planned_validator_count,
                                                                 int completed_validator_count,
                                                                 int published_stage_count,
                                                                 int planned_stage_count) {
    const bool source_integrity_frozen = source_integrity_stage_count == 25 && source_integrity_complete;
    const bool checkpoint_counts_aligned = planned_checkpoint_count == 10 &&
                                           completed_checkpoint_count == published_stage_count &&
                                           published_stage_count == planned_stage_count;
    const bool validator_counts_aligned = planned_validator_count == 10 &&
                                          completed_validator_count == published_stage_count;
    const bool no_side_effects = true;
    const bool validation_passed =
        source_integrity_frozen && checkpoint_counts_aligned && validator_counts_aligned && no_side_effects;

    return "{\"candidateSubmissionPrecheckValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateRequestPackageIntegrityPublishedStageCount\":" +
           std::to_string(source_integrity_stage_count) +
           ",\"sourceCandidateRequestPackageIntegrityComplete\":" + json_bool(source_integrity_complete) +
           ",\"plannedSubmissionPrecheckCheckpointCount\":" + std::to_string(planned_checkpoint_count) +
           ",\"completedSubmissionPrecheckCheckpointCount\":" + std::to_string(completed_checkpoint_count) +
           ",\"plannedSubmissionPrecheckValidatorCount\":" + std::to_string(planned_validator_count) +
           ",\"completedSubmissionPrecheckValidatorCount\":" + std::to_string(completed_validator_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"sourceIntegrityFrozen\":" + json_bool(source_integrity_frozen) +
           ",\"checkpointCountsAligned\":" + json_bool(checkpoint_counts_aligned) +
           ",\"validatorCountsAligned\":" + json_bool(validator_counts_aligned) +
           ",\"additionalRequestEchoCreated\":false"
           ",\"additionalSubmissionPrecheckEchoCreated\":false"
           ",\"documentIntakeOpened\":false"
           ",\"reviewedRealCandidateDocumentRead\":false"
           ",\"syntheticCandidateDocumentAccepted\":false"
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

} // namespace minikv::shard_preview_candidate_submission_precheck_validation
