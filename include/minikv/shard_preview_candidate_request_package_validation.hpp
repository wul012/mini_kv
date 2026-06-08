#pragma once

#include <string>

namespace minikv::shard_preview_candidate_request_package_validation {

std::string format_candidate_document_request_package_validation_json(int planned_item_count,
                                                                     int planned_check_count,
                                                                     int planned_gate_count,
                                                                     int completed_item_count,
                                                                     int completed_check_count,
                                                                     int completed_gate_count,
                                                                     bool source_intake_audit_chain_complete,
                                                                     int published_stage_count);

} // namespace minikv::shard_preview_candidate_request_package_validation
