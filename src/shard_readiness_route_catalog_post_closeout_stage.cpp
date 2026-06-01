#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-stage-descriptor-split",
    14,
    "v214",
    "split post-closeout continuity stage descriptor constants while preserving the JSON contract",
    "v213",
    "fixtures/release/shard-readiness-v213.json",
    "fnv1a64:81d869972aeed221",
    "frozen v213 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
