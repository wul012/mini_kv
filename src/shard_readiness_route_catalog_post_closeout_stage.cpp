#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-rollout-catalog-audit",
    129,
    "v329",
    "audits route preview verification report rollout catalog continuity with no active router",
    "v328",
    "fixtures/release/shard-readiness-v328.json",
    "fnv1a64:2193962195ba633a",
    "frozen v328 fixture is the source baseline for route preview verification report rollout-catalog-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
