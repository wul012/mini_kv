#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-final-closeout-package-audit",
    61,
    "v261",
    "audits the Node v549 final closeout package from frozen v260 evidence",
    "v260",
    "fixtures/release/shard-readiness-v260.json",
    "fnv1a64:f30c2273908b1a76",
    "frozen v260 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
