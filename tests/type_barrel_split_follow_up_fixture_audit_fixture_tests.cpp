#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1406; version <= 1417; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewTypeBarrelSplitFollowUpFixtureAudit\"");
        assert_contains(fixture,
                        "\"typeBarrelSplitFollowUpFixtureAuditReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceFollowUpReleaseVersion\":\"v1405\"");
        assert_contains(fixture, "\"sourceFollowUpPublishedStageCount\":20");
        assert_contains(fixture, "\"sourceFollowUpNonParticipationFixtureFrozen\":true");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvRuntimeReadsSourceFixturePayload\":false");
        assert_contains(fixture, "\"miniKvImportsNodeModules\":false");
        assert_contains(fixture, "\"miniKvExecutesNodeChecks\":false");
        assert_contains(fixture, "\"profileBoundaryEndpointRead\":false");
        assert_contains(fixture, "\"evidenceEndpointRead\":false");
        assert_contains(fixture, "\"nodeStableBarrelImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeTestOnlyShellModulesImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeCredentialResolverShellExecutedByMiniKv\":false");
        assert_contains(fixture, "\"typeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1417\"");
    assert_contains(current, "\"typeBarrelSplitFollowUpNonParticipationReleaseVersion\":\"v1405\"");
    assert_contains(current, "\"typeBarrelSplitFollowUpFixtureAuditReleaseVersion\":\"v1417\"");

    return 0;
}
