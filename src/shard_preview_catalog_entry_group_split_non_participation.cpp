#include "minikv/shard_preview_catalog_entry_group_split_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_production_live_capture_release_evidence_review_non_participation.hpp"
#include "minikv/shard_preview_catalog_entry_group_split_non_participation_checks.hpp"
#include "minikv/shard_preview_catalog_entry_group_split_non_participation_stages.hpp"
#include "minikv/shard_preview_catalog_entry_group_split_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_catalog_entry_group_split_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_catalog_entry_group_split_non_participation_checks;
namespace source_integrity = shard_preview_production_live_capture_release_evidence_review_non_participation;
namespace stages = shard_preview_catalog_entry_group_split_non_participation_stages;
namespace validation = shard_preview_catalog_entry_group_split_non_participation_validation;

constexpr std::string_view command_name = "SHARDROUTECATALOGENTRYGROUPSPLITNONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan = "docs/plans3/v1756-controlled-read-only-shard-preview-type-module-catalog-entry-group-split-roadmap.md";
constexpr int current_stage_count = 35;
constexpr int source_integrity_planned_stage_count = 35;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_starts_catalog_split = false;
constexpr bool mini_kv_reads_catalog_endpoint = false;
constexpr bool mini_kv_consumes_catalog_records = false;

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
    return stages::catalog_entry_group_split_non_participation_stages()[static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::catalog_entry_group_split_non_participation_stages(),
        current_stage_count,
        stages::planned_catalog_entry_group_split_non_participation_stage_count(),
        stages::first_catalog_entry_group_split_non_participation_release_number());
}

bool source_integrity_complete() {
    return source_integrity::published_stage_count() == source_integrity_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_catalog_entry_group_split_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(stages::catalog_entry_group_split_non_participation_stages(), current_stage_count);
}

