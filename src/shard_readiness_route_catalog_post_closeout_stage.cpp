#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-package-audit",
    24,
    "v224",
    "audit the Node v505 readiness handoff archive verification route evidence package from frozen v223 evidence",
    "v223",
    "fixtures/release/shard-readiness-v223.json",
    "fnv1a64:b183e9895c167ee3",
    "frozen v223 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
