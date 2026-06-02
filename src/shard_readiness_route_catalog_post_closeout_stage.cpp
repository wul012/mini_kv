#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-stability-archive-package-audit",
    38,
    "v238",
    "audits the Node v522 stability archive package from frozen v237 evidence",
    "v237",
    "fixtures/release/shard-readiness-v237.json",
    "fnv1a64:241dc6d5438fd5ee",
    "frozen v237 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
