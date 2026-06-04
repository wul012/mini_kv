#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-consumer-handoff",
    172,
    "v372",
    "records Node consumer handoff semantics for archive closeout fields",
    "v371",
    "fixtures/release/shard-readiness-v371.json",
    "fnv1a64:827d3e6a37f521e4",
    "frozen v371 fixture is the source baseline for route preview verification report archive-closeout-consumer-handoff evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout