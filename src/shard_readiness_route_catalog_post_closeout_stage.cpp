#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-tcp-smoke-contract",
    104,
    "v304",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-tcp-smoke-contract with no active router",
    "v303",
    "fixtures/release/shard-readiness-v303.json",
    "fnv1a64:2193962195ba633a",
    "frozen v303 fixture is the source baseline for route preview verification tcp-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
