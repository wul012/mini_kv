#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace minikv::runtime_evidence_receipts {

std::string read_command_list_digest(const std::vector<std::string>& commands);
std::string format_live_read_session_hint_json(std::int64_t uptime_seconds,
                                               const std::vector<std::string>& read_commands);

std::string smoke_failure_taxonomy_digest();
std::string format_smoke_failure_taxonomy_json();
std::string format_smoke_operator_window_proof_json();

std::string format_runtime_ci_evidence_hint_json();
std::string format_runtime_artifact_retention_evidence_json();

std::string binary_provenance_digest();
std::string format_runtime_binary_provenance_hint_json();

std::string retention_provenance_check_digest();
std::string format_runtime_retention_provenance_check_json();

std::string retention_provenance_replay_marker_digest();
std::string format_runtime_retention_provenance_replay_marker_json();

} // namespace minikv::runtime_evidence_receipts
