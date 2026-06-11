#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    using minikv::test_support::assert_contains;
    using minikv::test_support::read_fixture_text;
    using minikv::test_support::read_shard_readiness_fixture;
    using minikv::test_support::release_fixture_path;

    const char* stages[] = {
        "code-walkthrough-quality-gate-non-participation-plan-intake",
        "code-walkthrough-quality-gate-non-participation-standard-section-governance",
        "code-walkthrough-quality-gate-non-participation-no-placeholder-floor",
        "code-walkthrough-quality-gate-non-participation-archive-fixture-alignment",
        "code-walkthrough-quality-gate-non-participation-test-and-quality-scan",
        "code-walkthrough-quality-gate-non-participation-clean-ci-closeout",
    };

    for (int version = 1586; version <= 1591; ++version) {
        const auto fixture = read_shard_readiness_fixture(version);
        const int index = version - 1586;
        assert_contains(fixture, "\"releaseVersion\":\"v" + std::to_string(version) + "\"");
        assert_contains(fixture, "\"shardPreviewCodeWalkthroughQualityGateNonParticipation\"");
        assert_contains(fixture,
                        "\"codeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v" +
                            std::to_string(version) + "\"");
        assert_contains(fixture, "\"codeWalkthroughQualityGateNonParticipationStage\":\"" +
                                     std::string{stages[index]} + "\"");
        assert_contains(fixture, "\"sourceNodeCodeWalkthroughQualityGateReleaseVersion\":\"Node v2077\"");
        assert_contains(fixture, "\"sourceNodeQualityRunRange\":\"Node v2068-v2077\"");
        assert_contains(fixture,
                        "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationReleaseVersion\":\"v1585\"");
        assert_contains(fixture,
                        "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationFixturePath\":"
                        "\"fixtures/release/shard-readiness-v1585.json\"");
        assert_contains(fixture,
                        "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationPublishedStageCount\":20");
        assert_contains(fixture,
                        "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationComplete\":true");
        assert_contains(fixture, "\"walkthroughQualityGateOnly\":true");
        assert_contains(fixture, "\"codeWalkthroughMayBeOmittedForTinyMaintenance\":true");
        assert_contains(fixture, "\"placeholderWalkthroughAllowed\":false");
        assert_contains(fixture, "\"unsafeProductionClaimAllowed\":false");
        assert_contains(fixture, "\"bulkHistoricalRelocationAllowed\":false");
        assert_contains(fixture, "\"nodeWalkthroughFilesReadByMiniKv\":false");
        assert_contains(fixture, "\"miniKvScansNodeRepository\":false");
        assert_contains(fixture, "\"miniKvExecutesNodeQualityRoute\":false");
        assert_contains(fixture, "\"miniKvStartsNodeServices\":false");
        assert_contains(fixture, "\"miniKvStartsJavaService\":false");
        assert_contains(fixture, "\"miniKvStartsMiniKvService\":false");
        assert_contains(fixture, "\"miniKvMutatesNodeDocs\":false");
        assert_contains(fixture, "\"miniKvMutatesJavaDocs\":false");
        assert_contains(fixture, "\"activeRouterInstalled\":false");
        assert_contains(fixture, "\"writeRoutingAllowed\":false");
        assert_contains(fixture, "\"touchesWal\":false");
        assert_contains(fixture, "\"executionAllowed\":false");
    }

    const auto current = read_fixture_text(release_fixture_path("shard-readiness.json"));
    assert_contains(current, "\"releaseVersion\":\"v1591\"");
    assert_contains(
        current,
        "\"status\":\"code-walkthrough-quality-gate-non-participation-clean-ci-closeout-read-only\"");
    assert_contains(current, "\"codeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_contains(current, "\"codeWalkthroughQualityGateNonParticipationValidationPassed\":true");
    assert_contains(current, "\"commandCount\":90");

    return 0;
}
