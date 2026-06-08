#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view artifact_preflight_command =
    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON";

} // namespace

int main() {
    const std::string artifact_preflight =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_preflight::
            format_signed_approval_capture_artifact_preflight_json();
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-preflight.v1\"");
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\"");
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"signedApprovalCaptureArtifactPreflightMode\":\"controlled-read-only-disabled-operator-value-supply-signed-approval-capture-artifact-preflight\"");
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"sourceSignedApprovalCapturePreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREPREFLIGHTJSON\"");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\"");
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"sourceSignedApprovalCapturePreflightFixturePath\":\"fixtures/release/shard-readiness-v710.json\"");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"sourceSignedApprovalCapturePreflightPublishedStageCount\":25");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"sourceSignedApprovalCapturePreflightChainComplete\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightReleaseRangeStart\":\"v711\"");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightReleaseRangeEnd\":\"v");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightFragmentCount\":25");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightSealCount\":25");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightGateCount\":25");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightCatalogsAligned\":true");
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"operatorValueSupplyApprovalPacketState\":\"capture-artifact-preflight-required-no-artifact-present\"");
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"signedApprovalCaptureArtifactPreflightState\":\"artifact-preflight-only-no-artifact-materialized\"");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightDeclared\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightOnly\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"captureArtifactPreflightOnly\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"sourceCapturePreflightPresent\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalArtifactDraftPresent\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalArtifactMaterialized\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"artifactDigestMaterialized\":false");
    minikv::test_support::assert_contains(artifact_preflight, "\"signatureCaptured\":false");
    minikv::test_support::assert_contains(artifact_preflight, "\"approvalGrantPresent\":false");
    minikv::test_support::assert_contains(artifact_preflight, "\"approvalGrantEmitted\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"artifactPreflightRequestIdRequired\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"artifactPreflightRequestIdPresent\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"manualCaptureWindowOpen\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"captureChannelOpen\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signatureMaterialAccepted\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"rawSignatureMaterialRejected\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"submittedOperatorValueCount\":0");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"acceptedOperatorValueCount\":0");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"importedEvidenceValueCount\":0");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"persistedOperatorValueCount\":0");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"readyForSignedApprovalCaptureArtifactPreflightReview\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"readyForSignedApprovalArtifactDraft\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"readyForSignedApprovalCapture\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"operatorValueSubmitted\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"operatorValueAccepted\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"automaticSiblingImportAllowed\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"runtimePayloadAccepted\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"secretValueStored\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"credentialValueStored\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"rawEndpointStored\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"rawSignatureMaterialStored\":false");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightFragmentHelperApplied\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightSealHelperApplied\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightGateHelperApplied\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"signedApprovalCaptureArtifactPreflightValidationHelperApplied\":true");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"moduleSplit\":[\"artifact_preflight_core\",\"artifact_preflight_stages\"");
    minikv::test_support::assert_contains(
        artifact_preflight,
        "\"fragmentCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_PREFLIGHT_REQUEST_ID\"");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"sealCode\":\"ARTIFACT_PREFLIGHT_REQUEST_ID_SEAL\"");
    minikv::test_support::assert_contains(artifact_preflight,
                                          "\"gateCode\":\"ARTIFACT_PREFLIGHT_GATE_REQUEST_ID_REVIEW\"");
    minikv::test_support::assert_read_only_evidence_boundaries(artifact_preflight);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(
               shard,
               "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactPreflight\":") == 1);
    minikv::test_support::assert_contains(
        shard,
        "\"status\":\"route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-parse-rejection-read-only\"");
    minikv::test_support::assert_contains(
        shard,
        "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftInstructionPreflight\":");
    minikv::test_support::assert_contains(shard, "\"instructionPreflightReleaseVersion\":\"v785\"");
    minikv::test_support::assert_contains(shard,
                                          "\"sourceSignedApprovalCapturePreflightReleaseVersion\":\"v710\"");
    minikv::test_support::assert_contains(shard,
                                          "\"signedApprovalCaptureArtifactPreflightDeclared\":true");
    minikv::test_support::assert_contains(shard,
                                          "\"signedApprovalCaptureArtifactPreflightOnly\":true");
    minikv::test_support::assert_contains(shard, "\"signedApprovalArtifactDraftPresent\":false");
    minikv::test_support::assert_contains(shard, "\"signedApprovalArtifactMaterialized\":false");
    minikv::test_support::assert_contains(shard, "\"approvalGrantEmitted\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON");
    assert(result.response == artifact_preflight);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(
        result.response,
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON(category=read,mutates_store=no,"
        "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(
        result.response,
        "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON\","
        "\"category\":\"read\",\"mutates_store\":false,"
        "\"touches_wal\":false");

    const auto explain =
        processor.execute("EXPLAINJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON");
    const auto check =
        processor.execute("CHECKJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTPREFLIGHTJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response,
                                                           check.response,
                                                           artifact_preflight_command);

    return 0;
}
