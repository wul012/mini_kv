#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace profiles = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles;

    const auto records = profiles::signed_approval_capture_artifact_draft_instruction_preflight_profiles();
    assert(records.size() == 25);
    assert(records.front().profile_code == std::string{"instruction_preflight_request_manifest_profile"});
    assert(records.back().profile_code == std::string{"instruction_preflight_closeout_profile"});
    assert(profiles::planned_signed_approval_capture_artifact_draft_instruction_preflight_profile_count() == 25);

    const auto first = profiles::format_signed_approval_capture_artifact_draft_instruction_preflight_profiles_json(1);
    assert_contains(first, "\"profileCode\":\"instruction_preflight_request_manifest_profile\"");
    assert_contains(first, "\"readOnly\":true");
    assert_contains(first, "\"instructionTextAllowed\":false");
    assert_contains(first, "\"approvalGrantAllowed\":false");
    assert_contains(first, "\"operatorValueImportAllowed\":false");
    assert_contains(first, "\"executionAllowed\":false");
    assert(first.find("\"profileCode\":\"instruction_preflight_artifact_preflight_digest_profile\"") ==
           std::string::npos);
    return 0;
}