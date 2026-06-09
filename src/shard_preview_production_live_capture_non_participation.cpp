#include "minikv/shard_preview_production_live_capture_non_participation.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_integrity.hpp"
#include "minikv/shard_preview_production_live_capture_non_participation_checks.hpp"
#include "minikv/shard_preview_production_live_capture_non_participation_stages.hpp"
#include "minikv/shard_preview_production_live_capture_non_participation_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_production_live_capture_non_participation {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_production_live_capture_non_participation_checks;
namespace source_integrity = shard_preview_operator_value_supply_profile_section_integrity;
namespace stages = shard_preview_production_live_capture_non_participation_stages;
namespace validation = shard_preview_production_live_capture_non_participation_validation;

constexpr std::string_view command_name = "SHARDROUTEPRODUCTIONLIVECAPTURENONPARTICIPATIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1631-production-live-probe-read-only-window-live-capture-module-split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_integrity_planned_stage_count = 10;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_starts_live_capture = false;
constexpr bool mini_kv_reads_raw_endpoint = false;

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
    return stages::production_live_capture_non_participation_stages()[static_cast<std::size_t>(
        current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::production_live_capture_non_participation_stages(),
        current_stage_count,
        stages::planned_production_live_capture_non_participation_stage_count(),
        stages::first_production_live_capture_non_participation_release_number());
}

bool source_integrity_complete() {
    return source_integrity::published_stage_count() == source_integrity_planned_stage_count;
}

int completed_check_count() {
    return current_stage_count;
}

} // namespace

std::string format_production_live_capture_non_participation_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::production_live_capture_non_participation_stages(),
        current_stage_count);
}

std::string production_live_capture_non_participation_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_production_live_capture_non_participation_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_integrity::published_stage_count();
    const bool source_complete = source_integrity_complete();
    const int planned_check_count = checks::planned_production_live_capture_non_participation_check_count();
    const int check_count = completed_check_count();
    const bool non_participation_chain_complete =
        current_stage_count == stages::planned_production_live_capture_non_participation_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-preview-production-live-capture-non-participation.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(command_name) +
           ",\"productionLiveCaptureNonParticipationMode\":\"controlled-read-only-production-live-capture-non-participation-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeProductionLiveProbeReadOnlyWindowLiveCaptureReleaseRange\":\"Node v1607-v1631\""
           ",\"sourceNodeProductionLiveProbeReadOnlyWindowLiveCaptureCloseoutReleaseVersion\":\"Node v1631\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeModuleSplitOnly\":true"
           ",\"sourceNodeHttpSmokeIsNodeOnly\":true"
           ",\"sourceIntegrityCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON\""
           ",\"sourceIntegrityContract\":\"shard-route-preview-operator-value-supply-profile-section-integrity.v1\""
           ",\"sourceIntegrityReleaseVersion\":\"v1170\""
           ",\"sourceIntegrityFixturePath\":\"fixtures/release/shard-readiness-v1170.json\""
           ",\"sourceIntegrityPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceIntegrityComplete\":" + json_bool(source_complete) +
           ",\"sourceIntegrityStatus\":\"route-preview-operator-value-supply-profile-section-integrity-closeout-summary-read-only\""
           ",\"sourceIntegrityDigestMarker\":" +
           json_string(source_integrity::operator_value_supply_profile_section_integrity_digest_marker()) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1170\""
           ",\"productionLiveCaptureNonParticipationStage\":" + json_string(stage.stage) +
           ",\"productionLiveCaptureNonParticipationStageSequence\":" + std::to_string(stage.sequence) +
           ",\"productionLiveCaptureNonParticipationReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"productionLiveCaptureNonParticipationReleaseRangeStart\":\"v1171\""
           ",\"productionLiveCaptureNonParticipationReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_production_live_capture_non_participation_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedProductionLiveCaptureNonParticipationCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedProductionLiveCaptureNonParticipationCheckCount\":" +
           std::to_string(check_count) +
           ",\"productionLiveCaptureNonParticipationDeclared\":true"
           ",\"productionLiveCaptureNonParticipationOnly\":true"
           ",\"productionLiveCaptureNonParticipationChainComplete\":" +
           json_bool(non_participation_chain_complete) +
           ",\"sourceIntegrityFixtureFrozen\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"nodeLiveCaptureModuleSplitFocus\":" +
           json_string_array({"profile-types",
                              "stable-entrypoint-reexport",
                              "builder-parts",
                              "captured-record-conversion",
                              "readiness-smoke-checks",
                              "state-resolution",
                              "blockers-warnings-recommendations",
                              "digest-input-shape",
                              "endpoint-constants",
                              "markdown-renderer"}) +
           ",\"nodeLiveCaptureExecuted\":false"
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvStartsLiveCapture\":" + json_bool(mini_kv_starts_live_capture) +
           ",\"miniKvConsumesLiveCaptureRecords\":false"
           ",\"miniKvReadsRawEndpoint\":" + json_bool(mini_kv_reads_raw_endpoint) +
           ",\"miniKvImportsEndpointConstants\":false"
           ",\"miniKvServiceStarted\":false"
           ",\"javaServiceStarted\":false"
           ",\"siblingServiceStarted\":false"
           ",\"liveEndpointRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"credentialValueRead\":false"
           ",\"secretValueRead\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"productionLiveReadAllowed\":false"
           ",\"probeWindowOpened\":false"
           ",\"moduleSplit\":[\"production_live_capture_non_participation_core\",\"production_live_capture_non_participation_stages\",\"production_live_capture_non_participation_checks\",\"production_live_capture_non_participation_validation\"]"
           ",\"stageCatalog\":" + format_production_live_capture_non_participation_stage_catalog_json() +
           ",\"nonParticipationChecks\":" +
           checks::format_production_live_capture_non_participation_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_production_live_capture_non_participation_validation_json(
               source_stage_count,
               source_complete,
               node_requires_fresh_minikv_evidence,
               mini_kv_starts_live_capture,
               mini_kv_reads_raw_endpoint,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_production_live_capture_non_participation_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1170 remains the frozen source integrity fixture before v1171-v1195 live capture non-participation evidence",
                              "Node v1607-v1631 is treated as production live capture module-split maintenance context only",
                              "mini-kv does not start live capture services or sibling services",
                              "endpoint constants raw endpoint values credentials runtime payload routers writes WAL and execution remain disabled"}) +
           ",\"activeRouterInstalled\":false"
           ",\"documentRouterInstalled\":false"
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

std::string production_live_capture_non_participation_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_production_live_capture_non_participation_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_production_live_capture_non_participation
