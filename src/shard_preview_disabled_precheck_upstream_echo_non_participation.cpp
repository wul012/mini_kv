#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation_checks.hpp"
#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation_stages.hpp"
#include "minikv/shard_preview_disabled_precheck_upstream_echo_non_participation_validation.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_disabled_precheck_upstream_echo_non_participation_checks;
namespace source_fixture_audit = shard_preview_type_barrel_split_follow_up_fixture_audit;
namespace stages = shard_preview_disabled_precheck_upstream_echo_non_participation_stages;
namespace validation = shard_preview_disabled_precheck_upstream_echo_non_participation_validation;

constexpr std::string_view command_name = "SHARDROUTEDISABLEDPRECHECKUPSTREAMECHONONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1878-disabled-precheck-upstream-echo-module-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_fixture_audit_planned_stage_count = 12;
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
    return stages::disabled_precheck_upstream_echo_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::disabled_precheck_upstream_echo_non_participation_stages(),
        current_stage_count,
        stages::planned_disabled_precheck_upstream_echo_non_participation_stage_count(),
        stages::first_disabled_precheck_upstream_echo_non_participation_release_number());
}

bool source_fixture_audit_complete() {
    return source_fixture_audit::published_stage_count() == source_fixture_audit_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_disabled_precheck_upstream_echo_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::disabled_precheck_upstream_echo_non_participation_stages(), current_stage_count);
}

std::string disabled_precheck_upstream_echo_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_disabled_precheck_upstream_echo_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_fixture_audit::published_stage_count();
    const bool source_complete = source_fixture_audit_complete();
    const int planned_check_count =
        checks::planned_disabled_precheck_upstream_echo_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool non_participation_chain_complete =
        current_stage_count == stages::planned_disabled_precheck_upstream_echo_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-disabled-precheck-upstream-echo-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"disabledPrecheckUpstreamEchoNonParticipationMode\":\"controlled-read-only-disabled-precheck-upstream-echo-non-participation\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeDisabledPrecheckUpstreamEchoReleaseRange\":\"Node v1867-v1878\""
           ",\"sourceNodeDisabledPrecheckUpstreamEchoCloseoutReleaseVersion\":\"Node v1878\""
           ",\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v115\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeDisabledPrecheckUpstreamEchoSplitOnly\":true"
           ",\"sourceNodeModuleSplitMaintenanceOnly\":true"
           ",\"sourceNodeRequiresSiblingStartup\":false"
           ",\"sourceNodeNoServiceStartupConstraint\":true"
           ",\"sourceFixtureAuditCommand\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON\""
           ",\"sourceFixtureAuditContract\":\"shard-route-preview-type-barrel-split-follow-up-fixture-audit.v1\""
           ",\"sourceFixtureAuditReleaseVersion\":\"v1417\""
           ",\"sourceFixtureAuditFixturePath\":\"fixtures/release/shard-readiness-v1417.json\""
           ",\"sourceFixtureAuditPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceFixtureAuditComplete\":" + json_bool(source_complete) +
           ",\"sourceFixtureAuditStatus\":" +
           json_string(source_fixture_audit::type_barrel_split_follow_up_fixture_audit_status()) +
           ",\"sourceFixtureAuditDigestMarker\":" +
           json_string(source_fixture_audit::type_barrel_split_follow_up_fixture_audit_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1417\""
           ",\"disabledPrecheckUpstreamEchoNonParticipationStage\":" + json_string(stage.stage) +
           ",\"disabledPrecheckUpstreamEchoNonParticipationStageSequence\":" +
           std::to_string(stage.sequence) +
           ",\"disabledPrecheckUpstreamEchoNonParticipationReleaseVersion\":" +
           json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"disabledPrecheckUpstreamEchoNonParticipationReleaseRangeStart\":\"v1418\""
           ",\"disabledPrecheckUpstreamEchoNonParticipationReleaseRangeEnd\":" +
           json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_disabled_precheck_upstream_echo_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedDisabledPrecheckUpstreamEchoNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"disabledPrecheckUpstreamEchoNonParticipationDeclared\":true"
           ",\"disabledPrecheckUpstreamEchoNonParticipationOnly\":true"
           ",\"disabledPrecheckUpstreamEchoNonParticipationChainComplete\":" +
           json_bool(non_participation_chain_complete) +
           ",\"sourceFixtureAuditFrozen\":true"
           ",\"sourceFixtureAuditDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"disabledPrecheckUpstreamEchoFocus\":" +
           json_string_array({"source-fixture-audit-freeze",
                              "node-plan-intake",
                              "command-catalog-read-only",
                              "current-shardjson-rollover",
                              "versioned-fixture-chain-continuity",
                              "stable-barrel-no-import",
                              "constants-reference-no-import",
                              "checks-messages-no-execution",
                              "core-loader-no-execution",
                              "renderer-export-stability",
                              "node-test-boundary",
                              "no-sibling-service-startup",
                              "endpoint-handle-no-read",
                              "raw-endpoint-no-parse",
                              "credential-secret-no-read",
                              "no-active-router",
                              "no-write-routing",
                              "no-wal-load-restore-compact",
                              "cli-standalone-read-smoke",
                              "tcp-standalone-read-smoke",
                              "fixture-parity-current",
                              "archive-screenshot-retention",
                              "walkthrough-index-retention",
                              "whitespace-quality-gate",
                              "clean-workspace-ci-closeout"}) +
           ",\"nodeDisabledPrecheckStableBarrelImportedByMiniKv\":false"
           ",\"nodeDisabledPrecheckConstantsImportedByMiniKv\":false"
           ",\"nodeDisabledPrecheckReferenceBuildersImportedByMiniKv\":false"
           ",\"nodeDisabledPrecheckChecksExecutedByMiniKv\":false"
           ",\"nodeDisabledPrecheckMessagesExecutedByMiniKv\":false"
           ",\"nodeDisabledPrecheckCoreLoaderExecutedByMiniKv\":false"
           ",\"nodeDisabledPrecheckSiblingFallbackParsedByMiniKv\":false"
           ",\"nodeRendererExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeNpmTestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvReadsEndpoints\":" + json_bool(mini_kv_reads_endpoints) +
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
           ",\"moduleSplit\":[\"disabled_precheck_upstream_echo_non_participation_core\",\"disabled_precheck_upstream_echo_non_participation_stages\",\"disabled_precheck_upstream_echo_non_participation_checks\",\"disabled_precheck_upstream_echo_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_disabled_precheck_upstream_echo_non_participation_stage_catalog_json() +
           ",\"disabledPrecheckUpstreamEchoNonParticipationChecks\":" +
           checks::format_disabled_precheck_upstream_echo_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_disabled_precheck_upstream_echo_non_participation_validation_json(
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
               stages::planned_disabled_precheck_upstream_echo_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1417 remains the frozen fixture-audit source before v1418-v1442 non-participation evidence",
                              "Node v1867-v1878 is an internal disabled-precheck upstream echo module split with no fresh mini-kv evidence requirement",
                              "mini-kv does not import Node service barrels constants reference builders checks messages loaders renderers typecheck build Vitest or npm test behavior",
                              "endpoints raw endpoints credentials managed audit connection schema migration ledger writes routers WAL load restore compact and execution remain disabled"}) +
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

std::string disabled_precheck_upstream_echo_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_disabled_precheck_upstream_echo_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation
