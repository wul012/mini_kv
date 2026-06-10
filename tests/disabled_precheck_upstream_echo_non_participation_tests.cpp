#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence = minikv::shard_preview_disabled_precheck_upstream_echo_non_participation;

    const auto json = evidence::format_disabled_precheck_upstream_echo_non_participation_json();
    assert_contains(json, "\"contract\":\"shard-route-disabled-precheck-upstream-echo-non-participation.v1\"");
    assert_contains(json, "\"command\":\"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v1878-disabled-precheck-upstream-echo-module-split-roadmap.md\"");
    assert_contains(json, "\"sourceNodeDisabledPrecheckUpstreamEchoReleaseRange\":\"Node v1867-v1878\"");
    assert_contains(json, "\"sourceNodeDisabledPrecheckUpstreamEchoCloseoutReleaseVersion\":\"Node v1878\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v115\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_contains(json, "\"sourceFixtureAuditCommand\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON\"");
    assert_contains(json, "\"sourceFixtureAuditReleaseVersion\":\"v1417\"");
    assert_contains(json, "\"sourceFixtureAuditFixturePath\":\"fixtures/release/shard-readiness-v1417.json\"");
    assert_contains(json, "\"sourceFixtureAuditPublishedStageCount\":12");
    assert_contains(json, "\"sourceFixtureAuditComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1417\"");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationReleaseVersion\":\"v1442\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1441\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1441.json\"");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationReleaseRangeStart\":\"v1418\"");
    assert_contains(json, "\"plannedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":25");
    assert_contains(json, "\"completedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":25");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationOnly\":true");
    assert_contains(json, "\"sourceFixtureAuditFrozen\":true");
    assert_contains(json, "\"stable-barrel-no-import\"");
    assert_contains(json, "\"credential-secret-no-read\"");
    assert_contains(json, "\"wal-load-restore-compact-disabled\"");
    assert_contains(json, "\"nodeDisabledPrecheckStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledPrecheckConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeDisabledPrecheckCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"miniKvReadsEndpoints\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"schemaMigrationExecuted\":false");
    assert_contains(json, "\"ledgerWriteExecuted\":false");
    assert_contains(json, "\"typeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"disabledPrecheckUpstreamEchoNonParticipationValidationPassed\":true");

    const auto digest = evidence::disabled_precheck_upstream_echo_non_participation_digest_marker();
    assert(digest.find(
               "v1442-route-preview-disabled-precheck-upstream-echo-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 25);
    assert(evidence::disabled_precheck_upstream_echo_non_participation_status() ==
           "route-preview-disabled-precheck-upstream-echo-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
