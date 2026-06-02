#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-cmake-linked-test-helper",
    65,
    "v265",
    "extracts common CMake registration for plain linked test targets",
    "v264",
    "fixtures/release/shard-readiness-v264.json",
    "fnv1a64:84d8aaf28b495b68",
    "frozen v264 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
