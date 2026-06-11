#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1491; version <= 1505; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture,
                        "\"shardPreviewDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipation\"");
        assert_contains(fixture,
                        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceReleaseWindowReadinessPacketSplitReleaseVersion\":\"v1490\"");
        assert_contains(fixture, "\"sourceReleaseWindowReadinessPacketSplitPublishedStageCount\":15");
        assert_contains(fixture, "\"sourceReleaseWindowReadinessPacketSplitNonParticipationFixtureFrozen\":true");
        assert_contains(fixture,
                        "\"sourceNodeDisabledFakeHarnessContractUpstreamEchoVerificationSplitCloseoutReleaseVersion\":\"Node v1967\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v127\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalJavaReleaseVersionRange\":\"Java v122-v126\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"freshJavaEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvImportsNodeModules\":false");
        assert_contains(fixture, "\"miniKvExecutesNodeChecks\":false");
        assert_contains(fixture, "\"miniKvStartsServices\":false");
        assert_contains(fixture, "\"miniKvReadsEndpoints\":false");
        assert_contains(fixture, "\"miniKvReadsCredentials\":false");
        assert_contains(fixture, "\"miniKvParsesHistoricalReceipts\":false");
        assert_contains(fixture, "\"miniKvReadsHistoricalMiniKvV127ReceiptFile\":false");
        assert_contains(fixture, "\"miniKvScansJavaEvidence\":false");
        assert_contains(fixture, "\"endpointHandleRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"credentialValueRead\":false");
        assert_contains(fixture, "\"managedAuditConnectionOpened\":false");
        assert_contains(fixture, "\"nodeDisabledFakeHarnessContractUpstreamEchoPublicEntrypointImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDisabledFakeHarnessContractUpstreamEchoConstantsImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDisabledFakeHarnessContractUpstreamEchoReferencesImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDisabledFakeHarnessContractUpstreamEchoPolicyExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDisabledFakeHarnessContractUpstreamEchoCoreExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDisabledFakeHarnessContractUpstreamEchoLoaderExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDisabledFakeHarnessContractUpstreamEchoVerificationExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeTypecheckExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeVitestExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBuildExecutedByMiniKv\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1525\"");
    assert_contains(current, "\"releaseWindowReadinessPacketSplitNonParticipationReleaseVersion\":\"v1490\"");
    assert_contains(current,
                    "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_contains(current,
                    "\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");

    return 0;
}
