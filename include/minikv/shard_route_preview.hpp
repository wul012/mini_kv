#pragma once

#include <string>
#include <string_view>

namespace minikv::shard_route_preview {

struct RoutePreview {
    std::string key;
    int slot = 0;
    std::string shard_id;
    std::string route_mode;
    std::string preview_hash;
    bool pinned_fixture_sample = false;
};

RoutePreview preview_key(std::string_view key);

std::string format_route_text(std::string_view key);
std::string format_route_json(std::string_view key);
std::string format_rollout_json();
std::string rollout_digest_marker();

} // namespace minikv::shard_route_preview
