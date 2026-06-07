#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace profiles =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness_profiles;

    assert(profiles::planned_signed_approval_capture_artifact_draft_authoring_readiness_profile_count() == 25);
    const std::string first =
        profiles::format_signed_approval_capture_artifact_draft_authoring_readiness_profiles_json(1);
    minikv::test_support::assert_contains(first, "\"profileCode\":\"AUTHORING_READINESS_PROFILE_REQUEST_MANIFEST\"");
    minikv::test_support::assert_contains(first, "\"readOnly\":true");
    minikv::test_support::assert_contains(first, "\"executionAllowed\":false");
    minikv::test_support::assert_contains(first, "\"draftTextAllowed\":false");
    minikv::test_support::assert_contains(first, "\"artifactMaterializationAllowed\":false");

    const std::string all =
        profiles::format_signed_approval_capture_artifact_draft_authoring_readiness_profiles_json(25);
    minikv::test_support::assert_contains(all, "\"profileCode\":\"AUTHORING_READINESS_PROFILE_CLOSEOUT\"");
    return 0;
}
