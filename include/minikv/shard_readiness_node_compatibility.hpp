#pragma once

#include <string>

namespace minikv::shard_readiness::node_compatibility {

std::string format_route_group_split_compatibility_json();
std::string format_route_split_compatibility_window_json();
std::string format_route_split_compatibility_window_audit_json();

} // namespace minikv::shard_readiness::node_compatibility
