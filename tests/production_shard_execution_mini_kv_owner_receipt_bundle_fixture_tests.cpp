#include "test_support.hpp"

#include <string>
#include <vector>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_current_shard_readiness_status;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;
    using minikv::test_support::release_fixture_path;

    const std::vector<std::string> stages{
        "production-shard-execution-mini-kv-owner-receipt-bundle-plan-intake",
        "production-shard-execution-mini-kv-owner-receipt-bundle-abort-rollback",
        "production-shard-execution-mini-kv-owner-receipt-bundle-lifecycle-no-start",
        "production-shard-execution-mini-kv-owner-receipt-bundle-cleanup-no-start-proof",
        "production-shard-execution-mini-kv-owner-receipt-bundle-clean-ci-closeout",
    };

    for (int release = 1597; release <= 1601; ++release) {
        const auto fixture = read_shard_readiness_fixture(release);
        const int index = release - 1597;
        assert_contains(fixture, "\"shardPreviewProductionShardExecutionMiniKvOwnerReceiptBundle\":{");
        assert_contains(fixture, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseVersion\":\"v" +
                                     std::to_string(release) + "\"");
        assert_contains(fixture, "\"productionShardExecutionMiniKvOwnerReceiptBundleStage\":\"" +
                                     stages[static_cast<std::size_t>(index)] + "\"");
        assert_contains(fixture, "\"productionShardExecutionMiniKvOwnerReceiptBundleStageSequence\":" +
                                     std::to_string(index + 1));
        assert_contains(fixture, "\"sourceOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
        assert_contains(fixture, "\"miniKvReceiptSlotCount\":4");
        assert_contains(fixture, "\"miniKvReceiptsComplete\":true");
        assert_contains(fixture, "\"javaOwnerReceiptsPresent\":false");
        assert_contains(fixture, "\"crossProjectCleanupReconciliationReceiptPresent\":false");
        assert_contains(fixture, "\"archiveRoot\":\"f\"");
        assert_contains(fixture, "\"archiveRootIsF\":true");
        assert_contains(fixture, "\"combinedArchiveFolderAllowed\":false");
        assert_contains(fixture, "\"abortExecutionPerformed\":false");
        assert_contains(fixture, "\"rollbackExecutionPerformed\":false");
        assert_contains(fixture, "\"lifecycleServiceStarted\":false");
        assert_contains(fixture, "\"cleanupExecutionPerformed\":false");
        assert_contains(fixture, "\"activeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"mutatesStore\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1606\"");
    assert_current_shard_readiness_status(current);
    assert_contains(current, "\"commandCount\":93");
    assert_contains(current, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_contains(current, "\"productionShardExecutionMiniKvOwnerReceiptBundleValidationPassed\":true");
    assert_contains(current, "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseVersion\":\"v1606\"");
    assert_contains(current, "\"versionedArchiveDirectories\":[\"f/1597/\",\"f/1598/\",\"f/1599/\",\"f/1600/\",\"f/1601/\"]");

    return 0;
}
