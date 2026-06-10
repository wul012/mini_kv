#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1296; version <= 1330; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewCatalogEntryGroupSplitNonParticipation\"");
        assert_contains(fixture,
                        "\"catalogEntryGroupSplitNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceIntegrityReleaseVersion\":\"v1295\"");
        assert_contains(fixture, "\"sourceIntegrityPublishedStageCount\":35");
        assert_contains(fixture, "\"sourceNodeCatalogEntryGroupSplitCloseoutReleaseVersion\":\"Node v1756\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvStartsCatalogSplit\":false");
        assert_contains(fixture, "\"miniKvConsumesCatalogRecords\":false");
        assert_contains(fixture, "\"miniKvReadsCatalogEndpoint\":false");
        assert_contains(fixture, "\"catalogEndpointRead\":false");
        assert_contains(fixture, "\"catalogRecordsImported\":false");
        assert_contains(fixture, "\"catalogRegistryExecuted\":false");
        assert_contains(fixture, "\"catalogRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1330\"");
    assert_contains(current, "\"productionLiveCaptureReleaseEvidenceReviewNonParticipationReleaseVersion\":\"v1295\"");
    assert_contains(current, "\"catalogEntryGroupSplitNonParticipationReleaseVersion\":\"v1330\"");

    return 0;
}
