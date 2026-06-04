#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-no-router-audit",
    177,
    "v377",
    "audits that archive closeout evidence installs no active router",
    "v376",
    "fixtures/release/shard-readiness-v376.json",
    "fnv1a64:ac8ca08205ce8f0b",
    "frozen v376 fixture is the source baseline for route preview verification report archive-closeout-no-router-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout