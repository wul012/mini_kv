#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-operator-handoff",
    110,
    "v310",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-operator-handoff with no active router",
    "v309",
    "fixtures/release/shard-readiness-v309.json",
    "fnv1a64:2193962195ba633a",
    "frozen v309 fixture is the source baseline for route preview verification operator-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
