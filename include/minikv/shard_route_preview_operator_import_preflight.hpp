#pragma once

#include <string>

namespace minikv::shard_route_preview_operator_import_preflight {

std::string format_import_preflight_stage_catalog_json();
std::string format_import_preflight_json();
std::string import_preflight_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_operator_import_preflight
