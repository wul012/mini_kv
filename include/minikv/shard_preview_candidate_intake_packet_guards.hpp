#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_intake_packet_guards {

struct IntakePacketGuard {
    int sequence;
    std::string_view guard_code;
    std::string_view blocks;
    std::string_view reason;
};

std::span<const IntakePacketGuard> candidate_intake_packet_guards();
int planned_candidate_intake_packet_guard_count();
std::string format_candidate_intake_packet_guards_json(int published_guard_count);

} // namespace minikv::shard_preview_candidate_intake_packet_guards
