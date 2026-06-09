#pragma once

#include <string>

namespace minikv::shard_preview_candidate_intake_packet_validation {

std::string format_candidate_intake_packet_validation_json(int source_precheck_stage_count,
                                                           bool source_precheck_complete,
                                                           int planned_slot_count,
                                                           int completed_slot_count,
                                                           int planned_guard_count,
                                                           int completed_guard_count,
                                                           int published_stage_count,
                                                           int planned_stage_count);

} // namespace minikv::shard_preview_candidate_intake_packet_validation
