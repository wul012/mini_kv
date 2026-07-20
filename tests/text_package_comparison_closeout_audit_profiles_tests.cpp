#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace profiles = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles;

    const auto records =
        profiles::signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles();
    assert(records.size() == 25);
    assert(records.front().profile_code ==
           std::string{"draft_text_package_comparison_closeout_audit_source_freeze_profile"});
    assert(records.back().profile_code == std::string{"draft_text_package_comparison_closeout_audit_summary_profile"});
    assert(profiles::
               planned_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profile_count() ==
           25);

    const auto first =
        profiles::format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles_json(1);
    assert_contains(first, "\"profileCode\":\"draft_text_package_comparison_closeout_audit_source_freeze_profile\"");
    assert_contains(first, "\"boundary\":\"source-fixture-only\"");
    assert_contains(first, "\"draftTextPackageAcceptanceAllowed\":false");
    assert(first.find("\"profileCode\":\"draft_text_package_comparison_closeout_audit_release_range_profile\"") ==
           std::string::npos);

    const auto all =
        profiles::format_signed_approval_capture_artifact_draft_text_package_comparison_closeout_audit_profiles_json(
            99);
    assert_contains(all, "\"profileCode\":\"draft_text_package_comparison_closeout_audit_summary_profile\"");
    assert_contains(all, "\"runtimePayloadAllowed\":false");
    assert_contains(all, "\"executionAllowed\":false");
    return 0;
}
