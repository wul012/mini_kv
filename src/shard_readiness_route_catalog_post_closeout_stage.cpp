#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-fixture-freeze",
    105,
    "v305",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-fixture-freeze with no active router",
    "v304",
    "fixtures/release/shard-readiness-v304.json",
    "fnv1a64:2193962195ba633a",
    "frozen v304 fixture is the source baseline for route preview verification fixture-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
