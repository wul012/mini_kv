#pragma once

#include <string>

namespace minikv::shard_preview_candidate_profile_section {

std::string format_candidate_profile_section_json();
std::string format_candidate_profile_section_stage_catalog_json();
std::string candidate_profile_section_digest_marker();
std::string candidate_profile_section_status();
int published_stage_count();

} // namespace minikv::shard_preview_candidate_profile_section
