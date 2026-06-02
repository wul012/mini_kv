#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-live-smoke-boundary-reconfirmation",
    54,
    "v254",
    "reconfirms mini-kv read-only boundaries for the Node v549 live-smoke archive verification line",
    "v253",
    "fixtures/release/shard-readiness-v253.json",
    "fnv1a64:efc86787cea8b3b6",
    "frozen v253 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
