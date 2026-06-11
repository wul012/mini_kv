#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks.hpp"
#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages.hpp"
#include "minikv/shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation.hpp"
#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_implementation_plan_upstream_echo_closeout_non_participation_checks;
namespace source_sandbox = shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation;
namespace stages = shard_preview_implementation_plan_upstream_echo_closeout_non_participation_stages;
namespace validation = shard_preview_implementation_plan_upstream_echo_closeout_non_participation_validation;

constexpr std::string_view command_name =
    "SHARDROUTEIMPLEMENTATIONPLANUPSTREAMECHOCLOSEOUTNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1934-implementation-plan-upstream-echo-module-split-closeout-roadmap.md";
constexpr int current_stage_count = 3;
constexpr int source_sandbox_planned_stage_count = 30;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_imports_node_modules = false;
constexpr bool mini_kv_executes_node_checks = false;
constexpr bool mini_kv_starts_services = false;
constexpr bool mini_kv_reads_endpoints = false;
constexpr bool mini_kv_reads_credentials = false;

std::string json_string(std::string_view value) {
    return runtime_evidence::json_string(value);
}

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

std::string json_string_array(const std::vector<std::string>& values) {
    return runtime_evidence::json_string_array(values);
}

const StageRecord& current_stage() {
    return stages::implementation_plan_upstream_echo_closeout_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::implementation_plan_upstream_echo_closeout_non_participation_stages(),
        current_stage_count,
        stages::planned_implementation_plan_upstream_echo_closeout_non_participation_stage_count(),
        stages::first_implementation_plan_upstream_echo_closeout_non_participation_release_number());
}

bool source_sandbox_complete() {
    return source_sandbox::published_stage_count() == source_sandbox_planned_stage_count;
}

int completed_check_count() {
    return checks::planned_implementation_plan_upstream_echo_closeout_non_participation_check_count();
}

} // namespace

std::string format_implementation_plan_upstream_echo_closeout_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::implementation_plan_upstream_echo_closeout_non_participation_stages(), current_stage_count);
}

std::string implementation_plan_upstream_echo_closeout_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_implementation_plan_upstream_echo_closeout_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_sandbox::published_stage_count();
    const bool source_complete = source_sandbox_complete();
    const int planned_check_count =
        checks::planned_implementation_plan_upstream_echo_closeout_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool chain_complete =
        current_stage_count == stages::planned_implementation_plan_upstream_echo_closeout_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-implementation-plan-upstream-echo-closeout-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationMode\":\"controlled-read-only-implementation-plan-upstream-echo-closeout-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeImplementationPlanUpstreamEchoReleaseRange\":\"Node v1904-v1934\""
           ",\"sourceNodeImplementationPlanUpstreamEchoCloseoutReleaseVersion\":\"Node v1934\""
           ",\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v126\""
           ",\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v121\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeImplementationPlanUpstreamEchoCloseoutDocumentationOnly\":true"
           ",\"sourceNodeRequiresSiblingStartup\":false"
           ",\"sourceNodeNoServiceStartupConstraint\":true"
           ",\"sourceSandboxEndpointCommand\":\"SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON\""
           ",\"sourceSandboxEndpointContract\":\"shard-route-sandbox-endpoint-credential-resolver-upstream-echo-non-participation.v1\""
           ",\"sourceSandboxEndpointReleaseVersion\":\"v1472\""
           ",\"sourceSandboxEndpointFixturePath\":\"fixtures/release/shard-readiness-v1472.json\""
           ",\"sourceSandboxEndpointPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceSandboxEndpointComplete\":" + json_bool(source_complete) +
           ",\"sourceSandboxEndpointStatus\":" +
           json_string(source_sandbox::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_status()) +
           ",\"sourceSandboxEndpointDigestMarker\":" +
           json_string(source_sandbox::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1472\""
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationStage\":" + json_string(stage.stage) +
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseRangeStart\":\"v1473\""
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_implementation_plan_upstream_echo_closeout_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedImplementationPlanUpstreamEchoCloseoutNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationDeclared\":true"
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationOnly\":true"
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceSandboxEndpointCredentialResolverUpstreamEchoFixtureFrozen\":true"
           ",\"sourceSandboxEndpointCredentialResolverUpstreamEchoDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeCloseoutObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"implementationPlanUpstreamEchoCloseoutFocus\":" +
           json_string_array({"source-sandbox-endpoint-fixture-freeze",
                              "node-closeout-plan-intake",
                              "ownership-map-metadata-only",
                              "historical-sibling-reference-only",
                              "endpoint-credential-boundary-closed",
                              "router-write-wal-execution-disabled",
                              "command-shardjson-fixture-parity",
                              "clean-workspace-ci-closeout"}) +
           ",\"nodeImplementationPlanStableBarrelImportedByMiniKv\":false"
           ",\"nodeImplementationPlanConstantsImportedByMiniKv\":false"
           ",\"nodeImplementationPlanReferencesImportedByMiniKv\":false"
           ",\"nodeImplementationPlanChecksExecutedByMiniKv\":false"
           ",\"nodeImplementationPlanMessagesExecutedByMiniKv\":false"
           ",\"nodeImplementationPlanCoreLoaderExecutedByMiniKv\":false"
           ",\"nodeRendererExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"endpointHandleRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"schemaMigrationExecuted\":false"
           ",\"ledgerWriteExecuted\":false"
           ",\"externalRequestSent\":false"
           ",\"moduleSplit\":[\"implementation_plan_upstream_echo_closeout_non_participation_core\",\"implementation_plan_upstream_echo_closeout_non_participation_stages\",\"implementation_plan_upstream_echo_closeout_non_participation_checks\",\"implementation_plan_upstream_echo_closeout_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_implementation_plan_upstream_echo_closeout_non_participation_stage_catalog_json() +
           ",\"implementationPlanUpstreamEchoCloseoutNonParticipationChecks\":" +
           checks::format_implementation_plan_upstream_echo_closeout_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_implementation_plan_upstream_echo_closeout_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_imports_node_modules,
               mini_kv_executes_node_checks,
               mini_kv_starts_services,
               mini_kv_reads_endpoints,
               mini_kv_reads_credentials,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_implementation_plan_upstream_echo_closeout_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1472 remains the frozen sandbox endpoint credential resolver upstream echo non-participation source",
                              "Node v1934 is documentation-only implementation-plan upstream echo closeout maintenance",
                              "Node consumes historical mini-kv v126 and Java v121 references; no fresh sibling evidence or service startup is required",
                              "mini-kv records ownership-map non-participation without importing Node modules or reading endpoints credentials managed connections routers writes WAL or execution"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"typeRouterInstalled\":false"
           ",\"routerActivationAllowed\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"writeCommandsAllowed\":false"
           ",\"mutatesStore\":false"
           ",\"adminCommandsAllowed\":false"
           ",\"loadRestoreCompactAllowed\":false"
           ",\"touchesWal\":false"
           ",\"startsServices\":false"
           ",\"startsMiniKvService\":false"
           ",\"startsSiblingServices\":false"
           ",\"liveReadAllowed\":false"
           ",\"runtimeProbeAllowed\":false"
           ",\"filesystemReadPerformed\":false"
           ",\"runtimeArchiveWalkAllowed\":false"
           ",\"siblingMutationAllowed\":false"
           ",\"readOnly\":true"
           ",\"executionAllowed\":false}";
}

std::string implementation_plan_upstream_echo_closeout_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_implementation_plan_upstream_echo_closeout_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_implementation_plan_upstream_echo_closeout_non_participation
