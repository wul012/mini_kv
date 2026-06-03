#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-fixture-freeze",
    85,
    "v285",
    "freezes route preview rollout fixture baseline",
    "v284",
    "fixtures/release/shard-readiness-v284.json",
    "fnv1a64:2193962195ba633a",
    "frozen v284 fixture is the source baseline for route-preview-fixture-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout