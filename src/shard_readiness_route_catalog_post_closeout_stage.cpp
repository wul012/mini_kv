#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-continuity-audit",
    148,
    "v348",
    "audits archive continuity and keeps the evidence contract read-only",
    "v347",
    "fixtures/release/shard-readiness-v347.json",
    "fnv1a64:06a7a5ba45eba419",
    "frozen v347 fixture is the source baseline for route preview verification report archive-continuity-audit evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
