#pragma once

#include <string_view>

namespace minikv::shard_readiness::route_catalog_post_closeout {

struct PostCloseoutStageDescriptor {
    std::string_view continuity_stage;
    int stage_sequence;
    std::string_view stage_release_version;
    std::string_view stage_scope;
    std::string_view source_frozen_release_version;
    std::string_view source_frozen_fixture_path;
    std::string_view source_frozen_digest;
    std::string_view source_baseline_assertion;
};

const PostCloseoutStageDescriptor& current_stage_descriptor();

} // namespace minikv::shard_readiness::route_catalog_post_closeout
