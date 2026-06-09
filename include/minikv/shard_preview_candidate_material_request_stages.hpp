#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>

namespace minikv::shard_preview_candidate_material_request_stages {

std::span<const shard_route_preview_stage_catalog::StageRecord> candidate_material_request_stages();
int first_candidate_material_request_release_number();
int planned_candidate_material_request_stage_count();

} // namespace minikv::shard_preview_candidate_material_request_stages
