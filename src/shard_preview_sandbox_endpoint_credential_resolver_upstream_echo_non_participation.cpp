#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation.hpp"
#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks.hpp"
#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages.hpp"
#include "minikv/shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks;
namespace source_disabled_precheck = shard_preview_disabled_precheck_upstream_echo_non_participation;
namespace stages = shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages;
namespace validation = shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation;

constexpr std::string_view command_name =
    "SHARDROUTESANDBOXENDPOINTCREDENTIALRESOLVERUPSTREAMECHONONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1903-sandbox-endpoint-credential-resolver-upstream-echo-module-split-roadmap.md";
constexpr int current_stage_count = 30;
constexpr int source_disabled_precheck_planned_stage_count = 25;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_imports_node_modules = false;
constexpr bool mini_kv_executes_node_checks = false;
constexpr bool mini_kv_starts_services = false;
constexpr bool mini_kv_reads_endpoint_handles = false;
constexpr bool mini_kv_parses_raw_endpoint = false;
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
    return stages::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages(),
        current_stage_count,
        stages::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_count(),
        stages::first_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_release_number());
}

bool source_disabled_precheck_complete() {
    return source_disabled_precheck::published_stage_count() == source_disabled_precheck_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages(), current_stage_count);
}

std::string sandbox_endpoint_credential_resolver_upstream_echo_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_disabled_precheck::published_stage_count();
    const bool source_complete = source_disabled_precheck_complete();
    const int planned_check_count =
        checks::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool non_participation_chain_complete =
        current_stage_count ==
            stages::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-sandbox-endpoint-credential-resolver-upstream-echo-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationMode\":\"controlled-read-only-sandbox-endpoint-credential-resolver-upstream-echo-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoReleaseRange\":\"Node v1879-v1903\""
           ",\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoCloseoutReleaseVersion\":\"Node v1903\""
           ",\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v114\""
           ",\"sourceNodeConsumesHistoricalJavaReleaseVersion\":\"Java v105\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeSandboxEndpointCredentialResolverUpstreamEchoSplitOnly\":true"
           ",\"sourceNodeModuleSplitMaintenanceOnly\":true"
           ",\"sourceNodeRequiresSiblingStartup\":false"
           ",\"sourceNodeNoServiceStartupConstraint\":true"
           ",\"sourceDisabledPrecheckCommand\":\"SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON\""
           ",\"sourceDisabledPrecheckContract\":\"shard-route-disabled-precheck-upstream-echo-non-participation.v1\""
           ",\"sourceDisabledPrecheckReleaseVersion\":\"v1442\""
           ",\"sourceDisabledPrecheckFixturePath\":\"fixtures/release/shard-readiness-v1442.json\""
           ",\"sourceDisabledPrecheckPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceDisabledPrecheckComplete\":" + json_bool(source_complete) +
           ",\"sourceDisabledPrecheckStatus\":" +
           json_string(source_disabled_precheck::disabled_precheck_upstream_echo_non_participation_status()) +
           ",\"sourceDisabledPrecheckDigestMarker\":" +
           json_string(source_disabled_precheck::disabled_precheck_upstream_echo_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1442\""
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationStage\":" + json_string(stage.stage) +
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseRangeStart\":\"v1443\""
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(
               stages::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedSandboxEndpointCredentialResolverUpstreamEchoNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationDeclared\":true"
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationOnly\":true"
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationChainComplete\":" +
           json_bool(non_participation_chain_complete) +
           ",\"sourceDisabledPrecheckUpstreamEchoFixtureFrozen\":true"
           ",\"sourceDisabledPrecheckUpstreamEchoDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"sandboxEndpointCredentialResolverUpstreamEchoFocus\":" +
           json_string_array({"source-disabled-precheck-freeze",
                              "node-plan-intake",
                              "command-catalog-read-only",
                              "current-shardjson-rollover",
                              "versioned-fixture-chain-continuity",
                              "stable-barrel-no-import",
                              "route-constant-no-import",
                              "node-reference-no-import",
                              "java-reference-no-import",
                              "historical-minikv-reference-frozen",
                              "historical-helper-no-execution",
                              "readiness-check-no-execution",
                              "blocker-message-no-execution",
                              "core-loader-no-execution",
                              "renderer-export-stability",
                              "typecheck-boundary",
                              "build-vitest-boundary",
                              "no-service-startup",
                              "endpoint-handle-no-read",
                              "raw-endpoint-no-parse",
                              "credential-secret-no-read",
                              "managed-connection-closed",
                              "no-active-router",
                              "no-write-routing",
                              "no-wal-load-restore-compact",
                              "cli-standalone-read-smoke",
                              "tcp-standalone-read-smoke",
                              "fixture-parity-current",
                              "archive-walkthrough-retention",
                              "clean-workspace-ci-closeout"}) +
           ",\"nodeSandboxEndpointCredentialResolverStableBarrelImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverConstantsImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverReferencesImportedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverChecksExecutedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverMessagesExecutedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverCoreLoaderExecutedByMiniKv\":false"
           ",\"nodeSandboxEndpointCredentialResolverHistoricalFallbackExecutedByMiniKv\":false"
           ",\"nodeRendererExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeNpmTestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpointHandles\":" + json_bool(mini_kv_reads_endpoint_handles) +
           ",\"miniKvParsesRawEndpoint\":" + json_bool(mini_kv_parses_raw_endpoint) +
           ",\"miniKvReadsCredentials\":" + json_bool(mini_kv_reads_credentials) +
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"endpointHandleRead\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"evidenceEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"managedAuditConnectionOpened\":false"
           ",\"schemaMigrationExecuted\":false"
           ",\"ledgerWriteExecuted\":false"
           ",\"externalRequestSent\":false"
           ",\"moduleSplit\":[\"sandbox_endpoint_credential_resolver_upstream_echo_non_participation_core\",\"sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stages\",\"sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks\",\"sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation\"]"
           ",\"stageCatalog\":" +
           format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_catalog_json() +
           ",\"sandboxEndpointCredentialResolverUpstreamEchoNonParticipationChecks\":" +
           checks::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_checks_json(
               check_count) +
           ",\"validation\":" +
           validation::format_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_imports_node_modules,
               mini_kv_executes_node_checks,
               mini_kv_starts_services,
               mini_kv_reads_endpoint_handles,
               mini_kv_parses_raw_endpoint,
               mini_kv_reads_credentials,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1442 remains the frozen disabled-precheck upstream echo non-participation source before v1443-v1472 evidence",
                              "Node v1879-v1903 is an internal sandbox endpoint credential resolver upstream echo module split with no fresh mini-kv evidence requirement",
                              "mini-kv does not import Node barrels constants references checks messages loaders renderers typecheck build Vitest or npm test behavior",
                              "endpoint handles raw endpoints credentials managed audit connection schema migration ledger writes external requests routers WAL load restore compact and execution remain disabled"}) +
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

std::string sandbox_endpoint_credential_resolver_upstream_echo_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_sandbox_endpoint_credential_resolver_upstream_echo_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_sandbox_endpoint_credential_resolver_upstream_echo_non_participation
