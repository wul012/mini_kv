#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_checks.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_stages.hpp"
#include "minikv/shard_preview_type_barrel_split_follow_up_non_participation_validation.hpp"
#include "minikv/shard_preview_type_barrel_split_non_participation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_type_barrel_split_follow_up_non_participation_checks;
namespace source_type_split = shard_preview_type_barrel_split_non_participation;
namespace stages = shard_preview_type_barrel_split_follow_up_non_participation_stages;
namespace validation = shard_preview_type_barrel_split_follow_up_non_participation_validation;

constexpr std::string_view command_name = "SHARDROUTETYPEBARRELSPLITFOLLOWUPNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1846-controlled-read-only-shard-preview-type-barrel-split-roadmap.md";
constexpr int current_stage_count = 20;
constexpr int source_type_split_planned_stage_count = 25;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_starts_follow_up = false;
constexpr bool mini_kv_imports_node_types = false;
constexpr bool mini_kv_reads_profile_endpoint = false;
constexpr bool mini_kv_executes_node_checks = false;

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
    return stages::type_barrel_split_follow_up_non_participation_stages()
        [static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::type_barrel_split_follow_up_non_participation_stages(),
        current_stage_count,
        stages::planned_type_barrel_split_follow_up_non_participation_stage_count(),
        stages::first_type_barrel_split_follow_up_non_participation_release_number());
}

bool source_type_split_complete() {
    return source_type_split::published_stage_count() == source_type_split_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_type_barrel_split_follow_up_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::type_barrel_split_follow_up_non_participation_stages(), current_stage_count);
}

