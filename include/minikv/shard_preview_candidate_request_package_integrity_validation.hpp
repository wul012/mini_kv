#pragma once

#include <string>

namespace minikv::shard_preview_candidate_request_package_integrity_validation {

std::string format_candidate_request_package_integrity_validation_json(int source_published_stage_count,
                                                                       bool source_chain_complete,
                                                                       int planned_check_count,
                                                                       int completed_check_count,
                                                                       int published_stage_count,
                                                                       int planned_stage_count);

} // namespace minikv::shard_preview_candidate_request_package_integrity_validation
