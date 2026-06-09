#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace integrity = minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity;

    const auto json = integrity::format_signed_approval_artifact_draft_profile_section_integrity_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-signed-approval-artifact-draft-profile-section-integrity.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONINTEGRITYJSON\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceProfileSectionCommand\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceProfileSectionReleaseVersion\":\"v1060\"");
    assert_contains(json, "\"sourceProfileSectionFixturePath\":\"fixtures/release/shard-readiness-v1060.json\"");
    assert_contains(json, "\"sourceProfileSectionPublishedStageCount\":25");
    assert_contains(json, "\"sourceProfileSectionComplete\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionIntegrityReleaseVersion\":\"v1085\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1084\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1084.json\"");
    assert_contains(json, "\"publishedStageCount\":25");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"plannedSignedApprovalArtifactDraftProfileSectionIntegrityCheckCount\":25");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionIntegrityOnly\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionIntegrityChainComplete\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionCount\":5");
    assert_contains(json, "\"nodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":5");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"nodeCatalogImported\":false");
    assert_contains(json, "\"miniKvConsumesSignedApprovalMaterial\":false");
    assert_contains(json, "\"draftArtifactCreated\":false");
    assert_contains(json, "\"signedApprovalCreated\":false");
    assert_contains(json, "\"materialIntakeOpened\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionIntegrityValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = integrity::signed_approval_artifact_draft_profile_section_integrity_digest_marker();
    assert(digest.find("v1085-route-preview-signed-approval-artifact-draft-profile-section-integrity-closeout-summary-25-of-25-stages") !=
           std::string::npos);
    assert(integrity::signed_approval_artifact_draft_profile_section_integrity_status() ==
           "route-preview-signed-approval-artifact-draft-profile-section-integrity-closeout-summary-read-only");
    assert(integrity::published_stage_count() == 25);
}
