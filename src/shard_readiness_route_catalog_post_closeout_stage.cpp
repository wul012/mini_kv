#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-closure-snapshot",
    31,
    "v231",
    "snapshot the v221-v230 Node v505 readiness handoff archive verification route continuity closure",
    "v230",
    "fixtures/release/shard-readiness-v230.json",
    "fnv1a64:ae9c20d8cb446fb5",
    "frozen v230 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
