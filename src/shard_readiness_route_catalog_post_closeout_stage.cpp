#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-command-contract",
    99,
    "v299",
    "adds read-only SHARDROUTEVERIFYJSON preview verification command with no active router",
    "v298",
    "fixtures/release/shard-readiness-v298.json",
    "fnv1a64:a58cf80e8ddfc6e3",
    "frozen v298 fixture is the source baseline for route preview verification command contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
