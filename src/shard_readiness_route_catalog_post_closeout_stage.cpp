#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-checkjson-contract",
    82,
    "v282",
    "freezes CHECKJSON route preview no-execution contract",
    "v281",
    "fixtures/release/shard-readiness-v281.json",
    "fnv1a64:2193962195ba633a",
    "frozen v281 fixture is the source baseline for route-preview-checkjson-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout