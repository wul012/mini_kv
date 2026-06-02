#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-twenty-version-run-closeout-alignment-audit",
    34,
    "v234",
    "audits the Node v522 twenty-version run closeout alignment from frozen v233 evidence",
    "v233",
    "fixtures/release/shard-readiness-v233.json",
    "fnv1a64:ebf4eddbb67bc47b",
    "frozen v233 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
