#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1226; version <= 1260; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewProductionLiveCaptureArchiveVerificationNonParticipation\"");
        assert_contains(fixture,
                        "\"productionLiveCaptureArchiveVerificationNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceIntegrityReleaseVersion\":\"v1225\"");
        assert_contains(fixture, "\"sourceIntegrityPublishedStageCount\":30");
        assert_contains(
            fixture,
            "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveVerificationCloseoutReleaseVersion\":\"Node v1686\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvStartsCaptureArchiveVerification\":false");
        assert_contains(fixture, "\"miniKvReadsVerificationEndpoint\":false");
        assert_contains(fixture, "\"verificationEndpointRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"verificationPayloadImported\":false");
        assert_contains(fixture, "\"verificationFixtureImported\":false");
        assert_contains(fixture, "\"archiveVerificationRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1360\"");
    assert_contains(current, "\"productionLiveCaptureArchiveNonParticipationReleaseVersion\":\"v1225\"");
    assert_contains(current, "\"productionLiveCaptureArchiveVerificationNonParticipationReleaseVersion\":\"v1260\"");
    assert_contains(current, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":\"v1295\"");
    assert_contains(current, "\"catalogEntryGroupSplitNonParticipationReleaseVersion\":\"v1360\"");

    return 0;
}
