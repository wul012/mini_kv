#include "test_support.hpp"

#include <cassert>
#include <string>
#include <vector>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::assert_current_shard_readiness_status;
    using minikv::test_support::read_shard_readiness_fixture;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::release_fixture_path;

    const std::vector<std::string> stages{
        "production-shard-execution-owner-receipt-request-packet-plan-intake",
        "production-shard-execution-owner-receipt-request-packet-mini-kv-slot-catalog",
        "production-shard-execution-owner-receipt-request-packet-java-cleanup-slot-catalog",
        "production-shard-execution-owner-receipt-request-packet-unsigned-boundary-validation",
        "production-shard-execution-owner-receipt-request-packet-clean-ci-closeout",
    };

    for (int release = 1592; release <= 1596; ++release) {
        const auto fixture = read_shard_readiness_fixture(release);
        const int index = release - 1592;
        assert_contains(fixture, "\"shardPreviewProductionShardExecutionOwnerReceiptRequestPacket\":{");
        assert_contains(fixture, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseVersion\":\"v" +
                                     std::to_string(release) + "\"");
        assert_contains(fixture, "\"productionShardExecutionOwnerReceiptRequestPacketStage\":\"" +
                                     stages[static_cast<std::size_t>(index)] + "\"");
        assert_contains(fixture, "\"productionShardExecutionOwnerReceiptRequestPacketStageSequence\":" +
                                     std::to_string(index + 1));
        assert_contains(fixture, "\"sourceCodeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
        assert_contains(fixture, "\"ownerReceiptRequestPacketOnly\":true");
        assert_contains(fixture, "\"requestedReceiptSlotCount\":9");
        assert_contains(fixture, "\"signedMiniKvReceiptPresent\":false");
        assert_contains(fixture, "\"signedJavaReceiptPresent\":false");
        assert_contains(fixture, "\"signedCrossProjectCleanupReceiptPresent\":false");
        assert_contains(fixture, "\"perVersionArchiveRequired\":true");
        assert_contains(fixture, "\"combinedArchiveFolderAllowed\":false");
        assert_contains(fixture, "\"activeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"mutatesStore\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1601\"");
    assert_current_shard_readiness_status(current);
    assert_contains(current, "\"commandCount\":92");
    assert_contains(current, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");
    assert_contains(current, "\"productionShardExecutionMiniKvOwnerReceiptBundleReleaseVersion\":\"v1601\"");
    assert_contains(current, "\"productionShardExecutionOwnerReceiptRequestPacketValidationPassed\":true");
    assert_contains(current, "\"versionedArchiveDirectories\":[\"e/1592/\",\"e/1593/\",\"e/1594/\",\"e/1595/\",\"e/1596/\"]");
    assert_contains(current, "\"combinedArchiveFolderAllowed\":false");

    return 0;
}
