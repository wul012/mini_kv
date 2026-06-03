#pragma once

#include <string>
#include <string_view>

namespace minikv::shard_route_preview_verification_report {

std::string format_report_json(std::string_view key);
std::string format_rollout_json();
std::string rollout_digest_marker();

} // namespace minikv::shard_route_preview_verification_report
