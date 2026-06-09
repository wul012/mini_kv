#pragma once

#include <string>

namespace minikv::shard_preview_candidate_material_request_validation {

std::string format_candidate_material_request_validation_json(int source_intake_packet_stage_count,
                                                              bool source_intake_packet_complete,
                                                              int planned_item_count,
                                                              int completed_item_count,
                                                              int planned_check_count,
                                                              int completed_check_count,
                                                              int published_stage_count,
                                                              int planned_stage_count);

} // namespace minikv::shard_preview_candidate_material_request_validation
