#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-index-audit",
    169,
    "v369",
    "audits archive closeout index continuity while preserving no-router boundaries",
    "v368",
    "fixtures/release/shard-readiness-v368.json",
    "fnv1a64:38573d7451372fd6",
    "frozen v368 fixture is the source baseline for route preview verification report archive-closeout-index-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout