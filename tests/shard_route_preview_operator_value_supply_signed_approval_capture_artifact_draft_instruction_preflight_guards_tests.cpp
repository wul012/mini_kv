#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace guards = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards;

    const auto records = guards::signed_approval_capture_artifact_draft_instruction_preflight_guards();
    assert(records.size() == 25);
    assert(records.front().guard_code == std::string{"request_manifest_guard"});
    assert(records.back().guard_code == std::string{"closeout_guard"});
    assert(guards::planned_signed_approval_capture_artifact_draft_instruction_preflight_guard_count() == 25);

    const auto first = guards::format_signed_approval_capture_artifact_draft_instruction_preflight_guards_json(1);
    assert_contains(first, "\"guardCode\":\"request_manifest_guard\"");
    assert_contains(first, "\"draftInstructionTextBlocked\":true");
    assert_contains(first, "\"runtimeExecutionBlocked\":true");
    assert_contains(first, "\"writeRouteBlocked\":true");
    assert_contains(first, "\"siblingMutationBlocked\":true");
    assert(first.find("\"guardCode\":\"artifact_preflight_digest_guard\"") == std::string::npos);
    return 0;
}