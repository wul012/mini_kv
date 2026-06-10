#pragma once

#include <string>

namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_validation {

std::string format_type_barrel_split_follow_up_fixture_audit_validation_json(
    int source_follow_up_stage_count,
    bool source_follow_up_complete,
    bool node_requires_fresh_minikv_evidence,
    bool mini_kv_runtime_reads_source_fixture_payload,
    bool mini_kv_imports_node_modules,
    bool mini_kv_executes_node_checks,
    bool mini_kv_starts_services,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_type_barrel_split_follow_up_fixture_audit_validation
