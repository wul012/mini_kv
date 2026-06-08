#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace validation =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_validation;

    const auto passed =
        validation::format_signed_approval_capture_artifact_draft_text_package_review_preflight_validation_json(
            25,
            25,
            25,
            true,
            1);
    assert_contains(passed, "\"draftTextPackageReviewPreflightValidationPassed\":true");
    assert_contains(passed, "\"sourceInstructionPreflightChainComplete\":true");
    assert_contains(passed, "\"reviewCatalogsAligned\":true");
    assert_contains(passed, "\"publishedReviewCriteriaGuarded\":true");
    assert_contains(passed, "\"readyForSignedApprovalCapture\":false");
    assert_contains(passed, "\"draftTextPackageAcceptanceAllowed\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed =
        validation::format_signed_approval_capture_artifact_draft_text_package_review_preflight_validation_json(
            25,
            24,
            25,
            true,
            1);
    assert_contains(failed, "\"draftTextPackageReviewPreflightValidationPassed\":false");
    assert_contains(failed, "\"reviewCatalogsAligned\":false");
}