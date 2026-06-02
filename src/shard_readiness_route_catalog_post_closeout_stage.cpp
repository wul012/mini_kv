#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-batch-package",
    29,
    "v229",
    "package the v221-v228 Node v505 readiness handoff archive verification route continuity run",
    "v228",
    "fixtures/release/shard-readiness-v228.json",
    "fnv1a64:7a4fe3614ff923cd",
    "frozen v228 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
