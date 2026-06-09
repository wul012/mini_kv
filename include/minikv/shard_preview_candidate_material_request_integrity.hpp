#pragma once

#include <string>

namespace minikv::shard_preview_candidate_material_request_integrity {

std::string format_candidate_material_request_integrity_json();
std::string format_candidate_material_request_integrity_stage_catalog_json();
std::string candidate_material_request_integrity_digest_marker();
std::string candidate_material_request_integrity_status();
int published_stage_count();

} // namespace minikv::shard_preview_candidate_material_request_integrity
