#include "minikv/shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::shard_preview_signed_approval_artifact_draft_text_package_profile_section_validation;

    const auto passed = validation::format_signed_approval_artifact_draft_text_package_profile_section_validation_json(
        25, true, 25, true, 25, true, 25, true, 20, true, 9, 9, false, false, false, 25, 25, 25, 25);
    assert_contains(passed, "\"signedApprovalArtifactDraftTextPackageProfileSectionValidationPassed\":true");
    assert_contains(passed, "\"sourcesFrozen\":true");
    assert_contains(passed, "\"rendererSplitScopeAligned\":true");
    assert_contains(passed, "\"countsAligned\":true");
    assert_contains(passed, "\"secretValueRead\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed_count =
        validation::format_signed_approval_artifact_draft_text_package_profile_section_validation_json(
            25, true, 25, true, 25, true, 25, true, 20, true, 8, 9, false, false, false, 25, 25, 25, 25);
    assert_contains(failed_count, "\"signedApprovalArtifactDraftTextPackageProfileSectionValidationPassed\":false");
    assert_contains(failed_count, "\"rendererSplitScopeAligned\":false");

    const auto failed_source =
        validation::format_signed_approval_artifact_draft_text_package_profile_section_validation_json(
            25, true, 25, true, 25, true, 25, true, 19, true, 9, 9, false, false, false, 25, 25, 25, 25);
    assert_contains(failed_source, "\"signedApprovalArtifactDraftTextPackageProfileSectionValidationPassed\":false");
    assert_contains(failed_source, "\"sourcesFrozen\":false");

    return 0;
}
