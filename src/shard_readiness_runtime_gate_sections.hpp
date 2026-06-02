#pragma once

#include <string>

namespace minikv::shard_readiness::runtime_gate_sections {

std::string format_runtime_execution_artifact_intake_preflight_json();
std::string format_runtime_execution_artifact_intake_preflight_freeze_json();
std::string format_mini_kv_runtime_execution_artifact_candidate_json();
std::string format_mini_kv_runtime_execution_artifact_candidate_freeze_json();
std::string format_runtime_execution_approval_gate_input_precheck_json();
std::string format_runtime_execution_approval_gate_input_precheck_freeze_json();
std::string format_mini_kv_final_approval_gate_input_json();

} // namespace minikv::shard_readiness::runtime_gate_sections
