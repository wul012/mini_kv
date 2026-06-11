#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1418; version <= 1442; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewDisabledPrecheckUpstreamEchoNonParticipation\"");
        assert_contains(fixture,
                        "\"disabledPrecheckUpstreamEchoNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceFixtureAuditReleaseVersion\":\"v1417\"");
        assert_contains(fixture, "\"sourceFixtureAuditPublishedStageCount\":12");
        assert_contains(fixture, "\"sourceFixtureAuditFrozen\":true");
        assert_contains(fixture, "\"sourceNodeDisabledPrecheckUpstreamEchoCloseoutReleaseVersion\":\"Node v1878\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvImportsNodeModules\":false");
        assert_contains(fixture, "\"miniKvExecutesNodeChecks\":false");
        assert_contains(fixture, "\"miniKvReadsEndpoints\":false");
        assert_contains(fixture, "\"miniKvReadsCredentials\":false");
        assert_contains(fixture, "\"endpointHandleRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"credentialValueRead\":false");
        assert_contains(fixture, "\"nodeDisabledPrecheckStableBarrelImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDisabledPrecheckCoreLoaderExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeTypecheckExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeVitestExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBuildExecutedByMiniKv\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1475\"");
    assert_contains(current, "\"typeBarrelSplitFollowUpFixtureAuditReleaseVersion\":\"v1417\"");
    assert_contains(current, "\"disabledPrecheckUpstreamEchoNonParticipationReleaseVersion\":\"v1442\"");
    assert_contains(current,
                    "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseVersion\":\"v1472\"");
    assert_contains(current,
                    "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseVersion\":\"v1475\"");

    return 0;
}
