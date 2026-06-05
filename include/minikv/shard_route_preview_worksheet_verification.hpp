#pragma once

#include <string>

namespace minikv::shard_route_preview_worksheet_verification {

std::string format_worksheet_verification_stage_catalog_json();
std::string format_worksheet_verification_json();
std::string worksheet_verification_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_worksheet_verification
