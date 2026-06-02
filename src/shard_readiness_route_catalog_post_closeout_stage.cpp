#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-final-audit",
    32,
    "v232",
    "audit the v221-v231 Node v505 readiness handoff archive verification route continuity closure",
    "v231",
    "fixtures/release/shard-readiness-v231.json",
    "fnv1a64:e5ecccc9d95c6ffd",
    "frozen v231 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
