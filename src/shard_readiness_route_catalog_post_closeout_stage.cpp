#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-rollout-catalog-audit",
    108,
    "v308",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-rollout-catalog-audit with no active router",
    "v307",
    "fixtures/release/shard-readiness-v307.json",
    "fnv1a64:2193962195ba633a",
    "frozen v307 fixture is the source baseline for route preview verification rollout-catalog-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
