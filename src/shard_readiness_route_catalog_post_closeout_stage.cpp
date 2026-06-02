#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-batch-package-audit",
    30,
    "v230",
    "audit the v221-v229 Node v505 readiness handoff archive verification route batch package",
    "v229",
    "fixtures/release/shard-readiness-v229.json",
    "fnv1a64:443a838e25455f28",
    "frozen v229 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
