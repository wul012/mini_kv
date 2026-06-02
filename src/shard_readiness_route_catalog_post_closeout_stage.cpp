#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-v211-v218-batch-package",
    19,
    "v219",
    "package the v211-v218 post-closeout continuity run as read-only batch evidence",
    "v218",
    "fixtures/release/shard-readiness-v218.json",
    "fnv1a64:34d57ad0a803bc2c",
    "frozen v218 fixture is the source baseline",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
