#pragma once

#include <string>

namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_validation {

std::string format_blocked_execution_rehearsal_split_non_participation_validation_json(
    int source_disabled_fake_harness_stage_count,
    bool source_disabled_fake_harness_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    bool mini_kv_reads_endpoints,
    bool mini_kv_reads_credentials,
    bool mini_kv_parses_historical_receipts,
    bool mini_kv_scans_java_evidence,
    bool mini_kv_replays_wal_regression,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_blocked_execution_rehearsal_split_non_participation_validation
