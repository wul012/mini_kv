#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace audit =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit;

    const auto json = audit::format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\"");
    assert_contains(json,
                    "\"sourceDraftTextPackageReviewPreflightCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWPREFLIGHTJSON\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightFixturePath\":\"fixtures/release/shard-readiness-v810.json\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightChainComplete\":true");
    assert_contains(json, "\"draftTextPackageReviewCloseoutAuditStage\":\"route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-source-fixture-path\"");
    assert_contains(json, "\"draftTextPackageReviewCloseoutAuditReleaseVersion\":\"v819\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v818\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v818.json\"");
    assert_contains(json, "\"publishedStageCount\":9");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"auditCheckCount\":25");
    assert_contains(json, "\"auditBlockerCount\":25");
    assert_contains(json, "\"auditProfileCount\":25");
    assert_contains(json, "\"auditCatalogsAligned\":true");
    assert_contains(json, "\"completedAuditCheckCount\":9");
    assert_contains(json, "\"draftTextPackageReviewCloseoutAuditDeclared\":true");
    assert_contains(json, "\"draftTextPackageReviewCloseoutAuditOnly\":true");
    assert_contains(json, "\"preflightPackageReopened\":false");
    assert_contains(json, "\"draftTextPackageParsed\":false");
    assert_contains(json, "\"draftTextPackageAccepted\":false");
    assert_contains(json, "\"preflightPackageParsedByAudit\":false");
    assert_contains(json, "\"approvalGrantEmitted\":false");
    assert_contains(json, "\"submittedOperatorValueCount\":0");
    assert_contains(json, "\"importedEvidenceValueCount\":0");
    assert_contains(json, "\"readyForCloseoutAudit\":true");
    assert_contains(json, "\"readyForDraftTextPackageReview\":false");
    assert_contains(json, "\"runtimePayloadAccepted\":false");
    assert_contains(json, "\"auditCheckHelperApplied\":true");
    assert_contains(json, "\"auditBlockerHelperApplied\":true");
    assert_contains(json, "\"auditProfileHelperApplied\":true");
    assert_contains(json, "\"auditValidationHelperApplied\":true");
    assert_contains(json, "\"checkCode\":\"source_freeze_audit\"");
    assert_contains(json, "\"blockerCode\":\"source_freeze_audit_blocker\"");
    assert_contains(json, "\"profileCode\":\"draft_text_package_review_closeout_audit_source_freeze_profile\"");
    assert_contains(json, "\"draftTextPackageReviewCloseoutAuditValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = audit::signed_approval_capture_artifact_draft_text_package_review_closeout_audit_digest_marker();
    assert(digest.find("v819-route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-review-closeout-audit-source-fixture-path") != std::string::npos);
    assert(audit::published_stage_count() == 9);
}
