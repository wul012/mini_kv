#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations::
               planned_signed_approval_capture_preflight_attestation_count() == 25);

    const std::string attestations =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations::
            format_signed_approval_capture_preflight_attestations_json(3);
    minikv::test_support::assert_contains(attestations, "\"sequence\":1");
    minikv::test_support::assert_contains(
        attestations,
        "\"attestationCode\":\"CAPTURE_PREFLIGHT_ATTEST_REQUEST_ID_INPUT\"");
    minikv::test_support::assert_contains(attestations, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(attestations, "\"sequence\":25");
    minikv::test_support::assert_contains(
        attestations,
        "\"attestationCode\":\"CAPTURE_PREFLIGHT_ATTEST_CLOSEOUT_BOUNDARY\"");
    minikv::test_support::assert_contains(attestations, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(attestations, "\"operatorReviewRequired\":true");
    minikv::test_support::assert_contains(attestations, "\"operatorReviewPresent\":false");
    minikv::test_support::assert_contains(attestations, "\"signedApprovalCaptureAllowed\":false");
    minikv::test_support::assert_contains(attestations, "\"approvalGrantAllowed\":false");
    minikv::test_support::assert_contains(attestations, "\"valueImportAllowed\":false");
    minikv::test_support::assert_contains(attestations, "\"siblingMutationAllowed\":false");
    minikv::test_support::assert_contains(attestations, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(attestations, "\"executionAllowed\":true");

    const std::string completed_attestations =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_attestations::
            format_signed_approval_capture_preflight_attestations_json(25);
    minikv::test_support::assert_contains(completed_attestations, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_attestations, "\"publishedByCurrentStage\":true");

    return 0;
}
