#pragma once

#include <string>

namespace minikv::shard_preview_type_barrel_split_non_participation_validation {

std::string format_type_barrel_split_non_participation_validation_json(
    int source_catalog_split_stage_count,
    bool source_catalog_split_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_type_barrel_split,
    bool mini_kv_imports_node_types,
    bool mini_kv_consumes_profile_boundaries,
    bool mini_kv_reads_evidence_endpoint,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_type_barrel_split_non_participation_validation
