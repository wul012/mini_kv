#include "minikv/shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace evidence =
        minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation;
    using minikv::test_support::assert_contains;

    const auto json =
        evidence::format_runtime_execution_packet_approval_gate_archive_verification_non_participation_json();
    assert_contains(
        json,
        "\"contract\":\"shard-route-runtime-execution-packet-approval-gate-archive-verification-non-participation.v1\"");
    assert_contains(
        json,
        "\"command\":\"SHARDROUTERUNTIMEEXECUTIONPACKETAPPROVALGATEARCHIVEVERIFYNONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceNodePlan\":\"docs/plans3/"
                    "v399-post-java-mini-kv-runtime-execution-packet-approval-gate-review-archive-verification-roadmap.md\"");
    assert_contains(json, "\"sourceNodeArchiveVerificationReleaseVersion\":\"Node v399\"");
    assert_contains(json, "\"sourceNodeVerifiedArchiveReleaseVersion\":\"Node v398\"");
    assert_contains(json, "\"sourceNodeNextConsumer\":\"Node v400\"");
    assert_contains(json, "\"sourcePrecheckUpstreamReceiptVerificationSplitReleaseVersion\":\"v1545\"");
    assert_contains(json,
                    "\"sourcePrecheckUpstreamReceiptVerificationSplitFixturePath\":\"fixtures/release/"
                    "shard-readiness-v1545.json\"");
    assert_contains(json, "\"sourcePrecheckUpstreamReceiptVerificationSplitPublishedStageCount\":20");
    assert_contains(json, "\"sourcePrecheckUpstreamReceiptVerificationSplitComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1565\"");
    assert_contains(json,
                    "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseVersion\":\"v1585\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1584\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1584.json\"");
    assert_contains(
        json,
        "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseRangeStart\":\"v1566\"");
    assert_contains(json,
                    "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationReleaseRangeEnd\":\"v1585\"");
    assert_contains(json, "\"publishedStageCount\":20");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(
        json,
        "\"plannedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":20");
    assert_contains(
        json,
        "\"completedRuntimeExecutionPacketApprovalGateArchiveVerificationNonParticipationCheckCount\":20");
    assert_contains(
        json,
        "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationChainComplete\":true");
    assert_contains(
        json,
        "\"runtimeExecutionPacketApprovalGateArchiveVerificationNonParticipationValidationPassed\":true");
    assert_contains(json, "\"archiveVerificationOnly\":true");
    assert_contains(json, "\"approvalInputCount\":0");
    assert_contains(json, "\"requiredApprovalInputCount\":3");
    assert_contains(json, "\"runtimeArtifactCount\":0");
    assert_contains(json, "\"requiredRuntimeArtifactCount\":6");
    assert_contains(json, "\"presentRuntimeExecutionArtifactCount\":0");
    assert_contains(json, "\"missingRuntimeExecutionArtifactCount\":6");
    assert_contains(json, "\"approvalGateBlocked\":true");
    assert_contains(json, "\"nodeV400MustNotTreatArchiveVerificationAsApproval\":true");

    const std::string false_fields[] = {
        "sourceNodeRequiresFreshMiniKvEvidence",
        "sourceNodeRequiresFreshJavaEvidence",
        "sourceNodeStartsMiniKvService",
        "sourceNodeStartsJavaService",
        "sourceNodeRuntimeBehaviorAdded",
        "sourceNodeServiceStartupRequired",
        "sourceNodePortsOpened",
        "sourceNodeCrossProjectWritesAllowed",
        "nodeV398ArchiveFilesReadByMiniKv",
        "nodeV398ArchiveMarkdownParsedByMiniKv",
        "nodeV398ArchiveBrowserEvidenceRenderedByMiniKv",
        "nodeV398ArchiveReplayExecutedByMiniKv",
        "nodeV398ArchiveTreatedAsRuntimeApproval",
        "freshMiniKvEvidenceRequiredByNode",
        "freshJavaEvidenceRequiredByNode",
        "miniKvReadsNodeArchiveFiles",
        "miniKvReplaysNodeArchive",
        "miniKvStartsNodeServices",
        "miniKvStartsJavaService",
        "miniKvStartsMiniKvService",
        "miniKvReadsEndpoints",
        "miniKvReadsCredentials",
        "miniKvParsesRawEndpoint",
        "miniKvExecutesRuntimeProbe",
        "miniKvEnablesActiveRouter",
        "runtimeExecutionPacketPresent",
        "runtimeExecutionPacketExecutable",
        "runtimeGateApprovalPresent",
        "readyForRuntimeExecutionPacket",
        "readyForRuntimeLiveReadGate",
        "activeRouterInstalled",
        "writeRoutingAllowed",
        "mutatesStore",
        "touchesWal",
        "runtimeProbeExecuted",
        "executionAttempted",
        "executionAllowed",
    };
    for (const auto& field : false_fields) {
        assert_contains(json, "\"" + field + "\":false");
    }

    const auto digest =
        evidence::runtime_execution_packet_approval_gate_archive_verification_non_participation_digest_marker();
    assert(digest.find(
               "v1585-runtime-execution-packet-approval-gate-archive-verification-non-participation-clean-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 20);
    assert(evidence::runtime_execution_packet_approval_gate_archive_verification_non_participation_status() ==
           "runtime-execution-packet-approval-gate-archive-verification-non-participation-clean-ci-closeout-read-only");

    return 0;
}
