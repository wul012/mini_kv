#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-archive-files-consumption-handoff",
    56,
    "v256",
    "hands off mini-kv read-only evidence for Node v549 archive-files consumption",
    "v255",
    "fixtures/release/shard-readiness-v255.json",
    "fnv1a64:c0381228f6fd3e32",
    "frozen v255 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
