#pragma once

#include <string>

namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_validation {

std::string format_candidate_material_submission_precheck_integrity_validation_json(
    int source_precheck_stage_count,
    bool source_precheck_complete,
    int source_checkpoint_count,
    int source_validator_count,
    int source_material_request_item_count,
    int source_acceptance_check_count,
    int source_required_field_count,
    int source_submission_field_count,
    int source_gate_count,
    int planned_check_count,
    int completed_check_count,
    int published_stage_count,
    int planned_stage_count);

} // namespace minikv::shard_preview_candidate_material_submission_precheck_integrity_validation
