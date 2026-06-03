#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-no-router-audit",
    94,
    "v294",
    "audits that route preview still installs no active router",
    "v293",
    "fixtures/release/shard-readiness-v293.json",
    "fnv1a64:2193962195ba633a",
    "frozen v293 fixture is the source baseline for route-preview-no-router-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout