#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-release-catalog",
    25,
    "v225",
    "catalog the v221-v224 Node v505 readiness handoff archive verification route evidence releases",
    "v224",
    "fixtures/release/shard-readiness-v224.json",
    "fnv1a64:011a92d8d38f3557",
    "frozen v224 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
