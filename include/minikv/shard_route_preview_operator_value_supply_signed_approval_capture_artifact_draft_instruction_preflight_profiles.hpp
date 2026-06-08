#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles {

struct InstructionPreflightProfile {
    int sequence;
    std::string_view profile_code;
    std::string_view boundary;
    std::string_view review_prompt;
};

std::span<const InstructionPreflightProfile> signed_approval_capture_artifact_draft_instruction_preflight_profiles();
int planned_signed_approval_capture_artifact_draft_instruction_preflight_profile_count();
std::string format_signed_approval_capture_artifact_draft_instruction_preflight_profiles_json(int published_stage_count);

} // namespace minikv::shard_route_preview_operator_value_supply_signed_approval_capture_artifact_draft_instruction_preflight_profiles