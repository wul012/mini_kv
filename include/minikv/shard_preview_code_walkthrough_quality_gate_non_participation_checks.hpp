#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_checks {

struct CodeWalkthroughQualityGateNonParticipationCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const CodeWalkthroughQualityGateNonParticipationCheck>
code_walkthrough_quality_gate_non_participation_checks();
int planned_code_walkthrough_quality_gate_non_participation_check_count();
std::string format_code_walkthrough_quality_gate_non_participation_checks_json(int published_check_count);

} // namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_checks
