#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
    int source_disabled_precheck_stage_count,
    bool source_disabled_precheck_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    bool mini_kv_reads_endpoint_handles,
    bool mini_kv_parses_raw_endpoint,
    bool mini_kv_reads_credentials,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_disabled_precheck_stage_count == 25 && source_disabled_precheck_complete;
    const bool node_plan_observed_only = !node_requires_fresh_minikv_evidence;
    const bool mini_kv_boundary_closed = !mini_kv_imports_node_modules && !mini_kv_executes_node_checks &&
                                         !mini_kv_starts_services && !mini_kv_reads_endpoint_handles &&
                                         !mini_kv_parses_raw_endpoint && !mini_kv_reads_credentials;
    const bool counts_aligned = planned_check_count == 30 && completed_check_count == 30 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 30;
    const bool validation_passed =
        source_locked && node_plan_observed_only && mini_kv_boundary_closed && counts_aligned;

    return "{\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceDisabledPrecheckUpstreamEchoPublishedStageCount\":" +
           std::to_string(source_disabled_precheck_stage_count) +
           ",\"sourceDisabledPrecheckUpstreamEchoComplete\":" + json_bool(source_disabled_precheck_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"nodePlanObservedOnly\":" + json_bool(node_plan_observed_only) +
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpointHandles\":" + json_bool(mini_kv_reads_endpoint_handles) +
           ",\"miniKvParsesRawEndpoint\":" + json_bool(mini_kv_parses_raw_endpoint) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvBoundaryClosed\":" + json_bool(mini_kv_boundary_closed) +
           ",\"plannedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverConstantsImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverReferencesImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverChecksExecutedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverMessagesExecutedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false"
           ",\"nodeRendererExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"schemaMigrationExecuted\":false"
           ",\"ledgerWriteExecuted\":false"
           ",\"externalRequestSent\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation
