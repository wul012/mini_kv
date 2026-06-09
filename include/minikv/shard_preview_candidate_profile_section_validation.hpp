#pragma once

#include <string>

namespace minikv::shard_preview_candidate_profile_section_validation {

std::string format_candidate_profile_section_validation_json(int source_integrity_stage_count,
                                                             bool source_integrity_complete,
                                                             int candidate_section_count,
                                                             int node_migrated_section_count,
                                                             bool node_requires_fresh_minikv_evidence,
                                                             bool route_markdown_changed,
                                                             bool mini_kv_consumes_profile_material,
                                                             int planned_check_count,
                                                             int completed_check_count,
                                                             int published_stage_count,
                                                             int planned_stage_count);

} // namespace minikv::shard_preview_candidate_profile_section_validation
