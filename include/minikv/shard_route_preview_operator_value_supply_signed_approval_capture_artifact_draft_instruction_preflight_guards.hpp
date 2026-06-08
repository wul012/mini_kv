#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards {

struct InstructionPreflightGuard {
    int sequence;
    std::string_view guard_code;
    std::string_view blocks;
    std::string_view reason;
};

std::span<const InstructionPreflightGuard> signed_approval_capture_artifact_draft_instruction_preflight_guards();
int planned_signed_approval_capture_artifact_draft_instruction_preflight_guard_count();
std::string format_signed_approval_capture_artifact_draft_instruction_preflight_guards_json(int published_stage_count);

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_guards