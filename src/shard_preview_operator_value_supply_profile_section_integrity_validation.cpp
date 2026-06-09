#include "minikv/shard_preview_operator_value_supply_profile_section_integrity_validation.hpp"

#include "minikv/runtime_evidence.hpp"

namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_validation {
namespace {

std::string json_bool(bool value) {
    return runtime_evidence::json_bool(value);
}

} // namespace

std::string format_operator_value_supply_profile_section_integrity_validation_json(
    int source_profile_section_stage_count,
    bool source_profile_section_complete,
    int source_profile_section_count,
    int source_node_migrated_section_count,
    bool source_digest_pinned,
    bool source_command_locked,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_consumes_operator_values,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count) {
    const bool source_locked = source_profile_section_stage_count == 25 && source_profile_section_complete &&
                               source_digest_pinned && source_command_locked;
    const bool section_counts_aligned =
        source_profile_section_count == 4 && source_node_migrated_section_count == 4 &&
        !node_requires_fresh_minikv_evidence && !mini_kv_consumes_operator_values;
    const bool counts_aligned = planned_check_count == 10 && completed_check_count == 10 &&
                                published_stage_count == planned_stage_count && planned_stage_count == 10;
    const bool validation_passed = source_locked && section_counts_aligned && counts_aligned;

    return "{\"operatorValueSupplyProfileSectionIntegrityValidationPassed\":" + json_bool(validation_passed) +
           ",\"sourceProfileSectionPublishedStageCount\":" +
           std::to_string(source_profile_section_stage_count) +
           ",\"sourceProfileSectionComplete\":" + json_bool(source_profile_section_complete) +
           ",\"sourceProfileSectionDigestPinned\":" + json_bool(source_digest_pinned) +
           ",\"sourceProfileSectionCommandLocked\":" + json_bool(source_command_locked) +
           ",\"sourceLocked\":" + json_bool(source_locked) +
           ",\"sourceProfileSectionCount\":" + std::to_string(source_profile_section_count) +
           ",\"sourceNodeMigratedProfileSectionCount\":" +
           std::to_string(source_node_migrated_section_count) +
           ",\"nodeRequiresFreshMiniKvEvidence\":" + json_bool(node_requires_fresh_minikv_evidence) +
           ",\"miniKvConsumesOperatorValues\":" + json_bool(mini_kv_consumes_operator_values) +
           ",\"sectionCountsAligned\":" + json_bool(section_counts_aligned) +
           ",\"plannedOperatorValueSupplyProfileSectionIntegrityCheckCount\":" +
           std::to_string(planned_check_count) +
           ",\"completedOperatorValueSupplyProfileSectionIntegrityCheckCount\":" +
           std::to_string(completed_check_count) +
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
           ",\"credentialValueRead\":false"
           ",\"rawEndpointParsed\":false"
           ",\"rawEndpointStored\":false"
           ",\"materialIntakeOpened\":false"
           ",\"activeRouterInstalled\":false"
           ",\"writeRoutingAllowed\":false"
           ",\"touchesWal\":false"
           ",\"executionAllowed\":false}";
}

} // namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_validation
