#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1476; version <= 1490; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewReleaseWindowReadinessPacketSplitNonParticipation\"");
        assert_contains(fixture,
                        "\"releaseWindowReadinessPacketSplitNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceImplementationPlanReleaseVersion\":\"v1475\"");
        assert_contains(fixture, "\"sourceImplementationPlanPublishedStageCount\":3");
        assert_contains(fixture, "\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true");
        assert_contains(fixture,
                        "\"sourceNodeReleaseWindowReadinessPacketSplitCloseoutReleaseVersion\":\"Node v1937\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v70\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v61\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"freshJavaEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvImportsNodeModules\":false");
        assert_contains(fixture, "\"miniKvExecutesNodeChecks\":false");
        assert_contains(fixture, "\"miniKvStartsServices\":false");
        assert_contains(fixture, "\"miniKvReadsEndpoints\":false");
        assert_contains(fixture, "\"miniKvReadsCredentials\":false");
        assert_contains(fixture, "\"miniKvExecutesReleaseWindowPacket\":false");
        assert_contains(fixture, "\"miniKvReplaysRestoreDrillEvidence\":false");
        assert_contains(fixture, "\"endpointHandleRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"credentialValueRead\":false");
        assert_contains(fixture, "\"managedAuditConnectionOpened\":false");
        assert_contains(fixture, "\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeTypecheckExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeVitestExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBuildExecutedByMiniKv\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1505\"");
    assert_contains(current,
                    "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseVersion\":\"v1475\"");
    assert_contains(current,
                    "\"releaseWindowReadinessPacketSplitNonParticipationReleaseVersion\":\"v1490\"");
    assert_contains(current,
                    "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");

    return 0;
}
