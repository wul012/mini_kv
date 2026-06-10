#include "minikv/command.hpp"
#include "minikv/shard_readiness.hpp"
#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness.hpp"
#include "minikv/store.hpp"
#include "route_preview_evidence_assertions.hpp"
#include "test_support.hpp"

#include <cassert>
#include <string>
#include <string_view>

namespace {

constexpr std::string_view authoring_readiness_command =
    "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON";

} // namespace

int main() {
    namespace authoring =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_authoring_readiness;

    const std::string evidence =
        authoring::format_signed_approval_capture_artifact_draft_authoring_readiness_json();
    minikv::test_support::assert_contains(
        evidence,
        "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-authoring-readiness.v1\"");
    minikv::test_support::assert_contains(
        evidence,
        "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\"");
    minikv::test_support::assert_contains(evidence, "\"sourceArtifactPreflightReleaseVersion\":\"v735\"");
    minikv::test_support::assert_contains(evidence,
                                          "\"sourceArtifactPreflightFixturePath\":\"fixtures/release/shard-readiness-v735.json\"");
    minikv::test_support::assert_contains(evidence, "\"sourceArtifactPreflightPublishedStageCount\":25");
    minikv::test_support::assert_contains(evidence, "\"sourceArtifactPreflightChainComplete\":true");
    minikv::test_support::assert_contains(
        evidence,
        "\"signedApprovalCaptureArtifactDraftAuthoringReadinessReleaseRangeStart\":\"v736\"");
    minikv::test_support::assert_contains(evidence, "\"authoringRequirementCount\":25");
    minikv::test_support::assert_contains(evidence, "\"authoringBlockerCount\":25");
    minikv::test_support::assert_contains(evidence, "\"authoringProfileCount\":25");
    minikv::test_support::assert_contains(evidence, "\"authoringCatalogsAligned\":true");
    minikv::test_support::assert_contains(
        evidence, "\"signedApprovalCaptureArtifactDraftAuthoringReadinessDeclared\":true");
    minikv::test_support::assert_contains(
        evidence, "\"signedApprovalCaptureArtifactDraftAuthoringReadinessOnly\":true");
    minikv::test_support::assert_contains(evidence, "\"authoringReadinessOnly\":true");
    minikv::test_support::assert_contains(evidence, "\"signedApprovalArtifactDraftPresent\":false");
    minikv::test_support::assert_contains(evidence, "\"signedApprovalArtifactDraftMaterialized\":false");
    minikv::test_support::assert_contains(evidence, "\"signedApprovalArtifactDraftTextGenerated\":false");
    minikv::test_support::assert_contains(evidence, "\"approvalStatementTextGenerated\":false");
    minikv::test_support::assert_contains(evidence, "\"detachedSignaturePayloadPresent\":false");
    minikv::test_support::assert_contains(evidence, "\"approvalGrantEmitted\":false");
    minikv::test_support::assert_contains(evidence, "\"submittedOperatorValueCount\":0");
    minikv::test_support::assert_contains(evidence, "\"importedEvidenceValueCount\":0");
    minikv::test_support::assert_contains(evidence, "\"readyForDraftAuthoringReview\":true");
    minikv::test_support::assert_contains(evidence, "\"readyForSignedApprovalArtifactDraft\":false");
    minikv::test_support::assert_contains(evidence, "\"draftTextStored\":false");
    minikv::test_support::assert_contains(
        evidence,
        "\"requirementCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REQUEST_MANIFEST\"");
    minikv::test_support::assert_contains(
        evidence,
        "\"blockerCode\":\"SIGNED_APPROVAL_CAPTURE_ARTIFACT_DRAFT_AUTHORING_READINESS_REQUEST_MANIFEST_BLOCKER\"");
    minikv::test_support::assert_contains(evidence, "\"profileCode\":\"AUTHORING_READINESS_PROFILE_REQUEST_MANIFEST\"");
    minikv::test_support::assert_read_only_evidence_boundaries(evidence);

    const std::string shard = minikv::shard_readiness::format_json();
    assert(minikv::test_support::occurrence_count(
               shard,
               "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftAuthoringReadiness\":") == 1);
    minikv::test_support::assert_contains(
        shard,
        "\"status\":\"route-preview-type-barrel-split-non-participation-clean-workspace-ci-closeout-read-only\"");
    minikv::test_support::assert_contains(
        shard,
        "\"shardRoutePreviewOperatorValueSupplySignedApprovalCaptureArtifactDraftInstructionPreflight\":");
    minikv::test_support::assert_contains(shard, "\"instructionPreflightReleaseVersion\":\"v785\"");
    minikv::test_support::assert_contains(shard,
                                          "\"signedApprovalCaptureArtifactDraftAuthoringReadinessDeclared\":true");
    minikv::test_support::assert_contains(shard, "\"signedApprovalArtifactDraftTextGenerated\":false");
    minikv::test_support::assert_contains(shard, "\"approvalGrantEmitted\":false");

    minikv::Store store;
    minikv::CommandProcessor processor{store};

    auto result =
        processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON extra");
    assert(result.response == "ERR usage: SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON");

    result = processor.execute("SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON");
    assert(result.response == evidence);
    assert(store.size() == 0);

    result = processor.execute("COMMANDS");
    minikv::test_support::assert_contains(
        result.response,
        "SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON(category=read,mutates_store=no,"
        "touches_wal=no,stable=yes)");

    result = processor.execute("COMMANDSJSON");
    minikv::test_support::assert_contains(
        result.response,
        "\"name\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON\","
        "\"category\":\"read\",\"mutates_store\":false,"
        "\"touches_wal\":false");

    const auto explain =
        processor.execute("EXPLAINJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON");
    const auto check =
        processor.execute("CHECKJSON SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTAUTHORINGREADINESSJSON");
    minikv::test_support::assert_metadata_command_contract(explain.response,
                                                           check.response,
                                                           authoring_readiness_command);

    return 0;
}
