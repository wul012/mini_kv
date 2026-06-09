#pragma once

#include <string>

namespace minikv::shard_preview_candidate_material_request {

std::string format_candidate_material_request_json();
std::string format_candidate_material_request_stage_catalog_json();
std::string candidate_material_request_digest_marker();
std::string candidate_material_request_status();
int published_stage_count();

} // namespace minikv::shard_preview_candidate_material_request
