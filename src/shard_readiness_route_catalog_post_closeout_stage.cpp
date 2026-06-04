#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-closeout-summary",
    140,
    "v340",
    "summarizes route preview verification report closeout continuity with no active router",
    "v339",
    "fixtures/release/shard-readiness-v339.json",
    "fnv1a64:2193962195ba633a",
    "frozen v339 fixture is the source baseline for route preview verification report closeout summary evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
