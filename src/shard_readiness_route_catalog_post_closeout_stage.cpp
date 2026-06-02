#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-archive-verification-alignment-audit",
    49,
    "v249",
    "audits the Node v549 archive verification alignment from frozen v248 evidence",
    "v248",
    "fixtures/release/shard-readiness-v248.json",
    "fnv1a64:b001b36a42e8e809",
    "frozen v248 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
