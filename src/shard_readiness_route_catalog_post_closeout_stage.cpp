#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-catalog-alignment",
    162,
    "v362",
    "aligns the archive closeout command with command catalog and help surfaces",
    "v361",
    "fixtures/release/shard-readiness-v361.json",
    "fnv1a64:de79bbc43b243f12",
    "frozen v361 fixture is the source baseline for route preview verification report archive-closeout-catalog-alignment evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout