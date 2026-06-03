#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-release-package-audit",
    98,
    "v298",
    "audits route preview rollout release package",
    "v297",
    "fixtures/release/shard-readiness-v297.json",
    "fnv1a64:2193962195ba633a",
    "frozen v297 fixture is the source baseline for route-preview-release-package-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout