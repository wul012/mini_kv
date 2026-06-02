#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-package",
    23,
    "v223",
    "package the Node v505 readiness handoff archive verification route evidence from frozen v222 audit",
    "v222",
    "fixtures/release/shard-readiness-v222.json",
    "fnv1a64:fbb635a438eb4c03",
    "frozen v222 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
