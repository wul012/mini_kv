#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-boundary-field-facade-simplification",
    72,
    "v272",
    "removes the local boundary field JSON array wrapper and uses the shared runtime evidence formatter directly",
    "v271",
    "fixtures/release/shard-readiness-v271.json",
    "fnv1a64:e4ad76d27f327696",
    "frozen v271 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
