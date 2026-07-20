#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards.hpp"

#include <cassert>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace guards = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards;
    const auto records =
        guards::signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards();
    assert(records.size() == 10);
    assert(records.front().guard_code == std::string{"source_acceptance_precheck_guard"});
    assert(records.back().guard_code == std::string{"archive_closeout_guard"});
    assert(
        guards::
            planned_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guard_count() ==
        10);
    const auto none = guards::
        format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards_json(
            0);
    assert(none == "[]");
    const auto all = guards::
        format_signed_approval_capture_artifact_draft_text_package_compared_package_evidence_intake_audit_guards_json(
            99);
    assert_contains(all, "\"guardCode\":\"archive_closeout_guard\"");
    assert_contains(all, "\"guardBypassed\":false");
    return 0;
}