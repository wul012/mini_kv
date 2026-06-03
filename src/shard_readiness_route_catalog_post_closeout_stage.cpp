#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-fixture-freeze",
    126,
    "v326",
    "freezes route preview verification report rollout fixture baseline with no active router",
    "v325",
    "fixtures/release/shard-readiness-v325.json",
    "fnv1a64:2193962195ba633a",
    "frozen v325 fixture is the source baseline for route preview verification report fixture-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
