#pragma once

#include <string>

namespace minikv::shard_preview_operator_value_supply_profile_section_validation {

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
    int planned_stage_count);

} // namespace minikv::shard_preview_operator_value_supply_profile_section_validation
