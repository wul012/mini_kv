#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-rollout-catalog-audit",
    88,
    "v288",
    "audits route preview rollout catalog continuity",
    "v287",
    "fixtures/release/shard-readiness-v287.json",
    "fnv1a64:2193962195ba633a",
    "frozen v287 fixture is the source baseline for route-preview-rollout-catalog-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout