#pragma once

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_precheck {

std::string format_value_supply_precheck_stage_catalog_json();
std::string format_value_supply_precheck_json();
std::string value_supply_precheck_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_operator_value_supply_precheck
