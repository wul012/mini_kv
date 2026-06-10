#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1171; version <= 1195; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewProductionLiveCaptureNonParticipation\"");
        assert_contains(fixture, "\"productionLiveCaptureNonParticipationReleaseVersion\":\"v" +
                                     std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceIntegrityReleaseVersion\":\"v1170\"");
        assert_contains(fixture, "\"sourceIntegrityPublishedStageCount\":10");
        assert_contains(fixture, "\"sourceNodeProductionLiveProbeReadOnlyWindowLiveCaptureCloseoutReleaseVersion\":\"Node v1631\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvStartsLiveCapture\":false");
        assert_contains(fixture, "\"miniKvReadsRawEndpoint\":false");
        assert_contains(fixture, "\"liveEndpointRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"rawEndpointStored\":false");
        assert_contains(fixture, "\"credentialValueRead\":false");
        assert_contains(fixture, "\"productionLiveReadAllowed\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1225\"");
    assert_contains(current, "\"operatorValueSupplyProfileSectionIntegrityReleaseVersion\":\"v1170\"");
    assert_contains(current, "\"productionLiveCaptureNonParticipationReleaseVersion\":\"v1195\"");

    return 0;
}
