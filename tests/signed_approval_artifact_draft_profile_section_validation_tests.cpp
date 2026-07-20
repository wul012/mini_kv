#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_signed_approval_artifact_draft_profile_section_validation;

    const auto json = validation::format_signed_approval_artifact_draft_profile_section_validation_json(
        25, true, 25, true, 5, 5, false, false, false, 25, 25, 25, 25);
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionValidationPassed\":true");
    assert_contains(json, "\"sourceCandidateProfileSectionPublishedStageCount\":25");
    assert_contains(json, "\"sourceCandidateProfileSectionComplete\":true");
    assert_contains(json, "\"sourceDraftInstructionPreflightPublishedStageCount\":25");
    assert_contains(json, "\"sourceDraftInstructionPreflightComplete\":true");
    assert_contains(json, "\"sourcesFrozen\":true");
    assert_contains(json, "\"signedApprovalArtifactDraftProfileSectionCount\":5");
    assert_contains(json, "\"nodeMigratedSignedApprovalArtifactDraftProfileSectionCount\":5");
    assert_contains(json, "\"nodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"routeMarkdownChanged\":false");
    assert_contains(json, "\"miniKvConsumesSignedApprovalMaterial\":false");
    assert_contains(json, "\"rendererSplitScopeAligned\":true");
    assert_contains(json, "\"countsAligned\":true");
    assert_contains(json, "\"signedApprovalCreated\":false");
    assert_contains(json, "\"draftArtifactCreated\":false");
    assert_contains(json, "\"runtimePayloadImported\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto failing = validation::format_signed_approval_artifact_draft_profile_section_validation_json(
        25, true, 24, true, 5, 5, false, false, false, 25, 25, 25, 25);
    assert_contains(failing, "\"signedApprovalArtifactDraftProfileSectionValidationPassed\":false");
    assert_contains(failing, "\"sourcesFrozen\":false");
    return 0;
}
