#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-stability-archive-package",
    37,
    "v237",
    "packages mini-kv read-only evidence for the Node v522 stability archive handoff path",
    "v236",
    "fixtures/release/shard-readiness-v236.json",
    "fnv1a64:ffee5a44b8688bd6",
    "frozen v236 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
