#include "minikv/shard_preview_type_barrel_split_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_catalog_entry_group_split_non_participation.hpp"
#include "minikv/shard_preview_type_barrel_split_non_participation_checks.hpp"
#include "minikv/shard_preview_type_barrel_split_non_participation_stages.hpp"
#include "minikv/shard_preview_type_barrel_split_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_type_barrel_split_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_type_barrel_split_non_participation_checks;
namespace source_catalog_split = shard_preview_catalog_entry_group_split_non_participation;
namespace stages = shard_preview_type_barrel_split_non_participation_stages;
namespace validation = shard_preview_type_barrel_split_non_participation_validation;

constexpr std::string_view command_name = "SHARDROUTETYPEBARRELSPLITNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1846-controlled-read-only-shard-preview-type-barrel-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_catalog_split_planned_stage_count = 30;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_starts_type_barrel_split = false;
constexpr bool mini_kv_imports_node_types = false;
constexpr bool mini_kv_consumes_profile_boundaries = false;
constexpr bool mini_kv_reads_evidence_endpoint = false;

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
    return stages::type_barrel_split_non_participation_stages()[static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::type_barrel_split_non_participation_stages(),
        current_stage_count,
        stages::planned_type_barrel_split_non_participation_stage_count(),
        stages::first_type_barrel_split_non_participation_release_number());
}

bool source_catalog_split_complete() {
    return source_catalog_split::published_stage_count() == source_catalog_split_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_type_barrel_split_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::type_barrel_split_non_participation_stages(), current_stage_count);
}

std::string type_barrel_split_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_type_barrel_split_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_catalog_split::published_stage_count();
    const bool source_complete = source_catalog_split_complete();
    const int planned_check_count = checks::planned_type_barrel_split_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool non_participation_chain_complete =
        current_stage_count == stages::planned_type_barrel_split_non_participation_stage_count() && source_complete;

    return "{\"contract\":\"shard-route-preview-type-barrel-split-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"typeBarrelSplitNonParticipationMode\":\"controlled-read-only-shard-preview-type-barrel-split-non-participation-evidence\""
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
           ",\"sourceNodeLegacyAggregatorLineCount\":821"
           ",\"sourceIntegrityCommand\":\"SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON\""
           ",\"sourceIntegrityContract\":\"shard-route-preview-catalog-entry-group-split-non-participation.v1\""
           ",\"sourceIntegrityReleaseVersion\":\"v1360\""
           ",\"sourceIntegrityFixturePath\":\"fixtures/release/shard-readiness-v1360.json\""
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_complete) +
           ",\"sourceIntegrityStatus\":" + json_string(source_catalog_split::catalog_entry_group_split_non_participation_status()) +
           ",\"sourceIntegrityDigestMarker\":" +
           json_string(source_catalog_split::catalog_entry_group_split_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1360\""
           ",\"typeBarrelSplitNonParticipationStage\":" + json_string(stage.stage) +
           ",\"typeBarrelSplitNonParticipationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"typeBarrelSplitNonParticipationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"typeBarrelSplitNonParticipationReleaseRangeStart\":\"v1361\""
           ",\"typeBarrelSplitNonParticipationReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(stages::planned_type_barrel_split_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedTypeBarrelSplitNonParticipationCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedTypeBarrelSplitNonParticipationCheckCount\":" + std::to_string(check_count) +
           ",\"typeBarrelSplitNonParticipationDeclared\":true"
           ",\"typeBarrelSplitNonParticipationOnly\":true"
           ",\"typeBarrelSplitNonParticipationChainComplete\":" + json_bool(non_participation_chain_complete) +
           ",\"sourceCatalogEntryGroupSplitNonParticipationFixtureFrozen\":true"
           ",\"sourceCatalogEntryGroupSplitDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"nodeTypeBarrelSplitFocus\":" +
           json_string_array({"stable-type-barrel-split-boundary",
                              "aggregate-downstream-type-reexports-module",
                              "profile-only-type-module",
                              "legacy-type-entrypoint-compatibility-barrel",
                              "type-level-regression-coverage",
                              "reads-profile-boundary",
                              "preview-graph-profile-boundary",
                              "evidence-endpoint-profile-boundary",
                              "narrow-profile-boundaries-through-stable-barrel",
                              "focused-type-barrel-split-coverage",
                              "profile-import-typecheck",
                              "java-parallel-status-no-fresh-evidence",
                              "mini-kv-parallel-status-no-fresh-evidence",
                              "giant-type-aggregator-growth-guard",
                              "controlled-preview-route-renderer-tests",
                              "build-after-profile-boundary-split",
                              "segmented-vitest-coverage",
                              "split-explanation-archive",
                              "code-walkthrough-archive",
                              "generated-validation-output-cleanup",
                              "diff-line-count-ownership-inspection",
                              "commit-batch",
                              "tag-range",
                              "push-ci-polling",
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
           ",\"miniKvStartsTypeBarrelSplit\":" + json_bool(mini_kv_starts_type_barrel_split) +
           ",\"miniKvImportsNodeTypes\":" + json_bool(mini_kv_imports_node_types) +
           ",\"miniKvConsumesProfileBoundaries\":" + json_bool(mini_kv_consumes_profile_boundaries) +
           ",\"miniKvReadsEvidenceEndpoint\":" + json_bool(mini_kv_reads_evidence_endpoint) +
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
           ",\"moduleSplit\":[\"type_barrel_split_non_participation_core\",\"type_barrel_split_non_participation_stages\",\"type_barrel_split_non_participation_checks\",\"type_barrel_split_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_type_barrel_split_non_participation_stage_catalog_json() +
           ",\"nonParticipationChecks\":" + checks::format_type_barrel_split_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_type_barrel_split_non_participation_validation_json(source_stage_count,
                                                                                  source_complete,
                                                                                  node_requires_fresh_minikv_evidence,
                                                                                  mini_kv_starts_type_barrel_split,
                                                                                  mini_kv_imports_node_types,
                                                                                  mini_kv_consumes_profile_boundaries,
                                                                                  mini_kv_reads_evidence_endpoint,
                                                                                  planned_check_count,
                                                                                  check_count,
                                                                                  current_stage_count,
                                                                                  stages::planned_type_barrel_split_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1360 remains the frozen catalog entry group split non-participation fixture before v1361-v1385 type barrel split non-participation evidence",
                              "Node v1822-v1846 is treated as controlled read-only shard preview type barrel and profile-boundary maintenance context only",
                              "mini-kv does not import Node TypeScript barrels profiles re-exports route renderer tests typecheck build or Vitest behavior",
                              "profile endpoints raw endpoints credentials Node types routers writes WAL and execution remain disabled"}) +
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

std::string type_barrel_split_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(), current_stage_count, stages::planned_type_barrel_split_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_type_barrel_split_non_participation
