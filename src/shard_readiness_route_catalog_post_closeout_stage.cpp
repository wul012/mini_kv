#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-cli-smoke-contract",
    83,
    "v283",
    "records CLI smoke expectations for read-only route preview",
    "v282",
    "fixtures/release/shard-readiness-v282.json",
    "fnv1a64:2193962195ba633a",
    "frozen v282 fixture is the source baseline for route-preview-cli-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout