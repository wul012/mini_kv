#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-cli-smoke-contract",
    103,
    "v303",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-cli-smoke-contract with no active router",
    "v302",
    "fixtures/release/shard-readiness-v302.json",
    "fnv1a64:2193962195ba633a",
    "frozen v302 fixture is the source baseline for route preview verification cli-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
