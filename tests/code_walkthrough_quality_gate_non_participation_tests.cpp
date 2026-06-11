#include "minikv/shard_preview_code_walkthrough_quality_gate_non_participation.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace evidence = minikv::shard_preview_code_walkthrough_quality_gate_non_participation;
    using minikv::test_support::assert_contains;

    const auto json = evidence::format_code_walkthrough_quality_gate_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-code-walkthrough-quality-gate-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDCODEWALKTHROUGHQUALITYGATENONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
                    "v2077-post-code-walkthrough-quality-gate-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceNodeCodeWalkthroughQualityGateReleaseVersion\":\"Node v2077\"");
    assert_contains(json, "\"sourceNodeQualityRunRange\":\"Node v2068-v2077\"");
    assert_contains(json,
                    "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationCommand\":"
                    "\"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationReleaseVersion\":\"v1585\"");
    assert_contains(json,
                    "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationFixturePath\":"
                    "\"fixtures/release/shard-readiness-v1585.json\"");
    assert_contains(json, "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationPublishedStageCount\":20");
    assert_contains(json, "\"sourceRuntimeExecutionPacketApprovalGateArchiveVerificationComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1585\"");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationReleaseVersion\":\"v1591\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1590\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1590.json\"");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationReleaseRangeStart\":\"v1586\"");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationReleaseRangeEnd\":\"v1591\"");
    assert_contains(json, "\"publishedStageCount\":6");
    assert_contains(json, "\"plannedStageCount\":6");
    assert_contains(json, "\"plannedCodeWalkthroughQualityGateNonParticipationCheckCount\":12");
    assert_contains(json, "\"completedCodeWalkthroughQualityGateNonParticipationCheckCount\":12");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationChainComplete\":true");
    assert_contains(json, "\"codeWalkthroughQualityGateNonParticipationValidationPassed\":true");
    assert_contains(json, "\"walkthroughQualityGateOnly\":true");
    assert_contains(json, "\"codeWalkthroughWrittenWhenSubstantive\":true");
    assert_contains(json, "\"codeWalkthroughMayBeOmittedForTinyMaintenance\":true");
    assert_contains(json, "\"newWalkthroughVolumeActive\":true");
    assert_contains(json, "\"archiveScreenshotsMustMatchCommands\":true");
    assert_contains(json, "\"fixturesMustParseAsJson\":true");
    assert_contains(json, "\"trailingWhitespaceAllowed\":false");
    assert_contains(json, "\"testSectionsExplainProtectedBehavior\":true");
    assert_contains(json, "\"goal-role-non-goal\"");
    assert_contains(json, "\"boundary-field-interpretation\"");
    assert_contains(json, "\"test-behavior-protection\"");

    const std::string false_fields[] = {
        "sourceNodeRequiresFreshMiniKvEvidence",
        "sourceNodeRequiresFreshJavaEvidence",
        "sourceNodeRouteConsumedByMiniKv",
        "sourceNodeTestsExecutedByMiniKv",
        "sourceNodeWalkthroughFilesReadByMiniKv",
        "sourceNodeRepositoryScannedByMiniKv",
        "sourceNodeStartsMiniKvService",
        "sourceNodeStartsJavaService",
        "nodeWalkthroughFilesReadByMiniKv",
        "miniKvScansNodeRepository",
        "miniKvExecutesNodeQualityRoute",
        "miniKvStartsNodeServices",
        "miniKvStartsJavaService",
        "miniKvStartsMiniKvService",
        "miniKvMutatesNodeDocs",
        "miniKvMutatesJavaDocs",
        "placeholderWalkthroughAllowed",
        "unsafeProductionClaimAllowed",
        "bulkHistoricalRelocationAllowed",
        "historicalWalkthroughFilesMoved",
        "activeRouterInstalled",
        "documentRouterInstalled",
        "archiveRouterInstalled",
        "typeRouterInstalled",
        "routerActivationAllowed",
        "writeRoutingAllowed",
        "writeCommandsAllowed",
        "mutatesStore",
        "adminCommandsAllowed",
        "loadRestoreCompactAllowed",
        "touchesWal",
        "startsServices",
        "startsMiniKvService",
        "startsSiblingServices",
        "liveReadAllowed",
        "runtimeProbeExecuted",
        "executionAttempted",
        "executionAllowed",
    };
    for (const auto& field : false_fields) {
        assert_contains(json, "\"" + field + "\":false");
    }

    const auto digest = evidence::code_walkthrough_quality_gate_non_participation_digest_marker();
    assert(digest.find("v1591-code-walkthrough-quality-gate-non-participation-clean-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 6);
    assert(evidence::code_walkthrough_quality_gate_non_participation_status() ==
           "code-walkthrough-quality-gate-non-participation-clean-ci-closeout-read-only");

    return 0;
}
