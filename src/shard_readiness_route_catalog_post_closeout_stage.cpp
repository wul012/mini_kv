#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-final-closeout-package-audit",
    46,
    "v246",
    "audits the Node v522 final closeout package from frozen v245 evidence",
    "v245",
    "fixtures/release/shard-readiness-v245.json",
    "fnv1a64:a4303386b8081c3e",
    "frozen v245 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
