#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-archive-files-consumption-handoff-audit",
    57,
    "v257",
    "audits the Node v549 archive-files consumption handoff from frozen v256 evidence",
    "v256",
    "fixtures/release/shard-readiness-v256.json",
    "fnv1a64:f8a3b805dae7fa73",
    "frozen v256 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
