#include "minikv/shard_preview_candidate_profile_section.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace profile = minikv::shard_preview_candidate_profile_section;

    const auto json = profile::format_candidate_profile_section_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-candidate-document-profile-section-renderer-split.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTECANDIDATEPROFILESECTIONJSON\"");
    assert_contains(json, "\"sourceNodeProfileSectionReleaseVersion\":\"Node v1481\"");
    assert_contains(json, "\"sourceNodeCandidateProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewCandidateDocumentProfileSectionRenderer.ts\"");
    assert_contains(json, "\"sourceNodeProfileSectionGroupCount\":5");
    assert_contains(json, "\"sourceNodeMigratedCandidateProfileSectionCount\":5");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRouteMarkdownChanged\":false");
    assert_contains(json, "\"sourceNodeRuntimeBehaviorAdded\":false");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityReleaseVersion\":\"v1010\"");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityFixturePath\":\"fixtures/release/shard-readiness-v1010.json\"");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityPublishedStageCount\":25");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityComplete\":true");
    assert_contains(json, "\"candidateProfileSectionReleaseVersion\":\"v1035\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1034\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1034.json\"");
    assert_contains(json, "\"publishedStageCount\":25");
    assert_contains(json, "\"plannedStageCount\":25");
    assert_contains(json, "\"plannedProfileSectionCheckCount\":25");
    assert_contains(json, "\"completedProfileSectionCheckCount\":25");
    assert_contains(json, "\"candidateProfileSectionRendererSplitDeclared\":true");
    assert_contains(json, "\"candidateProfileSectionOnly\":true");
    assert_contains(json, "\"profileSectionChainComplete\":true");
    assert_contains(json, "\"routeFacingMarkdownStable\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"candidateProfileSectionCount\":5");
    assert_contains(json, "\"candidate-document-material-submission-precheck\"");
    assert_contains(json, "\"nodeRendererExecuted\":false");
    assert_contains(json, "\"miniKvConsumesProfileMaterial\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"materialSubmissionOpened\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"candidateDocumentEvaluated\":false");
    assert_contains(json, "\"candidateProfileSectionValidationPassed\":true");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto digest = profile::candidate_profile_section_digest_marker();
    assert(digest.find("v1035-route-preview-candidate-profile-section-closeout-summary-25-of-25-stages") !=
           std::string::npos);
    assert(profile::candidate_profile_section_status() ==
           "route-preview-candidate-profile-section-closeout-summary-read-only");
    assert(profile::published_stage_count() == 25);
}
