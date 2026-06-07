#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_controls.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_template_controls::
               planned_signed_approval_template_review_control_count() == 25);

    const std::string controls =
        minikv::shard_route_preview_operator_value_supply_signed_approval_template_controls::
            format_signed_approval_template_review_controls_json(3);
    minikv::test_support::assert_contains(controls, "\"sequence\":1");
    minikv::test_support::assert_contains(
        controls,
        "\"reviewControlCode\":\"SIGNED_APPROVAL_TEMPLATE_REVIEW_PACKET_ID\"");
    minikv::test_support::assert_contains(
        controls,
        "\"sourceReviewPackageControl\":\"APPROVAL_PACKET_REVIEW_PACKET_ID\"");
    minikv::test_support::assert_contains(controls, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(controls, "\"sequence\":25");
    minikv::test_support::assert_contains(
        controls,
        "\"reviewControlCode\":\"SIGNED_APPROVAL_TEMPLATE_REVIEW_CAPTURE_PREFLIGHT_HANDOFF\"");
    minikv::test_support::assert_contains(controls, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(controls, "\"sourceReviewControlBound\":true");
    minikv::test_support::assert_contains(controls, "\"operatorReviewRequired\":true");
    minikv::test_support::assert_contains(controls, "\"operatorReviewPresent\":false");
    minikv::test_support::assert_contains(controls, "\"signedApprovalCaptureAllowed\":false");
    minikv::test_support::assert_contains(controls, "\"valueImportAllowed\":false");
    minikv::test_support::assert_contains(controls, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(controls, "\"executionAllowed\":true");

    const std::string completed_controls =
        minikv::shard_route_preview_operator_value_supply_signed_approval_template_controls::
            format_signed_approval_template_review_controls_json(25);
    minikv::test_support::assert_contains(completed_controls, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_controls, "\"publishedByCurrentStage\":true");

    return 0;
}
