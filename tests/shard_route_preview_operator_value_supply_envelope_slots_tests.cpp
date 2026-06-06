#include "minikv/shard_route_preview_operator_value_supply_envelope_slots.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_envelope_slots::planned_value_supply_slot_count() == 25);

    const std::string slots =
        minikv::shard_route_preview_operator_value_supply_envelope_slots::format_value_supply_slots_json(1);
    minikv::test_support::assert_contains(slots, "\"sequence\":1");
    minikv::test_support::assert_contains(slots,
                                          "\"envelopeCode\":\"VALUE_SUPPLY_ENVELOPE_POLICY_SPLIT\"");
    minikv::test_support::assert_contains(slots,
                                          "\"sourceValueDraftCode\":\"VALUE_DRAFT_SLOT_TEMPLATE_SPLIT\"");
    minikv::test_support::assert_contains(slots, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(slots, "\"sequence\":25");
    minikv::test_support::assert_contains(slots, "\"envelopeCode\":\"VALUE_SUPPLY_RELEASE_PACKAGE\"");
    minikv::test_support::assert_contains(slots, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(slots, "\"envelopeState\":\"disabled-by-default\"");
    minikv::test_support::assert_contains(slots, "\"missingValuePolicy\":\"fail-closed\"");
    minikv::test_support::assert_contains(slots, "\"valueRedactionState\":\"redact-before-persist\"");
    minikv::test_support::assert_contains(slots, "\"sourceEvidenceProvenanceSupplied\":false");
    minikv::test_support::assert_contains(slots, "\"operatorValueSupplied\":false");
    minikv::test_support::assert_contains(slots, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(slots, "\"operatorValueImported\":false");
    minikv::test_support::assert_contains(slots, "\"operatorValuePersisted\":false");
    minikv::test_support::assert_contains(slots, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(slots, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(slots, "\"readOnly\":true");
    minikv::test_support::assert_contains(slots, "\"evidenceImportAllowed\":false");
    minikv::test_support::assert_contains(slots, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(slots, "\"executionAllowed\":true");

    const std::string completed_slots =
        minikv::shard_route_preview_operator_value_supply_envelope_slots::format_value_supply_slots_json(25);
    minikv::test_support::assert_contains(completed_slots, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_slots, "\"publishedByCurrentStage\":true");

    return 0;
}
