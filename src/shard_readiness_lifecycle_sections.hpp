#pragma once

#include <string>

namespace minikv::shard_readiness::lifecycle_sections {

std::string format_active_prototype_plan_json();
std::string format_active_prototype_plan_freeze_json();
std::string format_consumer_handoff_json();
std::string format_live_read_gate_plan_json();
std::string format_live_read_gate_plan_freeze_json();
std::string format_operator_service_lifecycle_template_json();
std::string format_operator_service_lifecycle_template_freeze_json();
std::string format_operator_service_lifecycle_evidence_json();
std::string format_operator_service_lifecycle_evidence_freeze_json();

} // namespace minikv::shard_readiness::lifecycle_sections
