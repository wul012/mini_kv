#include "minikv/shard_preview_blocked_execution_rehearsal_split_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence =
        minikv::shard_preview_blocked_execution_rehearsal_split_non_participation;

    const auto json =
        evidence::format_blocked_execution_rehearsal_split_non_participation_json();
    assert_contains(
        json,
        "\"contract\":\"shard-route-blocked-execution-rehearsal-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEBLOCKEDEXECUTIONREHEARSALSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v1968-v1982-blocked-execution-rehearsal-split-roadmap.md\"");
    assert_contains(json, "\"sourceNodeBlockedExecutionRehearsalSplitReleaseRange\":\"Node v1968-v1982\"");
    assert_contains(json, "\"sourceNodeBlockedExecutionRehearsalSplitCloseoutReleaseVersion\":\"Node v1982\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v99\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"wal-regression-evidence\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v90\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaEvidence\":\"context-normalization-evidence\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(
        json,
        "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitNonParticipationReleaseVersion\":\"v1505\"");
    assert_contains(json,
                    "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitPublishedStageCount\":15");
    assert_contains(json,
                    "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1505\"");
    assert_contains(json,
                    "\"blockedExecutionRehearsalSplitNonParticipationReleaseVersion\":\"v1525\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1524\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1524.json\"");
    assert_contains(json,
                    "\"blockedExecutionRehearsalSplitNonParticipationReleaseRangeStart\":\"v1506\"");
    assert_contains(json,
                    "\"blockedExecutionRehearsalSplitNonParticipationReleaseRangeEnd\":\"v1525\"");
    assert_contains(json, "\"publishedStageCount\":20");
    assert_contains(json, "\"plannedStageCount\":20");
    assert_contains(json, "\"plannedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"completedBlockedExecutionRehearsalSplitNonParticipationCheckCount\":20");
    assert_contains(json, "\"blockedExecutionRehearsalSplitNonParticipationChainComplete\":true");
    assert_contains(json, "\"sourceDisabledFakeHarnessContractUpstreamEchoVerificationSplitFixtureFrozen\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"freshJavaEvidenceRequiredByNode\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalPublicEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalReferencesParsedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalCoreExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalPolicyExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBlockedExecutionRehearsalLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodePreconditionIntakeExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"miniKvParsesHistoricalReceipts\":false");
    assert_contains(json, "\"miniKvReadsHistoricalMiniKvV99WalRegressionFile\":false");
    assert_contains(json, "\"miniKvScansJavaV90ContextNormalizationEvidence\":false");
    assert_contains(json, "\"miniKvScansJavaEvidence\":false");
    assert_contains(json, "\"miniKvReplaysWalRegression\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"loadRestoreCompactAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"blockedExecutionRehearsalSplitNonParticipationValidationPassed\":true");

    const auto digest =
        evidence::blocked_execution_rehearsal_split_non_participation_digest_marker();
    assert(digest.find(
               "v1525-route-preview-blocked-execution-rehearsal-split-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 20);
    assert(evidence::blocked_execution_rehearsal_split_non_participation_status() ==
           "route-preview-blocked-execution-rehearsal-split-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
