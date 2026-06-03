#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-sample-parity-audit",
    86,
    "v286",
    "audits pinned sample slots against existing shard readiness samples",
    "v285",
    "fixtures/release/shard-readiness-v285.json",
    "fnv1a64:2193962195ba633a",
    "frozen v285 fixture is the source baseline for route-preview-sample-parity-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout