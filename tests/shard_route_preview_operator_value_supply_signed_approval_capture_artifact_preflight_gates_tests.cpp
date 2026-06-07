#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates::
               planned_signed_approval_capture_artifact_preflight_gate_count() == 25);

    const std::string gates =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight_gates::
            format_signed_approval_capture_artifact_preflight_gates_json(1);
    minikv::test_support::assert_contains(gates, "\"sequence\":1");
    minikv::test_support::assert_contains(gates, "\"gateCode\":\"ARTIFACT_PREFLIGHT_GATE_REQUEST_ID_REVIEW\"");
    minikv::test_support::assert_contains(gates, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(gates, "\"sequence\":25");
    minikv::test_support::assert_contains(gates, "\"gateCode\":\"ARTIFACT_PREFLIGHT_GATE_CLOSEOUT\"");
    minikv::test_support::assert_contains(gates, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(gates, "\"gateCheckPassed\":true");
    minikv::test_support::assert_contains(gates, "\"blockedOperationAttempted\":false");
    minikv::test_support::assert_contains(gates, "\"artifactMaterializationAllowed\":false");
    minikv::test_support::assert_contains(gates, "\"approvalGrantAllowed\":false");
    minikv::test_support::assert_contains(gates, "\"valueImportAllowed\":false");
    minikv::test_support::assert_contains(gates, "\"writeRouteAllowed\":false");
    minikv::test_support::assert_contains(gates, "\"siblingMutationAllowed\":false");
    minikv::test_support::assert_contains(gates, "\"readOnly\":true");
    minikv::test_support::assert_contains(gates, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(gates, "\"executionAllowed\":true");

    return 0;
}
