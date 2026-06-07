#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template_fields.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_signed_approval_template_fields::
               planned_signed_approval_template_field_count() == 25);

    const std::string fields =
        minikv::shard_route_preview_operator_value_supply_signed_approval_template_fields::
            format_signed_approval_template_fields_json(1);
    minikv::test_support::assert_contains(fields, "\"sequence\":1");
    minikv::test_support::assert_contains(fields,
                                          "\"fieldCode\":\"SIGNED_APPROVAL_TEMPLATE_PACKET_ID\"");
    minikv::test_support::assert_contains(fields,
                                          "\"templateField\":\"signed_approval_packet_id\"");
    minikv::test_support::assert_contains(fields, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(fields, "\"sequence\":25");
    minikv::test_support::assert_contains(
        fields,
        "\"fieldCode\":\"SIGNED_APPROVAL_TEMPLATE_CAPTURE_PREFLIGHT_HANDOFF\"");
    minikv::test_support::assert_contains(fields, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(fields, "\"requiredBeforeSignedApprovalCapture\":true");
    minikv::test_support::assert_contains(fields, "\"presentInMiniKv\":false");
    minikv::test_support::assert_contains(fields, "\"signatureCaptured\":false");
    minikv::test_support::assert_contains(fields, "\"signedApprovalPresent\":false");
    minikv::test_support::assert_contains(fields, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(fields, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(fields, "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(fields, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(fields, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(fields, "\"readOnly\":true");
    minikv::test_support::assert_contains(fields, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(fields, "\"executionAllowed\":true");

    const std::string completed_fields =
        minikv::shard_route_preview_operator_value_supply_signed_approval_template_fields::
            format_signed_approval_template_fields_json(25);
    minikv::test_support::assert_contains(completed_fields, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_fields, "\"publishedByCurrentStage\":true");

    return 0;
}
