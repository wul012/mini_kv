#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v505-readiness-handoff-archive-verification-route-alignment-audit",
    22,
    "v222",
    "audit the Node v505 readiness handoff archive verification route alignment from frozen v221 evidence",
    "v221",
    "fixtures/release/shard-readiness-v221.json",
    "fnv1a64:d7c9c6a18205c598",
    "frozen v221 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
