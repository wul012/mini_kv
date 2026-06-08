#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace profiles =
        minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles;

    const auto records = profiles::signed_approval_capture_artifact_draft_text_package_review_preflight_profiles();
    assert(records.size() == 25);
    assert(records.front().profile_code == std::string{"draft_text_package_review_preflight_package_manifest_criterion_profile"});
    assert(records.back().profile_code == std::string{"draft_text_package_review_preflight_closeout_profile"});
    assert(profiles::planned_signed_approval_capture_artifact_draft_text_package_review_preflight_profile_count() == 25);

    const auto first = profiles::format_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles_json(1);
    assert_contains(first, "\"profileCode\":\"draft_text_package_review_preflight_package_manifest_criterion_profile\"");
    assert_contains(first, "\"draftTextPackageAcceptanceAllowed\":false");
    assert_contains(first, "\"executionAllowed\":false");
    assert(first.find("\"profileCode\":\"draft_text_package_review_preflight_instruction_preflight_digest_criterion_profile\"") ==
           std::string::npos);

    const auto all = profiles::format_signed_approval_capture_artifact_draft_text_package_review_preflight_profiles_json(99);
    assert_contains(all, "\"profileCode\":\"draft_text_package_review_preflight_closeout_profile\"");
    assert_contains(all, "\"operatorValueImportAllowed\":false");
}