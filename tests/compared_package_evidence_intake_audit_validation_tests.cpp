#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace validation = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation;
    const auto ok = validation::
        format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation_json(
            10, 10, 1, 0, true, 1);
    assert_contains(ok, "\"comparedPackageEvidenceIntakeAuditValidationPassed\":true");
    assert_contains(ok, "\"completedSlotCount\":1");
    assert_contains(ok, "\"completedGuardCount\":0");
    assert_contains(ok, "\"payloadAccepted\":false");
    assert_contains(ok, "\"executionAllowed\":false");

    const auto blocked = validation::
        format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_validation_json(
            10, 10, 0, 1, true, 1);
    assert_contains(blocked, "\"comparedPackageEvidenceIntakeAuditValidationPassed\":false");
    return 0;
}
