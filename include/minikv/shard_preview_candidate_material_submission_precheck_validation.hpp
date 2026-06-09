#pragma once

#include <string>

namespace minikv::shard_preview_candidate_material_submission_precheck_validation {

std::string format_candidate_material_submission_precheck_validation_json(int source_integrity_stage_count,
                                                                         bool source_integrity_complete,
                                                                         int source_material_request_item_count,
                                                                         int source_acceptance_check_count,
                                                                         int source_requested_field_count,
                                                                         int source_submission_field_count,
                                                                         int source_gate_count,
                                                                         int planned_checkpoint_count,
                                                                         int completed_checkpoint_count,
                                                                         int planned_validator_count,
                                                                         int completed_validator_count,
                                                                         int published_stage_count,
                                                                         int planned_stage_count);

} // namespace minikv::shard_preview_candidate_material_submission_precheck_validation
