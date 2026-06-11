#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_checks {

struct ReleaseWindowReadinessPacketSplitNonParticipationCheck {
    int sequence;
    std::string_view check_code;
    std::string_view source;
    std::string_view protects;
};

std::span<const ReleaseWindowReadinessPacketSplitNonParticipationCheck>
release_window_readiness_packet_split_non_participation_checks();
int planned_release_window_readiness_packet_split_non_participation_check_count();
std::string format_release_window_readiness_packet_split_non_participation_checks_json(
    int published_check_count);

} // namespace minikv::shard_preview_release_window_readiness_packet_split_non_participation_checks
