#include "minikv/shard_preview_candidate_intake_packet_slots.hpp"

#include "minikv/runtime_evidence.hpp"

#include <array>
#include <cstddef>
#include <stdexcept>

namespace minikv::shard_preview_candidate_intake_packet_slots {
namespace {

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

constexpr std::array<IntakePacketSlot, 10> slot_catalog = {{
    {1, "source-submission-precheck-fixture", "fixtures/release/shard-readiness-v930.json",
     "anchors the intake packet to the frozen v930 precheck"},
    {2, "node-v1421-plan", "docs/plans3/v1421-controlled-read-only-shard-preview-candidate-document-intake-packet-closeout-roadmap.md",
     "records Node intake packet scope as planning metadata"},
    {3, "document-envelope-placeholder", "future reviewed material contract",
     "names the envelope shape without accepting material"},
    {4, "requested-field-carry-forward", "v895 request package through v930 precheck",
     "keeps twenty requested fields visible without parsing values"},
    {5, "covered-source-checkpoints", "Node v1411 submission precheck",
     "carries twenty-five source checkpoints as metadata"},
    {6, "covered-source-validators", "Node v1411 submission precheck",
     "carries twenty-five source validators as metadata"},
    {7, "intake-guard-bundle", "candidate intake packet guards",
     "groups no-material and no-execution guards"},
    {8, "archive-handoff-reference", "mini-kv v955 archive",
     "points future consumers to versioned evidence instead of runtime archive walks"},
    {9, "command-surface-reference", "COMMANDS and COMMANDSJSON",
     "keeps the short intake packet command visible and read only"},
    {10, "closeout-summary", "candidate intake packet closeout",
     "summarizes the stopped state until reviewed material exists"},
}};

std::string format_slot_json(const IntakePacketSlot& slot) {
    return "{\"sequence\":" + std::to_string(slot.sequence) +
           ",\"slotCode\":" + json_string(slot.slot_code) +
           ",\"source\":" + json_string(slot.source) +
           ",\"purpose\":" + json_string(slot.purpose) +
           ",\"readOnly\":true"
           ",\"reviewedMaterialPresent\":false"
           ",\"materialImported\":false"
           ",\"documentIntakeOpened\":false"
           ",\"candidatePayloadImported\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace

std::span<const IntakePacketSlot> candidate_intake_packet_slots() {
    return {slot_catalog.data(), slot_catalog.size()};
}

int planned_candidate_intake_packet_slot_count() {
    return static_cast<int>(slot_catalog.size());
}

std::string format_candidate_intake_packet_slots_json(int published_slot_count) {
    if (published_slot_count < 0 || static_cast<std::size_t>(published_slot_count) > slot_catalog.size()) {
        throw std::out_of_range{"published intake packet slot count exceeds slot catalog size"};
    }

    std::string result = "[";
    for (int index = 0; index < published_slot_count; ++index) {
        if (index != 0) {
            result += ",";
        }
        result += format_slot_json(slot_catalog[static_cast<std::size_t>(index)]);
    }
    result += "]";
    return result;
}

} // namespace minikv::shard_preview_candidate_intake_packet_slots
