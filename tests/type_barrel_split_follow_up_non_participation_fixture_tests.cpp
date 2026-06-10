#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1386; version <= 1405; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewTypeBarrelSplitFollowUpNonParticipation\"");
        assert_contains(fixture,
                        "\"typeBarrelSplitFollowUpNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceTypeBarrelSplitReleaseVersion\":\"v1385\"");
        assert_contains(fixture, "\"sourceTypeBarrelSplitPublishedStageCount\":25");
        assert_contains(fixture, "\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvStartsTypeBarrelFollowUp\":false");
        assert_contains(fixture, "\"miniKvImportsNodeTypes\":false");
        assert_contains(fixture, "\"miniKvReadsProfileEndpoint\":false");
        assert_contains(fixture, "\"profileBoundaryEndpointRead\":false");
        assert_contains(fixture, "\"evidenceEndpointRead\":false");
        assert_contains(fixture, "\"nodeStableBarrelImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeProfileTypesImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeAggregateReexportsImportedByMiniKv\":false");
        assert_contains(fixture, "\"typeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1405\"");
    assert_contains(current, "\"typeBarrelSplitNonParticipationReleaseVersion\":\"v1385\"");
    assert_contains(current, "\"typeBarrelSplitFollowUpNonParticipationReleaseVersion\":\"v1405\"");

    return 0;
}
