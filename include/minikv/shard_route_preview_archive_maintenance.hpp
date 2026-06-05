#pragma once

#include <string>

namespace minikv::shard_route_preview_archive_maintenance {

std::string format_maintenance_stage_catalog_json();
std::string format_maintenance_json();
std::string maintenance_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_archive_maintenance
