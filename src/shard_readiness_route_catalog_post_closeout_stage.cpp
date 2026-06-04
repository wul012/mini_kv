#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-checkjson-contract",
    164,
    "v364",
    "pins CHECKJSON semantics for the archive closeout command",
    "v363",
    "fixtures/release/shard-readiness-v363.json",
    "fnv1a64:e38a70be9e5ca290",
    "frozen v363 fixture is the source baseline for route preview verification report archive-closeout-checkjson-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout