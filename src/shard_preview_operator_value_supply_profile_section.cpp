#include "minikv/shard_preview_operator_value_supply_profile_section.hpp"

#include "minikv/runtime_evidence.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_checks.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_stages.hpp"
#include "minikv/shard_preview_operator_value_supply_profile_section_validation.hpp"
#include "minikv/shard_route_preview_operator_import_preflight.hpp"
#include "minikv/shard_route_preview_operator_value_draft.hpp"
#include "minikv/shard_route_preview_operator_value_supply_envelope.hpp"
#include "minikv/shard_route_preview_stage_catalog.hpp"
#include "minikv/shard_route_preview_stage_chain.hpp"

#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace minikv::shard_preview_operator_value_supply_profile_section {
namespace {

using StageRecord = shard_route_preview_stage_catalog::StageRecord;
namespace checks = shard_preview_operator_value_supply_profile_section_checks;
namespace import_preflight = shard_route_preview_operator_import_preflight;
namespace stages = shard_preview_operator_value_supply_profile_section_stages;
namespace validation = shard_preview_operator_value_supply_profile_section_validation;
namespace value_draft = shard_route_preview_operator_value_draft;
namespace value_supply_envelope = shard_route_preview_operator_value_supply_envelope;

constexpr std::string_view profile_section_command = "SHARDROUTEVALUESUPPLYPROFILESECTIONJSON";
constexpr std::string_view source_node_plan =
    "docs/plans3/v1581-controlled-read-only-shard-preview-operator-evidence-value-supply-profile-section-renderer-"
    "split-roadmap.md";
constexpr int current_stage_count = 25;
constexpr int source_import_preflight_planned_stage_count = 25;
constexpr int source_value_draft_planned_stage_count = 25;
constexpr int source_value_supply_envelope_planned_stage_count = 25;
constexpr int value_supply_profile_section_count = 4;
constexpr int node_migrated_value_supply_profile_section_count = 4;
constexpr bool node_requires_fresh_minikv_evidence = false;
constexpr bool route_markdown_changed = false;
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
    return stages::operator_value_supply_profile_section_stages()[static_cast<std::size_t>(current_stage_count - 1)];
}

shard_route_preview_stage_chain::StageChainReport current_stage_chain_report() {
    return shard_route_preview_stage_chain::inspect_stage_chain(
        stages::operator_value_supply_profile_section_stages(),
        current_stage_count,
        stages::planned_operator_value_supply_profile_section_stage_count(),
        stages::first_operator_value_supply_profile_section_release_number());
}

int completed_profile_section_check_count() {
    return current_stage_count;
}

bool source_import_preflight_complete() {
    return import_preflight::published_stage_count() == source_import_preflight_planned_stage_count;
}

bool source_value_draft_complete() {
    return value_draft::published_stage_count() == source_value_draft_planned_stage_count;
}

bool source_value_supply_envelope_complete() {
    return value_supply_envelope::published_stage_count() == source_value_supply_envelope_planned_stage_count;
}

} // namespace

std::string format_operator_value_supply_profile_section_stage_catalog_json() {
    return shard_route_preview_stage_catalog::format_stage_catalog_json(
        stages::operator_value_supply_profile_section_stages(),
        current_stage_count);
}

std::string operator_value_supply_profile_section_status() {
    return std::string{current_stage().stage} + "-read-only";
}

