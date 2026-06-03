#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-no-router-audit",
    114,
    "v314",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-no-router-audit with no active router",
    "v313",
    "fixtures/release/shard-readiness-v313.json",
    "fnv1a64:2193962195ba633a",
    "frozen v313 fixture is the source baseline for route preview verification no-router-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
