#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-rollout-catalog",
    128,
    "v328",
    "catalogs route preview verification report rollout stages with no active router",
    "v327",
    "fixtures/release/shard-readiness-v327.json",
    "fnv1a64:2193962195ba633a",
    "frozen v327 fixture is the source baseline for route preview verification report rollout-catalog evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
