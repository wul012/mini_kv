#include "minikv/shard_preview_type_barrel_split_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_type_barrel_split_non_participation;

    const auto json = evidence::format_type_barrel_split_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-type-barrel-split-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v1846-controlled-read-only-shard-preview-type-barrel-split-roadmap.md\"");
    assert_contains(json, "\"sourceNodeTypeBarrelSplitReleaseRange\":\"Node v1822-v1846\"");
    assert_contains(json, "\"sourceNodeTypeBarrelSplitCloseoutReleaseVersion\":\"Node v1846\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeTypeBarrelSplitOnly\":true");
    assert_contains(json, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_contains(json, "\"sourceNodeLegacyAggregatorLineCount\":821");
    assert_contains(json, "\"sourceIntegrityCommand\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceIntegrityReleaseVersion\":\"v1360\"");
    assert_contains(json, "\"sourceIntegrityPublishedStageCount\":30");
    assert_contains(json, "\"typeBarrelSplitNonParticipationReleaseVersion\":\"v1385\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1384\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1384.json\"");
    assert_contains(json, "\"typeBarrelSplitNonParticipationReleaseRangeStart\":\"v1361\"");
    assert_contains(json, "\"plannedTypeBarrelSplitNonParticipationCheckCount\":25");
    assert_contains(json, "\"completedTypeBarrelSplitNonParticipationCheckCount\":25");
    assert_contains(json, "\"typeBarrelSplitNonParticipationOnly\":true");
    assert_contains(json, "\"stable-type-barrel-split-boundary\"");
    assert_contains(json, "\"profile-only-type-module\"");
    assert_contains(json, "\"evidence-endpoint-profile-boundary\"");
    assert_contains(json, "\"clean-workspace-ci-closeout\"");
    assert_contains(json, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeAggregateReexportsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileBoundaryEndpointReadByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvStartsTypeBarrelSplit\":false");
    assert_contains(json, "\"miniKvImportsNodeTypes\":false");
    assert_contains(json, "\"miniKvConsumesProfileBoundaries\":false");
    assert_contains(json, "\"profileBoundaryEndpointRead\":false");
    assert_contains(json, "\"evidenceEndpointRead\":false");
    assert_contains(json, "\"typeExportSurfaceExecuted\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"typeBarrelSplitNonParticipationValidationPassed\":true");

    const auto digest = evidence::type_barrel_split_non_participation_digest_marker();
    assert(digest.find("v1385-route-preview-type-barrel-split-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 25);
    assert(evidence::type_barrel_split_non_participation_status() ==
           "route-preview-type-barrel-split-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
