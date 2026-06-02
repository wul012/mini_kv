#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-final-closeout-package",
    45,
    "v245",
    "packages final mini-kv read-only evidence for the Node v522 closeout path",
    "v244",
    "fixtures/release/shard-readiness-v244.json",
    "fnv1a64:1a55e1d39f2827ac",
    "frozen v244 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
