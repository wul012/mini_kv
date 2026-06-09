#include "minikv/shard_preview_operator_value_supply_profile_section_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_operator_value_supply_profile_section_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_operator_value_supply_profile_section_validation_json(
    int source_import_preflight_stage_count,
    bool source_import_preflight_complete,
    int source_value_draft_stage_count,
    bool source_value_draft_complete,
    int source_value_supply_envelope_stage_count,
    bool source_value_supply_envelope_complete,
    int value_supply_profile_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_operator_values,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool sources_frozen = source_import_preflight_stage_count == 25 && source_import_preflight_complete &&
                                source_value_draft_stage_count == 25 && source_value_draft_complete &&
                                source_value_supply_envelope_stage_count == 25 &&
                                source_value_supply_envelope_complete;
    const bool renderer_split_scope_aligned =
        value_supply_profile_section_count == 4 && node_migrated_section_count == 4 &&
        !node_requires_fresh_minikv_evidence && !route_markdown_changed && !mini_kv_consumes_operator_values;
    const bool counts_aligned = planned_check_count == 25 && completed_check_count == 25 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 25;
    const bool validation_passed = sources_frozen && renderer_split_scope_aligned && counts_aligned;

    return "{\"operatorValueSupplyProfileSectionValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceImportPreflightPublishedStageCount\":" +
           std::to_string(source_import_preflight_stage_count) +
           ",\"sourceImportPreflightComplete\":" + json_bool(source_import_preflight_complete) +
           ",\"sourceValueDraftPublishedStageCount\":" + std::to_string(source_value_draft_stage_count) +
           ",\"sourceValueDraftComplete\":" + json_bool(source_value_draft_complete) +
           ",\"sourceValueSupplyEnvelopePublishedStageCount\":" +
           std::to_string(source_value_supply_envelope_stage_count) +
           ",\"sourceValueSupplyEnvelopeComplete\":" + json_bool(source_value_supply_envelope_complete) +
           ",\"freshSiblingIntakeRepresentedByEnvelopePlan\":true"
           ",\"sourcesFrozen\":" + json_bool(sources_frozen) +
           ",\"operatorValueSupplyProfileSectionCount\":" + std::to_string(value_supply_profile_section_count) +
           ",\"nodeMigratedOperatorValueSupplyProfileSectionCount\":" +
           std::to_string(node_migrated_section_count) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"routeMarkdownChanged\":" + json_bool(route_markdown_changed) +
           ",\"miniKvConsumesOperatorValues\":" + json_bool(mini_kv_consumes_operator_values) +
           ",\"rendererSplitScopeAligned\":" + json_bool(renderer_split_scope_aligned) +
           ",\"plannedOperatorValueSupplyProfileSectionCheckCount\":" + std::to_string(planned_check_count) +
           ",\"completedOperatorValueSupplyProfileSectionCheckCount\":" + std::to_string(completed_check_count) +
           ",\"publishedStageCount\":" + std::to_string(published_stage_count) +
           ",\"plannedStageCount\":" + std::to_string(planned_stage_count) +
           ",\"countsAligned\":" + json_bool(counts_aligned) +
           ",\"nodeRendererExecuted\":false"
           ",\"nodeCatalogImported\":false"
           ",\"freshMiniKvEvidenceRequiredByNode\":false"
           ",\"operatorValueImported\":false"
           ",\"operatorValueAccepted\":false"
           ",\"freshSiblingEvidenceImported\":false"
           ",\"runtimePayloadImported\":false"
           ",\"runtimePayloadAccepted\":false"
           ",\"syntheticEvidenceAccepted\":false"
           ",\"secretValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"materialIntakeOpened\":false"
           ",\"governanceEchoCreated\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_operator_value_supply_profile_section_validation
