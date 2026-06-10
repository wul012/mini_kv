#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation_checks.hpp"

#include <cassert>
#include <stdexcept>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace checks = minikv::shard_preview_production_live_capture_release_evidence_review_non_participation_checks;

    const auto records = checks::production_live_capture_release_evidence_review_non_participation_checks();
    assert(checks::planned_production_live_capture_release_evidence_review_non_participation_check_count() == 35);
    assert(records.size() == 35);
    assert(records.front().check_code ==
           std::string{"source-archive-verification-non-participation-fixture-frozen"});
    assert(records.back().check_code == std::string{"no-release-evidence-review-chain-created"});

    const auto json = checks::format_production_live_capture_release_evidence_review_non_participation_checks_json(35);
    assert_contains(json, "\"checkCode\":\"node-release-review-endpoint-constant-context\"");
    assert_contains(json, "\"checkCode\":\"node-archive-verification-upstream-consumer-context\"");
    assert_contains(json, "\"checkCode\":\"node-status-live-probe-route-wiring-context\"");
    assert_contains(json, "\"checkCode\":\"no-release-evidence-review-chain-created\"");
    assert_contains(json, "\"releaseEvidenceReviewNonParticipationOnly\":true");
    assert_contains(json, "\"nodeReleaseEvidenceReviewExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewLoaderExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewRendererExecuted\":false");
    assert_contains(json, "\"nodeReleaseReviewEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseReviewDigestGeneratedByMiniKv\":false");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"freshJavaEvidenceRequiredByNode\":false");
    assert_contains(json, "\"miniKvReleaseEvidenceReviewStarted\":false");
    assert_contains(json, "\"miniKvServiceStarted\":false");
    assert_contains(json, "\"releaseReviewEndpointRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"releaseReviewPayloadImported\":false");
    assert_contains(json, "\"releaseReviewFixtureImported\":false");
    assert_contains(json, "\"releaseEvidenceReviewRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");

    bool threw = false;
    try {
        (void)checks::format_production_live_capture_release_evidence_review_non_participation_checks_json(36);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    return 0;
}
