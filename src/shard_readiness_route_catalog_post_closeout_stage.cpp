#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-rollout-catalog",
    107,
    "v307",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-rollout-catalog with no active router",
    "v306",
    "fixtures/release/shard-readiness-v306.json",
    "fnv1a64:2193962195ba633a",
    "frozen v306 fixture is the source baseline for route preview verification rollout-catalog evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
