#include "minikv/shard_readiness_route_catalog_post_closeout_stage.hpp"

namespace minikv::shard_readiness::route_catalog_post_closeout {
namespace {

constexpr PostCloseoutStageDescriptor descriptor{
    "post-closeout-continuity-node-v522-twenty-version-run-closeout-alignment",
    33,
    "v233",
    "align mini-kv post-closeout continuity with the Node v522 twenty-version run closeout while preserving read-only boundaries",
    "v232",
    "fixtures/release/shard-readiness-v232.json",
    "fnv1a64:b4781a6b786af6f5",
    "frozen v232 fixture is the source baseline for Node v522 post-closeout continuity",
};

} // namespace

const PostCloseoutStageDescriptor& current_stage_descriptor() {
    return descriptor;
}

} // namespace minikv::shard_readiness::route_catalog_post_closeout
