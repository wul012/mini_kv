#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_validation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace validation = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_validation;

    const auto passed = validation::
        format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_validation_json(25, 25, 25,
                                                                                                             true, 1);
    assert_contains(passed, "\"draftTextPackageComparisonCloseoutAuditValidationPassed\":true");
    assert_contains(passed, "\"sourceReviewCloseoutAuditChainComplete\":true");
    assert_contains(passed, "\"comparisonCatalogsAligned\":true");
    assert_contains(passed, "\"publishedAuditChecksGuarded\":true");
    assert_contains(passed, "\"readyForDraftTextPackageComparison\":false");
    assert_contains(passed, "\"draftTextPackageAcceptanceAllowed\":false");
    assert_contains(passed, "\"executionAllowed\":false");

    const auto failed = validation::
        format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_validation_json(25, 24, 25,
                                                                                                             true, 1);
    assert_contains(failed, "\"draftTextPackageComparisonCloseoutAuditValidationPassed\":false");
    assert_contains(failed, "\"comparisonCatalogsAligned\":false");
    return 0;
}
