#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace profile = minikv::shard_preview_signed_approval_artifact_draft_profile_section;

    const auto json = profile::format_signed_approval_artifact_draft_profile_section_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-signed-approval-artifact-draft-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEVALUESUPPLYSIGNEDAPPROVALCAPTUREARTIFACTDRAFTPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionReleaseVersion\":\"Node v1506\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewSignedApprovalArtifactDraftProfileSectionRenderer.ts\"");
    assert_contains(json, "\"sourceNodeSignedApprovalArtifactDraftProfileSectionGroupCount\":5");
    assert_contains(json, "\"sourceNodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":5");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRouteMarkdownChanged\":false");
    assert_contains(json, "\"sourceNodeRuntimeBehaviorAdded\":false");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1035\"");
    assert_contains(json, "\"sourceCandidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_contains(json, "\"sourceCandidateProfileSectionPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftInstructionPreflightReleaseVersion\":\"v785\"");
    assert_contains(json, "\"sourceDraftInstructionPreflightPublishedStageCount\":25");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionReleaseVersion\":\"v1060\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1059\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1059.json\"");
    assert_contains(json, "\"publishedStageCount\":25");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"plannedSignedApprovalArtifactDraftProfileSectionCheckCount\":25");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionOnly\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionChainComplete\":true");
    assert_contains(json, "\"routeFacingMarkdownStable\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"signed-approval-artifact-draft-instruction-preflight\"");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"miniKvConsumesSignedApprovalMaterial\":false");
    assert_contains(json, "\"draftArtifactCreated\":false");
    assert_contains(json, "\"signedApprovalCreated\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"materialIntakeOpened\":false");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionValidationPassed\":true");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = profile::signed_approval_artifact_draft_profile_section_digest_marker();
    assert(digest.find("v1060-route-preview-signed-approval-artifact-draft-profile-section-closeout-summary-25-of-25-stages") !=
           std::string::npos);
    assert(profile::signed_approval_artifact_draft_profile_section_status() ==
           "route-preview-signed-approval-artifact-draft-profile-section-closeout-summary-read-only");
    assert(profile::published_stage_count() == 25);
}
