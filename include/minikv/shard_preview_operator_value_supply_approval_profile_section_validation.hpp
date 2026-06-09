#pragma once

#include <string>

namespace minikv::shard_preview_operator_value_supply_approval_profile_section_validation {

std::string format_operator_value_supply_approval_profile_section_validation_json(
    int source_approval_template_stage_count,
    bool source_approval_template_complete,
    int source_signed_approval_template_stage_count,
    bool source_signed_approval_template_complete,
    int source_capture_preflight_stage_count,
    bool source_capture_preflight_complete,
    int source_artifact_preflight_stage_count,
    bool source_artifact_preflight_complete,
    int approval_profile_section_count,
    int node_migrated_section_count,
    bool node_requires_fresh_minikv_evidence,
    bool route_markdown_changed,
    bool mini_kv_consumes_approval_material,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_operator_value_supply_approval_profile_section_validation
