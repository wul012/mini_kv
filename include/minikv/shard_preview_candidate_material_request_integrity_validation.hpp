#pragma once

#include <string>

namespace minikv::shard_preview_candidate_material_request_integrity_validation {

std::string format_candidate_material_request_integrity_validation_json(int source_material_request_stage_count,
                                                                        bool source_material_request_complete,
                                                                        int source_material_request_item_count,
                                                                        int source_acceptance_check_count,
                                                                        int source_material_request_gate_count,
                                                                        int source_requested_field_count,
                                                                        int planned_check_count,
                                                                        int completed_check_count,
                                                                        int published_stage_count,
                                                                        int planned_stage_count);

} // namespace minikv::shard_preview_candidate_material_request_integrity_validation
