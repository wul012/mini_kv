#pragma once

#include <span>
#include <string>
#include <string_view>

namespace minikv::shard_preview_candidate_intake_packet_slots {

struct IntakePacketSlot {
    int sequence;
    std::string_view slot_code;
    std::string_view source;
    std::string_view purpose;
};

std::span<const IntakePacketSlot> candidate_intake_packet_slots();
int planned_candidate_intake_packet_slot_count();
std::string format_candidate_intake_packet_slots_json(int published_slot_count);

} // namespace minikv::shard_preview_candidate_intake_packet_slots
