#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-node-consumer-handoff",
    91,
    "v291",
    "documents Node consumer handoff for route preview evidence",
    "v290",
    "fixtures/release/shard-readiness-v290.json",
    "fnv1a64:2193962195ba633a",
    "frozen v290 fixture is the source baseline for route-preview-node-consumer-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout