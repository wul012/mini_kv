#pragma once

#include <string>

namespace minikv::shard_route_preview_audit_closeout_archive_verification {

std::string format_archive_verification_stage_catalog_json();
std::string format_archive_verification_json();
std::string archive_verification_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_audit_closeout_archive_verification
