#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v498-consumer-readiness-handshake-package",
    17,
    "v217",
    "package the Node v498 consumer readiness batch closeout archive handshake as read-only evidence",
    "v216",
    "fixtures/release/shard-readiness-v216.json",
    "fnv1a64:e4fc4c802f59ad80",
    "frozen v216 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
