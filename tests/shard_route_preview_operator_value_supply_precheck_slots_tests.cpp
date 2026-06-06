#include "minikv/shard_route_preview_operator_value_supply_precheck_slots.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_precheck_slots::
               planned_value_supply_precheck_slot_count() == 25);

    const std::string slots =
        minikv::shard_route_preview_operator_value_supply_precheck_slots::
            format_value_supply_precheck_slots_json(1);
    minikv::test_support::assert_contains(slots, "\"sequence\":1");
    minikv::test_support::assert_contains(
        slots,
        "\"precheckCode\":\"VALUE_SUPPLY_PRECHECK_REQUIREMENT_CATALOG_SPLIT\"");
    minikv::test_support::assert_contains(slots,
                                          "\"sourceEnvelopeCode\":\"VALUE_SUPPLY_ENVELOPE_POLICY_SPLIT\"");
    minikv::test_support::assert_contains(slots, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(slots, "\"sequence\":25");
    minikv::test_support::assert_contains(slots, "\"precheckCode\":\"VALUE_SUPPLY_PRECHECK_RELEASE_PACKAGE\"");
    minikv::test_support::assert_contains(slots, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(slots, "\"approvalPacketRequired\":true");
    minikv::test_support::assert_contains(slots, "\"approvalPacketPresent\":false");
    minikv::test_support::assert_contains(slots, "\"operatorIdentityRequired\":true");
    minikv::test_support::assert_contains(slots, "\"operatorIdentityPresent\":false");
    minikv::test_support::assert_contains(slots, "\"approvalTimestampRequired\":true");
    minikv::test_support::assert_contains(slots, "\"approvalTimestampPresent\":false");
    minikv::test_support::assert_contains(slots, "\"perValueProvenanceRequired\":true");
    minikv::test_support::assert_contains(slots, "\"perValueProvenanceSupplied\":false");
    minikv::test_support::assert_contains(slots, "\"missingValueRejected\":true");
    minikv::test_support::assert_contains(slots, "\"malformedValueRejected\":true");
    minikv::test_support::assert_contains(slots, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(slots, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(slots, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(slots, "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(slots, "\"readOnly\":true");
    minikv::test_support::assert_contains(slots, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(slots, "\"executionAllowed\":true");

    const std::string completed_slots =
        minikv::shard_route_preview_operator_value_supply_precheck_slots::
            format_value_supply_precheck_slots_json(25);
    minikv::test_support::assert_contains(completed_slots, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_slots, "\"publishedByCurrentStage\":true");

    return 0;
}
