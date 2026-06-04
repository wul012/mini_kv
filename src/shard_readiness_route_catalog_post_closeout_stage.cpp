#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-index-audit",
    144,
    "v344",
    "audits archive index continuity while preserving no-router boundaries",
    "v343",
    "fixtures/release/shard-readiness-v343.json",
    "fnv1a64:2ff62b23da30ba9d",
    "frozen v343 fixture is the source baseline for route preview verification report archive-index-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
