#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>

namespace minikv::shard_preview_candidate_intake_packet_stages {

std::span<const shard_route_preview_stage_catalog::StageRecord> candidate_intake_packet_stages();
int first_candidate_intake_packet_release_number();
int planned_candidate_intake_packet_stage_count();

} // namespace minikv::shard_preview_candidate_intake_packet_stages
