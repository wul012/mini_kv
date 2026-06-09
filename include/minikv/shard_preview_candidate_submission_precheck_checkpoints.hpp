#pragma once

#include "minikv/shard_route_preview_stage_catalog.hpp"

#include <span>

namespace minikv::shard_preview_candidate_submission_precheck_checkpoints {

std::span<const shard_route_preview_stage_catalog::StageRecord> candidate_submission_precheck_checkpoints();
int first_candidate_submission_precheck_release_number();
int planned_candidate_submission_precheck_checkpoint_count();

} // namespace minikv::shard_preview_candidate_submission_precheck_checkpoints
