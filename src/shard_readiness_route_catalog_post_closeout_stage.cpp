#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-stability-closeout-audit",
    42,
    "v242",
    "audits the Node v522 stability closeout snapshot from frozen v241 evidence",
    "v241",
    "fixtures/release/shard-readiness-v241.json",
    "fnv1a64:40293ba67109b6e0",
    "frozen v241 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
