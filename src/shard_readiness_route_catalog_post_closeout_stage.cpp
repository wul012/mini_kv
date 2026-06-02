#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-shard-readiness-history-fixture-loop",
    64,
    "v264",
    "collapses shard readiness historical fixture loading and difference checks into version-indexed helpers",
    "v263",
    "fixtures/release/shard-readiness-v263.json",
    "fnv1a64:1e9e1013daf0ea8b",
    "frozen v263 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
