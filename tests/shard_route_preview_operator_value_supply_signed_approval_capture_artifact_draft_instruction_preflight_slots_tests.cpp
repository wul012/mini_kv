#include "minikv/shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}

} // namespace

int main() {
    namespace slots = minikv::
        shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots;

    const auto records = slots::signed_approval_capture_artifact_draft_instruction_preflight_slots();
    assert(records.size() == 25);
    assert(records.front().slot_code == std::string{"request_manifest_slot"});
    assert(records.back().slot_code == std::string{"closeout_slot"});
    assert(slots::planned_signed_approval_capture_artifact_draft_instruction_preflight_slot_count() == 25);

    const auto first = slots::format_signed_approval_capture_artifact_draft_instruction_preflight_slots_json(1);
    assert_contains(first, "\"slotCode\":\"request_manifest_slot\"");
    assert_contains(first, "\"instructionTextMaterialized\":false");
    assert_contains(first, "\"materializesDraftText\":false");
    assert(first.find("\"slotCode\":\"artifact_preflight_digest_slot\"") == std::string::npos);

    const auto all = slots::format_signed_approval_capture_artifact_draft_instruction_preflight_slots_json(99);
    assert_contains(all, "\"slotCode\":\"closeout_slot\"");
    assert_contains(all, "\"acceptsSignatureMaterial\":false");
    assert_contains(all, "\"importsOperatorValue\":false");
    return 0;
}