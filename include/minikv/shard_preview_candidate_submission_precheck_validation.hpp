#pragma once

#include <string>

namespace minikv::shard_preview_candidate_submission_precheck_validation {

std::string format_candidate_submission_precheck_validation_json(int source_integrity_stage_count,
                                                                 bool source_integrity_complete,
                                                                 int planned_checkpoint_count,
                                                                 int completed_checkpoint_count,
                                                                 int planned_validator_count,
                                                                 int completed_validator_count,
                                                                 int published_stage_count,
                                                                 int planned_stage_count);

} // namespace minikv::shard_preview_candidate_submission_precheck_validation
