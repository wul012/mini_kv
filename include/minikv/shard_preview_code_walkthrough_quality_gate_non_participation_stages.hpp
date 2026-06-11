#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>

namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_stages {

std::span<const shard_route_preview_stage_catalog::StageRecord>
code_walkthrough_quality_gate_non_participation_stages();
int first_code_walkthrough_quality_gate_non_participation_release_number();
int planned_code_walkthrough_quality_gate_non_participation_stage_count();

} // namespace minikv::shard_preview_code_walkthrough_quality_gate_non_participation_stages
