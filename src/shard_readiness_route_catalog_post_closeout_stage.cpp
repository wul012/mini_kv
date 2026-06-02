#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-final-verification-ready",
    62,
    "v262",
    "prepares final mini-kv read-only evidence readiness for the Node v549 archive verification line",
    "v261",
    "fixtures/release/shard-readiness-v261.json",
    "fnv1a64:1c833e424df779d8",
    "frozen v261 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
