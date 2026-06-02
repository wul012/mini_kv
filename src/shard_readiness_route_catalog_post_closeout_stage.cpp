#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-ci-catalog-health-snapshot",
    39,
    "v239",
    "snapshots mini-kv read-only evidence for the Node v522 CI and route catalog health path",
    "v238",
    "fixtures/release/shard-readiness-v238.json",
    "fnv1a64:cb420f72e2db405d",
    "frozen v238 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
