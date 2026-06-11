#include <cassert>
#include "test_support.hpp"

#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1506; version <= 1525; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewBlockedExecutionRehearsalSplitNonParticipation\"");
        assert_contains(fixture,
                        "\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(
            fixture,
            "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
        assert_contains(fixture,
                        "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitPublishedStageCount\":15");
        assert_contains(fixture,
                        "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitComplete\":true");
        assert_contains(fixture,
                        "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true");
        assert_contains(fixture,
                        "\"sourceNodeBlockedExecutionRehearsalSplitCloseoutReleaseVersion\":\"Node v1982\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v99\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v90\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"freshJavaEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvImportsNodeModules\":false");
        assert_contains(fixture, "\"miniKvExecutesNodeChecks\":false");
        assert_contains(fixture, "\"miniKvStartsServices\":false");
        assert_contains(fixture, "\"miniKvReadsEndpoints\":false");
        assert_contains(fixture, "\"miniKvReadsCredentials\":false");
        assert_contains(fixture, "\"miniKvParsesHistoricalReceipts\":false");
        assert_contains(fixture, "\"miniKvReadsHistoricalMiniKvV99WalRegressionFile\":false");
        assert_contains(fixture, "\"miniKvScansJavaV90ContextNormalizationEvidence\":false");
        assert_contains(fixture, "\"miniKvScansJavaEvidence\":false");
        assert_contains(fixture, "\"miniKvReplaysWalRegression\":false");
        assert_contains(fixture, "\"endpointHandleRead\":false");
        assert_contains(fixture, "\"rawEndpointParsed\":false");
        assert_contains(fixture, "\"credentialValueRead\":false");
        assert_contains(fixture, "\"managedAuditConnectionOpened\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodePreconditionIntakeExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeTypecheckExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeVitestExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBuildExecutedByMiniKv\":false");
        assert_contains(fixture, "\"activeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1545\"");
    assert_contains(
        current,
        "\"disabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_contains(current,
                    "\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_contains(current,
                    "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v1545\"");

    return 0;
}
