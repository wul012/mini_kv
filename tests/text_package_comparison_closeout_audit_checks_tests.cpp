#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace checks =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks;

    const auto records = checks::signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks();
    assert(records.size() == 25);
    assert(records.front().check_code == std::string{"source_freeze_audit"});
    assert(records.back().check_code == std::string{"comparison_closeout_summary"});
    assert(checks::planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_check_count() == 25);

    const auto first = checks::format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks_json(1);
    assert_contains(first, "\"checkCode\":\"source_freeze_audit\"");
    assert_contains(first, "\"readsRuntimeArchive\":false");
    assert_contains(first, "\"parsesDraftTextPackage\":false");
    assert(first.find("\"checkCode\":\"release_range_audit\"") == std::string::npos);

    const auto all = checks::format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_checks_json(99);
    assert_contains(all, "\"checkCode\":\"comparison_closeout_summary\"");
    assert_contains(all, "\"acceptsDraftTextPackage\":false");
    assert_contains(all, "\"mutatesSiblingState\":false");
}
