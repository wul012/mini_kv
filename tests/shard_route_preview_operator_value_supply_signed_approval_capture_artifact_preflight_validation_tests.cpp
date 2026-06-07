#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation.hpp"
#include "test_support.hpp"

#include <string>

int main() {
    const std::string validation =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_validation::
            format_signed_approval_capture_artifact_preflight_validation_json(25, 25, 25, true, 1);

    minikv::test_support::assert_contains(validation, "\"fragmentCount\":25");
    minikv::test_support::assert_contains(validation, "\"sealCount\":25");
    minikv::test_support::assert_contains(validation, "\"gateCount\":25");
    minikv::test_support::assert_contains(validation, "\"catalogCountsAligned\":true");
    minikv::test_support::assert_contains(validation, "\"plannedCountMet\":true");
    minikv::test_support::assert_contains(validation, "\"sourceCapturePreflightChainComplete\":true");
    minikv::test_support::assert_contains(validation, "\"artifactPreflightValidationPassed\":true");
    minikv::test_support::assert_contains(validation,
                                          "\"readyForSignedApprovalCaptureArtifactPreflightReview\":true");
    minikv::test_support::assert_contains(validation, "\"readyForSignedApprovalArtifactDraft\":false");
    minikv::test_support::assert_contains(validation, "\"readyForSignedApprovalCapture\":false");
    minikv::test_support::assert_contains(validation, "\"artifactMaterializationAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"approvalGrantAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"valueImportAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"writeRouteAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"siblingMutationAllowed\":false");
    minikv::test_support::assert_contains(validation, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(validation, "\"executionAllowed\":true");

    return 0;
}
