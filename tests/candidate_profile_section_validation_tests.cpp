#include "minikv/shard_preview_candidate_profile_section_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_candidate_profile_section_validation;

    const auto json = validation::format_candidate_profile_section_validation_json(
        25, true, 5, 5, false, false, false, 25, 25, 25, 25);
    assert_contains(json, "\"candidateProfileSectionValidationPassed\":true");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityPublishedStageCount\":25");
    assert_contains(json, "\"sourceCandidateMaterialSubmissionPrecheckIntegrityComplete\":true");
    assert_contains(json, "\"sourceIntegrityFrozen\":true");
    assert_contains(json, "\"candidateProfileSectionCount\":5");
    assert_contains(json, "\"nodeMigratedCandidateProfileSectionCount\":5");
    assert_contains(json, "\"nodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"routeMarkdownChanged\":false");
    assert_contains(json, "\"miniKvConsumesProfileMaterial\":false");
    assert_contains(json, "\"rendererSplitScopeAligned\":true");
    assert_contains(json, "\"profileSectionCountsAligned\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"documentIntakeOpened\":false");
    assert_contains(json, "\"materialPayloadImported\":false");
    assert_contains(json, "\"documentRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    const auto failing = validation::format_candidate_profile_section_validation_json(
        25, true, 5, 4, false, false, false, 25, 25, 25, 25);
    assert_contains(failing, "\"candidateProfileSectionValidationPassed\":false");
    assert_contains(failing, "\"rendererSplitScopeAligned\":false");
}
