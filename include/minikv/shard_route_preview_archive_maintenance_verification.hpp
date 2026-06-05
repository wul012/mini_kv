#pragma once

#include <string>

namespace minikv::shard_route_preview_archive_maintenance_verification {

std::string format_verification_stage_catalog_json();
std::string format_verification_json();
std::string verification_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_archive_maintenance_verification
