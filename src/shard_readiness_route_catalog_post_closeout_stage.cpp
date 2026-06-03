#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-command-catalog-alignment",
    100,
    "v300",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-command-catalog-alignment with no active router",
    "v299",
    "fixtures/release/shard-readiness-v299.json",
    "fnv1a64:2193962195ba633a",
    "frozen v299 fixture is the source baseline for route preview verification command-catalog-alignment evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
