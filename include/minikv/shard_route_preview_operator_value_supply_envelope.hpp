#pragma once

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_envelope {

std::string format_value_supply_stage_catalog_json();
std::string format_value_supply_envelope_json();
std::string value_supply_envelope_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_operator_value_supply_envelope
