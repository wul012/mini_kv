#include "minikv/shard_preview_candidate_intake_packet_slots.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace slots = minikv::shard_preview_candidate_intake_packet_slots;

    const auto catalog = slots::candidate_intake_packet_slots();
    assert(catalog.size() == 10);
    assert(slots::planned_candidate_intake_packet_slot_count() == 10);
    assert(catalog.front().slot_code == std::string{"source-submission-precheck-fixture"});
    assert(catalog.back().slot_code == std::string{"closeout-summary"});

    const auto partial_json = slots::format_candidate_intake_packet_slots_json(2);
    assert_contains(partial_json, "\"slotCode\":\"source-submission-precheck-fixture\"");
    assert_contains(partial_json, "\"slotCode\":\"node-v1421-plan\"");
    assert(partial_json.find("\"slotCode\":\"document-envelope-placeholder\"") == std::string::npos);

    const auto full_json = slots::format_candidate_intake_packet_slots_json(10);
    assert_contains(full_json, "\"slotCode\":\"archive-handoff-reference\"");
    assert_contains(full_json, "\"reviewedMaterialPresent\":false");
    assert_contains(full_json, "\"materialImported\":false");
    assert_contains(full_json, "\"documentIntakeOpened\":false");
    assert_contains(full_json, "\"writeRoutingAllowed\":false");
    assert_contains(full_json, "\"touchesWal\":false");
    assert_contains(full_json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)slots::format_candidate_intake_packet_slots_json(11);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    return 0;
}
