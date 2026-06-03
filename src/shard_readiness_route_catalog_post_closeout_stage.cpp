#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-operator-handoff",
    131,
    "v331",
    "documents operator handoff for read-only route preview verification report with no active router",
    "v330",
    "fixtures/release/shard-readiness-v330.json",
    "fnv1a64:2193962195ba633a",
    "frozen v330 fixture is the source baseline for route preview verification report operator-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
