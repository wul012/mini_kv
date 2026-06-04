#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-index",
    168,
    "v368",
    "indexes the archive closeout stages for downstream read-only consumers",
    "v367",
    "fixtures/release/shard-readiness-v367.json",
    "fnv1a64:27ac4e8d0e9cffa7",
    "frozen v367 fixture is the source baseline for route preview verification report archive-closeout-index evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout