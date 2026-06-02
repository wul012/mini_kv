#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-boundary-catalog-group-table-split",
    69,
    "v269",
    "moves the boundary catalog group table into a dedicated source file",
    "v268",
    "fixtures/release/shard-readiness-v268.json",
    "fnv1a64:efe3aa43bc5101cc",
    "frozen v268 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
