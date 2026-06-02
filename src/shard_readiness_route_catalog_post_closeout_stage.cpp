#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-v211-v219-batch-package-audit",
    20,
    "v220",
    "audit the v211-v219 post-closeout continuity batch package from frozen v219 evidence",
    "v219",
    "fixtures/release/shard-readiness-v219.json",
    "fnv1a64:78b10360a474df83",
    "frozen v219 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
