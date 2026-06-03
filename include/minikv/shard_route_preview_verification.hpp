#pragma once

#include "minikv/shard_route_preview.hpp"

#include <string>
#include <string_view>

namespace minikv::shard_route_preview_verification {

struct RoutePreviewVerification {
    shard_route_preview::RoutePreview preview;
    bool slot_within_range = false;
    bool single_shard = false;
    bool preview_only = false;
    bool pinned_sample_consistent = false;
    bool hash_present = false;
    bool read_only = true;
    bool no_active_router = true;
    bool no_write_routing = true;
    bool no_execution = true;
    int check_count = 0;
    int passed_count = 0;
};

RoutePreviewVerification verify_key(std::string_view key);

std::string format_verification_json(std::string_view key);
std::string format_rollout_json();
std::string rollout_digest_marker();

} // namespace minikv::shard_route_preview_verification