std::string format_operator_value_supply_profile_section_json() {
    const auto& stage = current_stage();
    const auto stage_chain = current_stage_chain_report();
    const int import_preflight_stage_count = import_preflight::published_stage_count();
    const bool import_preflight_complete = source_import_preflight_complete();
    const int value_draft_stage_count = value_draft::published_stage_count();
    const bool value_draft_complete = source_value_draft_complete();
    const int value_supply_envelope_stage_count = value_supply_envelope::published_stage_count();
    const bool value_supply_envelope_complete = source_value_supply_envelope_complete();
    const int planned_check_count = checks::planned_operator_value_supply_profile_section_check_count();
    const int check_count = completed_profile_section_check_count();
    const bool chain_complete =
        current_stage_count == stages::planned_operator_value_supply_profile_section_stage_count() &&
        import_preflight_complete && value_draft_complete && value_supply_envelope_complete;

    return "{\"contract\":\"shard-route-preview-operator-value-supply-profile-section-renderer-split.v1\""
           ",\"project\":\"mini-kv\""
           ",\"command\":" + json_string(profile_section_command) +
           ",\"operatorValueSupplyProfileSectionMode\":\"controlled-read-only-operator-value-supply-profile-section-renderer-split-evidence\""
           ",\"sourceNodePlan\":" + json_string(source_node_plan) +
           ",\"sourceNodeOperatorValueSupplyProfileSectionReleaseVersion\":\"Node v1581\""
           ",\"sourceNodeValueSupplyProfileSectionRendererModule\":\"controlledReadOnlyShardPreviewOperatorEvidenceValueSupplyProfileSectionRenderer.ts\""
           ",\"sourceNodeValueSupplyAggregatorGroup\":\"operator-evidence-value-supply-profile-section-aggregator\""
           ",\"sourceNodeImportValueDraftRendererGroup\":\"import-preflight-and-value-draft-profile-section-renderer\""
           ",\"sourceNodeSiblingEnvelopeRendererGroup\":\"fresh-sibling-intake-and-value-supply-envelope-profile-section-renderer\""
           ",\"sourceNodeValueSupplyProfileSectionGroupCount\":" +
           std::to_string(value_supply_profile_section_count) +
           ",\"sourceNodeMigratedValueSupplyProfileSectionCount\":" +
           std::to_string(node_migrated_value_supply_profile_section_count) +
           ",\"sourceNodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"sourceNodeRequiresFreshJavaEvidence\":false"
           ",\"sourceNodeStartsMiniKvService\":false"
           ",\"sourceNodeStartsJavaService\":false"
           ",\"sourceNodeRouteMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"sourceNodeRuntimeBehaviorAdded\":false"
           ",\"sourceNodeVersionMarkers\":" +
           json_string_array({"Node v886", "Node v911", "Node v936", "Node v961"}) +
           ",\"sourceNodeTypeCatalogOrderRange\":\"217-220\""
           ",\"sourceCurrentShardReadinessReleaseVersion\":\"v1135\""
           ",\"sourceCurrentShardReadinessFixturePath\":\"fixtures/release/shard-readiness-v1135.json\""
           ",\"sourceImportPreflightCommand\":\"SHARDROUTEIMPORTPREFLIGHTJSON\""
           ",\"sourceImportPreflightReleaseVersion\":\"v560\""
           ",\"sourceImportPreflightFixturePath\":\"fixtures/release/shard-readiness-v560.json\""
           ",\"sourceImportPreflightPublishedStageCount\":" + std::to_string(import_preflight_stage_count) +
           ",\"sourceImportPreflightComplete\":" + json_bool(import_preflight_complete) +
           ",\"sourceImportPreflightDigestMarker\":" +
           json_string(import_preflight::import_preflight_digest_marker()) +
           ",\"sourceValueDraftCommand\":\"SHARDROUTEVALUEDRAFTJSON\""
           ",\"sourceValueDraftReleaseVersion\":\"v585\""
           ",\"sourceValueDraftFixturePath\":\"fixtures/release/shard-readiness-v585.json\""
           ",\"sourceValueDraftPublishedStageCount\":" + std::to_string(value_draft_stage_count) +
           ",\"sourceValueDraftComplete\":" + json_bool(value_draft_complete) +
           ",\"sourceValueDraftDigestMarker\":" + json_string(value_draft::value_draft_digest_marker()) +
           ",\"sourceFreshSiblingIntakeReleaseVersion\":\"Node v936\""
           ",\"sourceFreshSiblingIntakePlanOnly\":true"
           ",\"sourceFreshSiblingIntakeCommandPresent\":false"
           ",\"sourceFreshSiblingIntakeRepresentedByEnvelopePlan\":true"
           ",\"sourceValueSupplyEnvelopeCommand\":\"SHARDROUTEVALUESUPPLYJSON\""
           ",\"sourceValueSupplyEnvelopeReleaseVersion\":\"v610\""
           ",\"sourceValueSupplyEnvelopeFixturePath\":\"fixtures/release/shard-readiness-v610.json\""
           ",\"sourceValueSupplyEnvelopePublishedStageCount\":" +
           std::to_string(value_supply_envelope_stage_count) +
           ",\"sourceValueSupplyEnvelopeComplete\":" + json_bool(value_supply_envelope_complete) +
           ",\"sourceValueSupplyEnvelopeDigestMarker\":" +
           json_string(value_supply_envelope::value_supply_envelope_digest_marker()) +
           ",\"operatorValueSupplyProfileSectionStage\":" + json_string(stage.stage) +
           ",\"operatorValueSupplyProfileSectionStageSequence\":" + std::to_string(stage.sequence) +
           ",\"operatorValueSupplyProfileSectionReleaseVersion\":" + json_string(stage.release_version) +
           ",\"sourceFrozenReleaseVersion\":" + json_string(stage.source_frozen_release_version) +
           ",\"sourceFrozenFixturePath\":" + json_string(stage.source_frozen_fixture_path) +
           ",\"operatorValueSupplyProfileSectionReleaseRangeStart\":\"v1136\""
           ",\"operatorValueSupplyProfileSectionReleaseRangeEnd\":" + json_string(stage.release_version) +
           ",\"publishedStageCount\":" + std::to_string(current_stage_count) +
           ",\"plannedStageCount\":" +
           std::to_string(stages::planned_operator_value_supply_profile_section_stage_count()) +
           ",\"stageChain\":" + shard_route_preview_stage_chain::format_stage_chain_report_json(stage_chain) +
           ",\"plannedOperatorValueSupplyProfileSectionCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedOperatorValueSupplyProfileSectionCheckCount\":" + std::to_string(check_count) +
           ",\"operatorValueSupplyProfileSectionRendererSplitDeclared\":true"
           ",\"operatorValueSupplyProfileSectionOnly\":true"
           ",\"operatorValueSupplyProfileSectionChainComplete\":" + json_bool(chain_complete) +
           ",\"sourceCurrentFixtureFrozen\":true"
           ",\"sourceValueSupplyEvidenceFrozen\":true"
           ",\"nodeRendererSplitObservedAsReadOnlyPlan\":true"
           ",\"routeFacingMarkdownStable\":true"
           ",\"routeFacingOutputChanged\":false"
           ",\"nodeRuntimeBehaviorAdded\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"freshJavaEvidenceRequiredByNode\":false"
           ",\"operatorValueSupplyProfileSectionNames\":" +
           json_string_array({"operator-evidence-import-preflight",
                              "operator-evidence-value-draft",
                              "operator-evidence-fresh-sibling-intake",
                              "operator-evidence-value-supply-envelope"}) +
           ",\"operatorValueSupplyProfileSectionCount\":" +
           std::to_string(value_supply_profile_section_count) +
           ",\"nodeMigratedOperatorValueSupplyProfileSectionCount\":" +
           std::to_string(node_migrated_value_supply_profile_section_count) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"miniKvConsumesOperatorValues\":" + json_bool(mini_kv_consumes_operator_values) +
           ",\"operatorValueImported\":false"
           ",\"operatorValueAccepted\":false"
           ",\"operatorValuePersisted\":false"
           ",\"freshSiblingEvidenceImported\":false"
           ",\"freshSiblingEvidenceAccepted\":false"
           ",\"reviewedMaterialPresent\":false"
           ",\"externalReviewedMaterialConsumed\":false"
           ",\"materialIntakeOpened\":false"
           ",\"materialPayloadImported\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"syntheticEvidenceAccepted\":false"
           ",\"secretValueRead\":false"
           ",\"credentialValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"typeCatalogImported\":false"
           ",\"moduleSplit\":[\"operator_value_supply_profile_section_core\",\"operator_value_supply_profile_section_stages\",\"operator_value_supply_profile_section_checks\",\"operator_value_supply_profile_section_validation\"]"
           ",\"stageCatalog\":" + format_operator_value_supply_profile_section_stage_catalog_json() +
           ",\"profileSectionChecks\":" + checks::format_operator_value_supply_profile_section_checks_json(check_count) +
           ",\"validation\":" +
           validation::format_operator_value_supply_profile_section_validation_json(
               import_preflight_stage_count,
               import_preflight_complete,
               value_draft_stage_count,
               value_draft_complete,
               value_supply_envelope_stage_count,
               value_supply_envelope_complete,
               value_supply_profile_section_count,
               node_migrated_value_supply_profile_section_count,
               node_requires_fresh_minikv_evidence,
               route_markdown_changed,
               mini_kv_consumes_operator_values,
               planned_check_count,
               check_count,
               current_stage_count,
               stages::planned_operator_value_supply_profile_section_stage_count()) +
           ",\"diagnostics\":" +
           json_string_array({"Node v1581 is treated as a value supply profile-section renderer ownership split",
                              "v1135 remains the frozen current fixture before v1136-v1160 value supply profile-section evidence",
                              "v560 v585 and v610 remain the frozen value supply source lineage",
                              "fresh sibling intake is represented as Node v936 plan metadata only",
                              "mini-kv does not import operator values or consume synthetic evidence",
                              "renderer execution catalog import router write routing WAL touch service startup runtime probes secret reads raw endpoint parsing and execution remain disabled"}) +
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

std::string operator_value_supply_profile_section_digest_marker() {
    return shard_route_preview_stage_catalog::format_digest_marker(
        current_stage(),
        current_stage_count,
        stages::planned_operator_value_supply_profile_section_stage_count());
}

int published_stage_count() {
    return current_stage_count;
}

} // namespace minikv::shard_preview_operator_value_supply_profile_section
