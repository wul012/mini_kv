#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v498-consumer-readiness-handshake-package-audit",
    18,
    "v218",
    "audit the Node v498 consumer readiness handshake package while preserving read-only boundaries",
    "v217",
    "fixtures/release/shard-readiness-v217.json",
    "fnv1a64:c335f2b6ab5014b7",
    "frozen v217 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
