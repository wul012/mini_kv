#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-index",
    143,
    "v343",
    "indexes the archive manifest stages for downstream read-only consumers",
    "v342",
    "fixtures/release/shard-readiness-v342.json",
    "fnv1a64:be6931b2ab6fd239",
    "frozen v342 fixture is the source baseline for route preview verification report archive-index evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
