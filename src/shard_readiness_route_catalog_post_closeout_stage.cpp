#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-retention-map",
    170,
    "v370",
    "maps retained fixtures, screenshots, explanations, and walkthroughs as closeout evidence metadata",
    "v369",
    "fixtures/release/shard-readiness-v369.json",
    "fnv1a64:1a17d2e62f244c81",
    "frozen v369 fixture is the source baseline for route preview verification report archive-closeout-retention-map evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout