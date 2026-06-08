#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace controls =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_guards;

    const auto records = controls::signed_approval_capture_artifact_draft_text_package_review_preflight_guards();
    assert(records.size() == 25);
    assert(records.front().guard_code == std::string{"package_manifest_criterion_control"});
    assert(records.back().guard_code == std::string{"closeout_control"});
    assert(controls::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_guard_count() == 25);

    const auto first = controls::format_signed_approval_capture_artifact_draft_text_package_review_preflight_guards_json(1);
    assert_contains(first, "\"controlCode\":\"package_manifest_criterion_control\"");
    assert_contains(first, "\"draftTextPackageAcceptanceBlocked\":true");
    assert_contains(first, "\"runtimeExecutionBlocked\":true");
    assert(first.find("\"controlCode\":\"instruction_preflight_digest_criterion_control\"") == std::string::npos);

    const auto all = controls::format_signed_approval_capture_artifact_draft_text_package_review_preflight_guards_json(99);
    assert_contains(all, "\"controlCode\":\"closeout_control\"");
    assert_contains(all, "\"writeRouteBlocked\":true");
    assert_contains(all, "\"siblingMutationBlocked\":true");
}