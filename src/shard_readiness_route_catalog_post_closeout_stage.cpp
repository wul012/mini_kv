#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-cmake-standalone-source-dir-test-helper",
    73,
    "v273",
    "extracts standalone source-dir test registration into the shared CMake testing module",
    "v272",
    "fixtures/release/shard-readiness-v272.json",
    "fnv1a64:4f819aca4d500c49",
    "frozen v272 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
