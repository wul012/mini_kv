#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1196; version <= 1225; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewProductionLiveCaptureArchiveNonParticipation\"");
        assert_contains(fixture, "\"productionLiveCaptureArchiveNonParticipationReleaseVersion\":\"v" +
                                     std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceIntegrityReleaseVersion\":\"v1195\"");
        assert_contains(fixture, "\"sourceIntegrityPublishedStageCount\":25");
        assert_contains(fixture,
                        "\"sourceNodeProductionLiveProbeReadOnlyWindowCaptureArchiveCloseoutReleaseVersion\":\"Node v1656\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvStartsCaptureArchive\":false");
        assert_contains(fixture, "\"miniKvReadsArchiveEndpoint\":false");
        assert_contains(fixture, "\"archiveEndpointRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"archivePayloadImported\":false");
        assert_contains(fixture, "\"archiveFixtureImported\":false");
        assert_contains(fixture, "\"archiveRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1260\"");
    assert_contains(current, "\"productionLiveCaptureNonParticipationReleaseVersion\":\"v1195\"");
    assert_contains(current, "\"productionLiveCaptureArchiveNonParticipationReleaseVersion\":\"v1225\"");

    return 0;
}
