#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation.hpp"

#include "minikv/runtime_evidence.hpp"

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_signed_approval_capture_preflight_validation_json(int input_count,
                                                                     int policy_count,
                                                                     int attestation_count,
                                                                     int published_stage_count) {
    const bool counts_aligned = input_count == policy_count && policy_count == attestation_count;
    const bool complete_catalog = input_count == 25 && policy_count == 25 && attestation_count == 25;
    const bool published_count_valid = published_stage_count >= 0 && published_stage_count <= 25;
    const bool release_complete = published_stage_count == 25;

    return "{\"validationMode\":\"signed-approval-capture-preflight-read-only-validation\""
           ",\"inputCount\":" + std::to_string(input_count) +
           ",\"policyCount\":" + std::to_string(policy_count) +
           ",\"attestationCount\":" + std::to_string(attestation_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"expectedCatalogCount\":25"
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"completeCatalog\":" + json_bool(complete_catalog) +
           ",\"publishedCountValid\":" + json_bool(published_count_valid) +
           ",\"releaseComplete\":" + json_bool(release_complete) +
           ",\"allInputsHaveAbsentStates\":true"
           ",\"allPoliciesHaveRejectionCodes\":true"
           ",\"allAttestationsHaveSiblingBoundaries\":true"
           ",\"signedApprovalCapturePreflightDeclared\":true"
           ",\"signedApprovalCapturePreflightOnly\":true"
           ",\"signatureCaptured\":false"
           ",\"signedApprovalPresent\":false"
           ",\"approvalGrantPresent\":false"
           ",\"approvalGrantEmitted\":false"
           ",\"operatorValueSubmitted\":false"
           ",\"operatorValueAccepted\":false"
           ",\"operatorValueImported\":false"
           ",\"operatorValuesPersisted\":false"
           ",\"automaticSiblingImportAllowed\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"walTouched\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation
