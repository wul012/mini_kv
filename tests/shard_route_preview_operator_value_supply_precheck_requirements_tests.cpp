#include "minikv/shard_route_preview_operator_value_supply_precheck_requirements.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    assert(minikv::shard_route_preview_operator_value_supply_precheck_requirements::
               planned_precheck_requirement_count() == 25);

    const std::string requirements =
        minikv::shard_route_preview_operator_value_supply_precheck_requirements::
            format_precheck_requirements_json(1);
    minikv::test_support::assert_contains(requirements, "\"sequence\":1");
    minikv::test_support::assert_contains(
        requirements,
        "\"requirementCode\":\"VALUE_SUPPLY_PRECHECK_REQUIREMENT_CATALOG_SPLIT\"");
    minikv::test_support::assert_contains(requirements,
                                          "\"withheldField\":\"operator_supplied_value\"");
    minikv::test_support::assert_contains(requirements, "\"publishedByCurrentStage\":true");
    minikv::test_support::assert_contains(requirements, "\"sequence\":25");
    minikv::test_support::assert_contains(requirements,
                                          "\"requirementCode\":\"VALUE_SUPPLY_PRECHECK_RELEASE_PACKAGE\"");
    minikv::test_support::assert_contains(requirements, "\"publishedByCurrentStage\":false");
    minikv::test_support::assert_contains(requirements, "\"requiredBeforeApproval\":true");
    minikv::test_support::assert_contains(requirements, "\"presentInMiniKv\":false");
    minikv::test_support::assert_contains(requirements, "\"approvalPacketPresent\":false");
    minikv::test_support::assert_contains(requirements, "\"precheckOnly\":true");
    minikv::test_support::assert_contains(requirements, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(requirements, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(requirements, "\"executionAllowed\":false");
    minikv::test_support::assert_not_contains(requirements, "\"executionAllowed\":true");

    const std::string completed_requirements =
        minikv::shard_route_preview_operator_value_supply_precheck_requirements::
            format_precheck_requirements_json(25);
    minikv::test_support::assert_contains(completed_requirements, "\"sequence\":25");
    minikv::test_support::assert_contains(completed_requirements, "\"publishedByCurrentStage\":true");

    return 0;
}
