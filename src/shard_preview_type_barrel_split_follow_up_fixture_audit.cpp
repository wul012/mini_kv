#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_checks.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_stages.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_fixture_audit_validation.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_type_barrel_split_follow_up_fixture_audit_checks;
namespace source_follow_up = shard_preview_type_barrel_split_follow_up_non_participation;
namespace stages = shard_preview_type_barrel_split_follow_up_fixture_audit_stages;
namespace validation = shard_preview_type_barrel_split_follow_up_fixture_audit_validation;

constexpr std::string_view command_name = "SHARDROUTETYPEBARRELSPLITFOLLOWUPFIXTUREAUDITJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1866-test-only-shell-upstream-echo-module-split-roadmap.md";
constexpr int current_stage_count = 12;
constexpr int source_follow_up_planned_stage_count = 20;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_runtime_reads_source_fixture_payload = false;
constexpr bool mini_kv_imports_node_modules = false;
constexpr bool mini_kv_executes_node_checks = false;
constexpr bool mini_kv_starts_services = false;

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
    return stages::type_barrel_split_follow_up_fixture_audit_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::type_barrel_split_follow_up_fixture_audit_stages(),
        current_stage_count,
        stages::planned_type_barrel_split_follow_up_fixture_audit_stage_count(),
        stages::first_type_barrel_split_follow_up_fixture_audit_release_number());
}

bool source_follow_up_complete() {
    return source_follow_up::published_stage_count() == source_follow_up_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_type_barrel_split_follow_up_fixture_audit_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::type_barrel_split_follow_up_fixture_audit_stages(), current_stage_count);
}

