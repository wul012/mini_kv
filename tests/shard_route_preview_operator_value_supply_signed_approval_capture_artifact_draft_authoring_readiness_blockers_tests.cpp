#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace blockers =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_blockers;

    assert(blockers::planned_signed_approval_capture_artifact_draft_authoring_readiness_blocker_count() == 25);
    const std::string first =
        blockers::format_signed_approval_capture_artifact_draft_authoring_readiness_blockers_json(1);
    minikv::test_support::assert_contains(
        first,
        "\"blockerCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REQUEST_MANIFEST_BLOCKER\"");
    minikv::test_support::assert_contains(first, "\"runtimeExecutionBlocked\":true");
    minikv::test_support::assert_contains(first, "\"writeRouteBlocked\":true");
    minikv::test_support::assert_contains(first, "\"siblingMutationBlocked\":true");

    const std::string all =
        blockers::format_signed_approval_capture_artifact_draft_authoring_readiness_blockers_json(25);
    minikv::test_support::assert_contains(
        all,
        "\"blockerCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_CLOSEOUT_BLOCKER\"");
    return 0;
}
