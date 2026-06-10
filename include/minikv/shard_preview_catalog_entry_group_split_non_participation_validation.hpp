#pragma once

#include <string>

namespace minikv::shard_preview_catalog_entry_group_split_non_participation_validation {

std::string format_catalog_entry_group_split_non_participation_validation_json(
    int source_integrity_stage_count,
    bool source_integrity_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_catalog_split,
    bool mini_kv_reads_catalog_endpoint,
    bool mini_kv_consumes_catalog_records,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_catalog_entry_group_split_non_participation_validation
