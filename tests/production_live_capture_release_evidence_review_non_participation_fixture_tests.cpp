#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1261; version <= 1295; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewProductionLiveCaptureReleaseEvidenceReviewNonParticipation\"");
        assert_contains(fixture,
                        "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceIntegrityReleaseVersion\":\"v1260\"");
        assert_contains(fixture, "\"sourceIntegrityPublishedStageCount\":35");
        assert_contains(
            fixture,
            "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureReleaseEvidenceReviewCloseoutReleaseVersion\":\"Node v1721\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvStartsReleaseEvidenceReview\":false");
        assert_contains(fixture, "\"miniKvReadsReleaseReviewEndpoint\":false");
        assert_contains(fixture, "\"releaseReviewEndpointRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"releaseReviewPayloadImported\":false");
        assert_contains(fixture, "\"releaseReviewFixtureImported\":false");
        assert_contains(fixture, "\"releaseEvidenceReviewRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1385\"");
    assert_contains(current, "\"productionLiveCaptureArchiveVerificationNonParticipationReleaseVersion\":\"v1260\"");
    assert_contains(current, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":\"v1295\"");
    assert_contains(current, "\"catalogEntryGroupSplitNonParticipationReleaseVersion\":\"v1360\"");

    return 0;
}
