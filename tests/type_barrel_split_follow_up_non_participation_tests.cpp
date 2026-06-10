#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_type_barrel_split_follow_up_non_participation;

    const auto json = evidence::format_type_barrel_split_follow_up_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-type-barrel-split-follow-up-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v1846-controlled-read-only-shard-preview-type-barrel-split-roadmap.md\"");
    assert_contains(json, "\"sourceNodeTypeBarrelSplitReleaseRange\":\"Node v1822-v1846\"");
    assert_contains(json, "\"sourceNodeTypeBarrelSplitCloseoutReleaseVersion\":\"Node v1846\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_contains(json, "\"sourceTypeBarrelSplitCommand\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceTypeBarrelSplitContract\":\"shard-route-preview-type-barrel-split-non-participation.v1\"");
    assert_contains(json, "\"sourceTypeBarrelSplitReleaseVersion\":\"v1385\"");
    assert_contains(json, "\"sourceTypeBarrelSplitFixturePath\":\"fixtures/release/shard-readiness-v1385.json\"");
    assert_contains(json, "\"sourceTypeBarrelSplitPublishedStageCount\":25");
    assert_contains(json, "\"sourceTypeBarrelSplitComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1385\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationReleaseVersion\":\"v1405\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1404\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1404.json\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationReleaseRangeStart\":\"v1386\"");
    assert_contains(json, "\"plannedTypeBarrelSplitFollowUpNonParticipationCheckCount\":20");
    assert_contains(json, "\"completedTypeBarrelSplitFollowUpNonParticipationCheckCount\":20");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationOnly\":true");
    assert_contains(json, "\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"source-fixture-freeze\"");
    assert_contains(json, "\"profile-endpoint-no-read\"");
    assert_contains(json, "\"router-write-wal-guard\"");
    assert_contains(json, "\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeAggregateReexportsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileBoundaryEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsTypeBarrelFollowUp\":false");
    assert_contains(json, "\"miniKvImportsNodeTypes\":false");
    assert_contains(json, "\"miniKvReadsProfileEndpoint\":false");
    assert_contains(json, "\"profileBoundaryEndpointRead\":false");
    assert_contains(json, "\"evidenceEndpointRead\":false");
    assert_contains(json, "\"typeExportSurfaceExecuted\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"typeBarrelSplitFollowUpNonParticipationValidationPassed\":true");

    const auto digest = evidence::type_barrel_split_follow_up_non_participation_digest_marker();
    assert(digest.find("v1405-route-preview-type-barrel-split-follow-up-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 20);
    assert(evidence::type_barrel_split_follow_up_non_participation_status() ==
           "route-preview-type-barrel-split-follow-up-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