std::string type_barrel_split_follow_up_fixture_audit_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_type_barrel_split_follow_up_fixture_audit_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_follow_up::published_stage_count();
    const bool source_complete = source_follow_up_complete();
    const int planned_check_count = checks::planned_type_barrel_split_follow_up_fixture_audit_check_count();
    const int check_count = completed_check_count();
    const bool fixture_audit_chain_complete =
        current_stage_count == stages::planned_type_barrel_split_follow_up_fixture_audit_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-preview-type-barrel-split-follow-up-fixture-audit.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"typeBarrelSplitFollowUpFixtureAuditMode\":\"controlled-read-only-shard-preview-type-barrel-split-follow-up-fixture-audit\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeTestOnlyShellSplitReleaseRange\":\"Node v1847-v1866\""
           ",\"sourceNodeTestOnlyShellSplitCloseoutReleaseVersion\":\"Node v1866\""
           ",\"sourceNodeConsumesHistoricalMiniKvReleaseVersion\":\"v116\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeTestOnlyShellSplitOnly\":true"
           ",\"sourceNodeModuleSplitMaintenanceOnly\":true"
           ",\"sourceNodeRequiresSiblingStartup\":false"
           ",\"sourceNodeNoServiceStartupConstraint\":true"
           ",\"sourceFollowUpCommand\":\"SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON\""
           ",\"sourceFollowUpContract\":\"shard-route-preview-type-barrel-split-follow-up-non-participation.v1\""
           ",\"sourceFollowUpReleaseVersion\":\"v1405\""
           ",\"sourceFollowUpFixturePath\":\"fixtures/release/shard-readiness-v1405.json\""
           ",\"sourceFollowUpPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceFollowUpComplete\":" + json_bool(source_complete) +
           ",\"sourceFollowUpStatus\":" +
           json_string(source_follow_up::type_barrel_split_follow_up_non_participation_status()) +
           ",\"sourceFollowUpDigestMarker\":" +
           json_string(source_follow_up::type_barrel_split_follow_up_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1405\""
           ",\"typeBarrelSplitFollowUpFixtureAuditStage\":" + json_string(stage.stage) +
           ",\"typeBarrelSplitFollowUpFixtureAuditStageSequence\":" + std::to_string(stage.sequence) +
           ",\"typeBarrelSplitFollowUpFixtureAuditReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"typeBarrelSplitFollowUpFixtureAuditReleaseRangeStart\":\"v1406\""
           ",\"typeBarrelSplitFollowUpFixtureAuditReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_type_barrel_split_follow_up_fixture_audit_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedTypeBarrelSplitFollowUpFixtureAuditCheckCount\":" +
           std::to_string(check_count) +
           ",\"typeBarrelSplitFollowUpFixtureAuditDeclared\":true"
           ",\"typeBarrelSplitFollowUpFixtureAuditOnly\":true"
           ",\"typeBarrelSplitFollowUpFixtureAuditChainComplete\":" + json_bool(fixture_audit_chain_complete) +
           ",\"sourceFollowUpNonParticipationFixtureFrozen\":true"
           ",\"sourceFollowUpDigestPinned\":true"
           ",\"currentFixtureParityAudited\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"fixtureAuditFocus\":" +
           json_string_array({"source-follow-up-fixture-freeze",
                              "current-fixture-parity-audit",
                              "versioned-fixture-continuity",
                              "command-catalog-read-only-audit",
                              "cli-payload-boundary-audit",
                              "tcp-standalone-command-audit",
                              "archive-screenshot-retention-audit",
                              "walkthrough-index-retention-audit",
                              "node-test-only-shell-no-import",
                              "no-router-write-wal-audit",
                              "whitespace-quality-gate-audit",
                              "clean-workspace-ci-closeout-audit"}) +
           ",\"nodeStableBarrelImportedByMiniKv\":false"
           ",\"nodeProfileTypesImportedByMiniKv\":false"
           ",\"nodeTestOnlyShellModulesImportedByMiniKv\":false"
           ",\"nodeCredentialResolverShellExecutedByMiniKv\":false"
           ",\"nodeFakeTransportExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvRuntimeReadsSourceFixturePayload\":" + json_bool(mini_kv_runtime_reads_source_fixture_payload) +
           ",\"miniKvImportsNodeModules\":" + json_bool(mini_kv_imports_node_modules) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvStartsServices\":" + json_bool(mini_kv_starts_services) +
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"evidenceEndpointRead\":false"
           ",\"typeProfileRecordsImported\":false"
           ",\"profileBoundaryRecordsImported\":false"
           ",\"typeExportSurfaceExecuted\":false"
           ",\"typeRegressionCoverageExecuted\":false"
           ",\"routeRendererCoverageExecuted\":false"
           ",\"typecheckExecuted\":false"
           ",\"vitestExecuted\":false"
           ",\"nodeBuildExecuted\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"moduleSplit\":[\"type_barrel_split_follow_up_fixture_audit_core\",\"type_barrel_split_follow_up_fixture_audit_stages\",\"type_barrel_split_follow_up_fixture_audit_checks\",\"type_barrel_split_follow_up_fixture_audit_validation\"]"
           ",\"stageCatalog\":" + format_type_barrel_split_follow_up_fixture_audit_stage_catalog_json() +
           ",\"fixtureAuditChecks\":" +
           checks::format_type_barrel_split_follow_up_fixture_audit_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_type_barrel_split_follow_up_fixture_audit_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_runtime_reads_source_fixture_payload,
               mini_kv_imports_node_modules,
               mini_kv_executes_node_checks,
               mini_kv_starts_services,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_type_barrel_split_follow_up_fixture_audit_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1405 remains the frozen type barrel split follow-up non-participation source fixture before v1406-v1417 fixture audit evidence",
                              "Node v1847-v1866 remains test-only shell upstream echo module split maintenance context only",
                              "mini-kv does not import Node modules TypeScript barrels test-only shell modules route renderer tests typecheck build or Vitest behavior",
                              "profile endpoints raw endpoints credentials routers writes WAL restore load compact and execution remain disabled"}) +
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

std::string type_barrel_split_follow_up_fixture_audit_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(), current_stage_count, stages::planned_type_barrel_split_follow_up_fixture_audit_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit
