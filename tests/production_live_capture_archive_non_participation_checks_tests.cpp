#include "minikv/shard_preview_production_live_capture_archive_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_production_live_capture_archive_non_participation_checks;

    const auto records = checks::production_live_capture_archive_non_participation_checks();
    assert(checks::planned_production_live_capture_archive_non_participation_check_count() == 30);
    assert(records.size() == 30);
    assert(records.front().check_code == std::string{"source-live-capture-non-participation-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-archive-non-participation-chain-created"});

    const auto json = checks::format_production_live_capture_archive_non_participation_checks_json(30);
    assert_contains(json, "\"checkCode\":\"node-archive-endpoint-constant-context\"");
    assert_contains(json, "\"checkCode\":\"mini-kv-command-catalog-parity\"");
    assert_contains(json, "\"checkCode\":\"no-archive-non-participation-chain-created\"");
    assert_contains(json, "\"nonParticipationOnly\":true");
    assert_contains(json, "\"nodeCaptureArchiveExecuted\":false");
    assert_contains(json, "\"nodeArchiveLoaderExecuted\":false");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"freshJavaEvidenceRequiredByNode\":false");
    assert_contains(json, "\"miniKvCaptureArchiveStarted\":false");
    assert_contains(json, "\"miniKvServiceStarted\":false");
    assert_contains(json, "\"archiveEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"archivePayloadImported\":false");
    assert_contains(json, "\"archiveFixtureImported\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_production_live_capture_archive_non_participation_checks_json(31);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
