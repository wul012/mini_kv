#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-stage-descriptor-freeze",
    15,
    "v215",
    "freeze the split post-closeout stage descriptor as a versioned source baseline",
    "v214",
    "fixtures/release/shard-readiness-v214.json",
    "fnv1a64:05b61e656f9925b7",
    "frozen v214 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
