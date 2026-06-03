#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-node-consumer-handoff",
    132,
    "v332",
    "documents Node consumer handoff for route preview verification report evidence with no active router",
    "v331",
    "fixtures/release/shard-readiness-v331.json",
    "fnv1a64:2193962195ba633a",
    "frozen v331 fixture is the source baseline for route preview verification report node-consumer-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
