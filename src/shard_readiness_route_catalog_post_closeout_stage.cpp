#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v549-archive-verification-alignment",
    48,
    "v248",
    "aligns mini-kv post-closeout continuity with the Node v549 latest sibling live-smoke archive verification route archive verification plan",
    "v247",
    "fixtures/release/shard-readiness-v247.json",
    "fnv1a64:9fb71e13c517fff8",
    "frozen v247 fixture is the source baseline for Node v549 latest sibling archive verification continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
