#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1526; version <= 1545; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewPrecheckUpstreamReceiptVerificationSplitNonParticipation\"");
        assert_contains(fixture,
                        "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourceBlockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
        assert_contains(fixture,
                        "\"sourceBlockedExecutionRehearsalSplitNonParticipationFixturePath\":\"fixtures/release/"
                        "shard-readiness-v1525.json\"");
        assert_contains(fixture, "\"sourceBlockedExecutionRehearsalSplitNonParticipationPublishedStageCount\":20");
        assert_contains(fixture, "\"sourceBlockedExecutionRehearsalSplitNonParticipationComplete\":true");
        assert_contains(fixture, "\"sourceBlockedExecutionRehearsalSplitNonParticipationFixtureFrozen\":true");
        assert_contains(fixture,
                        "\"sourceNodePrecheckUpstreamReceiptVerificationSplitCloseoutReleaseVersion\":\"Node v2002\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v108\"");
        assert_contains(fixture, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v99\"");
        assert_contains(fixture, "\"freshMiniKvEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"freshJavaEvidenceRequiredByNode\":false");
        assert_contains(fixture, "\"miniKvImportsNodeModules\":false");
        assert_contains(fixture, "\"miniKvExecutesNodeChecks\":false");
        assert_contains(fixture, "\"miniKvStartsServices\":false");
        assert_contains(fixture, "\"miniKvReadsEndpoints\":false");
        assert_contains(fixture, "\"miniKvReadsCredentials\":false");
        assert_contains(fixture, "\"miniKvParsesHistoricalReceipts\":false");
        assert_contains(fixture, "\"miniKvReadsHistoricalMiniKvV108NonParticipationFile\":false");
        assert_contains(fixture, "\"miniKvScansJavaV99Evidence\":false");
        assert_contains(fixture, "\"miniKvScansJavaEvidence\":false");
        assert_contains(fixture, "\"miniKvExecutesDownstreamChecks\":false");
        assert_contains(fixture, "\"miniKvMutatesLargeFileInventory\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationPublicLoaderImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationTypesImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationConstantsImportedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationReferencesParsedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationCheckPolicyExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationMessagePolicyExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationRendererExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodePrecheckUpstreamReceiptVerificationEntrypointExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeCodeHealthScanExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeLargeFileInventoryMutatedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDirectTestsExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDownstreamCodeHealthExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeDownstreamRehearsalGuardExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeTypecheckExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeVitestExecutedByMiniKv\":false");
        assert_contains(fixture, "\"nodeBuildExecutedByMiniKv\":false");
        assert_contains(fixture, "\"activeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"loadRestoreCompactAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1545\"");
    assert_contains(current, "\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_contains(current,
                    "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v1545\"");

    return 0;
}
