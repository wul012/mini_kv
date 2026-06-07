#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_capture_artifact_preflight_validation_json(int fragment_count,
                                                                              int seal_count,
                                                                              int gate_count,
                                                                              bool source_chain_complete,
                                                                              int published_stage_count) {
    const bool catalog_counts_aligned = fragment_count == seal_count && seal_count == gate_count;
    const bool planned_count_met = fragment_count == 25 && seal_count == 25 && gate_count == 25;

    return "{\"fragmentCount\":" + std::to_string(fragment_count) +
           ",\"sealCount\":" + std::to_string(seal_count) +
           ",\"gateCount\":" + std::to_string(gate_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"catalogCountsAligned\":" + json_bool(catalog_counts_aligned) +
           ",\"plannedCountMet\":" + json_bool(planned_count_met) +
           ",\"sourceCapturePreflightChainComplete\":" + json_bool(source_chain_complete) +
           ",\"artifactPreflightValidationPassed\":" +
           json_bool(catalog_counts_aligned && planned_count_met && source_chain_complete) +
           ",\"readyForSignedApprovalCaptureArtifactPreflightReview\":true"
           ",\"readyForSignedApprovalArtifactDraft\":false"
           ",\"readyForSignedApprovalCapture\":false"
           ",\"artifactMaterializationAllowed\":false"
           ",\"artifactDigestMaterialized\":false"
           ",\"signedApprovalArtifactDraftPresent\":false"
           ",\"approvalGrantAllowed\":false"
           ",\"valueImportAllowed\":false"
           ",\"writeRouteAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation
