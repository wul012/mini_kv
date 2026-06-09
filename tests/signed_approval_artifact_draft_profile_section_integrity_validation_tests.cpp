#include "minikv/shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation.hpp"

#include <cassert>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::shard_preview_signed_approval_artifact_draft_profile_section_integrity_validation;

    const auto good = validation::format_signed_approval_artifact_draft_profile_section_integrity_validation_json(
        25, true, 5, 5, false, false, false, false, false, 25, 25, 25, 25);
    assert_contains(good, "\"signedApprovalArtifactDraftProfileSectionIntegrityValidationPassed\":true");
    assert_contains(good, "\"sourceProfileSectionFrozen\":true");
    assert_contains(good, "\"profileScopeAligned\":true");
    assert_contains(good, "\"countsAligned\":true");
    assert_contains(good, "\"signedApprovalCreated\":false");
    assert_contains(good, "\"materialIntakeOpened\":false");
    assert_contains(good, "\"executionAllowed\":false");

    const auto bad = validation::format_signed_approval_artifact_draft_profile_section_integrity_validation_json(
        25, true, 4, 5, false, false, false, false, false, 25, 25, 25, 25);
    assert_contains(bad, "\"signedApprovalArtifactDraftProfileSectionIntegrityValidationPassed\":false");
    assert_contains(bad, "\"profileScopeAligned\":false");
}
