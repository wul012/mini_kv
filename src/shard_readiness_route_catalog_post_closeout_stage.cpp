#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-release-package",
    138,
    "v338",
    "packages route preview verification report rollout evidence for release consumption with no active router",
    "v337",
    "fixtures/release/shard-readiness-v337.json",
    "fnv1a64:2193962195ba633a",
    "frozen v337 fixture is the source baseline for route preview verification report release-package evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
