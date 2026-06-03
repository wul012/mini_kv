#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-tcp-smoke-contract",
    84,
    "v284",
    "records TCP smoke expectations for read-only route preview",
    "v283",
    "fixtures/release/shard-readiness-v283.json",
    "fnv1a64:2193962195ba633a",
    "frozen v283 fixture is the source baseline for route-preview-tcp-smoke-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout