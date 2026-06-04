#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-audit",
    160,
    "v360",
    "audits final archive closeout continuity for the twenty-version archive chain",
    "v359",
    "fixtures/release/shard-readiness-v359.json",
    "fnv1a64:5dfc904fa37a1b23",
    "frozen v359 fixture is the source baseline for route preview verification report archive-closeout-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
