#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-stage-descriptor-audit",
    16,
    "v216",
    "audit the frozen split stage descriptor while preserving downstream read-only semantics",
    "v215",
    "fixtures/release/shard-readiness-v215.json",
    "fnv1a64:b745fa9e79265743",
    "frozen v215 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
