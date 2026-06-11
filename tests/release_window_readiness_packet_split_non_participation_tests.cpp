#include "minikv/shard_preview_release_window_readiness_packet_split_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence =
        minikv::shard_preview_release_window_readiness_packet_split_non_participation;

    const auto json = evidence::format_release_window_readiness_packet_split_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-release-window-readiness-packet-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTERELEASEWINDOWREADINESSPACKETSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v1935-v1937-release-window-readiness-packet-split-roadmap.md\"");
    assert_contains(json, "\"sourceNodeReleaseWindowReadinessPacketSplitReleaseRange\":\"Node v1935-v1937\"");
    assert_contains(json, "\"sourceNodeReleaseWindowReadinessPacketSplitCloseoutReleaseVersion\":\"Node v1937\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v70\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvEvidence\":\"restore-drill-evidence\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v61\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaEvidence\":\"rollback-approval-fixture\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeStartsMiniKvService\":false");
    assert_contains(json, "\"sourceNodeStartsJavaService\":false");
    assert_contains(json, "\"sourceNodeReleaseWindowReadinessPacketSplitOnly\":true");
    assert_contains(json, "\"sourceImplementationPlanCommand\":\"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceImplementationPlanContract\":\"shard-route-implementation-plan-upstream-echo-closeout-non-participation.v1\"");
    assert_contains(json, "\"sourceImplementationPlanReleaseVersion\":\"v1475\"");
    assert_contains(json, "\"sourceImplementationPlanFixturePath\":\"fixtures/release/shard-readiness-v1475.json\"");
    assert_contains(json, "\"sourceImplementationPlanPublishedStageCount\":3");
    assert_contains(json, "\"sourceImplementationPlanComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1475\"");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationReleaseVersion\":\"v1490\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1489\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1489.json\"");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationReleaseRangeStart\":\"v1476\"");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationReleaseRangeEnd\":\"v1490\"");
    assert_contains(json, "\"publishedStageCount\":15");
    assert_contains(json, "\"plannedStageCount\":15");
    assert_contains(json, "\"plannedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":15");
    assert_contains(json, "\"completedReleaseWindowReadinessPacketSplitNonParticipationCheckCount\":15");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationDeclared\":true");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationOnly\":true");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationChainComplete\":true");
    assert_contains(json, "\"sourceImplementationPlanUpstreamEchoCloseoutFixtureFrozen\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketStableEntrypointImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketEvidenceImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketPolicyImportedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeReleaseWindowReadinessPacketRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeProductionReleaseDryRunEnvelopeExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeStatusRoutesExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"miniKvExecutesReleaseWindowPacket\":false");
    assert_contains(json, "\"miniKvReplaysRestoreDrillEvidence\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"schemaMigrationExecuted\":false");
    assert_contains(json, "\"ledgerWriteExecuted\":false");
    assert_contains(json, "\"externalRequestSent\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"releaseWindowReadinessPacketSplitNonParticipationValidationPassed\":true");

    const auto digest = evidence::release_window_readiness_packet_split_non_participation_digest_marker();
    assert(digest.find(
               "v1490-route-preview-release-window-readiness-packet-split-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 15);
    assert(evidence::release_window_readiness_packet_split_non_participation_status() ==
           "route-preview-release-window-readiness-packet-split-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
