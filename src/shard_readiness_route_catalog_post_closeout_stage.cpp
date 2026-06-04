#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-release-package",
    159,
    "v359",
    "packages archive manifest evidence for release consumption",
    "v358",
    "fixtures/release/shard-readiness-v358.json",
    "fnv1a64:c740e83477a521b8",
    "frozen v358 fixture is the source baseline for route preview verification report archive-release-package evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
