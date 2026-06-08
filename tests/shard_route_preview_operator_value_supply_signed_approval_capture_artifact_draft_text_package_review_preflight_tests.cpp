#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace preflight =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight;

    const auto json = preflight::format_signed_approval_capture_artifact_draft_text_package_review_preflight_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\"");
    assert_contains(json,
                    "\"sourceDraftInstructionPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTINSTRUCTIONPREFLIGHTJSON\"");
    assert_contains(json, "\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\"");
    assert_contains(json, "\"sourceDraftInstructionPreflightFixturePath\":\"fixtures/release/shard-readiness-v785.json\"");
    assert_contains(json, "\"sourceDraftInstructionPreflightPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftInstructionPreflightChainComplete\":true");
    assert_contains(json, "\"draftTextPackageReviewPreflightStage\":\"route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-provenance-policy-criterion\"");
    assert_contains(json, "\"draftTextPackageReviewPreflightReleaseVersion\":\"v804\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v803\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v803.json\"");
    assert_contains(json, "\"publishedStageCount\":19");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"reviewCriterionCount\":25");
    assert_contains(json, "\"rejectionControlCount\":25");
    assert_contains(json, "\"reviewProfileCount\":25");
    assert_contains(json, "\"reviewCatalogsAligned\":true");
    assert_contains(json, "\"preparedReviewCriterionCount\":19");
    assert_contains(json, "\"parsedDraftTextPackageCount\":0");
    assert_contains(json, "\"acceptedDraftTextPackageCount\":0");
    assert_contains(json, "\"draftTextPackageReviewPreflightDeclared\":true");
    assert_contains(json, "\"draftTextPackageReviewPreflightOnly\":true");
    assert_contains(json, "\"draftTextPackagePresent\":false");
    assert_contains(json, "\"draftTextPackageParsed\":false");
    assert_contains(json, "\"draftTextPackageAccepted\":false");
    assert_contains(json, "\"signedApprovalArtifactDraftPresent\":false");
    assert_contains(json, "\"signedApprovalArtifactDraftTextGenerated\":false");
    assert_contains(json, "\"detachedSignaturePayloadPresent\":false");
    assert_contains(json, "\"approvalGrantPresent\":false");
    assert_contains(json, "\"approvalGrantEmitted\":false");
    assert_contains(json, "\"submittedOperatorValueCount\":0");
    assert_contains(json, "\"importedEvidenceValueCount\":0");
    assert_contains(json, "\"readyForDraftTextPackageReview\":true");
    assert_contains(json, "\"readyForSignedApprovalCapture\":false");
    assert_contains(json, "\"runtimePayloadAccepted\":false");
    assert_contains(json, "\"draftTextPackageReviewCriterionHelperApplied\":true");
    assert_contains(json, "\"draftTextPackageReviewRejectionControlHelperApplied\":true");
    assert_contains(json, "\"draftTextPackageReviewProfileHelperApplied\":true");
    assert_contains(json, "\"draftTextPackageReviewValidationHelperApplied\":true");
    assert_contains(json, "\"criterionCode\":\"package_manifest_criterion\"");
    assert_contains(json, "\"criterionCode\":\"provenance_policy_criterion\"");
    assert_contains(json, "\"controlCode\":\"provenance_policy_criterion_control\"");
    assert_contains(json, "\"profileCode\":\"draft_text_package_review_preflight_provenance_policy_criterion_profile\"");
    assert_contains(json, "\"draftTextPackageReviewPreflightValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = preflight::signed_approval_capture_artifact_draft_text_package_review_preflight_digest_marker();
    assert(digest.find("v804-route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-preflight-provenance-policy-criterion") != std::string::npos);
    assert(preflight::published_stage_count() == 19);
}
