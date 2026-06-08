#include "minikv/shard_preview_candidate_request_package_integrity_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_candidate_request_package_integrity_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_candidate_request_package_integrity_validation_json(int source_published_stage_count,
                                                                       bool source_chain_complete,
                                                                       int planned_check_count,
                                                                       int completed_check_count,
                                                                       int published_stage_count,
                                                                       int planned_stage_count) {
    const bool source_package_frozen = source_published_stage_count == 15 && source_chain_complete;
    const bool integrity_counts_aligned = planned_check_count == 25 && completed_check_count == published_stage_count &&
                                          published_stage_count == planned_stage_count;
    const bool no_side_effects = true;
    const bool validation_passed = source_package_frozen && integrity_counts_aligned && no_side_effects;

    return "{\"candidateRequestPackageIntegrityValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceCandidateDocumentRequestPackagePublishedStageCount\":" +
           std::to_string(source_published_stage_count) +
           ",\"sourceCandidateDocumentRequestPackageChainComplete\":" + json_bool(source_chain_complete) +
           ",\"plannedIntegrityCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedIntegrityCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"sourcePackageFrozen\":" + json_bool(source_package_frozen) +
           ",\"integrityCountsAligned\":" + json_bool(integrity_counts_aligned) +
           ",\"additionalRequestEchoCreated\":false"
           ",\"documentIntakeOpened\":false"
           ",\"realCandidateDocumentRead\":false"
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

} // namespace minikv::shard_preview_candidate_request_package_integrity_validation
