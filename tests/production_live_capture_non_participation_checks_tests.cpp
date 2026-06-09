#include "minikv/shard_preview_production_live_capture_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_production_live_capture_non_participation_checks;

    const auto records = checks::production_live_capture_non_participation_checks();
    assert(checks::planned_production_live_capture_non_participation_check_count() == 25);
    assert(records.size() == 25);
    assert(records.front().check_code == std::string{"source-integrity-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-live-capture-chain-created"});

    const auto json = checks::format_production_live_capture_non_participation_checks_json(25);
    assert_contains(json, "\"checkCode\":\"node-live-capture-profile-type-context\"");
    assert_contains(json, "\"checkCode\":\"node-endpoint-constant-context\"");
    assert_contains(json, "\"checkCode\":\"no-live-capture-chain-created\"");
    assert_contains(json, "\"nonParticipationOnly\":true");
    assert_contains(json, "\"nodeLiveCaptureExecuted\":false");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"freshJavaEvidenceRequiredByNode\":false");
    assert_contains(json, "\"miniKvLiveCaptureStarted\":false");
    assert_contains(json, "\"miniKvServiceStarted\":false");
    assert_contains(json, "\"liveEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"rawEndpointStored\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"productionLiveReadAllowed\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_production_live_capture_non_participation_checks_json(26);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
