#pragma once

#include <string>

namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit {

std::string format_audit_json();
std::string format_audit_stage_catalog_json();
std::string audit_digest_marker();
int published_stage_count();

} // namespace minikv::shard_route_preview_verification_report_archive_closeout_verification_audit