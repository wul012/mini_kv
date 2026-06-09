#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace profile = minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section;

    const auto json = profile::format_signed_approval_artifact_draft_text_package_profile_section_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-signed-approval-artifact-draft-text-package-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTTEXTPACKAGEPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"Node v1531\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1085\"");
    assert_contains(json, "\"sourceSignedApprovalDraftProfileSectionIntegrityReleaseVersion\":\"v1085\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewPreflightReleaseVersion\":\"v810\"");
    assert_contains(json, "\"sourceDraftTextPackageReviewCloseoutAuditReleaseVersion\":\"v835\"");
    assert_contains(json, "\"sourceDraftTextPackageComparisonCloseoutAuditReleaseVersion\":\"v860\"");
    assert_contains(json, "\"sourceComparedPackageEvidenceIntakeAuditReleaseVersion\":\"v880\"");
    assert_contains(json, "\"signedApprovalArtifactDraftTextPackageProfileSectionReleaseVersion\":\"v1110\"");
    assert_contains(json, "\"signedApprovalArtifactDraftTextPackageProfileSectionCount\":9");
    assert_contains(json, "\"nodeMigratedSignedApprovalArtifactDraftTextPackageProfileSectionCount\":9");
    assert_contains(json, "\"text-package-comparison-acceptance-precheck\"");
    assert_contains(json, "\"compared-evidence-candidate-intake\"");
    assert_contains(json, "\"textPackageMaterialImported\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"signedApprovalArtifactDraftTextPackageProfileSectionValidationPassed\":true");

    const auto digest = profile::signed_approval_artifact_draft_text_package_profile_section_digest_marker();
    assert(digest.find(
               "v1110-route-preview-signed-approval-artifact-draft-text-package-profile-section-closeout-summary") !=
           std::string::npos);
    assert(profile::published_stage_count() == 25);
    assert(profile::signed_approval_artifact_draft_text_package_profile_section_status() ==
           "route-preview-signed-approval-artifact-draft-text-package-profile-section-closeout-summary-read-only");

    return 0;
}
