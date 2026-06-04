#pragma once

#include <string>

namespace minikv::shard_route_preview_verification_report_archive {

std::string format_archive_json();
std::string format_stage_catalog_json();
std::string archive_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_verification_report_archive
