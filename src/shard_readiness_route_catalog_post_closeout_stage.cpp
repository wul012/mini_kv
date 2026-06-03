#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-history-fallback-freeze",
    109,
    "v309",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-history-fallback-freeze with no active router",
    "v308",
    "fixtures/release/shard-readiness-v308.json",
    "fnv1a64:2193962195ba633a",
    "frozen v308 fixture is the source baseline for route preview verification history-fallback-freeze evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
