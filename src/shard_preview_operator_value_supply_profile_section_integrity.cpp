#include "minikv/shard_preview_operator_value_supply_profile_section_integrity.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_checks.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_stages.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_validation.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_operator_value_supply_profile_section_integrity {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_operator_value_supply_profile_section_integrity_checks;
namespace source_profile = shard_preview_operator_value_supply_profile_section;
namespace stages = shard_preview_operator_value_supply_profile_section_integrity_stages;
namespace validation = shard_preview_operator_value_supply_profile_section_integrity_validation;

constexpr std::string_view integrity_command = "SHARDROUTEVALUESUPPLYPROFILESECTIONINTEGRITYJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1606-production-live-probe-read-only-window-readiness-packet-module-split-roadmap.md";
constexpr int current_stage_count = 10;
constexpr int source_profile_section_planned_stage_count = 25;
constexpr int source_profile_section_count = 4;
constexpr int source_node_migrated_section_count = 4;
constexpr bool source_digest_pinned = true;
constexpr bool source_command_locked = true;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool mini_kv_consumes_operator_values = false;

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
    return stages::operator_value_supply_profile_section_integrity_stages()[static_cast<std::size_t>(
        current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::operator_value_supply_profile_section_integrity_stages(),
        current_stage_count,
        stages::planned_operator_value_supply_profile_section_integrity_stage_count(),
        stages::first_operator_value_supply_profile_section_integrity_release_number());
}

int completed_integrity_check_count() {
    return current_stage_count;
}

bool source_profile_section_complete() {
    return source_profile::published_stage_count() == source_profile_section_planned_stage_count;
}

} // namespace

std::string format_operator_value_supply_profile_section_integrity_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::operator_value_supply_profile_section_integrity_stages(),
        current_stage_count);
}

std::string operator_value_supply_profile_section_integrity_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_operator_value_supply_profile_section_integrity_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int source_stage_count = source_profile::published_stage_count();
    const bool source_complete = source_profile_section_complete();
    const int planned_check_count = checks::planned_operator_value_supply_profile_section_integrity_check_count();
    const int check_count = completed_integrity_check_count();
    const bool integrity_chain_complete =
        current_stage_count == stages::planned_operator_value_supply_profile_section_integrity_stage_count() &&
        source_complete;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-profile-section-integrity.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(integrity_command) +
           ",\"operatorValueSupplyProfileSectionIntegrityMode\":\"controlled-read-only-operator-value-supply-profile-section-integrity-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeProductionLiveProbeReadOnlyWindowReadinessPacketReleaseVersion\":\"Node v1606\""
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeModuleSplitOnly\":true"
           ",\"sourceProfileSectionCommand\":\"SHARDROUTEVALUESUPPLYPROFILESECTIONJSON\""
           ",\"sourceProfileSectionContract\":\"shard-route-preview-operator-value-supply-profile-section-renderer-split.v1\""
           ",\"sourceProfileSectionReleaseVersion\":\"v1160\""
           ",\"sourceProfileSectionFixturePath\":\"fixtures/release/shard-readiness-v1160.json\""
           ",\"sourceProfileSectionPublishedStageCount\":" + std::to_string(source_stage_count) +
           ",\"sourceProfileSectionComplete\":" + json_bool(source_complete) +
           ",\"sourceProfileSectionStatus\":\"route-preview-operator-value-supply-profile-section-closeout-summary-read-only\""
           ",\"sourceProfileSectionDigestMarker\":" +
           json_string(source_profile::operator_value_supply_profile_section_digest_marker()) +
           ",\"sourceProfileSectionDigestPinned\":" + json_bool(source_digest_pinned) +
           ",\"sourceProfileSectionCommandLocked\":" + json_bool(source_command_locked) +
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1160\""
           ",\"operatorValueSupplyProfileSectionIntegrityStage\":" + json_string(stage.stage) +
           ",\"operatorValueSupplyProfileSectionIntegrityStageSequence\":" + std::to_string(stage.sequence) +
           ",\"operatorValueSupplyProfileSectionIntegrityReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"operatorValueSupplyProfileSectionIntegrityReleaseRangeStart\":\"v1161\""
           ",\"operatorValueSupplyProfileSectionIntegrityReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_operator_value_supply_profile_section_integrity_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedOperatorValueSupplyProfileSectionIntegrityCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedOperatorValueSupplyProfileSectionIntegrityCheckCount\":" +
           std::to_string(check_count) +
           ",\"operatorValueSupplyProfileSectionIntegrityDeclared\":true"
           ",\"operatorValueSupplyProfileSectionIntegrityOnly\":true"
           ",\"operatorValueSupplyProfileSectionIntegrityChainComplete\":" + json_bool(integrity_chain_complete) +
           ",\"sourceProfileSectionFixtureFrozen\":true"
           ",\"sourceProfileSectionDigestStable\":true"
           ",\"versionedFixtureChainOnly\":true"
           ",\"nodeParallelMaintenanceObservedAsReadOnlyPlan\":true"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"operatorValueSupplyProfileSectionNames\":" +
           json_string_array({"operator-evidence-import-preflight",
                              "operator-evidence-value-draft",
                              "operator-evidence-fresh-sibling-intake",
                              "operator-evidence-value-supply-envelope"}) +
           ",\"operatorValueSupplyProfileSectionCount\":" +
           std::to_string(source_profile_section_count) +
           ",\"nodeMigratedOperatorValueSupplyProfileSectionCount\":" +
           std::to_string(source_node_migrated_section_count) +
           ",\"sourceLineageMarkers\":" +
           json_string_array({"v560-import-preflight",
                              "v585-value-draft",
                              "Node-v936-fresh-sibling-intake-plan",
                              "v610-value-supply-envelope",
                              "v1160-value-supply-profile-section"}) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvConsumesOperatorValues\":" + json_bool(mini_kv_consumes_operator_values) +
           ",\"operatorValueImported\":false"
           ",\"operatorValueAccepted\":false"
           ",\"operatorValuePersisted\":false"
           ",\"freshSiblingEvidenceImported\":false"
           ",\"freshSiblingEvidenceAccepted\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"syntheticEvidenceAccepted\":false"
           ",\"secretValueRead\":false"
           ",\"credentialValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"moduleSplit\":[\"operator_value_supply_profile_section_integrity_core\",\"operator_value_supply_profile_section_integrity_stages\",\"operator_value_supply_profile_section_integrity_checks\",\"operator_value_supply_profile_section_integrity_validation\"]"
           ",\"stageCatalog\":" + format_operator_value_supply_profile_section_integrity_stage_catalog_json() +
           ",\"integrityChecks\":" +
           checks::format_operator_value_supply_profile_section_integrity_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_operator_value_supply_profile_section_integrity_validation_json(
               source_stage_count,
               source_complete,
               source_profile_section_count,
               source_node_migrated_section_count,
               source_digest_pinned,
               source_command_locked,
               node_requires_fresh_minikv_evidence,
               mini_kv_consumes_operator_values,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_operator_value_supply_profile_section_integrity_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"v1160 remains the frozen value supply profile section fixture before v1161-v1170 integrity evidence",
                              "Node v1606 is treated as parallel readiness packet module-split maintenance context only",
                              "mini-kv does not import operator values or fresh sibling evidence",
                              "runtime payload synthetic evidence credential value raw endpoint router write WAL service startup and execution remain disabled"}) +
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

std::string operator_value_supply_profile_section_integrity_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_operator_value_supply_profile_section_integrity_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_operator_value_supply_profile_section_integrity