std::string catalog_entry_group_split_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_catalog_entry_group_split_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_integrity::published_stage_count();
    const bool source_complete = source_integrity_complete();
    const int planned_check_count = checks::planned_catalog_entry_group_split_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool non_participation_chain_complete =
        current_stage_count == stages::planned_catalog_entry_group_split_non_participation_stage_count() && source_complete;

    return "{\"contract\":\"shard-route-preview-catalog-entry-group-split-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"catalogEntryGroupSplitNonParticipationMode\":\"controlled-read-only-shard-preview-type-module-catalog-entry-group-split-non-participation-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeCatalogEntryGroupSplitReleaseRange\":\"Node v1722-v1756\""
           ",\"sourceNodeCatalogEntryGroupSplitCloseoutReleaseVersion\":\"Node v1756\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeCatalogEntryGroupSplitOnly\":true"
           ",\"sourceNodeRequiresSiblingStartup\":false"
           ",\"sourceNodeNoServiceStartupConstraint\":true"
           ",\"sourceIntegrityCommand\":\"SHARDROUTEPRODUCTIONLIVECAPTURERELEASEEVIDENCEREVIEWNONPARTICIPATIONJSON\""
           ",\"sourceIntegrityContract\":\"shard-route-preview-production-live-capture-release-evidence-review-non-participation.v1\""
           ",\"sourceIntegrityReleaseVersion\":\"v1295\""
           ",\"sourceIntegrityFixturePath\":\"fixtures/release/shard-readiness-v1295.json\""
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_complete) +
           ",\"sourceIntegrityStatus\":\"route-preview-production-live-capture-release-evidence-review-non-participation-closeout-summary-read-only\""
           ",\"sourceIntegrityDigestMarker\":" +
           json_string(source_integrity::production_live_capture_release_evidence_review_non_participation_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1295\""
           ",\"catalogEntryGroupSplitNonParticipationStage\":" + json_string(stage.stage) +
           ",\"catalogEntryGroupSplitNonParticipationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"catalogEntryGroupSplitNonParticipationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"catalogEntryGroupSplitNonParticipationReleaseRangeStart\":\"v1296\""
           ",\"catalogEntryGroupSplitNonParticipationReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(stages::planned_catalog_entry_group_split_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedCatalogEntryGroupSplitNonParticipationCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedCatalogEntryGroupSplitNonParticipationCheckCount\":" + std::to_string(check_count) +
           ",\"catalogEntryGroupSplitNonParticipationDeclared\":true"
           ",\"catalogEntryGroupSplitNonParticipationOnly\":true"
           ",\"catalogEntryGroupSplitNonParticipationChainComplete\":" + json_bool(non_participation_chain_complete) +
           ",\"sourceReleaseEvidenceReviewNonParticipationFixtureFrozen\":true"
           ",\"sourceReleaseEvidenceReviewDigestPinned\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"nodeCatalogEntryGroupSplitFocus\":" +
           json_string_array({"catalog-interfaces-types-module",
                              "stable-profile-entrypoint-constant",
                              "foundation-live-read-only-window-entry-group",
                              "operator-evidence-supply-entry-group",
                              "signed-approval-capture-draft-entry-group",
                              "text-package-preflight-entry-group",
                              "text-package-evidence-entry-group",
                              "candidate-document-profile-section-entry-group",
                              "aggregate-entry-group-registry",
                              "stable-public-catalog-entrypoint-api",
                              "defensive-copy-behavior",
                              "computed-module-counts-and-reexport-counts",
                              "catalog-validation-checks",
                              "contiguous-split-entry-groups",
                              "entry-group-id-order",
                              "markdown-renderer-stability",
                              "controlled-read-only-shard-preview-route-tests",
                              "review-artifact-barrel-tests",
                              "typecheck-focused-catalog-tests",
                              "adjacent-catalog-route-review-artifact-tests",
                              "build-after-catalog-entry-split",
                              "segmented-vitest-verification",
                              "no-fresh-java-evidence-required",
                              "no-fresh-mini-kv-evidence-required",
                              "no-service-startup-constraint",
                              "six-entry-group-ownership-boundary",
                              "future-catalog-growth-stop-condition",
                              "catalog-module-below-giant-file-threshold",
                              "focused-entry-group-size-boundaries",
                              "explanation-archive",
                              "code-walkthrough-archive",
                              "generated-validation-output-cleanup",
                              "catalog-split-commit",
                              "tag-and-push",
                              "github-actions-closeout"}) +
           ",\"nodeCatalogEntryRecordsImportedByMiniKv\":false"
           ",\"nodeCatalogRegistryExecutedByMiniKv\":false"
           ",\"nodeCatalogMarkdownRenderedByMiniKv\":false"
           ",\"nodeCatalogEndpointReadByMiniKv\":false"
           ",\"nodeRouteTestsExecutedByMiniKv\":false"
           ",\"nodeTypecheckExecutedByMiniKv\":false"
           ",\"nodeVitestExecutedByMiniKv\":false"
           ",\"nodeBuildExecutedByMiniKv\":false"
           ",\"miniKvStartsCatalogSplit\":" + json_bool(mini_kv_starts_catalog_split) +
           ",\"miniKvConsumesCatalogRecords\":" + json_bool(mini_kv_consumes_catalog_records) +
           ",\"miniKvReadsCatalogEndpoint\":" + json_bool(mini_kv_reads_catalog_endpoint) +
           ",\"miniKvImportsCatalogConstants\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"catalogEndpointRead\":false"
           ",\"catalogRecordsImported\":false"
           ",\"catalogRecordsAccepted\":false"
           ",\"catalogRegistryExecuted\":false"
           ",\"catalogMarkdownRendered\":false"
           ",\"catalogValidationExecuted\":false"
           ",\"catalogFixtureImported\":false"
           ",\"catalogDigestGeneratedByMiniKv\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"moduleSplit\":[\"catalog_entry_group_split_non_participation_core\",\"catalog_entry_group_split_non_participation_stages\",\"catalog_entry_group_split_non_participation_checks\",\"catalog_entry_group_split_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_catalog_entry_group_split_non_participation_stage_catalog_json() +
           ",\"nonParticipationChecks\":" + checks::format_catalog_entry_group_split_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_catalog_entry_group_split_non_participation_validation_json(source_stage_count,
                                                     source_complete,
                                                     node_requires_fresh_minikv_evidence,
                                                     mini_kv_starts_catalog_split,
                                                     mini_kv_reads_catalog_endpoint,
                                                     mini_kv_consumes_catalog_records,
                                                     planned_check_count,
                                                     check_count,
                                                     current_stage_count,
                                                     stages::planned_catalog_entry_group_split_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1295 remains the frozen source release evidence review non-participation fixture before v1296-v1330 catalog entry group split non-participation evidence",
                              "Node v1722-v1756 is treated as controlled read-only shard preview type module catalog entry group split maintenance context only",
                              "mini-kv does not import Node catalog entries registries Markdown renderers routes typecheck build or Vitest behavior",
                              "catalog endpoints raw endpoints credentials payload fixtures routers writes WAL and execution remain disabled"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
           ",\"archiveRouterInstalled\":false"
           ",\"catalogRouterInstalled\":false"
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

std::string catalog_entry_group_split_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(), current_stage_count, stages::planned_catalog_entry_group_split_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_catalog_entry_group_split_non_participation
