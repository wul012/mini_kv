#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-no-execution-audit",
    116,
    "v316",
    "advances read-only SHARDROUTEVERIFYJSON preview verification to route-preview-verification-no-execution-audit with no active router",
    "v315",
    "fixtures/release/shard-readiness-v315.json",
    "fnv1a64:2193962195ba633a",
    "frozen v315 fixture is the source baseline for route preview verification no-execution-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
