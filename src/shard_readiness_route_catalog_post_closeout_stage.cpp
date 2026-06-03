#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-release-package",
    97,
    "v297",
    "packages route preview rollout evidence for release consumption",
    "v296",
    "fixtures/release/shard-readiness-v296.json",
    "fnv1a64:2193962195ba633a",
    "frozen v296 fixture is the source baseline for route-preview-release-package evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout