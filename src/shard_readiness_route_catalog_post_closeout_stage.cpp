#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-cmake-source-dir-test-helper",
    66,
    "v266",
    "extracts common CMake registration for source-dir fixture test targets",
    "v265",
    "fixtures/release/shard-readiness-v265.json",
    "fnv1a64:16f6535206b72859",
    "frozen v265 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
