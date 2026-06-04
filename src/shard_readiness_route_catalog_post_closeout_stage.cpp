#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-continuity-map",
    147,
    "v347",
    "maps v320-v340 continuity from report rollout through closeout and archive",
    "v346",
    "fixtures/release/shard-readiness-v346.json",
    "fnv1a64:5a2f46c60bb2a2e6",
    "frozen v346 fixture is the source baseline for route preview verification report archive-continuity-map evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
