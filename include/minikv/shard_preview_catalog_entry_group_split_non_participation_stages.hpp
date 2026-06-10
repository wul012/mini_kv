#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>

namespace minikv::shard_preview_catalog_entry_group_split_non_participation_stages {

std::span<const shard_route_preview_stage_catalog::StageRecord> catalog_entry_group_split_non_participation_stages();
int first_catalog_entry_group_split_non_participation_release_number();
int planned_catalog_entry_group_split_non_participation_stage_count();

} // namespace minikv::shard_preview_catalog_entry_group_split_non_participation_stages
