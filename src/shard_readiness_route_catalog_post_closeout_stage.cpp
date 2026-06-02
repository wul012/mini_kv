#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-tcp-resp-server-harness-extraction",
    75,
    "v275",
    "extracts shared TCP RESP server lifecycle handling into the test support harness",
    "v274",
    "fixtures/release/shard-readiness-v274.json",
    "fnv1a64:66e6baa3d02e74bb",
    "frozen v274 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
