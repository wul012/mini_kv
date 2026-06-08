#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots {

struct InstructionPreflightSlot {
    int sequence;
    std::string_view slot_code;
    std::string_view source_instruction;
    std::string_view meaning;
};

std::span<const InstructionPreflightSlot> signed_approval_capture_artifact_draft_instruction_preflight_slots();
int planned_signed_approval_capture_artifact_draft_instruction_preflight_slot_count();
std::string format_signed_approval_capture_artifact_draft_instruction_preflight_slots_json(int published_stage_count);

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_slots