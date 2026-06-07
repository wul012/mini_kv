#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies::
               planned_signed_approval_capture_preflight_policy_count() == 25);

    const std::string policies =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies::
            format_signed_approval_capture_preflight_policies_json(2);
    minikv::test_support::assert_contains(policies, "\"sequence\":1");
    minikv::test_support::assert_contains(
        policies,
        "\"policyCode\":\"CAPTURE_PREFLIGHT_REQUIRE_REQUEST_ID\"");
    minikv::test_support::assert_contains(policies,
                                          "\"rejectionCode\":\"REJECT_MISSING_CAPTURE_PREFLIGHT_REQUEST_ID\"");
    minikv::test_support::assert_contains(policies, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(policies, "\"sequence\":25");
    minikv::test_support::assert_contains(
        policies,
        "\"policyCode\":\"CAPTURE_PREFLIGHT_CLOSEOUT_BOUNDARY\"");
    minikv::test_support::assert_contains(policies, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(policies, "\"blocksSignedApprovalCapture\":true");
    minikv::test_support::assert_contains(policies, "\"blocksApprovalGrant\":true");
    minikv::test_support::assert_contains(policies, "\"blocksOperatorValueImport\":true");
    minikv::test_support::assert_contains(policies, "\"blocksAutomaticSiblingImport\":true");
    minikv::test_support::assert_contains(policies, "\"blocksRuntimePayload\":true");
    minikv::test_support::assert_contains(policies, "\"blocksRouterActivation\":true");
    minikv::test_support::assert_contains(policies, "\"blocksWrites\":true");
    minikv::test_support::assert_contains(policies, "\"blocksExecution\":true");
    minikv::test_support::assert_contains(policies, "\"readOnly\":true");
    minikv::test_support::assert_not_contains(policies, "\"blocksExecution\":false");

    const std::string completed_policies =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_policies::
            format_signed_approval_capture_preflight_policies_json(25);
    minikv::test_support::assert_contains(completed_policies, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_policies, "\"publishedByCurrentStage\":true");

    return 0;
}
