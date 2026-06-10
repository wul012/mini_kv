#pragma once

#include <string>

namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_validation {

std::string format_type_barrel_split_follow_up_non_participation_validation_json(
    int source_type_barrel_split_stage_count,
    bool source_type_barrel_split_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_follow_up,
    bool mini_kv_imports_node_types,
    bool mini_kv_reads_profile_endpoint,
    bool mini_kv_executes_node_checks,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_type_barrel_split_follow_up_non_participation_validation
