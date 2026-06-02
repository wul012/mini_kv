#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-release-catalog-audit",
    26,
    "v226",
    "audit the v221-v225 Node v505 readiness handoff archive verification route release catalog",
    "v225",
    "fixtures/release/shard-readiness-v225.json",
    "fnv1a64:b720e589ca8d9bc7",
    "frozen v225 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
