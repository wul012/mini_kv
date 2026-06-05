#pragma once

#include <string>

namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit_closeout {

std::string format_closeout_json();
std::string format_closeout_stage_catalog_json();
std::string closeout_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit_closeout
