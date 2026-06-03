#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-release-package",
    117,
    "v317",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-release-package with no active router",
    "v316",
    "fixtures/release/shard-readiness-v316.json",
    "fnv1a64:2193962195ba633a",
    "frozen v316 fixture is the source baseline for route preview verification release-package evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
