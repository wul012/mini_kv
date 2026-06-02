#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-boundary-catalog-maintenance-split",
    71,
    "v271",
    "moves boundary catalog maintenance JSON formatting into a dedicated source file",
    "v270",
    "fixtures/release/shard-readiness-v270.json",
    "fnv1a64:de9b42aac6fd10cf",
    "frozen v270 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
