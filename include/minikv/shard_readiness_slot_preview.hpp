#pragma once

#include <string>

namespace minikv::shard_readiness::slot_preview {

int slot_count();
std::string format_route_samples_json();
std::string format_shard_map_json();
std::string format_slot_table_preview_json();

} // namespace minikv::shard_readiness::slot_preview
