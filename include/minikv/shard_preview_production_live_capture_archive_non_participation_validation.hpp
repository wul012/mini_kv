#pragma once

#include <string>

namespace minikv::shard_preview_production_live_capture_archive_non_participation_validation {

std::string format_production_live_capture_archive_non_participation_validation_json(
    int source_integrity_stage_count,
    bool source_integrity_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_starts_capture_archive,
    bool mini_kv_reads_archive_endpoint,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_production_live_capture_archive_non_participation_validation
