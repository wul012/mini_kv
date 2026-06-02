#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "maintenance-refactor-tcp-resp-test-support-extraction",
    74,
    "v274",
    "extracts shared TCP RESP socket and protocol helpers into a dedicated test support header",
    "v273",
    "fixtures/release/shard-readiness-v273.json",
    "fnv1a64:d744fba839270c98",
    "frozen v273 fixture is the source baseline for maintenance-only refactor continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
