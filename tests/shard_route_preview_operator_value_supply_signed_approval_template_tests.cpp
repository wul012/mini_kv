#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_template.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view signed_approval_template_command =
    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON";

} // namespace

int main() {
    const std::string signed_approval_template =
        minikv::shard_route_preview_operator_value_supply_signed_approval_template::
            format_signed_approval_template_json();
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-template.v1\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"signedApprovalTemplateMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-template\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"sourceApprovalTemplateCommand\":\"SHARDROUTEVALUESUPPLYAPPROVALTEMPLATEJSON\"");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"sourceApprovalTemplateReleaseVersion\":\"v660\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"sourceApprovalTemplateFixturePath\":\"fixtures/release/shard-readiness-v660.json\"");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"sourceApprovalTemplatePublishedStageCount\":25");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"sourceApprovalTemplateChainComplete\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateReleaseRangeStart\":\"v661\"");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateReleaseRangeEnd\":\"v");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateFieldCount\":25");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateClauseCount\":25");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateReviewControlCount\":25");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateCatalogsAligned\":true");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"operatorValueSupplyApprovalPacketState\":\"signed-template-required-not-present\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"signedApprovalTemplateState\":\"template-only-no-signature-present\"");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateDeclared\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateOnly\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalRequired\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalPresent\":false");
    minikv::test_support::assert_contains(signed_approval_template, "\"signatureCaptured\":false");
    minikv::test_support::assert_contains(signed_approval_template, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"sourceReviewPackageDigestRequired\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"sourceReviewPackageDigestSupplied\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"typedValueEnvelopeRequired\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"typedValueEnvelopeSupplied\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"redactedValueDigestRequired\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"redactedValueDigestSupplied\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateCheckCount\":21");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplatePassedCount\":21");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"readyForSignedApprovalTemplatePreflightReview\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"readyForSignedApprovalCapturePreflight\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"readyForSignedApprovalCapture\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"operatorValuesPersisted\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"secretValueStored\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateFieldHelperApplied\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateClauseHelperApplied\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateControlHelperApplied\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"signedApprovalTemplateValidationHelperApplied\":true");
    minikv::test_support::assert_contains(signed_approval_template,
                                          "\"singleSignedApprovalTemplateSectionExpected\":true");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"fieldCode\":\"SIGNED_APPROVAL_TEMPLATE_PACKET_ID\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"clauseCode\":\"SIGNED_APPROVAL_TEMPLATE_PACKET_ID_CLAUSE\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"reviewControlCode\":\"SIGNED_APPROVAL_TEMPLATE_REVIEW_PACKET_ID\"");
    minikv::test_support::assert_contains(
        signed_approval_template,
        "\"fieldCode\":\"SIGNED_APPROVAL_TEMPLATE_CAPTURE_PREFLIGHT_HANDOFF\"");
    minikv::test_support::assert_read_only_evidence_boundaries(signed_approval_template);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(
               shard,
               "\"shardRoutePreviewOperatorValueSupplySignedApprovalTemplate\":") == 1);
    minikv::test_support::assert_contains(
        shard,
        "\"status\":\"route-preview-candidate-submission-precheck-closeout-summary-read-only\"");
    minikv::test_support::assert_contains(shard,
                                          "\"sourceApprovalTemplateReleaseVersion\":\"v660\"");
    minikv::test_support::assert_contains(shard,
                                          "\"readyForSignedApprovalCapturePreflight\":false");
    minikv::test_support::assert_contains(shard,
                                          "\"signedApprovalTemplateDeclared\":true");
    minikv::test_support::assert_contains(shard,
                                          "\"signedApprovalTemplateOnly\":true");
    minikv::test_support::assert_contains(shard, "\"signatureCaptured\":false");
    minikv::test_support::assert_contains(shard, "\"signedApprovalPresent\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON");
    assert(result.response == signed_approval_template);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(
        result.response,
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON(category=read,mutates_store=no,"
        "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(
        result.response,
        "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\","
        "\"category\":\"read\",\"mutates_store\":false,"
        "\"touches_wal\":false");

    const auto explain =
        processor.execute("EXPLAINJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON");
    const auto check =
        processor.execute("CHECKJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response,
                                                           check.response,
                                                           signed_approval_template_command);

    return 0;
}
