#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-boundary-catalog-index-split",
    70,
    "v270",
    "moves boundary catalog index JSON formatting into a dedicated source file",
    "v269",
    "fixtures/release/shard-readiness-v269.json",
    "fnv1a64:d18e6a49fe86a53d",
    "frozen v269 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
