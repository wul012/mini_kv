#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs::
               planned_signed_approval_capture_preflight_input_count() == 25);

    const std::string inputs =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs::
            format_signed_approval_capture_preflight_inputs_json(1);
    minikv::test_support::assert_contains(inputs, "\"sequence\":1");
    minikv::test_support::assert_contains(
        inputs,
        "\"inputCode\":\"SIGNED_APPROVAL_CAPTURE_PREFLIGHT_REQUEST_ID\"");
    minikv::test_support::assert_contains(inputs,
                                          "\"inputField\":\"capture_preflight_request_id\"");
    minikv::test_support::assert_contains(inputs, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(inputs, "\"sequence\":25");
    minikv::test_support::assert_contains(
        inputs,
        "\"inputCode\":\"SIGNED_APPROVAL_CAPTURE_PREFLIGHT_CLOSEOUT\"");
    minikv::test_support::assert_contains(inputs, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(inputs, "\"requiredBeforeSignedApprovalCapture\":true");
    minikv::test_support::assert_contains(inputs, "\"presentInMiniKv\":false");
    minikv::test_support::assert_contains(inputs, "\"capturedByMiniKv\":false");
    minikv::test_support::assert_contains(inputs, "\"signedApprovalPresent\":false");
    minikv::test_support::assert_contains(inputs, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(inputs, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(inputs, "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(inputs, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(inputs, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(inputs, "\"readOnly\":true");
    minikv::test_support::assert_contains(inputs, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(inputs, "\"executionAllowed\":true");

    const std::string completed_inputs =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight_inputs::
            format_signed_approval_capture_preflight_inputs_json(25);
    minikv::test_support::assert_contains(completed_inputs, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_inputs, "\"publishedByCurrentStage\":true");

    return 0;
}
