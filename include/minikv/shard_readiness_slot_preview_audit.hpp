#pragma once

#include <string>

namespace minikv::shard_readiness::slot_preview_audit {

std::string format_slot_table_preview_audit_json();
std::string format_slot_table_preview_audit_maintenance_json();

} // namespace minikv::shard_readiness::slot_preview_audit
