#include "minikv/shard_preview_candidate_intake_packet_guards.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {
void assert_contains(const std::string& text, const std::string& fragment) {
    assert(text.find(fragment) != std::string::npos);
}
} // namespace

int main() {
    namespace guards = minikv::shard_preview_candidate_intake_packet_guards;

    const auto catalog = guards::candidate_intake_packet_guards();
    assert(catalog.size() == 10);
    assert(guards::planned_candidate_intake_packet_guard_count() == 10);
    assert(catalog.front().guard_code == std::string{"real-material-required-but-absent"});
    assert(catalog.back().guard_code == std::string{"no-extra-intake-echo"});

    const auto partial_json = guards::format_candidate_intake_packet_guards_json(3);
    assert_contains(partial_json, "\"guardCode\":\"real-material-required-but-absent\"");
    assert_contains(partial_json, "\"guardCode\":\"synthetic-material-rejected\"");
    assert_contains(partial_json, "\"guardCode\":\"payload-import-blocked\"");
    assert(partial_json.find("\"guardCode\":\"candidate-evaluation-blocked\"") == std::string::npos);

    const auto full_json = guards::format_candidate_intake_packet_guards_json(10);
    assert_contains(full_json, "\"guardCode\":\"document-router-blocked\"");
    assert_contains(full_json, "\"guardCode\":\"write-routing-blocked\"");
    assert_contains(full_json, "\"guardCode\":\"no-extra-intake-echo\"");
    assert_contains(full_json, "\"documentIntakeOpened\":false");
    assert_contains(full_json, "\"candidatePayloadImported\":false");
    assert_contains(full_json, "\"writeRoutingAllowed\":false");
    assert_contains(full_json, "\"touchesWal\":false");
    assert_contains(full_json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)guards::format_candidate_intake_packet_guards_json(11);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
    return 0;
}
