#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>

namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_stages {

std::span<const shard_route_preview_stage_catalog::StageRecord>
disabled_precheck_upstream_echo_non_participation_stages();
int first_disabled_precheck_upstream_echo_non_participation_release_number();
int planned_disabled_precheck_upstream_echo_non_participation_stage_count();

} // namespace minikv::shard_preview_disabled_precheck_upstream_echo_non_participation_stages
