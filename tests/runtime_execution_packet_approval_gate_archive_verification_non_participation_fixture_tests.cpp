#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;

    for (int version = 1566; version <= 1585; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture,
                        "\"shardPreviewRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipation\"");
        assert_contains(
            fixture,
            "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseVersion\":\"v" +
                std::to_string(version) + "\"");
        assert_contains(fixture, "\"sourcePrecheckUpstreamReceiptVerificationSplitReleaseVersion\":\"v1545\"");
        assert_contains(fixture,
                        "\"sourcePrecheckUpstreamReceiptVerificationSplitFixturePath\":\"fixtures/release/"
                        "shard-readiness-v1545.json\"");
        assert_contains(fixture, "\"sourcePrecheckUpstreamReceiptVerificationSplitPublishedStageCount\":20");
        assert_contains(fixture, "\"sourcePrecheckUpstreamReceiptVerificationSplitComplete\":true");
        assert_contains(fixture, "\"sourcePrecheckUpstreamReceiptVerificationSplitFixtureFrozen\":true");
        assert_contains(fixture, "\"sourceNodeArchiveVerificationReleaseVersion\":\"Node v399\"");
        assert_contains(fixture, "\"sourceNodeVerifiedArchiveReleaseVersion\":\"Node v398\"");
        assert_contains(fixture, "\"sourceNodeNextConsumer\":\"Node v400\"");
        assert_contains(fixture, "\"archiveVerificationOnly\":true");
        assert_contains(fixture, "\"approvalInputCount\":0");
        assert_contains(fixture, "\"requiredApprovalInputCount\":3");
        assert_contains(fixture, "\"runtimeArtifactCount\":0");
        assert_contains(fixture, "\"requiredRuntimeArtifactCount\":6");
        assert_contains(fixture, "\"nodeV400MustNotTreatArchiveVerificationAsApproval\":true");
        assert_contains(fixture, "\"miniKvReadsNodeArchiveFiles\":false");
        assert_contains(fixture, "\"miniKvReplaysNodeArchive\":false");
        assert_contains(fixture, "\"miniKvStartsNodeServices\":false");
        assert_contains(fixture, "\"miniKvStartsJavaService\":false");
        assert_contains(fixture, "\"miniKvStartsMiniKvService\":false");
        assert_contains(fixture, "\"miniKvReadsEndpoints\":false");
        assert_contains(fixture, "\"miniKvReadsCredentials\":false");
        assert_contains(fixture, "\"miniKvParsesRawEndpoint\":false");
        assert_contains(fixture, "\"miniKvExecutesRuntimeProbe\":false");
        assert_contains(fixture, "\"miniKvEnablesActiveRouter\":false");
        assert_contains(fixture, "\"runtimeExecutionPacketPresent\":false");
        assert_contains(fixture, "\"runtimeGateApprovalPresent\":false");
        assert_contains(fixture, "\"activeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(minikv::test_support::release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1596\"");
    assert_contains(current,
                    "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v1545\"");
    assert_contains(current,
                    "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseVersion\":\"v1585\"");
    assert_contains(current, "\"codeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_contains(current, "\"productionShardExecutionOwnerReceiptRequestPacketReleaseVersion\":\"v1596\"");

    return 0;
}
