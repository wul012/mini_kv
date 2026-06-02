#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-archive-verification-route-readiness-snapshot",
    50,
    "v250",
    "snapshots mini-kv read-only evidence for the Node v549 archive verification route readiness path",
    "v249",
    "fixtures/release/shard-readiness-v249.json",
    "fnv1a64:04af1f491dc7c7fd",
    "frozen v249 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
