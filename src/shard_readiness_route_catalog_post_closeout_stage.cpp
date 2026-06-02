#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-stability-closeout-snapshot",
    41,
    "v241",
    "snapshots mini-kv read-only evidence for the Node v522 stability closeout path",
    "v240",
    "fixtures/release/shard-readiness-v240.json",
    "fnv1a64:b9a88b7ea2392cc6",
    "frozen v240 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
