#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-checkpoint-audit",
    28,
    "v228",
    "audit the Node v505 readiness handoff archive verification route checkpoint from frozen v227 evidence",
    "v227",
    "fixtures/release/shard-readiness-v227.json",
    "fnv1a64:2b6e2366ce59bc20",
    "frozen v227 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
