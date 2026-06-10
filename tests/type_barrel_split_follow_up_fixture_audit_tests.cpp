#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_type_barrel_split_follow_up_fixture_audit;

    const auto json = evidence::format_type_barrel_split_follow_up_fixture_audit_json();
    assert_contains(json, "\"contract\":\"shard-route-preview-type-barrel-split-follow-up-fixture-audit.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/v1866-test-only-shell-upstream-echo-module-split-roadmap.md\"");
    assert_contains(json, "\"sourceNodeTestOnlyShellSplitReleaseRange\":\"Node v1847-v1866\"");
    assert_contains(json, "\"sourceNodeTestOnlyShellSplitCloseoutReleaseVersion\":\"Node v1866\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v116\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_contains(json, "\"sourceFollowUpCommand\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceFollowUpContract\":\"shard-route-preview-type-barrel-split-follow-up-non-participation.v1\"");
    assert_contains(json, "\"sourceFollowUpReleaseVersion\":\"v1405\"");
    assert_contains(json, "\"sourceFollowUpFixturePath\":\"fixtures/release/shard-readiness-v1405.json\"");
    assert_contains(json, "\"sourceFollowUpPublishedStageCount\":20");
    assert_contains(json, "\"sourceFollowUpComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1405\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditReleaseVersion\":\"v1417\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1416\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1416.json\"");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditReleaseRangeStart\":\"v1406\"");
    assert_contains(json, "\"plannedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":12");
    assert_contains(json, "\"completedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":12");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditOnly\":true");
    assert_contains(json, "\"sourceFollowUpNonParticipationFixtureFrozen\":true");
    assert_contains(json, "\"source-follow-up-fixture-freeze\"");
    assert_contains(json, "\"current-fixture-parity-audit\"");
    assert_contains(json, "\"node-test-only-shell-no-import\"");
    assert_contains(json, "\"no-router-write-wal-audit\"");
    assert_contains(json, "\"nodeStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeProfileTypesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeTestOnlyShellModulesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeCredentialResolverShellExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvRuntimeReadsSourceFixturePayload\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"profileBoundaryEndpointRead\":false");
    assert_contains(json, "\"evidenceEndpointRead\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"typeBarrelSplitFollowUpFixtureAuditValidationPassed\":true");

    const auto digest = evidence::type_barrel_split_follow_up_fixture_audit_digest_marker();
    assert(digest.find("v1417-route-preview-type-barrel-split-follow-up-fixture-audit-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 12);
    assert(evidence::type_barrel_split_follow_up_fixture_audit_status() ==
           "route-preview-type-barrel-split-follow-up-fixture-audit-clean-workspace-ci-closeout-read-only");

    return 0;
}
