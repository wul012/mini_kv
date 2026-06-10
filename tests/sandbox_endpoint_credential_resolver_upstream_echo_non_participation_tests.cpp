#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.hpp"

#include <cassert>
#include <string>

namespace {

void assert_contains(const std::string& text, const std::string& needle) {
    assert(text.find(needle) != std::string::npos);
}

} // namespace

int main() {
    namespace evidence =
        minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation;

    const auto json = evidence::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_json();
    assert_contains(json,
                    "\"contract\":\"shard-route-sandbox-endpoint-credential-resolver-upstream-echo-non-participation.v1\"");
    assert_contains(json,
                    "\"command\":\"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_contains(json, "\"sourceNodePlan\":\"docs/plans3/"
                          "v1903-sandbox-endpoint-credential-resolver-upstream-echo-module-split-roadmap.md\"");
    assert_contains(json,
                    "\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoReleaseRange\":\"Node v1879-v1903\"");
    assert_contains(json,
                    "\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoCloseoutReleaseVersion\":\"Node v1903\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v114\"");
    assert_contains(json, "\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v105\"");
    assert_contains(json, "\"sourceNodeRequiresFreshMiniKvEvidence\":false");
    assert_contains(json, "\"sourceNodeRequiresFreshJavaEvidence\":false");
    assert_contains(json, "\"sourceNodeStartsMiniKvService\":false");
    assert_contains(json, "\"sourceNodeStartsJavaService\":false");
    assert_contains(json, "\"sourceNodeRuntimeBehaviorAdded\":false");
    assert_contains(json, "\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoSplitOnly\":true");
    assert_contains(json, "\"sourceNodeRequiresSiblingStartup\":false");
    assert_contains(json, "\"sourceDisabledPrecheckCommand\":\"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON\"");
    assert_contains(json,
                    "\"sourceDisabledPrecheckContract\":\"shard-route-disabled-precheck-upstream-echo-non-participation.v1\"");
    assert_contains(json, "\"sourceDisabledPrecheckReleaseVersion\":\"v1442\"");
    assert_contains(json, "\"sourceDisabledPrecheckFixturePath\":\"fixtures/release/shard-readiness-v1442.json\"");
    assert_contains(json, "\"sourceDisabledPrecheckPublishedStageCount\":25");
    assert_contains(json, "\"sourceDisabledPrecheckComplete\":true");
    assert_contains(json, "\"sourceCurrentShardReadinessReleaseVersion\":\"v1442\"");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseVersion\":\"v1472\"");
    assert_contains(json, "\"sourceFrozenReleaseVersion\":\"v1471\"");
    assert_contains(json, "\"sourceFrozenFixturePath\":\"fixtures/release/shard-readiness-v1471.json\"");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseRangeStart\":\"v1443\"");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseRangeEnd\":\"v1472\"");
    assert_contains(json, "\"publishedStageCount\":30");
    assert_contains(json, "\"plannedStageCount\":30");
    assert_contains(json, "\"plannedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":30");
    assert_contains(json, "\"completedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":30");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationDeclared\":true");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationOnly\":true");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationChainComplete\":true");
    assert_contains(json, "\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true");
    assert_contains(json, "\"sourceDisabledPrecheckUpstreamEchoDigestPinned\":true");
    assert_contains(json, "\"versionedFixtureChainOnly\":true");
    assert_contains(json, "\"stable-barrel-no-import\"");
    assert_contains(json, "\"endpoint-handle-no-read\"");
    assert_contains(json, "\"raw-endpoint-no-parse\"");
    assert_contains(json, "\"credential-secret-no-read\"");
    assert_contains(json, "\"managed-connection-closed\"");
    assert_contains(json, "\"wal-load-restore-compact-disabled\"");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverConstantsImportedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverReferencesImportedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverChecksExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverMessagesExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeSandboxEndpointCredentialResolverHistoricalFallbackExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeRendererExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeTypecheckExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeVitestExecutedByMiniKv\":false");
    assert_contains(json, "\"nodeBuildExecutedByMiniKv\":false");
    assert_contains(json, "\"miniKvImportsNodeModules\":false");
    assert_contains(json, "\"miniKvExecutesNodeChecks\":false");
    assert_contains(json, "\"miniKvStartsServices\":false");
    assert_contains(json, "\"miniKvReadsEndpointHandles\":false");
    assert_contains(json, "\"miniKvParsesRawEndpoint\":false");
    assert_contains(json, "\"miniKvReadsCredentials\":false");
    assert_contains(json, "\"endpointHandleRead\":false");
    assert_contains(json, "\"rawEndpointParsed\":false");
    assert_contains(json, "\"credentialValueRead\":false");
    assert_contains(json, "\"secretValueRead\":false");
    assert_contains(json, "\"managedAuditConnectionOpened\":false");
    assert_contains(json, "\"schemaMigrationExecuted\":false");
    assert_contains(json, "\"ledgerWriteExecuted\":false");
    assert_contains(json, "\"externalRequestSent\":false");
    assert_contains(json, "\"activeRouterInstalled\":false");
    assert_contains(json, "\"writeRoutingAllowed\":false");
    assert_contains(json, "\"touchesWal\":false");
    assert_contains(json, "\"executionAllowed\":false");
    assert_contains(json, "\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":true");

    const auto digest = evidence::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_digest_marker();
    assert(digest.find(
               "v1472-route-preview-sandbox-endpoint-credential-resolver-upstream-echo-non-participation-clean-workspace-ci-closeout") !=
           std::string::npos);
    assert(evidence::published_stage_count() == 30);
    assert(evidence::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_status() ==
           "route-preview-sandbox-endpoint-credential-resolver-upstream-echo-non-participation-clean-workspace-ci-closeout-read-only");

    return 0;
}
