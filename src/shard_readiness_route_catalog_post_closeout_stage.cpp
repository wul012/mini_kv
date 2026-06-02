#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-final-closeout-package",
    60,
    "v260",
    "packages final mini-kv read-only evidence for the Node v549 archive verification line",
    "v259",
    "fixtures/release/shard-readiness-v259.json",
    "fnv1a64:4ecbafd95ed78ab6",
    "frozen v259 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
