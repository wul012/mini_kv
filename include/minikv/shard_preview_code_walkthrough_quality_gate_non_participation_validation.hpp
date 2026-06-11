#pragma once

#include <string>

namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_validation {

std::string format_code_walkthrough_quality_gate_non_participation_validation_json(
    int source_runtime_gate_stage_count,
    bool source_runtime_gate_complete,
    bool node_requires_fresh_minikv_evidence,
    bool node_requires_fresh_java_evidence,
    bool mini_kv_reads_node_walkthrough_files,
    bool mini_kv_scans_node_repository,
    bool mini_kv_executes_node_quality_route,
    bool mini_kv_starts_node_services,
    bool mini_kv_starts_java_service,
    bool mini_kv_starts_mini_kv_service,
    bool mini_kv_mutates_node_docs,
    bool mini_kv_mutates_java_docs,
    bool placeholder_walkthrough_allowed,
    bool bulk_historical_relocation_allowed,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_validation
