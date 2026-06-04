#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "feature-read-only-shard-route-preview-verification-report-archive-closeout-explain-contract",
    163,
    "v363",
    "pins EXPLAINJSON semantics for the archive closeout command",
    "v362",
    "fixtures/release/shard-readiness-v362.json",
    "fnv1a64:ac8f76721f29a0f0",
    "frozen v362 fixture is the source baseline for route preview verification report archive-closeout-explain-contract evidence",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout