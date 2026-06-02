#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-cmake-testing-module-split",
    67,
    "v267",
    "moves CMake test registration helpers into a dedicated testing module",
    "v266",
    "fixtures/release/shard-readiness-v266.json",
    "fnv1a64:fb40aa3d8da66ba6",
    "frozen v266 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
