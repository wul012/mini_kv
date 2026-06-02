#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-archive-verification-route-readiness-audit",
    51,
    "v251",
    "audits the Node v549 archive verification route readiness snapshot from frozen v250 evidence",
    "v250",
    "fixtures/release/shard-readiness-v250.json",
    "fnv1a64:dd4981f4ab14209f",
    "frozen v250 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
