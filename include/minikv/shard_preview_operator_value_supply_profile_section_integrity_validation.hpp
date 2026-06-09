#pragma once

#include <string>

namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_validation {

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
    int planned_stage_count);

} // namespace minikv::shard_preview_operator_value_supply_profile_section_integrity_validation