std::string type_barrel_split_follow_up_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_type_barrel_split_follow_up_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_type_split::published_stage_count();
    const bool source_complete = source_type_split_complete();
    const int planned_check_count = checks::planned_type_barrel_split_follow_up_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool follow_up_chain_complete =
        current_stage_count == stages::planned_type_barrel_split_follow_up_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-preview-type-barrel-split-follow-up-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"typeBarrelSplitFollowUpNonParticipationMode\":\"controlled-read-only-shard-preview-type-barrel-split-follow-up-non-participation-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeTypeBarrelSplitReleaseRange\":\"Node v1822-v1846\""
           ",\"sourceNodeTypeBarrelSplitCloseoutReleaseVersion\":\"Node v1846\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeTypeBarrelSplitOnly\":true"
           ",\"sourceNodeTypeSurfaceMaintenanceOnly\":true"
           ",\"sourceNodeRequiresSiblingStartup\":false"
           ",\"sourceNodeNoServiceStartupConstraint\":true"
           ",\"sourceTypeBarrelSplitCommand\":\"SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON\""
           ",\"sourceTypeBarrelSplitContract\":\"shard-route-preview-type-barrel-split-non-participation.v1\""
           ",\"sourceTypeBarrelSplitReleaseVersion\":\"v1385\""
           ",\"sourceTypeBarrelSplitFixturePath\":\"fixtures/release/shard-readiness-v1385.json\""
           ",\"sourceTypeBarrelSplitPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceTypeBarrelSplitComplete\":" + json_bool(source_complete) +
           ",\"sourceTypeBarrelSplitStatus\":" +
           json_string(source_type_split::type_barrel_split_non_participation_status()) +
           ",\"sourceTypeBarrelSplitDigestMarker\":" +
           json_string(source_type_split::type_barrel_split_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1385\""
           ",\"typeBarrelSplitFollowUpNonParticipationStage\":" + json_string(stage.stage) +
           ",\"typeBarrelSplitFollowUpNonParticipationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"typeBarrelSplitFollowUpNonParticipationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"typeBarrelSplitFollowUpNonParticipationReleaseRangeStart\":\"v1386\""
           ",\"typeBarrelSplitFollowUpNonParticipationReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_type_barrel_split_follow_up_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedTypeBarrelSplitFollowUpNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedTypeBarrelSplitFollowUpNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"typeBarrelSplitFollowUpNonParticipationDeclared\":true"
           ",\"typeBarrelSplitFollowUpNonParticipationOnly\":true"
           ",\"typeBarrelSplitFollowUpNonParticipationChainComplete\":" + json_bool(follow_up_chain_complete) +
           ",\"sourceTypeBarrelSplitNonParticipationFixtureFrozen\":true"
           ",\"sourceTypeBarrelSplitDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"followUpFocus\":" +
           json_string_array({"source-fixture-freeze",
                              "command-catalog-continuity",
                              "shardjson-current-rollover",
                              "versioned-fixture-chain",
                              "profile-endpoint-no-read",
                              "node-type-import-guard",
                              "route-renderer-no-execution",
                              "node-validation-no-execution",
                              "tcp-standalone-read-command-boundary",
                              "large-payload-cli-only-boundary",
                              "rolling-current-test-hardening",
                              "source-digest-pin",
                              "archive-screenshot-retention",
                              "walkthrough-index-continuity",
                              "generated-validation-cleanup",
                              "no-service-startup",
                              "router-write-wal-guard",
                              "fixture-parity-current",
                              "whitespace-quality-gate",
                              "clean-workspace-ci-closeout"}) +
           ",\"nodeStableBarrelImportedByMiniKv\":false"
           ",\"nodeProfileTypesImportedByMiniKv\":false"
           ",\"nodeAggregateReexportsImportedByMiniKv\":false"
           ",\"nodeNamedProfileBoundaryConsumedByMiniKv\":false"
           ",\"nodeProfileBoundaryEndpointReadByMiniKv\":false"
           ",\"nodeRouteRendererTestsExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvStartsTypeBarrelFollowUp\":" + json_bool(mini_kv_starts_follow_up) +
           ",\"miniKvImportsNodeTypes\":" + json_bool(mini_kv_imports_node_types) +
           ",\"miniKvReadsProfileEndpoint\":" + json_bool(mini_kv_reads_profile_endpoint) +
           ",\"miniKvExecutesNodeChecks\":" + json_bool(mini_kv_executes_node_checks) +
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"profileBoundaryEndpointRead\":false"
           ",\"evidenceEndpointRead\":false"
           ",\"typeProfileRecordsImported\":false"
           ",\"profileBoundaryRecordsImported\":false"
           ",\"profileBoundaryRecordsAccepted\":false"
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
           ",\"moduleSplit\":[\"type_barrel_split_follow_up_non_participation_core\",\"type_barrel_split_follow_up_non_participation_stages\",\"type_barrel_split_follow_up_non_participation_checks\",\"type_barrel_split_follow_up_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_type_barrel_split_follow_up_non_participation_stage_catalog_json() +
           ",\"nonParticipationChecks\":" +
           checks::format_type_barrel_split_follow_up_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_type_barrel_split_follow_up_non_participation_validation_json(source_stage_count,
                                                                                             source_complete,
                                                                                             node_requires_fresh_minikv_evidence,
                                                                                             mini_kv_starts_follow_up,
                                                                                             mini_kv_imports_node_types,
                                                                                             mini_kv_reads_profile_endpoint,
                                                                                             mini_kv_executes_node_checks,
                                                                                             planned_check_count,
                                                                                             check_count,
                                                                                             current_stage_count,
                                                                                             stages::planned_type_barrel_split_follow_up_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1385 remains the frozen type barrel split non-participation source fixture before v1386-v1405 follow-up evidence",
                              "Node v1822-v1846 remains controlled read-only shard preview type-surface maintenance context only",
                              "mini-kv does not import Node TypeScript barrels profiles re-exports route renderer tests typecheck build or Vitest behavior",
                              "profile endpoints raw endpoints credentials routers writes WAL and execution remain disabled"}) +
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

std::string type_barrel_split_follow_up_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(), current_stage_count, stages::planned_type_barrel_split_follow_up_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation
