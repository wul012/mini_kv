#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-release-package",
    180,
    "v380",
    "packages final archive closeout evidence for release consumption",
    "v379",
    "fixtures/release/shard-readiness-v379.json",
    "fnv1a64:01e736bc2963baae",
    "frozen v379 fixture is the source baseline for route preview verification report archive-closeout-release-package evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout