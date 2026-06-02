#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-alignment",
    21,
    "v221",
    "align post-closeout continuity evidence with Node v505 readiness handoff archive verification route",
    "v220",
    "fixtures/release/shard-readiness-v220.json",
    "fnv1a64:d9ecc44211f0fe02",
    "frozen v220 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
