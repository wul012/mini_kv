#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-operator-handoff",
    90,
    "v290",
    "documents operator handoff for read-only route preview",
    "v289",
    "fixtures/release/shard-readiness-v289.json",
    "fnv1a64:2193962195ba633a",
    "frozen v289 fixture is the source baseline for route-preview-operator-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout