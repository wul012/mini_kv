#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-node-consumer-handoff",
    111,
    "v311",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-node-consumer-handoff with no active router",
    "v310",
    "fixtures/release/shard-readiness-v310.json",
    "fnv1a64:2193962195ba633a",
    "frozen v310 fixture is the source baseline for route preview verification node-consumer-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
