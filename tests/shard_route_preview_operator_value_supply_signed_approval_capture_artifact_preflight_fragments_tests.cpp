#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments::
               planned_signed_approval_capture_artifact_preflight_fragment_count() == 25);

    const std::string fragments =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments::
            format_signed_approval_capture_artifact_preflight_fragments_json(1);
    minikv::test_support::assert_contains(fragments, "\"sequence\":1");
    minikv::test_support::assert_contains(
        fragments,
        "\"fragmentCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REQUEST_ID\"");
    minikv::test_support::assert_contains(fragments, "\"artifactField\":\"artifact_preflight_request_id\"");
    minikv::test_support::assert_contains(fragments, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(fragments, "\"sequence\":25");
    minikv::test_support::assert_contains(
        fragments,
        "\"fragmentCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_CLOSEOUT\"");
    minikv::test_support::assert_contains(fragments, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(fragments, "\"requiredBeforeSignedApprovalArtifactDraft\":true");
    minikv::test_support::assert_contains(fragments, "\"presentInMiniKv\":false");
    minikv::test_support::assert_contains(fragments, "\"materializedByMiniKv\":false");
    minikv::test_support::assert_contains(fragments, "\"signedApprovalArtifactDraftPresent\":false");
    minikv::test_support::assert_contains(fragments, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(fragments, "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(fragments, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(fragments, "\"readOnly\":true");
    minikv::test_support::assert_contains(fragments, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(fragments, "\"executionAllowed\":true");

    const std::string completed =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_fragments::
            format_signed_approval_capture_artifact_preflight_fragments_json(25);
    minikv::test_support::assert_contains(completed, "\"sequence\":25");
    minikv::test_support::assert_contains(completed, "\"publishedByCurrentStage\":true");

    return 0;
}
