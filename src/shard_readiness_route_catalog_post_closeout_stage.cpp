#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-no-write-audit",
    115,
    "v315",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-no-write-audit with no active router",
    "v314",
    "fixtures/release/shard-readiness-v314.json",
    "fnv1a64:2193962195ba633a",
    "frozen v314 fixture is the source baseline for route preview verification no-write-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
