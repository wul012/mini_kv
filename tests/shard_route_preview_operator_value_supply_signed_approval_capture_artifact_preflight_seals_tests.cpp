#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals::
               planned_signed_approval_capture_artifact_preflight_seal_count() == 25);

    const std::string seals =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_seals::
            format_signed_approval_capture_artifact_preflight_seals_json(1);
    minikv::test_support::assert_contains(seals, "\"sequence\":1");
    minikv::test_support::assert_contains(seals, "\"sealCode\":\"ARTIFACT_PREFLIGHT_REQUEST_ID_SEAL\"");
    minikv::test_support::assert_contains(
        seals,
        "\"sourceFragmentCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REQUEST_ID\"");
    minikv::test_support::assert_contains(seals, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(seals, "\"sequence\":25");
    minikv::test_support::assert_contains(seals, "\"sealCode\":\"ARTIFACT_PREFLIGHT_CLOSEOUT_SEAL\"");
    minikv::test_support::assert_contains(seals, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(seals, "\"sealSatisfiedByAbsence\":true");
    minikv::test_support::assert_contains(seals, "\"sealMaterialized\":false");
    minikv::test_support::assert_contains(seals, "\"artifactMaterializationAllowed\":false");
    minikv::test_support::assert_contains(seals, "\"approvalGrantAllowed\":false");
    minikv::test_support::assert_contains(seals, "\"valueImportAllowed\":false");
    minikv::test_support::assert_contains(seals, "\"writeRouteAllowed\":false");
    minikv::test_support::assert_contains(seals, "\"siblingMutationAllowed\":false");
    minikv::test_support::assert_contains(seals, "\"readOnly\":true");
    minikv::test_support::assert_contains(seals, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(seals, "\"executionAllowed\":true");

    return 0;
}
