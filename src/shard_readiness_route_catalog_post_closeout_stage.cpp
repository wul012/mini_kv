#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-boundary-field-name-table-split",
    68,
    "v268",
    "moves the read-only boundary field name table into a dedicated source file",
    "v267",
    "fixtures/release/shard-readiness-v267.json",
    "fnv1a64:c4c8467762fe031f",
    "frozen v267 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
