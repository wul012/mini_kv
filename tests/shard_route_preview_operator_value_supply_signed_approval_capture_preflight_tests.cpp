#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_preflight.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view capture_preflight_command =
    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON";

} // namespace

int main() {
    const std::string capture_preflight =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_preflight::
            format_signed_approval_capture_preflight_json();
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-preflight.v1\"");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\"");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"signedApprovalCapturePreflightMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-capture-preflight\"");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"sourceSignedApprovalTemplateCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALTEMPLATEJSON\"");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\"");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"sourceSignedApprovalTemplateFixturePath\":\"fixtures/release/shard-readiness-v685.json\"");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"sourceSignedApprovalTemplatePublishedStageCount\":25");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"sourceSignedApprovalTemplateChainComplete\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightReleaseRangeStart\":\"v686\"");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightReleaseRangeEnd\":\"v");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightInputCount\":25");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightPolicyCount\":25");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightAttestationCount\":25");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightCatalogsAligned\":true");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"operatorValueSupplyApprovalPacketState\":\"capture-preflight-required-no-approval-present\"");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"signedApprovalCapturePreflightState\":\"preflight-only-no-signature-captured\"");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightDeclared\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightOnly\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"capturePreflightOnly\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalTemplatePresent\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalPresent\":false");
    minikv::test_support::assert_contains(capture_preflight, "\"signatureCaptured\":false");
    minikv::test_support::assert_contains(capture_preflight, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(capture_preflight, "\"approvalGrantEmitted\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"capturePreflightRequestIdRequired\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"capturePreflightRequestIdPresent\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"manualCaptureWindowOpen\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"captureChannelOpen\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signatureMaterialAccepted\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"rawSignatureMaterialRejected\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"submittedOperatorValueCount\":0");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"acceptedOperatorValueCount\":0");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"importedEvidenceValueCount\":0");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"persistedOperatorValueCount\":0");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightCheckCount\":25");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightPassedCount\":25");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"readyForSignedApprovalCapturePreflightReview\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"readyForSignedApprovalCaptureArtifact\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"readyForSignedApprovalCapture\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"operatorValueSubmitted\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"secretValueStored\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"rawSignatureMaterialStored\":false");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightInputHelperApplied\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightPolicyHelperApplied\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightAttestationHelperApplied\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"signedApprovalCapturePreflightValidationHelperApplied\":true");
    minikv::test_support::assert_contains(capture_preflight,
                                          "\"singleSignedApprovalCapturePreflightSectionExpected\":true");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"inputCode\":\"SIGNED_APPROVAL_CAPTURE_PREFLIGHT_REQUEST_ID\"");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"policyCode\":\"CAPTURE_PREFLIGHT_REQUIRE_REQUEST_ID\"");
    minikv::test_support::assert_contains(
        capture_preflight,
        "\"attestationCode\":\"CAPTURE_PREFLIGHT_ATTEST_REQUEST_ID_INPUT\"");
    minikv::test_support::assert_read_only_evidence_boundaries(capture_preflight);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(
               shard,
               "\"shardRoutePreviewOperatorValueSupplySignedApprovalCapturePreflight\":") == 1);
    minikv::test_support::assert_contains(
        shard,
        "\"status\":\"route-preview-type-barrel-split-follow-up-non-participation-clean-workspace-ci-closeout-read-only\"");
    minikv::test_support::assert_contains(shard,
                                          "\"sourceSignedApprovalTemplateReleaseVersion\":\"v685\"");
    minikv::test_support::assert_contains(shard,
                                          "\"signedApprovalCapturePreflightDeclared\":true");
    minikv::test_support::assert_contains(shard,
                                          "\"signedApprovalCapturePreflightOnly\":true");
    minikv::test_support::assert_contains(shard, "\"signatureCaptured\":false");
    minikv::test_support::assert_contains(shard, "\"signedApprovalPresent\":false");
    minikv::test_support::assert_contains(shard, "\"approvalGrantEmitted\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON");
    assert(result.response == capture_preflight);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(
        result.response,
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON(category=read,mutates_store=no,"
        "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(
        result.response,
        "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\","
        "\"category\":\"read\",\"mutates_store\":false,"
        "\"touches_wal\":false");

    const auto explain =
        processor.execute("EXPLAINJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON");
    const auto check =
        processor.execute("CHECKJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response,
                                                           check.response,
                                                           capture_preflight_command);

    return 0;
}
