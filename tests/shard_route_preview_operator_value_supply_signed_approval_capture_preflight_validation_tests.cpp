#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation.hpp"
#include "test_support.hpp"

#include <string>

int main() {
    const std::string validation =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation::
            format_signed_approval_capture_preflight_validation_json(25, 25, 25, 25);
    minikv::test_support::assert_contains(
        validation,
        "\"validationMode\":\"signed-approval-capture-preflight-read-only-validation\"");
    minikv::test_support::assert_contains(validation, "\"inputCount\":25");
    minikv::test_support::assert_contains(validation, "\"policyCount\":25");
    minikv::test_support::assert_contains(validation, "\"attestationCount\":25");
    minikv::test_support::assert_contains(validation, "\"countsAligned\":true");
    minikv::test_support::assert_contains(validation, "\"completeCatalog\":true");
    minikv::test_support::assert_contains(validation, "\"publishedCountValid\":true");
    minikv::test_support::assert_contains(validation, "\"releaseComplete\":true");
    minikv::test_support::assert_contains(validation, "\"allInputsHaveAbsentStates\":true");
    minikv::test_support::assert_contains(validation, "\"allPoliciesHaveRejectionCodes\":true");
    minikv::test_support::assert_contains(validation, "\"allAttestationsHaveSiblingBoundaries\":true");
    minikv::test_support::assert_contains(validation, "\"signedApprovalCapturePreflightDeclared\":true");
    minikv::test_support::assert_contains(validation, "\"signedApprovalCapturePreflightOnly\":true");
    minikv::test_support::assert_contains(validation, "\"signatureCaptured\":false");
    minikv::test_support::assert_contains(validation, "\"signedApprovalPresent\":false");
    minikv::test_support::assert_contains(validation, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(validation, "\"approvalGrantEmitted\":false");
    minikv::test_support::assert_contains(validation, "\"operatorValueSubmitted\":false");
    minikv::test_support::assert_contains(validation, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(validation, "\"operatorValueImported\":false");
    minikv::test_support::assert_contains(validation, "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(validation, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(validation, "\"routerActivationAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"writeRoutingAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"walTouched\":false");
    minikv::test_support::assert_contains(validation, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(validation, "\"executionAllowed\":true");

    const std::string partial_validation =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_validation::
            format_signed_approval_capture_preflight_validation_json(25, 25, 25, 1);
    minikv::test_support::assert_contains(partial_validation, "\"releaseComplete\":false");

    return 0;
}
