#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-explain-contract",
    101,
    "v301",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-explain-contract with no active router",
    "v300",
    "fixtures/release/shard-readiness-v300.json",
    "fnv1a64:2193962195ba633a",
    "frozen v300 fixture is the source baseline for route preview verification explain-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
