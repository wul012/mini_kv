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
        "production-shard-execution-external-artifact-dry-run-closeout-plan-intake",
        "production-shard-execution-external-artifact-dry-run-closeout-batch-stop",
        "production-shard-execution-external-artifact-dry-run-closeout-missing-artifact-gate",
        "production-shard-execution-external-artifact-dry-run-closeout-real-receipt-gap",
        "production-shard-execution-external-artifact-dry-run-closeout-clean-ci-closeout",
    };

    for (int release = 1602; release <= 1606; ++release) {
        const auto fixture = read_shard_readiness_fixture(release);
        const int index = release - 1602;
        assert_contains(fixture, "\"shardPreviewProductionShardExecutionExternalArtifactDryRunCloseout\":{");
        assert_contains(fixture, "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseVersion\":\"v" +
                                     std::to_string(release) + "\"");
        assert_contains(fixture, "\"productionShardExecutionExternalArtifactDryRunCloseoutStage\":\"" +
                                     stages[static_cast<std::size_t>(index)] + "\"");
        assert_contains(fixture, "\"productionShardExecutionExternalArtifactDryRunCloseoutStageSequence\":" +
                                     std::to_string(index + 1));
        assert_contains(fixture, "\"sourceMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
        assert_contains(fixture, "\"dryRunBatchClosed\":true");
        assert_contains(fixture, "\"nodeOnlyGrowthStopped\":true");
        assert_contains(fixture, "\"realExternalArtifactPresent\":false");
        assert_contains(fixture, "\"externalArtifactParsed\":false");
        assert_contains(fixture, "\"externalArtifactStored\":false");
        assert_contains(fixture, "\"dryRunArtifactExecuted\":false");
        assert_contains(fixture, "\"realSignedApprovalPresent\":false");
        assert_contains(fixture, "\"realMiniKvSignedReceiptPresent\":false");
        assert_contains(fixture, "\"realCleanupReceiptPresent\":false");
        assert_contains(fixture, "\"archiveRoot\":\"f\"");
        assert_contains(fixture, "\"archiveRootIsF\":true");
        assert_contains(fixture, "\"combinedArchiveFolderAllowed\":false");
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
    assert_contains(current, "\"productionShardExecutionExternalArtifactDryRunCloseoutReleaseVersion\":\"v1606\"");
    assert_contains(current, "\"productionShardExecutionExternalArtifactDryRunCloseoutValidationPassed\":true");
    assert_contains(current, "\"versionedArchiveDirectories\":[\"f/1602/\",\"f/1603/\",\"f/1604/\",\"f/1605/\",\"f/1606/\"]");

    return 0;
}
