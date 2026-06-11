#pragma once

#include <string>

namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation {

std::string format_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation_json(
    int source_precheck_upstream_receipt_stage_count,
    bool source_precheck_upstream_receipt_complete,
    bool node_requires_fresh_minikv_evidence,
    bool node_requires_fresh_java_evidence,
    bool mini_kv_reads_node_archive_files,
    bool mini_kv_replays_node_archive,
    bool mini_kv_starts_node_services,
    bool mini_kv_starts_java_service,
    bool mini_kv_starts_mini_kv_service,
    bool mini_kv_reads_endpoints,
    bool mini_kv_reads_credentials,
    bool mini_kv_parses_raw_endpoint,
    bool mini_kv_executes_runtime_probe,
    bool mini_kv_enables_active_router,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_runtime_execution_packet_approval_gate_archive_verification_non_participation_validation
