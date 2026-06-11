#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_checks {

struct BlockedExecutionRehearsalSplitNonParticipationCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const BlockedExecutionRehearsalSplitNonParticipationCheck>
blocked_execution_rehearsal_split_non_participation_checks();
int planned_blocked_execution_rehearsal_split_non_participation_check_count();
std::string format_blocked_execution_rehearsal_split_non_participation_checks_json(int published_check_count);

} // namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_checks
