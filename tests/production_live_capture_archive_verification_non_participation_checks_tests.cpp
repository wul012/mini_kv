#include "minikv/shard_preview_production_live_capture_archive_verification_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_production_live_capture_archive_verification_non_participation_checks;

    const auto records = checks::production_live_capture_archive_verification_non_participation_checks();
    assert(checks::planned_production_live_capture_archive_verification_non_participation_check_count() == 35);
    assert(records.size() == 35);
    assert(records.front().check_code == std::string{"source-archive-non-participation-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-archive-verification-chain-created"});

    const auto json = checks::format_production_live_capture_archive_verification_non_participation_checks_json(35);
    assert_contains(json, "\"checkCode\":\"node-verification-endpoint-constant-context\"");
    assert_contains(json, "\"checkCode\":\"node-release-evidence-review-consumer-context\"");
    assert_contains(json, "\"checkCode\":\"mini-kv-command-catalog-parity\"");
    assert_contains(json, "\"checkCode\":\"no-archive-verification-chain-created\"");
    assert_contains(json, "\"verificationNonParticipationOnly\":true");
    assert_contains(json, "\"nodeCaptureArchiveVerificationExecuted\":false");
    assert_contains(json, "\"nodeArchiveVerificationLoaderExecuted\":false");
    assert_contains(json, "\"nodeArchiveVerificationRendererExecuted\":false");
    assert_contains(json, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_contains(json, "\"nodeVerificationEndpointReadByMiniKv\":false");
    assert_contains(json, "\"archiveDigestRecomputedByMiniKv\":false");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"freshJavaEvidenceRequiredByNode\":false");
    assert_contains(json, "\"miniKvCaptureArchiveVerificationStarted\":false");
    assert_contains(json, "\"miniKvServiceStarted\":false");
    assert_contains(json, "\"verificationEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"verificationPayloadImported\":false");
    assert_contains(json, "\"verificationFixtureImported\":false");
    assert_contains(json, "\"archiveVerificationRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_production_live_capture_archive_verification_non_participation_checks_json(36);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
