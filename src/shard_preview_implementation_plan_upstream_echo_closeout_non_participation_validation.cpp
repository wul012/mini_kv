#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
    int source_sandbox_endpoint_stage_count,
    bool source_sandbox_endpoint_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    bool mini_kv_reads_endpoints,
    bool mini_kv_reads_credentials,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_sandbox_endpoint_stage_count == 30 && source_sandbox_endpoint_complete;
    const bool node_plan_observed_only = !node_requires_fresh_minikv_evidence;
    const bool mini_kv_boundary_closed = !mini_kv_imports_node_modules && !mini_kv_executes_node_checks &&
                                         !mini_kv_starts_services && !mini_kv_reads_endpoints &&
                                         !mini_kv_reads_credentials;
    const bool counts_aligned = planned_check_count == 8 && completed_check_count == 8 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 3;
    const bool validation_passed =
        source_locked && node_plan_observed_only && mini_kv_boundary_closed && counts_aligned;

    return "{\"implementationPlanUpstreamEchoCloseoutNonParticipationValidationPassed\":" +
           json_bool(validation_passed) +
           ",\"sourceSandboxEndpointCredentialResolverUpstreamEchoPublishedStageCount\":" +
           std::to_string(source_sandbox_endpoint_stage_count) +
           ",\"sourceSandboxEndpointCredentialResolverUpstreamEchoComplete\":" +
           json_bool(source_sandbox_endpoint_complete) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"nodePlanObservedOnly\":" + json_bool(node_plan_observed_only) +
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvBoundaryClosed\":" + json_bool(mini_kv_boundary_closed) +
           ",\"plannedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":" +
           std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true"
           ",\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false"
           ",\"nodeImplementationPlanConstantsImportedByMiniKv\":false"
           ",\"nodeImplementationPlanReferencesImportedByMiniKv\":false"
           ",\"nodeImplementationPlanChecksExecutedByMiniKv\":false"
           ",\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false"
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation
