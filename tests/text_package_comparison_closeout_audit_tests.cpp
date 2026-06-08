#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace audit =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit;

    const auto json = audit::format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-preview-operator-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGECOMPARISONCLOSEOUTAUDITJSON\"");
    assert_contains(json,
                    "\"sourceDraftTextPackageReviewCloseoutAuditCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEREVIEWCLOSEOUTAUDITJSON\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewCloseoutAuditFixturePath\":\"fixtures/release/shard-readiness-v835.json\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewCloseoutAuditPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftTextPackageReviewCloseoutAuditChainComplete\":true");
    assert_contains(json, "\"draftTextPackageComparisonCloseoutAuditStage\":\"route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-sibling-mutation-rejection\"");
    assert_contains(json, "\"draftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v859\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v858\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v858.json\"");
    assert_contains(json, "\"signedApprovalCaptureArtifactDraftTextPackageComparisonCloseoutAuditReleaseRangeStart\":\"v836\"");
    assert_contains(json, "\"publishedStageCount\":24");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"comparisonCheckCount\":25");
    assert_contains(json, "\"acceptanceControlCount\":25");
    assert_contains(json, "\"comparisonProfileCount\":25");
    assert_contains(json, "\"comparisonCatalogsAligned\":true");
    assert_contains(json, "\"completedComparisonCheckCount\":24");
    assert_contains(json, "\"draftTextPackageComparisonCloseoutAuditDeclared\":true");
    assert_contains(json, "\"draftTextPackageComparisonCloseoutAuditOnly\":true");
    assert_contains(json, "\"preflightPackageReopened\":false");
    assert_contains(json, "\"draftTextPackageParsed\":false");
    assert_contains(json, "\"draftTextPackageAccepted\":false");
    assert_contains(json, "\"submittedDraftTextPackageParsed\":false");
    assert_contains(json, "\"detachedSignaturePayloadParsed\":false");
    assert_contains(json, "\"uncomparedPackageAccepted\":false");
    assert_contains(json, "\"unacceptablePackageAccepted\":false");
    assert_contains(json, "\"preflightPackageParsedByAudit\":false");
    assert_contains(json, "\"approvalGrantEmitted\":false");
    assert_contains(json, "\"submittedOperatorValueCount\":0");
    assert_contains(json, "\"importedEvidenceValueCount\":0");
    assert_contains(json, "\"readyForCloseoutAudit\":true");
    assert_contains(json, "\"readyForDraftTextPackageComparison\":false");
    assert_contains(json, "\"runtimePayloadAccepted\":false");
    assert_contains(json, "\"comparisonCheckHelperApplied\":true");
    assert_contains(json, "\"acceptanceControlHelperApplied\":true");
    assert_contains(json, "\"comparisonProfileHelperApplied\":true");
    assert_contains(json, "\"comparisonValidationHelperApplied\":true");
    assert_contains(json, "\"checkCode\":\"source_freeze_audit\"");
    assert_contains(json, "\"blockerCode\":\"source_freeze_audit_blocker\"");
    assert_contains(json, "\"profileCode\":\"draft_text_package_comparison_closeout_audit_source_freeze_profile\"");
    assert_contains(json, "\"draftTextPackageComparisonCloseoutAuditValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = audit::signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_digest_marker();
    assert(digest.find("v859-route-preview-value-supply-signed-approval-capture-artifact-draft-text-package-comparison-closeout-audit-sibling-mutation-rejection") != std::string::npos);
    assert(audit::published_stage_count() == 24);
}
