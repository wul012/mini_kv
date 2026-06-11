#include "minikv/shard_preview_precheck_upstream_receipt_verification_split_non_participation.hpp"

#include "test_support.hpp"

#include <cassert>
#include <string>

int main() {
    namespace evidence =
        minikv::shard_preview_precheck_upstream_receipt_verification_split_non_participation;
    using minikv::test_support::assert_contains;

    const auto json =
        evidence::format_precheck_upstream_receipt_verification_split_non_participation_json();
    assert_contains(
        json,
        "\"contract\":\"shard-route-precheck-upstream-receipt-verification-split-non-participation.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTEPRECHECKUPSTREAMRECEIPTVERIFICATIONSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v1983-v2002-precheck-upstream-receipt-verification-split-roadmap.md\"");
    assert_contains(json,
                    "\"sourceNodePrecheckUpstreamReceiptVerificationSplitReleaseRange\":\"Node v1983-v2002\"");
    assert_contains(json,
                    "\"sourceNodePrecheckUpstreamReceiptVerificationSplitCloseoutReleaseVersion\":\"Node v2002\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v108\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"non-participation-evidence\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v99\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaEvidence\":\"receipt-evidence\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_contains(json,
                    "\"sourceBlockedExecutionRehearsalSplitNonParticipationFixturePath\":\"fixtures/release/"
                    "shard-readiness-v1525.json\"");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationPublishedStageCount\":20");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1525\"");
    assert_contains(json,
                    "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseVersion\":\"v1545\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1544\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1544.json\"");
    assert_contains(json,
                    "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseRangeStart\":\"v1526\"");
    assert_contains(json,
                    "\"precheckUpstreamReceiptVerificationSplitNonParticipationReleaseRangeEnd\":\"v1545\"");
    assert_contains(json, "\"publishedStageCount\":20");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(json, "\"plannedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"completedPrecheckUpstreamReceiptVerificationSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"precheckUpstreamReceiptVerificationSplitNonParticipationChainComplete\":true");
    assert_contains(json, "\"precheckUpstreamReceiptVerificationSplitNonParticipationValidationPassed\":true");
    assert_contains(json, "\"sourceBlockedExecutionRehearsalSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"freshJavaEvidenceRequiredByNode\":false");

    const std::string false_fields[] = {
        "nodePrecheckUpstreamReceiptVerificationPublicLoaderImportedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationTypesImportedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationConstantsImportedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationReferencesParsedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationCheckPolicyExecutedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationMessagePolicyExecutedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationCoreExecutedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationRendererExecutedByMiniKv",
        "nodePrecheckUpstreamReceiptVerificationEntrypointExecutedByMiniKv",
        "nodeCodeHealthScanExecutedByMiniKv",
        "nodeLargeFileInventoryMutatedByMiniKv",
        "nodeDirectTestsExecutedByMiniKv",
        "nodeDownstreamCodeHealthExecutedByMiniKv",
        "nodeDownstreamRehearsalGuardExecutedByMiniKv",
        "nodeTypecheckExecutedByMiniKv",
        "nodeVitestExecutedByMiniKv",
        "nodeBuildExecutedByMiniKv",
        "miniKvImportsNodeModules",
        "miniKvExecutesNodeChecks",
        "miniKvStartsServices",
        "miniKvReadsEndpoints",
        "miniKvReadsCredentials",
        "miniKvParsesHistoricalReceipts",
        "miniKvReadsHistoricalMiniKvV108NonParticipationFile",
        "miniKvScansJavaV99Evidence",
        "miniKvScansJavaEvidence",
        "miniKvExecutesDownstreamChecks",
        "miniKvMutatesLargeFileInventory",
        "endpointHandleRead",
        "rawEndpointParsed",
        "credentialValueRead",
        "managedAuditConnectionOpened",
        "activeRouterInstalled",
        "writeRoutingAllowed",
        "loadRestoreCompactAllowed",
        "touchesWal",
        "executionAllowed",
    };
    for (const auto& field : false_fields) {
        assert_contains(json, "\"" + field + "\":false");
    }

    const auto digest =
        evidence::precheck_upstream_receipt_verification_split_non_participation_digest_marker();
    assert(digest.find(
               "v1545-route-preview-precheck-upstream-receipt-verification-split-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 20);
    assert(evidence::precheck_upstream_receipt_verification_split_non_participation_status() ==
           "route-preview-precheck-upstream-receipt-verification-split-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
