#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_approval_template.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view approval_template_command = "SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON";

} // namespace

int main() {
    const std::string approval_template =
        minikv::shard_route_preview_operator_value_supply_approval_template::
            format_value_supply_approval_template_json();
    minikv::test_support::assert_contains(
        approval_template,
        "\"contract\":\"shard-route-preview-operator-value-supply-approval-template.v1\"");
    minikv::test_support::assert_contains(approval_template,
                                          "\"command\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\"");
    minikv::test_support::assert_contains(
        approval_template,
        "\"approvalTemplateMode\":\"controlled-read-only-disabled-operator-value-supply-approval-template\"");
    minikv::test_support::assert_contains(approval_template,
                                          "\"sourceValueSupplyPrecheckCommand\":\"SHARDROUTEVALUESUPPLYPRECHECKJSON\"");
    minikv::test_support::assert_contains(approval_template,
                                          "\"sourceValueSupplyPrecheckReleaseVersion\":\"v635\"");
    minikv::test_support::assert_contains(
        approval_template,
        "\"sourceValueSupplyPrecheckFixturePath\":\"fixtures/release/shard-readiness-v635.json\"");
    minikv::test_support::assert_contains(approval_template,
                                          "\"sourceValueSupplyPrecheckPublishedStageCount\":25");
    minikv::test_support::assert_contains(approval_template,
                                          "\"sourceValueSupplyPrecheckChainComplete\":true");
    minikv::test_support::assert_contains(approval_template,
                                          "\"valueSupplyApprovalTemplateReleaseRangeStart\":\"v636\"");
    minikv::test_support::assert_contains(approval_template,
                                          "\"valueSupplyApprovalTemplateReleaseRangeEnd\":\"v");
    minikv::test_support::assert_contains(approval_template, "\"approvalTemplateSlotCount\":25");
    minikv::test_support::assert_contains(approval_template,
                                          "\"approvalTemplateSlotCountMatchesSourcePrecheck\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalTemplateFieldCount\":25");
    minikv::test_support::assert_contains(
        approval_template,
        "\"operatorValueSupplyApprovalPacketState\":\"draft-template-required-not-present\"");
    minikv::test_support::assert_contains(approval_template,
                                          "\"approvalPacketDraftState\":\"template-only-no-packet-present\"");
    minikv::test_support::assert_contains(approval_template,
                                          "\"operatorApprovalPacketTemplateDeclared\":true");
    minikv::test_support::assert_contains(approval_template,
                                          "\"operatorApprovalPacketTemplateOnly\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalTemplateOnly\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalPacketRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalPacketPresent\":false");
    minikv::test_support::assert_contains(approval_template, "\"approvalPacketAccepted\":false");
    minikv::test_support::assert_contains(approval_template, "\"approvalGrantRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(approval_template, "\"operatorIdentityRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"operatorIdentityPresent\":false");
    minikv::test_support::assert_contains(approval_template, "\"approvalTimestampRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalTimestampPresent\":false");
    minikv::test_support::assert_contains(approval_template, "\"approvalCorrelationIdRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalCorrelationIdPresent\":false");
    minikv::test_support::assert_contains(approval_template, "\"redactedValueDigestRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"redactedValueDigestSupplied\":false");
    minikv::test_support::assert_contains(approval_template, "\"typedValueEnvelopeRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"typedValueEnvelopeSupplied\":false");
    minikv::test_support::assert_contains(approval_template, "\"reviewRecordRequired\":true");
    minikv::test_support::assert_contains(approval_template, "\"reviewRecordPresent\":false");
    minikv::test_support::assert_contains(approval_template, "\"missingValueRejected\":true");
    minikv::test_support::assert_contains(approval_template, "\"malformedValueRejected\":true");
    minikv::test_support::assert_contains(approval_template, "\"valueSupplyApprovalTemplateCheckCount\":22");
    minikv::test_support::assert_contains(approval_template, "\"valueSupplyApprovalTemplatePassedCount\":22");
    minikv::test_support::assert_contains(approval_template, "\"readyForApprovalPacketDraftReview\":true");
    minikv::test_support::assert_contains(approval_template, "\"readyForOperatorValueSupplyApproval\":false");
    minikv::test_support::assert_contains(approval_template, "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(approval_template, "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(approval_template, "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(approval_template, "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(approval_template, "\"secretValueStored\":false");
    minikv::test_support::assert_contains(approval_template, "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(approval_template, "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(approval_template, "\"approvalTemplateSlotHelperApplied\":true");
    minikv::test_support::assert_contains(approval_template, "\"approvalTemplateFieldHelperApplied\":true");
    minikv::test_support::assert_contains(approval_template,
                                          "\"singleValueSupplyApprovalTemplateSectionExpected\":true");
    minikv::test_support::assert_contains(
        approval_template,
        "\"templateCode\":\"VALUE_SUPPLY_APPROVAL_TEMPLATE_FIELD_CATALOG_SPLIT\"");
    minikv::test_support::assert_contains(
        approval_template,
        "\"fieldCode\":\"VALUE_SUPPLY_APPROVAL_TEMPLATE_FIELD_CATALOG_SPLIT\"");
    minikv::test_support::assert_contains(
        approval_template,
        "\"templateCode\":\"VALUE_SUPPLY_APPROVAL_TEMPLATE_RELEASE_PACKAGE\"");
    minikv::test_support::assert_read_only_evidence_boundaries(approval_template);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(
               shard,
               "\"shardRoutePreviewOperatorValueSupplyApprovalTemplate\":") == 1);
    minikv::test_support::assert_contains(shard,
                                          "\"status\":\"route-preview-production-live-capture-non-participation-closeout-summary-read-only\"");
    minikv::test_support::assert_contains(shard, "\"sourceValueSupplyPrecheckReleaseVersion\":\"v635\"");
    minikv::test_support::assert_contains(shard, "\"readyForOperatorValueSupplyApproval\":false");
    minikv::test_support::assert_contains(shard, "\"approvalPacketRequired\":true");
    minikv::test_support::assert_contains(shard, "\"approvalPacketPresent\":false");
    minikv::test_support::assert_contains(shard, "\"operatorApprovalPacketTemplateDeclared\":true");
    minikv::test_support::assert_contains(shard, "\"approvalTemplateOnly\":true");
    minikv::test_support::assert_contains(shard, "\"missingValueRejected\":true");
    minikv::test_support::assert_contains(shard, "\"malformedValueRejected\":true");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON");
    assert(result.response == approval_template);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(result.response,
                                          "SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON(category=read,mutates_store=no,"
                                          "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(result.response,
                                          "\"name\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\","
                                          "\"category\":\"read\",\"mutates_store\":false,"
                                          "\"touches_wal\":false");

    const auto explain = processor.execute("EXPLAINJSON SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON");
    const auto check = processor.execute("CHECKJSON SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response,
                                                           check.response,
                                                           approval_template_command);

    return 0;
}
