#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace blockers = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers;

    const auto records = blockers::signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers();
    assert(records.size() == 25);
    assert(records.front().blocker_code == std::string{"source_freeze_audit_blocker"});
    assert(records.back().blocker_code == std::string{"closeout_audit_summary_blocker"});
    assert(
        blockers::planned_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blocker_count() ==
        25);

    const auto first =
        blockers::format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers_json(1);
    assert_contains(first, "\"blockerCode\":\"source_freeze_audit_blocker\"");
    assert_contains(first, "\"blocks\":\"rolling_current_as_source\"");
    assert_contains(first, "\"failClosed\":true");
    assert(first.find("\"blockerCode\":\"release_range_audit_blocker\"") == std::string::npos);

    const auto all =
        blockers::format_signed_approval_capture_artifact_draft_text_package_review_closeout_audit_blockers_json(99);
    assert_contains(all, "\"blockerCode\":\"closeout_audit_summary_blocker\"");
    assert_contains(all, "\"runtimeExecutionBlocked\":true");
    assert_contains(all, "\"writeRouteBlocked\":true");
    return 0;
}
