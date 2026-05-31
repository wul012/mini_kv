#pragma once

#include <string>

namespace minikv::shard_readiness::approval_inputs {

std::string format_final_approval_gate_input_freeze_json();
std::string format_template_validator_echo_json();
std::string format_template_validator_echo_freeze_json();
std::string format_canonical_approval_input_precheck_json();

} // namespace minikv::shard_readiness::approval_inputs
