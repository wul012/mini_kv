#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_precheck.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view precheck_command = "SHARDROUTEVALUESUPPLYPRECHECKJSON";

} // namespace

int main() {
    const std::string precheck =
        minikv::shard_route_preview_operator_value_supply_precheck::format_value_supply_precheck_json();
    minikv::test_support::assert_contains(precheck,
                                          "\"contract\":\"shard-route-preview-operator-value-supply-precheck.v1\"");
    minikv::test_support::assert_contains(precheck, "\"command\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\"");
    minikv::test_support::assert_contains(
        precheck,
        "\"precheckMode\":\"controlled-read-only-disabled-operator-value-supply-precheck\"");
    minikv::test_support::assert_contains(precheck, "\"sourceValueSupplyEnvelopeCommand\":\"SHARDROUTEVALUESUPPLYJSON\"");
    minikv::test_support::assert_contains(precheck, "\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\"");
    minikv::test_support::assert_contains(
        precheck,
        "\"sourceValueSupplyEnvelopeFixturePath\":\"fixtures/release/shard-readiness-v610.json\"");
    minikv::test_support::assert_contains(precheck, "\"sourceValueSupplyEnvelopePublishedStageCount\":25");
    minikv::test_support::assert_contains(precheck, "\"sourceValueSupplyEnvelopeChainComplete\":true");
    minikv::test_support::assert_contains(precheck, "\"valueSupplyPrecheckReleaseRangeStart\":\"v611\"");
    minikv::test_support::assert_contains(precheck, "\"valueSupplyPrecheckReleaseRangeEnd\":\"v");
    minikv::test_support::assert_contains(precheck, "\"precheckSlotCount\":25");
    minikv::test_support::assert_contains(precheck, "\"precheckSlotCountMatchesSourceEnvelope\":true");
    minikv::test_support::assert_contains(precheck, "\"precheckRequirementCount\":25");
    minikv::test_support::assert_contains(precheck, "\"operatorValueSupplyApprovalPacketState\":\"required-not-present\"");
    minikv::test_support::assert_contains(precheck, "\"approvalPacketRequired\":true");
    minikv::test_support::assert_contains(precheck, "\"approvalPacketPresent\":false");
    minikv::test_support::assert_contains(precheck, "\"operatorIdentityRequired\":true");
    minikv::test_support::assert_contains(precheck, "\"operatorIdentityPresent\":false");
    minikv::test_support::assert_contains(precheck, "\"approvalTimestampRequired\":true");
    minikv::test_support::assert_contains(precheck, "\"approvalTimestampPresent\":false");
    minikv::test_support::assert_contains(precheck, "\"perValueProvenanceRequired\":true");
    minikv::test_support::assert_contains(precheck, "\"perValueProvenanceSupplied\":false");
    minikv::test_support::assert_contains(precheck, "\"redactionBeforePersistRequired\":true");
    minikv::test_support::assert_contains(precheck, "\"malformedValuePolicy\":\"reject\"");
    minikv::test_support::assert_contains(precheck, "\"missingValuePolicy\":\"fail-closed\"");
    minikv::test_support::assert_contains(precheck, "\"missingValueRejected\":true");
    minikv::test_support::assert_contains(precheck, "\"malformedValueRejected\":true");
    minikv::test_support::assert_contains(precheck, "\"precheckCheckCount\":18");
    minikv::test_support::assert_contains(precheck, "\"precheckPassedCount\":18");
    minikv::test_support::assert_contains(precheck, "\"readyForDisabledValueSupplyPrecheck\":true");
    minikv::test_support::assert_contains(precheck, "\"readyForOperatorValueSupplyApproval\":false");
    minikv::test_support::assert_contains(precheck, "\"readyForOperatorValueSupply\":false");
    minikv::test_support::assert_contains(precheck, "\"approvalPacketAccepted\":false");
    minikv::test_support::assert_contains(precheck, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(precheck, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(precheck, "\"secretValueStored\":false");
    minikv::test_support::assert_contains(precheck, "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(precheck, "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(precheck, "\"valueSupplyPrecheckSlotHelperApplied\":true");
    minikv::test_support::assert_contains(precheck, "\"valueSupplyPrecheckRequirementHelperApplied\":true");
    minikv::test_support::assert_contains(precheck, "\"singleValueSupplyPrecheckSectionExpected\":true");
    minikv::test_support::assert_contains(
        precheck,
        "\"precheckCode\":\"VALUE_SUPPLY_PRECHECK_REQUIREMENT_CATALOG_SPLIT\"");
    minikv::test_support::assert_contains(precheck, "\"precheckCode\":\"VALUE_SUPPLY_PRECHECK_RELEASE_PACKAGE\"");
    minikv::test_support::assert_contains(
        precheck,
        "\"requirementCode\":\"VALUE_SUPPLY_PRECHECK_REQUIREMENT_CATALOG_SPLIT\"");
    minikv::test_support::assert_read_only_evidence_boundaries(precheck);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(shard, "\"shardRoutePreviewOperatorValueSupplyPrecheck\":") == 1);
    minikv::test_support::assert_contains(shard,
                                          "\"status\":\"route-preview-catalog-entry-group-split-non-participation-follow-up-github-actions-closeout-summary-read-only\"");
    minikv::test_support::assert_contains(shard, "\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\"");
    minikv::test_support::assert_contains(shard, "\"readyForDisabledValueSupplyPrecheck\":true");
    minikv::test_support::assert_contains(shard, "\"readyForOperatorValueSupplyApproval\":false");
    minikv::test_support::assert_contains(shard, "\"approvalPacketRequired\":true");
    minikv::test_support::assert_contains(shard, "\"approvalPacketPresent\":false");
    minikv::test_support::assert_contains(shard, "\"missingValueRejected\":true");
    minikv::test_support::assert_contains(shard, "\"malformedValueRejected\":true");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVALUESUPPLYPRECHECKJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYPRECHECKJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYPRECHECKJSON");
    assert(result.response == precheck);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(result.response,
                                          "SHARDROUTEVALUESUPPLYPRECHECKJSON(category=read,mutates_store=no,"
                                          "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(result.response,
                                          "\"name\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\","
                                          "\"category\":\"read\",\"mutates_store\":false,"
                                          "\"touches_wal\":false");

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEVALUESUPPLYPRECHECKJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEVALUESUPPLYPRECHECKJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response, check.response, precheck_command);

    return 0;
}
