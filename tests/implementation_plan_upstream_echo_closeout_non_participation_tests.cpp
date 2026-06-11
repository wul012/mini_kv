#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence =
        minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation;

    const auto json = evidence::format_implementation_plan_upstream_echo_closeout_non_participation_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-implementation-plan-upstream-echo-closeout-non-participation.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v1934-implementation-plan-upstream-echo-module-split-closeout-roadmap.md\"");
    assert_contains(json, "\"sourceNodeImplementationPlanUpstreamEchoReleaseRange\":\"Node v1904-v1934\"");
    assert_contains(json, "\"sourceNodeImplementationPlanUpstreamEchoCloseoutReleaseVersion\":\"Node v1934\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v126\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v121\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeStartsMiniKvService\":false");
    assert_contains(json, "\"sourceNodeStartsJavaService\":false");
    assert_contains(json, "\"sourceNodeRuntimeBehaviorAdded\":false");
    assert_contains(json, "\"sourceNodeImplementationPlanUpstreamEchoCloseoutDocumentationOnly\":true");
    assert_contains(json, "\"sourceNodeNoServiceStartupConstraint\":true");
    assert_contains(json, "\"sourceSandboxEndpointCommand\":\"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceSandboxEndpointContract\":\"shard-route-sandbox-endpoint-credential-resolver-upstream-echo-non-participation.v1\"");
    assert_contains(json, "\"sourceSandboxEndpointReleaseVersion\":\"v1472\"");
    assert_contains(json, "\"sourceSandboxEndpointFixturePath\":\"fixtures/release/shard-readiness-v1472.json\"");
    assert_contains(json, "\"sourceSandboxEndpointPublishedStageCount\":30");
    assert_contains(json, "\"sourceSandboxEndpointComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1472\"");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseVersion\":\"v1475\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1474\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1474.json\"");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseRangeStart\":\"v1473\"");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseRangeEnd\":\"v1475\"");
    assert_contains(json, "\"publishedStageCount\":3");
    assert_contains(json, "\"plannedStageCount\":3");
    assert_contains(json, "\"plannedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":8");
    assert_contains(json, "\"completedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":8");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationDeclared\":true");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationOnly\":true");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationChainComplete\":true");
    assert_contains(json, "\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true");
    assert_contains(json, "\"nodeCloseoutObservedAsReadOnlyPlan\":true");
    assert_contains(json, "\"freshMiniKvEvidenceRequiredByNode\":false");
    assert_contains(json, "\"freshJavaEvidenceRequiredByNode\":false");
    assert_contains(json, "\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanReferencesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanChecksExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanMessagesExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false");
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
    assert_contains(json, "\"externalRequestSent\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":true");

    const auto digest = evidence::implementation_plan_upstream_echo_closeout_non_participation_digest_marker();
    assert(digest.find(
               "v1475-route-preview-implementation-plan-upstream-echo-closeout-non-participation-clean-fixture-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 3);
    assert(evidence::implementation_plan_upstream_echo_closeout_non_participation_status() ==
           "route-preview-implementation-plan-upstream-echo-closeout-non-participation-clean-fixture-closeout-read-only");

    return 0;
}
