#pragma once

#include <string>

namespace minikv::shard_route_preview_operator_value_supply_precheck_slots {

int planned_value_supply_precheck_slot_count();
std::string format_value_supply_precheck_slots_json(int published_stage_count);

} // namespace minikv::shard_route_preview_operator_value_supply_precheck_slots
